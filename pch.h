
#ifndef PCH_H
#define PCH_H

//#ifdef _MSC_VER
//#define GLEW_STATIC
//#include "externals/include/glew/glew.h"
//#endif

#ifdef __APPLE__
#include <GL/glew.h>
#include <opengl/gl.h>  //mac
    #else
    #define GLEW_STATIC
    #include "externals/include/glew/glew.h"
    #include <gl/gl.h>  //win
#endif

#include <string>
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <float.h>
#include <time.h>

#include <list>
#include <math.h>

//#ifdef _MSC_VER
//#include <gl/gl.h>//win
//#endif
//#ifdef __APPLE__
//#include <opengl/gl.h>//mac
//#endif

#include <QtGlobal>
#include <QtMath>
#include <QFileInfo>
#include <QImage>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QThread>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QSize>
#include <QMatrix4x4>
#include <QDialog>
#include <QProgressDialog>
#include <QMainWindow>
#include <QLabel>
#include <QWheelEvent>
#include <QOpenGLTexture>
#include <QDateTime>
#include <QFileDialog>
#include <QToolButton>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QStandardPaths>
#include <QList>
#include <QGridLayout>
#include <QWidget>
#include <QMessageBox>
#include <QTimer>
#include <QObject>

#endif // PCH_H
