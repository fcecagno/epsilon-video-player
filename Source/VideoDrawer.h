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
    void run() {
        MediaHandler* videoHandler = loader->getVideoHandler();
        while (true) {
            VideoFrame* frame = (VideoFrame*) videoHandler->dequeueCond();
            videoWidget->present(frame);
        }
    }

public:
    VideoDrawer(VideoGL* widget, const QString& filename)
        : videoWidget(widget)
        , loader(new MediaLoader(filename))
    {
        loader->start();
    }
};

#endif // VIDEODRAWER_H
