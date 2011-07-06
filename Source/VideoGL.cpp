#include "VideoGL.h"

void VideoGL::paintGL() 
{
    if (frame) 
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glRasterPos2i((_width - frame->getWidth())/2, (_height-frame->getHeight())/2);
        glDrawPixels(frame->getWidth(), frame->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, frame->getData()->data());

        delete frame;
        frame = NULL;
    }
}

void VideoGL::resizeGL(int w, int h) 
{
	_width = w;
	_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
      
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, w, 0, h, 0, 1);
}