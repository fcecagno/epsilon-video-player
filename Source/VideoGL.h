#ifndef VIDEOGL_H
#define VIDEOGL_H

#ifdef _WIN32
#include <gl/glew.h>
#endif

#include <QtOpenGL/QGLWidget>
#include <iostream>
#include "VideoLoader.h"

class VideoGL : public QGLWidget {
    Q_OBJECT

private:
    MediaHandler* loader;
    VideoFrame* frame;
    QMutex mFrame;
    bool initialized;

public:

    VideoGL(QWidget* parent = 0)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
        , frame(NULL)
        , loader(NULL)
        , initialized(false)
    {
        startTimer(10);
    }

//    static quint32 next_power_of_two(quint32 k) {
//        if (k == 0)
//            return 1;
//        k--;
//        for (int i=1; i < 32; i<<=1)
//            k = k | k >> i;
//        return k+1;
//    }

    void createTexture(int width, int height) {
        glBindTexture(GL_TEXTURE_2D, 0);
//        GLint crop[4] = { 0, height, width, -height };
//        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glEnable(GL_TEXTURE_2D);
        glColor4f(1,1,1,1);

//        int width_po2 = next_power_of_two(width);
//        int height_po2 = next_power_of_two(height);
//        int buffer_size = width_po2 * height_po2 * 3;
//        quint8 buffer[buffer_size];
//        memset(buffer, '\0', buffer_size);
//        // Create The Texture
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
//                     width_po2,
//                     height_po2,
//                     0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    }

    void timerEvent(QTimerEvent *)
    {
        if (loader) {
            frame = (VideoFrame*) loader->dequeueCond();
            if (!initialized) {
                createTexture(frame->getWidth(), frame->getHeight());
                initialized = true;
                resize(frame->getWidth(), frame->getHeight());
            }
            update();
            qDebug() << "OK!";
        }
    }

    void setVideoLoader(MediaHandler* loader) {
        this->loader = loader;
    }

    void initializeGL() {
        qDebug() << "VideoGL::initializeGL";
//        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//        glClearColor (0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
//        glClearDepth (1.0f);						// Depth Buffer Setup
//        glDepthFunc (GL_LEQUAL);					// The Type Of Depth Testing (Less Or Equal)
//        glEnable(GL_DEPTH_TEST);					// Enable Depth Testing
//        glShadeModel (GL_SMOOTH);					// Select Smooth Shading
//        glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Set Perspective Calculations To Most Accurate

//        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		// Set The Texture Generation Mode For S To Sphere Mapping
//        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);		// Set The Texture Generation Mode For T To Sphere Mapping


        // some init functions..
//        glClearColor(0.0, 0.0, 0.0, 0.0);
//        glEnable(GL_LINE_SMOOTH);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
//        glLineWidth(30.0);
//        glLineStipple(1, 0xAAAA);
//        glEnable(GL_LINE_STIPPLE);
    }


    void present(VideoFrame* frame) {
//        QMutexLocker locker(&mVideoGL);
//        qDebug() << "VideoGL::present";
        this->frame = frame;
//        makeCurrent();

//        updateGL();
//        doneCurrent();
//        paintGL();
    }


    void paintGL() {
//        qDebug() << "VideoGL::paintGL";
//        glClear(GL_COLOR_BUFFER_BIT);
//        glColor3f(1.0, 1.0, 1.0);
//        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

//        glBegin(GL_LINE_STRIP);
//        glColor3f(0.80, 0.0, 0.0);
//        glVertex2f(-0.4, -0.5);
//        glColor3f(0.0, 0.80, 0.0);
//        glVertex2f(-0.5, 0.5);
//        glColor3f(0.0, 0.0, 0.80);
//        glVertex2f(0.45, 0.5);
//        glColor3f(0.80, 0.80, 0.0);
//        glVertex2f(0.5, -0.5);
//        glColor3f(0.80, 0.0, 0.0);
//        glVertex2f(1.0, -0.5);
//        glEnd();

//        glFlush();

//        return;

        if (frame) {
//            qDebug() << "VideoGL::paintGL frame";

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->getWidth(),
//                            frame->getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
//                            frame->getData()->data());
            glDrawPixels(frame->getWidth(), frame->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, frame->getData()->data());

//            GLuint textures;
//            glEnable(GL_TEXTURE_2D);
//            glDeleteTextures(1, &textures);
//            glGenTextures(1, &textures);

//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, textures);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

//            qDebug("%dx%d %d", frame->getWidth(), frame->getHeight(), frame->getData()->size());
//            for (int i = 0; i < 10; ++i)
//                std::cout << (unsigned char) frame->getData()->at(i);
//            std::cout << std::endl;

//            glTexImage2D(GL_TEXTURE_2D, 0, 1, frame->getWidth(), frame->getHeight(), 0, GL_LUMINANCE,GL_UNSIGNED_BYTE, frame->getData()->data());
//            glTexImage2D(GL_TEXTURE_2D, 0, 1, frame->getWidth(), frame->getHeight(), 0, GL_RGB32I, GL_UNSIGNED_BYTE, frame->getData()->data());

//            glShadeModel(GL_FLAT);
//            glBegin(GL_QUADS);
//            glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
//            glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0);
//            glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
//            glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
//            glEnd();
//            glBegin( GL_QUADS );
//            glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
//            glTexCoord2d(1.0,0.0); glVertex2d(1.0,0.0);
//            glTexCoord2d(1.0,1.0); glVertex2d(1.0,1.0);
//            glTexCoord2d(0.0,1.0); glVertex2d(0.0,1.0);
//            glEnd();

//            glFlush();

//            glDeleteTextures(1, &textures);
//            glDisable(GL_TEXTURE_2D);

            delete frame;
            frame = NULL;
        }
    }

    void resizeGL(int w, int h) {
        qDebug() << "ResizeGL " << w << " " << h;
        if (frame)
            glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
    }
};

#endif // VIDEOGL_H
