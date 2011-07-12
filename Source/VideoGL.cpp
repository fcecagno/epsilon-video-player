#include "VideoGL.h"

void VideoGL::paintGL() 
{
    if (frame) 
	{
		glEnable(GL_TEXTURE_2D);
		
		int h = frame->getHeight();
		int w = frame->getWidth();

		/*
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->getData());

		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex2f((_width - w)/2 + frame->getPosX(), (_height-h)/2 + frame->getPosY());
		glTexCoord2f(1.0, 0.0);
		glVertex2f((_width - w)/2 + frame->getPosX() + w, (_height-h)/2 + frame->getPosY());
		glTexCoord2f(1.0, 1.0);
		glVertex2f((_width - w)/2 + frame->getPosX() + w, (_height-h)/2 + frame->getPosY() + h);
		glTexCoord2f(0.0, 1.0);
		glVertex2f((_width - w)/2 + frame->getPosX(), (_height-h)/2 + frame->getPosY()  + h);
		glEnd();
		*/

		glRasterPos2i((_width - w)/2 + frame->getPosX(), (_height-h)/2 + frame->getPosY());
        glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, frame->getData()->data());
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
		if(hitCount < 1038) {
			if(oldFrame != NULL)
				delete oldFrame;
			oldFrame = frame;
			frame = (VideoFrame*) loader->dequeueCond();
			/*
			QImage img2((uchar*)(frame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_RGB888);
			string str("corresp");
			char buf[10];
			sprintf(buf, "%d", hitCount);
			str += buf;
			str += ".png";
			img2.save(str.c_str());
			*/		
			if(oldFrame != NULL) {

				if(hitCount == 0) {
					frame->setPosX(0);
					frame->setPosY(0);
				}
				else {
					RX::vec2 pos = _homography2.transform(hitCount-1, RX::vec2(oldFrame->getPosX(), oldFrame->getPosY()));
					frame->setPosX(pos.x);
					frame->setPosY(pos.y);
				}

				++hitCount;
			}
		}
		else
			exit(0);

        update();
    }
}