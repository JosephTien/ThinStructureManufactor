#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "pch.h"
#include "viewmanager.h"
#include "standarshader.h"
#include "standarshader.h"

class Ray3{
    public:
        QVector3D origin;
        QVector3D direction;
        Ray3(){}

        Ray3(QVector3D origin, QVector3D direction){//QVector3D origin
            this->origin = origin;
            this->direction = direction;
        }
};

class ModelViewer : public QOpenGLWidget
{
private:
    static const int tarlimit = 100;
    int tarnum = 0;
    int tarnum_ass = 0;
    int curtar = 0;
    int vis[tarlimit];
    int vis_ass[tarlimit];
    bool selectMode = false;

public:
    ModelViewer(QWidget* parent = 0);
    ViewManager *viewMgr;
    StandardVertexAttribute vertexAttribute[tarlimit];
    StandardVertexAttribute vertexAttribute_ass[tarlimit];
    void load(const char* filename);
    void reload(const char* filename, int tar);
    void load_rc(QFile * qfile);
    void load_rc_ass(QFile * qfile);
    void setColor_allrandom();
    void setColor_mySet();
    void setColor(int tar, float r,float g,float b);
    void setColor(int tar, std::vector<float> colors);
    void setColor_ass(int tar, float r,float g,float b);
    void reBuffer(int tar);
    void reBuffer();
    void reBuffer_ass(int tar);
    void reBuffer_ass();
    void deleteTar(int tar);
    void deleteTar_ass(int tar);
    int getTarnum();
    int getTarnum_ass();
    void setAllVis(int val);
    inline void setVis(int tar,int val){vis[tar]=val;}
    inline void setVis_ass(int tar,int val){vis_ass[tar]=val;}
    inline void setSelectMode(bool set){selectMode = set;}
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    struct para{
        float D=50;
        float H=50;
    }someparas;
    /********************/
    void loadPillar();
    void generateTest();
    void generateCube(QVector3D pup,QVector3D plow);
    void genTrack(QVector3D center, QVector3D platenorm, float radii, bool minerr);
    void genSpiralCav(QVector3D center, QVector3D platenorm, float radii,float hrate);
    void genSpiral(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float startAngle, float dec, float decu);
    void genSpiral_withhead(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float startAngle, float dec, float decu, float headAngle);
    void generateDisc(QVector3D center, QVector3D platenorm, float radii, int tarIdx_ass);
    void generateAssitDisc(int tarObj);
    void generateAssitDisc(int tarObj, QVector3D &c, float &r);
    void generateAssitDisc(QVector3D center, QVector3D platnorm, float radii);
    void generateAssitDisc(QVector3D center, QVector3D platnorm, float radii, int tar);
    void generateAssitPillar(QVector3D c, QVector3D n, float r, float l);
    void genSlotCav(QVector3D c,QVector3D dir, QVector3D up);
    /********************/
    int curSelecTar = 0;
    int assistMode = 0;
    int tarObj=0;
    void copyObj(int tar);
    void exchangeObj(int tar1, int tar2);
    void assignModel();
    int getPlaneParaBySelec(int tarObj, QVector3D &c, float &r);

protected:

    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    /********************/
    void loadObj(const char* filename, std::vector<float> &verts, std::vector<float> &norms, std::vector<unsigned int> &facs);
    void loadObjByTiny(const char* filename, std::vector<float> &verts, std::vector<float> &norms, std::vector<unsigned int> &facs);
    void loadObj_rc(QFile * qfile, std::vector<float> &verts, std::vector<float> &norms, std::vector<unsigned int> &facs);
    /********************/
    QVector3D rayIntersec(Ray3 ray,std::vector<float> vertices, std::vector<unsigned int> indices, int &resultnum);
    float triarea(float a, float b,float c){
        float s = (a+b+c)/2;
        return sqrt(s*(s-a)*(s-b)*(s-c));
    }

    StandardShader standardShader;
    //ViewManager *viewMgr;
    QSize viewportSize;
    float viewportAspect;
    int dpr;

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> colors;
    std::vector<unsigned int> indices;
    std::vector<float> empty;
    int winw = 400;
    int winh = 300;
};


#endif // MODELVIEWER_H
