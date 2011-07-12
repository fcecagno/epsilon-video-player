#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
//#include <gl/glew.h>
#endif

#include <iostream>
#include <RX/mat4.h>
#include <QtOpenGL/QGLWidget>
#include "Homography.h"
#include "Homography2.h"
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
	Homography2 _homography2;
	GLuint tex;

public:

    VideoGL(QWidget* parent = 0)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
        , frame(NULL)
		, oldFrame(NULL)
        , loader(NULL)
    {
		_homography2.load("results.txt");
        startTimer(50);
    }

    void timerEvent(QTimerEvent *);

    void setVideoLoader(MediaHandler* loader) {
        this->loader = loader;
    }

    void initializeGL() 
	{
		glGenTextures(1, &tex);
    } 

    void present(VideoFrame* frame) 
	{
        this->frame = frame;
    }

    void paintGL();
    void resizeGL(int w, int h);
};

#endif // VIDEOGL_H
