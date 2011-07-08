#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
#include <gl/glew.h>
#endif

#include <iostream>
#include <RX/mat4.h>
#include <QtOpenGL/QGLWidget>
#include "Homography.h"
#include "KLTCorresp.h"
#include "VideoLoader.h"

class VideoGL : public QGLWidget 
{
    Q_OBJECT

private:
    MediaHandler* loader;
    VideoFrame* frame, *oldFrame;
    QMutex mFrame;
    int _width, _height;

	KLTCorresp _klt;
	Homography _homography;

public:

    VideoGL(QWidget* parent = 0)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
        , frame(NULL)
        , loader(NULL)
    {
        startTimer(20);
    }

    void timerEvent(QTimerEvent *);

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
