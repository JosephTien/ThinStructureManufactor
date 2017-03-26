#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include "pch.h"
#include "looplist.h"
#include "geometry.h"
//#define minThre ((float)(1.0e-4))

typedef std::vector<QVector3D> vecq3d;

class ModelManager
{
public:
    ModelManager();
    /**valiable**/
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<float> colors_ori;
    std::vector<float> vertices_ori;
    std::vector<float> curvures;
    std::vector<std::vector<unsigned int>> neighbor;
    std::vector<unsigned int> indices;

    vecq3d selecPoints;
    std::vector<int> selecIdxs;
    std::vector<int> detourIdxs;
    std::vector<int> detourIdxs_all;//include vertices inside
    std::vector<unsigned int> connectorFaceIdxs;
    std::vector<unsigned int> connectorFaceIdxs_sup;
    std::set<Edge> edges;
    bool connectorReady = false;
    bool connectorFaceReady = false;
    QVector3D connectorNormal_ori;
    QVector3D connectorCenter_ori;
    float connectorRadii_ori;
    float connectorRadii2_ori;
    int detourSPIdx;//start point
    int detourCPIdx;//center point
    Plane cuttingPlane;
    bool connectReverse = false;

    /*basic management function*/
    void SetScale(float x, float y, float z);
    void normalize(float val);
    void genRandomColor();
    void paintCurvures();
    void SetScale(QVector3D xyz){applyed = false;scalex = xyz.x();scaley = xyz.y();scalez = xyz.z();}
    void SetRotation(QVector3D from, QVector3D to);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotate(QMatrix4x4 rot);
    void rotateTo(QVector3D vec);//target : (0,0,1)
    void rotateTo_u(QVector3D vec);//target : (0,1,0)
    void rotateFrom(QVector3D vec);//target : (0,0,1)
    void rotateFrom_u(QVector3D vec);//target : (0,1,0)
    void setViewRotation(QMatrix4x4 m);
    void translate(QVector3D movement);//depend on view
    void translate_pure(QVector3D movement);
    void setRotationAxis();
    void calNeighbor();
    void calCurvures(int rings);
    QVector3D GetScaleXYZ(){QVector3D xyz = {scalex,scaley,scalez};return xyz;}
    QMatrix4x4 GetScaleMatrix();
    QMatrix4x4 GetModelMatrix() {return translationMatrix * rotationMatrix * GetScaleMatrix();}
    void Reset();
    void ResetModel();
    void ResetView();
    void applyModelMatrix_force();
    void applyModelMatrix();
    void renewByMatrix();//assign new into ori
    void regenNormals();
    void scaleDepend_ori(QVector3D c, float valx, float valy, float valz);
    void fix();// clear uncovered point (not cover by indices)
    void setColors(float r,float g,float b);
    void saveColors(){colors_ori=colors;}
    void loadColors(){if(colors_ori.size()>0)colors=colors_ori;}
    void paintSelecIdxs();
    void paintDetour();
    unsigned int pushVertice_ori(QVector3D v);
    void putVertice_ori(unsigned int idx,QVector3D v);
    void putColor(unsigned int idx,QVector3D c);
    void pushColor(QVector3D c);
    void pushNormal(QVector3D n);
    void pushIndice(unsigned int a, unsigned int b, unsigned int c);
    vecq3d getIndicesVertice_ori(int idx);
    QVector3D getCenter_ori();
    QVector3D getMassCenter_ori();
    QVector3D getVertice_ori(int idx);
    QVector3D getVertice(int idx);
    QVector3D getColor(int idx);
    QVector3D getNormal_ori(int idx);//ori
    void refresh_with_normalize(float scale);
    void refresh();
    void clearSupportData();
    void ResetConnector();
    void ResetMainData();
    vecq3d getSelecPointsByIdxs();
    vecq3d getSelecPointsByIdxs_ori();
    void clearConnector();
    float minY();
    QVector3D minYVertice_ori();
    QVector3D minZVertice_ori();
    QVector3D averageVertice_ori();//get precision center by triangle area

    /*advanced calculation function*/
    std::vector<int> nRingNeighbor(int root, int rings);
    void fitAllSelecIdxs();//get nearest idx from selected position
    void fitSelecIdxs(int tar);
    int calDetourByPlane();
    int calDetourByPlane(QVector3D c,QVector3D n);
    float calCutArea();//by detour
    float calCutArea(QVector3D &c,QVector3D &n, float &radii, bool autofix, float &angleCal);//by custom plane
    void cutByDetour(int state);
    void regenByPlateIntersec();//generate new tri by intersec
    bool checkDetour(); // check if a loop detour
    void pullConnect(float val);
    void pushConnect(float val);
    void circleOnPlane(QVector3D c, QVector3D n, float radii, int div, int cmp);//draw a circle on plane（gen ctrl pt only）
    void circleOnPlane(QVector3D c, QVector3D n, float radii, int div);//cmp=0
    void linkContour(Plane mainPlane, std::vector<int> contourIdx);//used by circleOnPlane(), link the control point and gen the sup
    void produceFace(); //bad implement
    void extractPiece(int state);
    std::vector<int> bfs(std::vector<bool> &visited, int rootIdx);
    /*data support*/
    QVector3D detourNormal();
    QVector3D selecPointsNormal();
    QVector3D detourNormal_ori();
    QVector3D selecPointsNormal_ori();
    QVector3D selecPointsCenter();
    QVector3D selecPointsCenter_ori();
    QVector3D detourCenter();
    QVector3D detourCenter_ori();
    QVector3D detourNormal_ori(std::vector<int> detourIdxs_this);
    QVector3D detourCenter_ori(std::vector<int> detourIdxs_this);
    void moveVertice_ori(int idx, float x, float y, float z);
    void reverseDetours();
    void sortDetour(); // check the direction
    bool isCrossPlane(QVector3D v1, QVector3D v2, QVector3D s1, QVector3D s2, QVector3D s3);
    float pointDistanceToPlane(QVector3D v, QVector3D s1, QVector3D s2, QVector3D s3);
    float dotProduct(QVector3D a,QVector3D b);
    bool checkColumnBound(QVector3D c,QVector3D n, float r, float l);//check if a column is cross the model

//    /*advanced calculation function, unused code backup*/
//void fillByDetour(); //bad impelement. using cgal instead
//    int calDetourByBfs();
//    void gendetourPlane(QVector3D c);
//    void gendetourPlane();
//    void gendetourPlane_simple(QVector3D c);
//    void gendetourPlane_patch();
//    void gendetourPlane_convex();
//    void gendetourPlane_convex(std::vector<int> detourIdxs_this);
//    void cutByDetour_reverse();
private:
    QMatrix4x4 scaleMatrix;
    QMatrix4x4 translationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 viewRotationMatrix;
    QVector3D rotateXAxis;
    QVector3D rotateYAxis;
    float scalex, scaley, scalez;
    bool applyed = true;
    /* unused
    std::vector<std::vector<int>> detourIdxsQueue;
    int detourIdxsQueue_flag = 0;
    std::vector<std::vector<int>> detourIdxsList;
    vecq3d detourCenterList;
    */
};
#endif // MODELMANAGER_H
