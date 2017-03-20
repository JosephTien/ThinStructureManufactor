#include "geometry.h"

bool Column::faceIn(QVector3D v1, QVector3D v2, QVector3D v3){
    Triangle tri = Triangle(v1, v2, v3)._rotateBackToZPlane(c, n);
    QVector3D norm = tri.norm();
    QVector3D cent = tri.cent();
    float a = norm.x();
    float b = norm.y();
    float c = norm.z();
    float d = -(a*cent.x() + b*cent.y() + c * cent.z());
    float e =  - c * c ;
    float f = - 2 * c * d;
    float g = a*a*r*r+b*b*r*r-d*d;
    if(f*f-4*e*g<0){
        return false;
    }

    if(e != 0){//be careful of float error
        //return false;//super special case, assume not happen
        float lower = (-f-sqrt(f*f-4*e*g))/(2*e);
        float upper = (-f+sqrt(f*f-4*e*g))/(2*e);
        if(lower>upper){float temp = upper;upper = lower; lower = temp;}
        //std::cout << "lower : " << lower <<  " ; upper : " << upper << " ; inter : " <<  std::endl;
        if(lower<0 && upper < 0) return false;
        if(lower>l && upper > l) return false;
    }
    float minv = std::min(std::min(tri.v1.z(), tri.v2.z()), tri.v3.z());
    float maxv = std::max(std::max(tri.v1.z(), tri.v2.z()), tri.v3.z());
    if(minv<0 && maxv < 0) return false;
    if(minv>l && maxv > l) return false;
    return Triangle(v1, v2, v3).intersecWithCircle(this->c, n, r);
}

bool Triangle::intersecWithCircle(QVector3D c, QVector3D n, float r){

    Triangle tr = _rotateBackToZPlane(c, n);
    //std::cout << n.x() << " " << n.y() << " " << n.z() << std::endl;
    //std::cout << tr.v1.x() << " " << tr.v1.y() << " " << tr.v1.z() << std::endl;
    //std::cout << tr.v2.x() << " " << tr.v2.y() << " " << tr.v2.z() << std::endl;
    //std::cout << tr.v3.x() << " " << tr.v3.y() << " " << tr.v3.z() << std::endl;
    //std::cout << areaAll << " " << area1 + area2 + area3 << std::endl;

    /**************************************************/
    Line ls[3] = {Line(v1,v2),Line(v2,v3),Line(v3,v1)};
    bool istrue = false;
    for(int i=0;i<3;i++){
        if(ls[i].intersecWithCircle(c, n, r))return true;
    }
    Triangle tri = _rotateBackToZPlane(c, n);
    QVector2D v1 = QVector2D(tri.v1.x(), tri.v1.y());
    QVector2D v2 = QVector2D(tri.v2.x(), tri.v2.y());
    QVector2D v3 = QVector2D(tri.v3.x(), tri.v3.y());
    /*
    float v1angle = acosf(v1.normalized().x())/M_PI*180;if(v1.y()<0)v1angle = 360-v1angle;
    float v2angle = acosf(v2.normalized().x())/M_PI*180;if(v2.y()<0)v2angle = 360-v2angle;
    float v3angle = acosf(v3.normalized().x())/M_PI*180;if(v3.y()<0)v3angle = 360-v3angle;
    */
    float a1 = acosf(QVector2D::dotProduct(v1.normalized(),v2.normalized()))/M_PI*180;
    float a2 = acosf(QVector2D::dotProduct(v2.normalized(),v3.normalized()))/M_PI*180;
    float a3 = acosf(QVector2D::dotProduct(v3.normalized(),v1.normalized()))/M_PI*180;
    if(a1+a2+a3<359)return false;
    return true;
    /*
    float areaAll = area2D(v1, v2, v3);
    float area1 = area2D(QVector2D(0,0), v2, v3);
    float area2 = area2D(v1, QVector2D(0,0), v3);
    float area3 = area2D(v1, v2, QVector2D(0,0));
    std::cout << (area1 + area2 + area3) - areaAll << " " << std::endl;
    if((areaAll+minThre)<area1 + area2 + area3)return false;
    return true;
    */

}



