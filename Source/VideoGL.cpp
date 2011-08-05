#include "VideoGL.h"

VideoGL::VideoGL(QWidget* parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
, frame(NULL)
, oldFrame(NULL)
, loader(NULL)
, _homography(NULL)
{
	connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	_timer.setInterval(50);
}

void VideoGL::start()
{
	_timer.start();
}

void VideoGL::forwardOne()
{
	onTimer();
}

void VideoGL::stop()
{
	_timer.stop();
}


void VideoGL::initializeGL() 
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
} 

void VideoGL::paintGL() 
{
    if (frame) 
	{
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, tex );
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        int h = frame->getHeight();
        int w = frame->getWidth();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, frame->getData()->constData());

        glBegin( GL_QUADS );
        glTexCoord2f(0.0, 0.0); glVertex2f((_width - w)/2 + frame->getPosX(), (_height-h)/2 + frame->getPosY2());
        glTexCoord2f(1.0, 0.0); glVertex2f((_width - w)/2 + frame->getPosX2(), (_height-h)/2 + frame->getPosY2());
        glTexCoord2f(1.0, 1.0); glVertex2f((_width - w)/2 + frame->getPosX2(), (_height-h)/2 + frame->getPosY());
        glTexCoord2f(0.0, 1.0); glVertex2f((_width - w)/2 + frame->getPosX(), (_height-h)/2 + frame->getPosY());
        glEnd();

		glFlush();
        glDisable(GL_TEXTURE_2D);
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

    glClear(GL_DEPTH_BUFFER_BIT);
}

void VideoGL::onTimer()
{
	// hack
	static int hitCount = 0;
    if (loader) {
		// hack
		if(hitCount < 1034) {
			if(oldFrame != NULL)
				delete oldFrame;
			oldFrame = frame;
			frame = (VideoFrame*) loader->dequeueCond();

			/*
			QImage img2((uchar*)(frame->getData()->data()), frame->getWidth(), frame->getHeight(), QImage::Format_ARGB32);
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
					frame->setPosX2(frame->getWidth());
					frame->setPosY2(frame->getHeight());
				}
				else {
					RX::vec2d oldpos = RX::vec2d(0, 0);
					RX::vec2d oldpos2 = RX::vec2d(frame->getWidth(), frame->getHeight());

					RX::vec2d pos = _homography->transform(hitCount-1, oldpos);
					RX::vec2d pos2 = _homography->transform(hitCount-1, oldpos2);

					frame->setPosX(pos.x);
					frame->setPosY(pos.y);
					frame->setPosX2(pos2.x);
					frame->setPosY2(pos2.y);

				}

				++hitCount;
			}
		}

        update();
    }
}
