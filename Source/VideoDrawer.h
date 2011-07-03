#ifndef VIDEODRAWER_H
#define VIDEODRAWER_H

#include "VideoLoader.h"
#include "VideoGL.h"

class VideoDrawer : public QThread 
{
private:
    MediaLoader* loader;
    VideoGL* videoWidget;

protected:
//    void run() {
//        MediaHandler* videoHandler = loader->getVideoHandler();
//        while (true) {
////            VideoFrame* frame = (VideoFrame*) videoHandler->dequeueCond();
////            videoWidget->present(frame);
//            videoWidget->present(NULL);
//        }
//    }

public:
    VideoDrawer(VideoGL* widget, const QString& filename)
        : loader(new MediaLoader(filename))
        , videoWidget(widget)
    {
        videoWidget->setVideoLoader(loader->getVideoHandler());
        loader->start();
    }
};

#endif // VIDEODRAWER_H
