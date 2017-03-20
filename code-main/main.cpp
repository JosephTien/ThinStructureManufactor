#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include "pch.h"

/**
 * @brief setOpenGLSurfaceFormat
 * Set default OpenGL render surface of QOpenGLWidget.
 */
void setOpenGLSurfaceFormat()
{
    QSurfaceFormat format; 
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);
}

void initGLEW()
{
    QOpenGLContext *context = new QOpenGLContext();
    QOffscreenSurface *surface = new QOffscreenSurface();
    context->create();
    surface->create();
    context->makeCurrent(surface);
    glewExperimental = GL_TRUE;
    glewInit();
}


/**
 * @brief main
 * Entry point of this application.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{

    /**/
    unsigned seed;
    seed = (unsigned)time(NULL);
    srand(seed);
    /**/
    QApplication app(argc, argv);
    setOpenGLSurfaceFormat();
    initGLEW();
    MainWindow window;
    window.show();
    return app.exec();
}
