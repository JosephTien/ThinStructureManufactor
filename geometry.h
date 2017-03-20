#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "pch.h"
#include <math.h>
#define minThre ((float)(1.0e-4))
class printUtil{
public :
    static void print(QVector3D v){
        std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
    }
};
class Line{
public:
    Line(QVector3D v1, QVector3D v2){
        this->v1 = v1;
        this->v2 = v2;
    }
    QVector3D v1;
    QVector3D v2;
    void rotateBackToZPlane(QVector3D c, QVector3D n){
        Line t = _rotateBackToZPlane(c, n);
        this->v1 = t.v1;
        this->v2 = t.v2;
    }

    Line _rotateBackToZPlane(QVector3D c, QVector3D n){
        QVector3D v1 = this->v1;
        QVector3D v2 = this->v2;
        v1-=c;
        v2-=c;
        QMatrix4x4 rotationMat;
        n.normalize();
        rotationMat.rotate( acosf(QVector3D::dotProduct(n,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(n,QVector3D(0,0,1)));
        v1= rotationMat * v1;
        v2= rotationMat * v2;
        return Line(v1, v2);
    }

    bool intersecWithCircle(QVector3D c, QVector3D n, float r){
        Line l = _rotateBackToZPlane(c ,n);
        QVector2D v1 = QVector2D(l.v1.x(), l.v1.y());
        QVector2D v2 = QVector2D(l.v2.x(), l.v2.y());
        QVector2D v1v2 = v2-v1;
        QVector2D v1c = -v1;
        if(v2.length()<=r)return true;
        if(v1.length()<=r)return true;
        float dot = QVector2D::dotProduct(v1v2.normalized(), v1c);
        QVector2D is = dot/v1v2.length()*v1v2.normalized() + v1;
        float rate = (is-v1).length()/(v2-v1).length();
        if(is.length()<=r && QVector2D::dotProduct(v1v2, (is-v1))>=0&&rate<=1) return true;
        return false;
    }

};

class Triangle{
public:
    Triangle(QVector3D v1, QVector3D v2, QVector3D v3){
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;

    float disTo(QVector3D v){
        return ((v1+v2+v3)/3 - v).length();
    }

    bool intersecByRay(QVector3D q, QVector3D v){
        QVector3D p1 = v1;
        QVector3D p2 = v2;
        QVector3D p3 = v3;
        QVector3D n  = QVector3D::normal((p2-p1),(p3-p1));
        QVector3D r  = (p1-q);

        if(QVector3D::dotProduct(v,n)==0)return false;
        float t = QVector3D::dotProduct(r,n)/QVector3D::dotProduct(v,n);
        if(t<0)return false;
        QVector3D c = q + t*v;
        c = c - QVector3D::dotProduct((c-p1), n.normalized()) * n.normalized();

        float d1 = QVector3D::dotProduct(QVector3D::crossProduct(p1-c,p2-c),n);
        float d2 = QVector3D::dotProduct(QVector3D::crossProduct(p2-c,p3-c),n);
        float d3 = QVector3D::dotProduct(QVector3D::crossProduct(p3-c,p1-c),n);
        if((d1>=0&&d2>=0&&d3>=0)||(d1<=0&&d2<=0&&d3<=0)){
            return true;
        }
        return false;
    }

    float area(){
        return QVector3D::crossProduct(v2-v1, v3-v1).length();
    }

    QVector3D norm(){
        return QVector3D::crossProduct(v2-v1, v3-v1).normalized();
    }

    QVector3D cent(){
        return (v1+v2+v3)/3;
    }

    void rotateBackToZPlane(QVector3D c, QVector3D n){
        Triangle tri = _rotateBackToZPlane(c, n);
        tri.v1 = tri.v1;
        tri.v2 = tri.v2;
        tri.v3 = tri.v3;
    }

    Triangle _rotateBackToZPlane(QVector3D c, QVector3D n){
        QVector3D v1 = this->v1;
        QVector3D v2 = this->v2;
        QVector3D v3 = this->v3;
        v1-=c;
        v2-=c;
        v3-=c;
        QMatrix4x4 rotationMat;
        n.normalize();
        rotationMat.rotate( acosf(QVector3D::dotProduct(n,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(n,QVector3D(0,0,1)));
        v1=rotationMat*v1;
        v2=rotationMat*v2;
        v3=rotationMat*v3;
        return Triangle(v1, v2, v3);
    }

    float area2D(QVector2D v1, QVector2D v2, QVector2D v3){
        float a = (v1-v2).length();
        float b = (v2-v3).length();
        float c = (v3-v1).length();
        float s = (a+b+c)/2;
        return sqrt(s * (s-a) * (s-b) * (s-c));
    }

    bool intersecWithCircle(QVector3D c, QVector3D n, float r);

};

class Column{
public:
    QVector3D c;
    QVector3D n;
    float r;
    float l;
    Column(QVector3D c,QVector3D n, float r, float l){
        this->c = c;
        this->n = n;
        this->r = r;
        this->l = l;
    }
    bool pointIn(QVector3D v){
        //QVector3D vp = QVector3D::dotProduct((v-c),n)/n.length()*(v-c).normalized();
        QVector3D vp = QVector3D::dotProduct((v-c),n)/n.length()*n.normalized() + c;
        if((vp-c).length()<=l && (vp-v).length()<=r){
            return true;
        }
        return false;
    }

    bool faceIn(QVector3D v1, QVector3D v2, QVector3D v3);

};
class Arg{
public:
    float trans;
    float rot;
    float tile;
    float rate;
    float radii;
    float tuneRadii;
    QVector3D c;
    QVector3D cfix;
    QVector3D n;
    QVector3D n2;
    QVector3D nplate;
    QVector3D npillar;

    /*
    Arg(QVector3D c, QVector3D n1, QVector3D n2){
        this->c = c;
        this->n1 = n1;
        this->n2 = n2;
    }
    */

    Arg(QVector3D c, QVector3D n, QVector3D n2, float radii, float trans, float rot, float tile, float rate)
    {
        this->c = c;
        this->n = n;
        this->n2 = n2;
        this->radii = radii;
        this->trans = trans;
        this->rot = rot;
        this->tile = tile;
        this->rate = rate;
        setTuneArg();
    }
    void setRadii(float radii){
        this->radii = radii;
        tuneRadii = radii * 0.9f * rate;
        cfix = tan(tile/180* M_PI) * tuneRadii * nplate.normalized();
        if(tile<0)cfix*=-1;
    }

    void setTuneArg(){
        if(trans>=0){
            c += trans*n.normalized();
        }
        else{
            trans = -trans;
            c += trans*n2.normalized();

        }
        QVector3D axis = QVector3D::crossProduct(n, n2);;
        QMatrix4x4 mat;
        mat.rotate(rot, axis);
        nplate = mat * n;
        mat.rotate(tile, axis);
        npillar  = mat * n;
        //radii = calCutArea(c,nplate);
        tuneRadii = radii * 0.9f * rate;
        cfix = tan(tile/180* M_PI) * tuneRadii * nplate.normalized();
    }
};

class Pair{
public:
    QVector3D a;
    QVector3D b;
    Pair(QVector3D a,QVector3D b){
        this->a = a;
        this->b = b;
    }
    float distance(){
        return (a-b).length();
    }
    QVector3D mid(){
        return (a+b)/2;
    }
    bool projIntsec(QVector3D c, Pair ano){
        QVector3D ca = (a-c).normalized();
        QVector3D cb = (b-c).normalized();
        QVector3D canoa = (ano.a-c).normalized();
        QVector3D canob = (ano.b-c).normalized();
        if(QVector3D::dotProduct(QVector3D::crossProduct(ca, canoa), QVector3D::crossProduct(canoa, cb))>0)
            if(QVector3D::dotProduct(ca+cb,canoa)>0)return true;
        if(QVector3D::dotProduct(QVector3D::crossProduct(ca, canob), QVector3D::crossProduct(canob, cb))>0)
            if(QVector3D::dotProduct(ca+cb,canob)>0)return true;
        if(QVector3D::dotProduct(QVector3D::crossProduct(canoa, ca), QVector3D::crossProduct(ca, canob))>0)
            if(QVector3D::dotProduct(canoa+canob,ca)>0)return true;
        if(QVector3D::dotProduct(QVector3D::crossProduct(canoa, cb), QVector3D::crossProduct(cb, canob))>0)
            if(QVector3D::dotProduct(canoa+canob,cb)>0)return true;
        return false;
    }
    bool isLinked(Pair ano){
        if((a-ano.a).length()<minThre)return true;
        else if((b-ano.a).length()<minThre)return true;
        else if((a-ano.b).length()<minThre)return true;
        else if((b-ano.b).length()<minThre)return true;
        return false;
    }
    QVector3D proj(QVector3D v){
        QVector3D abn = (b - a).normalized();
        QVector3D av   = (v - a);
        //QVector3D v = pairs[i].mid();
        v = QVector3D::dotProduct(abn, av) * abn + a;
        return v;
    }
    float length(){
        return (a-b).length();

    }
};

class Edge{
public:
    Edge(unsigned int ia, unsigned int ib){
        this->ia = ia;
        this->ib = ib;
        if(this->ia>this->ib)swap(this->ia,this->ib);
    }
    bool operator < (const Edge& edge) const
    {
        if(this->ia == edge.ia){
            return this->ib < edge.ib;
        }
        return this->ia < edge.ia;
    }
    bool operator == (const Edge& edge) const
    {
        return (this->ia == edge.ia && this->ib == edge.ib);
    }
    bool operator > (const Edge& edge) const
    {
        if(this->ia == edge.ia){
            return this->ib > edge.ib;
        }
        return this->ia > edge.ia;
    }
    unsigned int mid;
private:
    void swap(unsigned int &a, unsigned int &b){
        unsigned int t;
        t=a;
        a=b;
        b=t;
    }
    unsigned int ia, ib;
};

class Plane{
public:
    QVector3D normal;
    QVector3D center;
    float radii;//option
    bool exist=false;
    Plane(){
        exist=false;
    }
    Plane(QVector3D normal, QVector3D center){
        this->normal=normal;
        this->center=center;
        exist=true;
    }
    Plane(QVector3D a, QVector3D b, QVector3D c){
        QVector3D center = (a+b+c)/3;
        QVector3D normal = QVector3D::crossProduct(a-center,b-center).normalized();
        this->normal=normal;
        this->center=center;
        exist=true;
    }
    bool isExist(){
        return exist;
    }
    void setRadii(float radii){
        this->radii = radii;
    }
    float distanceToPoint(QVector3D v){
        QVector3D vc = center-v;
        return -(vc.x()*normal.x()+vc.y()*normal.y()+vc.z()*normal.z());
    }
    bool isCrossBy(QVector3D v1, QVector3D v2){
        QVector3D c = center;
        QVector3D n = normal;
        QVector3D v1c = (c-v1);
        QVector3D v2c = (c-v2);
        float d1 = v1c.x()*n.x()+v1c.y()*n.y()+v1c.z()*n.z();
        float d2 = v2c.x()*n.x()+v2c.y()*n.y()+v2c.z()*n.z();

        if(std::fabs(d1) < minThre){
            return false;
        }else if(std::fabs(d2) < minThre){
            return false;
        }else if(d1*d2<0){
            return true;
        }
        return false;
    }
};

#endif // GEOMETRY_H
