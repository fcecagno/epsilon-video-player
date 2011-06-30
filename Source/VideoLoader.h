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
#include <QThread>
#include <QMutex>
#include <QSemaphore>

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

    QByteArray* getData() { return data; }
    quint32 getTimestamp() { return timestamp; }
};

class VideoFrame : public Frame {
private:
    int width, height;
public:
    VideoFrame(QByteArray* data, int width, int height, quint32 timestamp)
        : Frame(data, timestamp)
        , width(width)
        , height(height)
    {
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
};

class AudioFrame : public Frame {

};

class MediaHandler {
private:
    int streamIndex;
    QQueue<Frame*> queue;
    QMutex mQueue;
    QSemaphore availableFrames;
    quint32 mspf;
    AVCodec* codec;
    AVCodecContext* codecContext;
    int codecType;
    AVFrame* frame;
    int16_t samples[AVCODEC_MAX_AUDIO_FRAME_SIZE];

    void enqueue(Frame* frame) {
        QMutexLocker locker(&mQueue);
        queue.push_back(frame);
        availableFrames.release();
    }

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

        codecContext = context->streams[streamIndex]->codec;
        codec = avcodec_find_decoder(codecContext->codec_id);
        if (!codec) {
            qDebug("Codec %d nao suportado", context->streams[streamIndex]->codec->codec_id);
            return;
        }

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
//                qDebug() << "\\o\\";

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
                enqueue(myFrame);

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
//                qDebug() << "/o/";

                QByteArray* buffer = new QByteArray((char*) samples, outBufferSize);
                Frame* myFrame = new Frame(buffer, mspf * p->dts);
                mQueue.lock();
                queue.push_back(myFrame);
                mQueue.unlock();
            }
        }
    }

    Frame* dequeue() {
        QMutexLocker locker(&mQueue);
        Frame* frame = NULL;
        if (availableFrames.tryAcquire()) {
            frame = queue.first();
            queue.removeFirst();
        }
        return frame;
    }

    Frame* dequeueCond() {
        availableFrames.acquire();
        QMutexLocker locker(&mQueue);
        Frame* frame = queue.first();
        queue.removeFirst();
        return frame;
    }

    int getType() { return codecType; }

};

class MediaLoader : public QThread {
private:
    AVFormatContext* formatContext;
    QVector<MediaHandler*> mediaHandler;
    QString filename;

protected:
    void run();
public:

    MediaLoader(const QString& filename)
        : filename(filename)
    {
        av_register_all();

        int ret = av_open_input_file(&formatContext, filename.toAscii(), NULL, 0, NULL);
        if (ret != 0) {
            printf("Erro %d ao abrir o arquivo\n", AVERROR(ret));
            return;
        }

        ret = av_find_stream_info(formatContext);
        if (ret < 0) {
            printf("Erro %d ao recuperar informações de stream\n", AVERROR(ret));
            return;
        }

        dump_format(formatContext, 0, filename.toAscii(), 0);

        mediaHandler.clear();
        mediaHandler.resize(formatContext->nb_streams);
        for (int i = 0; i < formatContext->nb_streams; ++i)
            mediaHandler[i] = new MediaHandler(formatContext, i);
    }

    ~MediaLoader() {
        for (QVector<MediaHandler*>::iterator it = mediaHandler.begin(); it != mediaHandler.end(); ++it)
            delete (*it);
        av_close_input_file(formatContext);
    }

    MediaHandler* getVideoHandler() {
        for (QVector<MediaHandler*>::iterator it = mediaHandler.begin(); it != mediaHandler.end(); ++it)
            if ((*it)->getType() == CODEC_TYPE_VIDEO)
                return (*it);
        return NULL;
    }
};

#endif // VIDEOLOADER_H
