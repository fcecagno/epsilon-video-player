#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
//#include <gl/glew.h>
#endif

#include <iostream>
#include <RX/mat4.h>
#include <QtOpenGL/QGLWidget>
#include "Homography.h"
#include "VideoLoader.h"

class VideoGL : public QGLWidget 
{
    Q_OBJECT

private:
    MediaHandler *loader;
    VideoFrame *frame, *oldFrame;
    QMutex mFrame;
    int _width, _height;

	Homography *_homography;
	GLuint tex;

public:

    VideoGL(QWidget* parent = 0)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
        , frame(NULL)
		, oldFrame(NULL)
        , loader(NULL)
		, _homography(NULL)
    {
        startTimer(50);
    }

    void timerEvent(QTimerEvent *);

    void setVideoLoader(MediaHandler* loader) {
        this->loader = loader;
    }

	void setHomography(Homography* homography) {
        _homography = homography;
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
