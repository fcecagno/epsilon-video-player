#include "VideoGL.h"

void VideoGL::paintGL() 
{
    if (frame) 
	{
		glRasterPos2i((_width - frame->getWidth())/2 + frame->getPosX(), (_height-frame->getHeight())/2 + frame->getPosY());
        glDrawPixels(frame->getWidth(), frame->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, frame->getData()->data());
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void VideoGL::timerEvent(QTimerEvent *)
{
	// hack
	static int hitCount = 0;
    if (loader) {

		// hack
		if(hitCount < 1037) {
			if(oldFrame != NULL)
				delete oldFrame;
			oldFrame = frame;
			frame = (VideoFrame*) loader->dequeueCond();
					
			if(oldFrame != NULL) {

				if(hitCount == 0) {
					frame->setPosX(0);
					frame->setPosY(0);
				}
				else {
					RX::vec2 pos = _homography2.transform(hitCount-1, RX::vec2(oldFrame->getPosX(), oldFrame->getPosY()));
					frame->setPosX(pos.x);
					frame->setPosY(0);
				}

				++hitCount;

				/*
				QImage img1((uchar*)(oldFrame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_RGB888);
				QImage img2((uchar*)(frame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_RGB888);

				vector< pair<int, int> > f1, f2;
				_klt.findCorresp(img1.bits(), img2.bits(), frame->getWidth(), frame->getHeight());
					
				string str("corresp");
				char buf[10];
				sprintf(buf, "%d", hitCount);
				str += buf;
				str += ".txt";

				_klt.print(str);
				*/
			}
		}

        update();
    }
}