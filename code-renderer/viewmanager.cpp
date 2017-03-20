#include "viewmanager.h"

ViewManager* ViewManager::instance = NULL;

ViewManager* ViewManager::GetInstance()
{
    if(instance == NULL) {
        instance = new ViewManager;
    }
    return instance;
}

ViewManager::ViewManager()
{
    //eyePosition = QVector3D(0, 0, 2.5f);
    eyePosition = QVector3D(0, 0, 1000.0f);
    eyeLookPosition = QVector3D(0, 0, 0);
    QVector3D up = QVector3D(0, 1, 0);
    viewMatrix.lookAt(eyePosition, eyeLookPosition, up);
    viewVector = eyePosition - eyeLookPosition;
    viewVector.normalize();
    controlTar = -1;//all
}

QMatrix4x4 ViewManager::GetViewRotationMatrix()
{
    return viewRotationMatrix;
}
QMatrix4x4 ViewManager::GetViewTranslationMatrix()
{
    QMatrix4x4 mat = viewTranslationMatrix;
    mat.scale(initScaleFactor);
    mat.translate(translateFactor);
    return mat;
}

QMatrix4x4 ViewManager::GetViewTranslationRotationMatrix()
{
    return viewTranslationMatrix*viewRotationMatrix;
}

QMatrix4x4 ViewManager::GetViewMatrix()
{
    QMatrix4x4 rtn = viewMatrix*viewTranslationMatrix*viewRotationMatrix;
    rtn.scale(initScaleFactor);
    rtn.translate(translateFactor);
    return rtn;

}

QMatrix4x4 ViewManager::GetProjectionMatrix(float aspect)
{
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();
    if(ortho) {
        float size = 1.5f * zoom * initZoom;
        projectionMatrix.ortho(-aspect * size, aspect * size, -size, size, 1.5f-0.367f, 3.5f+0.367f); // 0.367 ~= (sqrt(3)-1)/2
    } else {
        //projectionMatrix.perspective(75.0f * zoom, aspect, 1.5f-0.367f, 3.5f+0.367f); // 0.367 ~= (sqrt(3)-1)/2
        eyelookdis = (eyePosition - eyeLookPosition).length();
        projectionMatrix.perspective(perspangle * zoom * initZoom, aspect, eyelookdis-clipdis, eyelookdis+clipdis);
    }
    return projectionMatrix;
}

QMatrix4x4 ViewManager::GetViewProjectionMatrix(float aspect)
{

    return GetProjectionMatrix(aspect) * GetViewMatrix();
}

QMatrix4x4 ViewManager::GetModelViewProjectionMatrix(float aspect)
{
    return GetViewProjectionMatrix(aspect);
}

QMatrix4x4 ViewManager::GetModelViewProjectionMatrix_ass(float aspect)
{
    return GetViewProjectionMatrix(aspect);
}

QMatrix4x4 ViewManager::GetModelViewProjectionMatrix(float aspect, int idx)
{
    return GetViewProjectionMatrix(aspect) * GetModelMatrix(idx);
}

QMatrix4x4 ViewManager::GetModelViewProjectionMatrix_ass(float aspect, int idx)
{
    return GetViewProjectionMatrix(aspect) * GetModelMatrix_ass(idx);
}


void ViewManager::keyPressEvent(QKeyEvent *event){
    if(controlType==0){
        QMatrix4x4 invrtRot = viewRotationMatrix.inverted();
        rotateYAxis = (invrtRot * QVector4D(0, 1, 0, 0)).toVector3D();
        rotateXAxis = (invrtRot * QVector4D(1, 0, 0, 0)).toVector3D();
    }else if(controlType==1){
        for(int i=0;i<objnum;i++){
            modelMgr[i].setRotationAxis();
        }
    }
    if(event->key() == Qt::Key_Left){
        if(controlType==0)
            viewRotationMatrix.rotate(-90, rotateYAxis);
        else if(controlType==1)
            modelMgr[controlTar].rotateX(-90);
    }else if(event->key() == Qt::Key_Right){
        if(controlType==0)
            viewRotationMatrix.rotate(90, rotateYAxis);
        else if(controlType==1)
            modelMgr[controlTar].rotateX(90);
    }else if(event->key() == Qt::Key_Up){
        if(controlType==0)
            viewRotationMatrix.rotate(-90, rotateXAxis);
        else if(controlType==1)
            modelMgr[controlTar].rotateY(-90);
    }else if(event->key() == Qt::Key_Down){
        if(controlType==0)
            viewRotationMatrix.rotate(90, rotateXAxis);
        else if(controlType==1)
            modelMgr[controlTar].rotateY(-90);
    }
    for(int i=0;i<objnum;i++){
        modelMgr[i].setViewRotation(viewRotationMatrix);
    }
}

void ViewManager::mousePressEvent(QMouseEvent *event)//right click also work
{
    if(controlTar<0){
        if( event->button() == Qt::LeftButton ) {
            lmbDown = true;
            lmbDownCoord = QVector2D( event->x(), event->y() );
            QMatrix4x4 invrtRot = viewRotationMatrix.inverted();
            rotateYAxis = (invrtRot * QVector4D(0, 1, 0, 0)).toVector3D();
            rotateXAxis = (invrtRot * QVector4D(1, 0, 0, 0)).toVector3D();
        } else if(event->button() == Qt::MiddleButton || event->button() == Qt::RightButton) {
            midDown = true;
            midDownCoord = QVector2D(event->x(), event->y());
        }
    }else{
        if( event->button() == Qt::LeftButton ) {
            lmbDown = true;
            lmbDownCoord = QVector2D( event->x(), event->y() );
            for(int i=0;i<objnum;i++){
                modelMgr[i].setRotationAxis();
            }
        } else if(event->button() == Qt::MiddleButton || event->button() == Qt::RightButton) {
            midDown = true;
            midDownCoord = QVector2D(event->x(), event->y());
        }
    }
}

void ViewManager::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton ) {
        lmbDown = false;
        if(autoReset)viewRotationMatrix.setToIdentity();
        for(int i=0;i<objnum;i++){
            modelMgr[i].setViewRotation(viewRotationMatrix);
        }
    } else if(event->button() == Qt::MiddleButton || event->button() == Qt::RightButton) {
        midDown = false;
        if(autoReset)viewTranslationMatrix.setToIdentity();
    }
}

void ViewManager::mouseMoveEvent(QMouseEvent *event)
{
    if(controlType==0){
        if( lmbDown ) {
            QVector2D coord = QVector2D( event->x(), event->y() );
            QVector2D diff = coord - lmbDownCoord;
            double factor = 0.2;
            diff.setX(diff.x()*allowAxis.x());
            diff.setY(diff.y()*allowAxis.y());
            viewRotationMatrix.rotate(diff.x() * factor, rotateYAxis);
            viewRotationMatrix.rotate(diff.y() * factor, rotateXAxis);
            lmbDownCoord = coord;
        } else if(midDown) {
            QVector2D coord = QVector2D(event->x(), event->y());
            QVector2D diff = coord - midDownCoord;
            diff.setX(diff.x()*allowAxis.x());
            diff.setY(diff.y()*allowAxis.y());
            diff *= QVector2D(1.0f, -1.0f) * 0.5;
            if(allowViewTranslation)viewTranslationMatrix.translate(QVector3D(diff, 0));
            midDownCoord = coord;
        }
    }else{
        if( lmbDown ) {
            QVector2D coord = QVector2D( event->x(), event->y() );
            QVector2D diff = coord - lmbDownCoord;
            diff.setX(diff.x()*allowAxis.x());
            diff.setY(diff.y()*allowAxis.y());
            double factor = 0.2;
            modelMgr[controlTar].rotateX(diff.x() * factor);
            modelMgr[controlTar].rotateY(diff.y() * factor);
            lmbDownCoord = coord;
        } else if(midDown) {
            QVector2D coord = QVector2D(event->x(), event->y());
            QVector2D diff = coord - midDownCoord;
            diff.setX(diff.x()*allowAxis.x());
            diff.setY(diff.y()*allowAxis.y());
            diff *= QVector2D(1.0f, -1.0f) * 0.5;
            modelMgr[controlTar].translate(QVector3D(diff, 0));
            midDownCoord = coord;
        }
    }
}

void ViewManager::wheelEvent(QWheelEvent *event)
{
    if(controlTar<0){
        QPoint wheelPosition = event->angleDelta();
        Zoom(wheelPosition.y() / 120.0f);
    }else{
        QPoint wheelPosition = event->angleDelta();
        float mul = wheelPosition.y();
        QVector3D xyz = modelMgr[controlTar].GetScaleXYZ();
        float factor = 1.05f;
        if(mul >= 1){
            xyz = xyz / factor;
        }else{
            xyz = xyz * factor;
        }
        modelMgr[controlTar].SetScale(xyz);
    }
}

void ViewManager::Zoom(float distance)
{
    zoom *= (1.0f + 0.05f * distance);
    zoom = qBound(0.1f, zoom, 2.0f);
}

void ViewManager::SetViewRotation(float theta, float phi)
{
    viewRotationMatrix.setToIdentity();
    viewRotationMatrix.rotate(theta, QVector3D(0, 1, 0));
    viewRotationMatrix.rotate(phi, QVector3D(1, 0, 0));
}

void ViewManager::SetViewRotation(float x, float y, float z)
{
    QVector3D v(x, y, z);
    v.normalize();
    QVector3D o(0, 0, 1);
    double angle = acos(QVector3D::dotProduct(v, o));
    viewRotationMatrix.setToIdentity();
    viewRotationMatrix.rotate(qRadiansToDegrees(angle), QVector3D::crossProduct(o, v));
}
void ViewManager::ResetView()
{
    zoom = 1.0f;
    viewTranslationMatrix.setToIdentity();
    viewRotationMatrix.setToIdentity();
    for(int i=0;i<objnum;i++)modelMgr[i].ResetView();
}
void ViewManager::ResetModel()
{
    for(int i=0;i<objnum;i++)modelMgr[i].ResetModel();
}
void ViewManager::Reset()
{
    ResetView();
    ResetModel();
}

