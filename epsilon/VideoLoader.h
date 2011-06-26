#ifndef VIDEOLOADER_H
#define VIDEOLOADER_H

// para resolver problemas de compilação no libavutil/common.h
#define UINT64_C(c) (c ## ULL)

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

#include <QDebug>
#include <QString>
#include <QQueue>
#include <QMap>

class Frame {
private:
    QByteArray* data;
    quint32 timestamp;
public:
    Frame(QByteArray* data, quint32 timestamp)
        : data(data)
        , timestamp(timestamp)
    {

    }
};

class MediaHandler {
private:
    int streamIndex;
    QQueue<Frame*> queue;
    quint32 mspf;
    AVCodec* codec;
    AVCodecContext* codecContext;
    int codecType;
    AVFrame* frame;
    int16_t samples[AVCODEC_MAX_AUDIO_FRAME_SIZE];

public:
    MediaHandler(AVFormatContext* context, int streamIndex)
        : streamIndex(streamIndex)
        , mspf((context->streams[streamIndex]->time_base.num * 1000) / context->streams[streamIndex]->time_base.den)
        , codecType(context->streams[streamIndex]->codec->codec_type)
    {
        frame = avcodec_alloc_frame();
        if (!frame) {
            qDebug() << "Frame não alocado";
            return;
        }

        codec = avcodec_find_decoder(context->streams[streamIndex]->codec->codec_id);
        if (!codec) {
            qDebug("Codec %d nao suportado", context->streams[streamIndex]->codec->codec_id);
            return;
        }

        codecContext = avcodec_alloc_context();
        if (avcodec_open(codecContext, codec) < 0) {
            qDebug() << "Falha ao abrir o codec";
            return;
        }
    }

    ~MediaHandler() {
        av_free(codecContext);
    }

    void handlePacket(AVPacket* p) {
        switch(codecType) {
            case CODEC_TYPE_VIDEO:
                handlePacketVideo(p);
                break;
            case CODEC_TYPE_AUDIO:
                handlePacketAudio(p);
                break;
            default:
                qDebug() << "Pacote de tipo inválido";
                break;
        }
    }

    void handlePacketVideo(AVPacket* p) {
        int gotPicture;
        int totalUsed = 0;
        int inBufferSize = p->size;

        while (totalUsed < inBufferSize) {
            AVPacket aux;
            av_init_packet(&aux);
            aux.data = p->data + totalUsed;
            aux.size = p->size - totalUsed;

            gotPicture = 0;
            int used = avcodec_decode_video2(codecContext, frame, &gotPicture, &aux);
            if (used < 0) {
                qDebug() << "Falha na decodificação";
                break;
            }
            av_free_packet(&aux);

            totalUsed += used;
            if (gotPicture) { // trata frame capturado
                qDebug() << "\\o\\";

                PixelFormat dstFormat = PIX_FMT_RGB24;
                AVFrame* deinterlacedFrame = avcodec_alloc_frame();
                AVFrame* formattedFrame = avcodec_alloc_frame();
                if (!deinterlacedFrame || !formattedFrame) {
                    qDebug() << "Não foi possível alocar o frame temporário";
                    return;
                }

                int outBufferSize = avpicture_get_size(codecContext->pix_fmt, codecContext->width, codecContext->height);
                QByteArray* outBufferDeinterlaced = new QByteArray(outBufferSize, '\0');
                avpicture_fill((AVPicture *) deinterlacedFrame, (uint8_t*) outBufferDeinterlaced->data(), codecContext->pix_fmt, codecContext->width, codecContext->height);

                if (avpicture_deinterlace((AVPicture *)deinterlacedFrame, (AVPicture *)frame,
                                          codecContext->pix_fmt, codecContext->width, codecContext->height) < 0) {
                    // se o desentrelaçamento falhar, faz uma cópia
                    av_picture_copy((AVPicture *)deinterlacedFrame, (AVPicture *)frame,
                                    codecContext->pix_fmt, codecContext->width, codecContext->height);
                }

                outBufferSize = avpicture_get_size(dstFormat, codecContext->width, codecContext->height);
                QByteArray* outBufferFormatted = new QByteArray(outBufferSize, '\0');
                avpicture_fill((AVPicture *) formattedFrame, (uint8_t*) outBufferFormatted->data(), dstFormat, codecContext->width, codecContext->height);

                SwsContext* scaleCtx = sws_getContext(
                        codecContext->width, codecContext->height, codecContext->pix_fmt,
                        codecContext->width, codecContext->height, dstFormat,
                        SWS_BILINEAR, NULL, NULL, NULL);
                if (!scaleCtx) {
                    qDebug() << "Não foi possível criar o contexto de transformação do frame";
                    return;
                }
                sws_scale(scaleCtx, deinterlacedFrame->data,
                          deinterlacedFrame->linesize, 0, codecContext->height,
                          formattedFrame->data, formattedFrame->linesize);
                sws_freeContext(scaleCtx);

                Frame* myFrame = new Frame(outBufferFormatted, mspf * p->dts);
                queue.push_back(myFrame);

                free(outBufferDeinterlaced);
                av_free(deinterlacedFrame);
                av_free(formattedFrame);
            }
        }
    }

    void handlePacketAudio(AVPacket* p) {
        int outBufferSize;
        int totalUsed = 0;
        int inBufferSize = p->size;

        while (totalUsed < inBufferSize) {
            AVPacket aux;
            av_init_packet(&aux);
            aux.data = p->data + totalUsed;
            aux.size = p->size - totalUsed;

            outBufferSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
            int used = avcodec_decode_audio3(codecContext, samples, &outBufferSize, &aux);
            if (used < 0) {
                qDebug() << "Falha na decodificação";
                break;
            }
            av_free_packet(&aux);

            totalUsed += used;
            if (outBufferSize > 0) { // trata o frame capturado
                qDebug() << "/o/";

                QByteArray* buffer = new QByteArray((char*) samples, outBufferSize);
                Frame* myFrame = new Frame(buffer, mspf * p->dts);
                queue.push_back(myFrame);
            }
        }
    }
};

class VideoLoader {
private:
    QMap<int, MediaHandler*> mediaHandler;

public:

    VideoLoader();
    ~VideoLoader() {
        for (QMap<int, MediaHandler*>::iterator it = mediaHandler.begin(); it != mediaHandler.end(); ++it)
            delete it.value();
    }

    void load(const QString& filename);

};

#endif // VIDEOLOADER_H
