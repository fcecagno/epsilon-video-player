#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
#include <gl/glew.h>
#endif

#include <iostream>
#include <RX/mat4.h>
#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include "Homography.h"
#include "VideoLoader.h"

class VideoGL : public QGLWidget 
{
    Q_OBJECT

public:
    VideoGL(QWidget* parent = 0);

	void start();
	void stop();
	void forwardOne();
    
    void setVideoLoader(MediaHandler* loader) { this->loader = loader; }
	void setHomography(Homography* homography) { _homography = homography; }

 	void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
	void onTimer();

private:
    MediaHandler *loader;
    VideoFrame *frame, *oldFrame;
	QMutex mFrame;
    int _width, _height;
	QTimer _timer;
	Homography *_homography;
	GLuint tex;
};

#endif // VIDEOGL_H
