#include "modelmanager.h"
#include "pch.h"

QVector3D ModelManager::detourNormal(){
    QVector3D c(0,0,0);
    vecq3d detourPoints;
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourPoints.push_back(getVertice(detourIdxs[i]));
        c += detourPoints[i];
    }
    c /= (float)detourPoints.size();
    QVector3D n(0,0,0);
    for(int i=1;i<(int)detourPoints.size();i++){
        n += QVector3D::crossProduct(detourPoints[i-1]-c, detourPoints[i]-c);
    }
    return n.normalized();
}

QVector3D ModelManager::selecPointsNormal(){
    vecq3d s = getSelecPointsByIdxs();
    QVector3D c = (s[0]+s[1]+s[2])/3;
    QVector3D n = QVector3D::crossProduct(s[0]-c,s[1]-c);
    return n;
}

QVector3D ModelManager::detourNormal_ori(std::vector<int> detourIdxs_this){
    QVector3D c(0,0,0);
    vecq3d detourPoints;
    for(int i=0;i<(int)detourIdxs_this.size();i++){
        detourPoints.push_back(getVertice_ori(detourIdxs_this[i]));
        c += detourPoints[i];
    }
    c /= (float)detourPoints.size();
    QVector3D n(0,0,0);
    for(int i=1;i<(int)detourPoints.size();i++){
        n += QVector3D::crossProduct(detourPoints[i-1]-c, detourPoints[i]-c);
    }
    return n.normalized();
}

QVector3D ModelManager::detourNormal_ori(){
    QVector3D c(0,0,0);
    vecq3d detourPoints;
    for(int i=0;i<(int)detourIdxs.size();i++){
        detourPoints.push_back(getVertice_ori(detourIdxs[i]));
        c += detourPoints[i];
    }
    c /= (float)detourPoints.size();
    QVector3D n(0,0,0);
    for(int i=1;i<(int)detourPoints.size();i++){
        n += QVector3D::crossProduct(detourPoints[i-1]-c, detourPoints[i]-c);
    }
    return n.normalized();
}

QVector3D ModelManager::selecPointsNormal_ori(){
    vecq3d s = getSelecPointsByIdxs_ori();
    QVector3D c = (s[0]+s[1]+s[2])/3;
    QVector3D n = QVector3D::crossProduct(s[0]-c,s[1]-c);
    return n;
}
void ModelManager::reverseDetours(){
    int ds = (int)detourIdxs.size();
    for(int i=1;i<(ds+1)/2;i++){
        unsigned int t = detourIdxs[i];
        detourIdxs[i] = detourIdxs[ds-i];
        detourIdxs[ds-i] = t;
    }
}
QVector3D ModelManager::selecPointsCenter(){
    vecq3d s = getSelecPointsByIdxs();
    return (s[0]+s[1]+s[2])/3;
}
QVector3D ModelManager::selecPointsCenter_ori(){
    vecq3d s = getSelecPointsByIdxs_ori();
    return (s[0]+s[1]+s[2])/3;
}
QVector3D ModelManager::detourCenter(){
    QVector3D c(0,0,0);
    for(int i=0;i<detourIdxs.size();i++){
        QVector3D v = getVertice(detourIdxs[i]);
        c+=v;
    }
    return c/detourIdxs.size();
}
QVector3D ModelManager::detourCenter_ori(){
    QVector3D c(0,0,0);
    int ds = detourIdxs.size();
    float total=0;
    for(int i=0;i<ds;i++){
        QVector3D v1 = getVertice_ori(detourIdxs[i]);
        QVector3D v2 = getVertice_ori(detourIdxs[(i+1)%ds]);
        float l = (v1-v2).length();
        c+=(v1+v2)*l/2;
        total+=l;
    }
    return c/total;
}
//QVector3D ModelManager::detourCenter_ori(){
//    QVector3D c(0,0,0);
//    for(int i=0;i<detourIdxs.size();i++){
//        QVector3D v = getVertice_ori(detourIdxs[i]);
//        c+=v;
//    }
//    return c/detourIdxs.size();
//}

QVector3D ModelManager::detourCenter_ori(std::vector<int> detourIdxs_this){
    QVector3D c(0,0,0);
    for(int i=0;i<detourIdxs_this.size();i++){
        QVector3D v = getVertice_ori(detourIdxs_this[i]);
        c+=v;
    }
    return c/detourIdxs_this.size();
}
float ModelManager::dotProduct(QVector3D a,QVector3D b){
    return a.x()*b.x()+a.y()*b.y()+a.z()*b.z();
}
float ModelManager::pointDistanceToPlane(QVector3D v,QVector3D s1, QVector3D s2, QVector3D s3){
    QVector3D c((s1+s2+s3)/3);
    QVector3D n = QVector3D::crossProduct(s2-s1, s3-s1).normalized();
    QVector3D vc = c-v;
    return -(vc.x()*n.x()+vc.y()*n.y()+vc.z()*n.z());
}

bool ModelManager::isCrossPlane(QVector3D v1, QVector3D v2, QVector3D s1, QVector3D s2, QVector3D s3){
    QVector3D c((s1+s2+s3)/3);
    QVector3D n = QVector3D::crossProduct(s2-s1, s3-s1).normalized();
    QVector3D v1c = (c-v1);
    QVector3D v2c = (c-v2);
    float d1 = v1c.x()*n.x()+v1c.y()*n.y()+v1c.z()*n.z();
    float d2 = v2c.x()*n.x()+v2c.y()*n.y()+v2c.z()*n.z();
    if(std::fabs(d1)<minThre || std::fabs(d2)<minThre){
        return false;
    }else if(d1*d2<0){
        return true;
    }
    return false;
}
void ModelManager::clearConnector(){
    if(connectorFaceIdxs.size()>0){
        std::vector<bool> exist;
        exist.resize(vertices_ori.size()/3,true);
        for(int i=0;i<connectorFaceIdxs.size();i++){
            exist[connectorFaceIdxs[i]]=false;
        }
        for(int i=0;i<connectorFaceIdxs_sup.size();i++){
            exist[connectorFaceIdxs_sup[i]]=false;
        }
        std::vector<unsigned int> indices_new;
        for(int i=0;i<indices.size();i+=3){
            if(exist[indices[i]]&&exist[indices[i+1]]&&exist[indices[i+2]]){
                indices_new.push_back(indices[i]);
                indices_new.push_back(indices[i+1]);
                indices_new.push_back(indices[i+2]);
            }
        }
        indices=indices_new;
        fix();
        connectorFaceIdxs.clear();
        connectorFaceIdxs_sup.clear();
        connectorFaceReady=false;
        applyed=false;applyModelMatrix();
    }
}

void ModelManager::moveVertice_ori(int idx, float x,float y, float z){
    putVertice_ori(idx, getVertice_ori(idx)+QVector3D(x,y,z));
}
