#include "VideoGL.h"

void VideoGL::paintGL() 
{
    if (frame) 
	{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glRasterPos2i((_width - frame->getWidth())/2, (_height-frame->getHeight())/2);
		//glRasterPos2i(frame->getPosX(), frame->getPosY());
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

void VideoGL::timerEvent(QTimerEvent *)
{
	// hack
	static int hitCount = 0;
    if (loader) {

		// hack
		if(hitCount < 1037) {
			oldFrame = frame;
			frame = (VideoFrame*) loader->dequeueCond();
			
			if((oldFrame != NULL)&&(hitCount < 3)) {
				++hitCount;
				QImage img1((uchar*)(oldFrame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_RGB888);
				QImage img2((uchar*)(frame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_RGB888);

				vector< pair<int, int> > f1, f2;
				_klt.findCorresp(img1.bits(), img2.bits(), frame->getWidth(), frame->getHeight(), &f1, &f2);
				_klt.print
				for(int i = 0; i < f1.size(); ++i) {
					if(f2[i] == pair<int,int>(-1,-1)) {
						f2.erase(f2.begin()+i);
						f1.erase(f1.begin()+i);
						--i;
					}
				}
				_homography.findHomography(f1, f2);
				_homography.print("homography.txt");
			
			}
		}

        update();
    }
}