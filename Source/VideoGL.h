#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
//#include <gl/glew.h>
#endif

#include <iostream>
#include <RX/mat4.h>
#include <QtOpenGL/QGLWidget>
#include "Homography2.h"
#include "VideoLoader.h"

class VideoGL : public QGLWidget 
{
    Q_OBJECT

private:
    MediaHandler* loader;
    VideoFrame* frame, *oldFrame;
    QMutex mFrame;
    int _width, _height;

	//KLTCorresp _klt;
	//Homography _homography;
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
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_FLAT);
        glEnable(GL_DEPTH_TEST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures( 1, &tex );
        glBindTexture( GL_TEXTURE_2D, tex );
    } 

    void present(VideoFrame* frame) 
	{
        this->frame = frame;
    }

    void paintGL();
    void resizeGL(int w, int h);
};

#endif // VIDEOGL_H
