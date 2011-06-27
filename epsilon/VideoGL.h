#ifndef VIDEOGL_H
#define VIDEOGL_H

#include <QtOpenGL/QGLWidget>
#include "VideoLoader.h"

class VideoGL : public QGLWidget {
    Q_OBJECT

private:
    VideoFrame* frame;

public:

    VideoGL(QWidget* parent)
        : QGLWidget(parent)
        , frame(NULL)
    {

    }

    void initializeGL() {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }


    void present(VideoFrame* frame) {
        this->frame = frame;
        updateGL();
    }


    void paintGL() {
        if (frame) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            GLuint* textures = NULL;
            glEnable(GL_TEXTURE_2D);
            glDeleteTextures(1, textures);
            glGenTextures(1, textures);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
            glTexImage2D(GL_TEXTURE_2D, 0, 1, frame->getWidth(), frame->getHeight(), 0, GL_LUMINANCE,GL_UNSIGNED_BYTE, frame->getData());

            glShadeModel(GL_FLAT);
            glBegin(GL_QUADS);
            glTexCoord2d(0.0,0.0); glVertex2d(-1.0,-1.0);
            glTexCoord2d(1.0,0.0); glVertex2d(+1.0,-1.0);
            glTexCoord2d(1.0,1.0); glVertex2d(+1.0,+1.0);
            glTexCoord2d(0.0,1.0); glVertex2d(-1.0,+1.0);
            glEnd();

            glDeleteTextures(1, textures);
            glDisable(GL_TEXTURE_2D);
        }
    }

    void resizeGL(int w, int h) {
        qDebug() << "ResizeGL " << w << " " << h;
        if (frame)
            glViewport(0, 0, frame->getWidth(), frame->getHeight());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
    }
};

#endif // VIDEOGL_H
