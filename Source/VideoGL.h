#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
#include <gl/glew.h>
#endif

#include <QtOpenGL/QGLWidget>
#include <iostream>
#include "VideoLoader.h"

class VideoGL : public QGLWidget 
{
    Q_OBJECT

private:
    MediaHandler* loader;
    VideoFrame* frame;
    QMutex mFrame;
    int _width, _height;

public:

    VideoGL(QWidget* parent = 0)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
        , frame(NULL)
        , loader(NULL)
    {
        startTimer(20);
    }

    void timerEvent(QTimerEvent *)
    {
		// hack
		static int hitCount = 0;
        if (loader) {
			// hack
			++hitCount;
			if(hitCount < 1038)
				frame = (VideoFrame*) loader->dequeueCond();
            update();
        }
    }

    void setVideoLoader(MediaHandler* loader) {
        this->loader = loader;
    }

    void initializeGL() 
	{
    }


    void present(VideoFrame* frame) 
	{
        this->frame = frame;
    }

    void paintGL();
    void resizeGL(int w, int h);
};

#endif // VIDEOGL_H
