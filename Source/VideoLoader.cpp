#include "VideoLoader.h"

void MediaLoader::run() 
{
    qDebug() << "MediaLoader::run";
    AVPacket packet;
    while (av_read_frame(formatContext, &packet) >= 0) {
        mediaHandler.at(packet.stream_index)->handlePacket(&packet);
        av_free_packet(&packet);
    }
    qDebug() << "Operacao concluida";
}
