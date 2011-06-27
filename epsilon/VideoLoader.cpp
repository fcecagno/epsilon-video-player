#include "VideoLoader.h"

void VideoLoader::run() {
    AVFormatContext* formatContext;

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

    AVPacket packet;
    while (av_read_frame(formatContext, &packet) >= 0) {
        MediaHandler* handler;
        if (mediaHandler.contains(packet.stream_index))
            handler = mediaHandler[packet.stream_index];
        else {
            handler = new MediaHandler(formatContext, packet.stream_index);
            mediaHandler.insert(packet.stream_index, handler);
        }
        handler->handlePacket(&packet);
        av_free_packet(&packet);
    }
    av_close_input_file(formatContext);
    qDebug() << "Operacao concluida";
}
