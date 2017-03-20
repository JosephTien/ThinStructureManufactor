#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "pch.h"
#include "modelmanager.h"

/**
 * @brief The ViewManager class
 * The ViewManager class provides viewing manipulation related functionalities.
 *
 * It is designed as a singelton class so that multiple OpenGL views can share
 * the same viewing property.
 *
 * To use the ViewManager class, first obtain the pointer to the singelton instance
 * by calling static function GetInstance(). Next, call mousePressEvent(),
 * mouseReleaseEvent(), mouseMoveEvent(), wheelEvent() in your event handlers
 * with the event objects.
 *
 * The viewing manipulation will be done for you in the class. When you are ready
 * to render something, call GetModelMatrix(), GetViewMatrix(), GetProjectionMatrix()
 * and their composite versions to get the MVP matrices which ecode current viewing
 * properties.
 */
class ViewManager
{
private:
    ViewManager();
    static ViewManager *instance;

    bool ortho = false;

    QMatrix4x4 viewTranslationMatrix;
    QMatrix4x4 viewRotationMatrix;
    QMatrix4x4 viewMatrix;
    QVector3D viewVector;
    QVector3D rotateXAxis;
    QVector3D rotateYAxis;

    bool lmbDown = false;
    bool midDown = false;
    QVector2D lmbDownCoord;
    QVector2D midDownCoord;

    static const int objnum = 100;
    int controlTar = -1;
    int controlType = 0;
public:
    bool allowViewTranslation = false;
    bool autoReset=false;
    QVector2D allowAxis={1,1};
    float calxdegree = 0;
    float calydegree = 0;
    ModelManager modelMgr[objnum];
    ModelManager modelMgr_ass[objnum];

    float eyelookdis;
    float perspangle = 75.0f;
    float clipdis = 500.0f;
    float zoom = 1.0f;
    QVector3D eyePosition;
    QVector3D eyeLookPosition;

    static ViewManager* GetInstance();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);
    QMatrix4x4 GetViewRotationMatrix();
    QMatrix4x4 GetViewTranslationMatrix();
    QMatrix4x4 GetViewTranslationRotationMatrix();
    QMatrix4x4 GetModelMatrix(int idx) {return modelMgr[idx].GetModelMatrix();}
    QMatrix4x4 GetModelMatrix_ass(int idx) {return modelMgr_ass[idx].GetModelMatrix();}
    QMatrix4x4 GetViewMatrix();
    QMatrix4x4 GetProjectionMatrix(float aspect);
    QMatrix4x4 GetViewProjectionMatrix(float aspect);
    QMatrix4x4 GetModelViewProjectionMatrix(float aspect);
    QMatrix4x4 GetModelViewProjectionMatrix(float aspect, int idx);
    QMatrix4x4 GetModelViewProjectionMatrix_ass(float aspect);
    QMatrix4x4 GetModelViewProjectionMatrix_ass(float aspect, int idx);
    QVector3D GetEyePosition() {return eyePosition;}
    QVector3D GetViewVector() {return viewVector;}
    bool GetOrtho() { return ortho; }

    void SetViewRotation(float theta, float phi);
    void SetViewRotation(float x, float y, float z);

    bool ToggleOrtho() { return ortho = !ortho; }
    void Zoom(float distance);
    void ResetView();
    void ResetModel();
    void Reset();

    void setControlTar(int tar){controlTar=tar;}
    void setControlType(int tar){controlType=tar;}//0 : view,//1 : model
    int getControlType(){return controlType;}//0 : view,//1 : model
    void setFixAxis(QVector2D aa){allowAxis = aa;}//1 : x; 2 : y;


    float initZoom = 1.0f;
    float initScaleFactor = 1.0f;
    QVector3D translateFactor=QVector3D(0, 0, 0);
    /*
    float initZoom=0.25f;
    float initScaleFactor = 2.0f;
    QVector3D translateFactor=QVector3D(0,-50,0);
    */
    inline float getZoom(){return zoom * initZoom;}


};

#endif // VIEWMANAGER_H
