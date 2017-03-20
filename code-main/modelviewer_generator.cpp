#include <modelviewer.h>
using namespace std;
void ModelViewer::generateCube(QVector3D pup,QVector3D plow){
    makeCurrent();
    float rec[] = {-1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,1,-1,1,-1,1,1,1,1,1};
    float pos[24];
    float e[] = {0};
    for(int i=0;i<24;i+=3){
        pos[i] = rec[i] > 0 ? rec[i]=pup.x():rec[i]=plow.x();
        pos[i+1] = rec[i+1] > 0 ? rec[i+1]=pup.y():rec[i+1]=plow.y();
        pos[i+2] = rec[i+2] > 0 ? rec[i+2]=pup.z():rec[i+2]=plow.z();
    }
    vector<float> empty;
    vector<float> normals(rec,rec+24);
    vector<float> positions(pos,pos+24);
    vector<float> colors = {0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f};
    vector<unsigned int> indices = {0,2,1,2,3,1,3,7,1,7,5,1,5,4,1,4,0,1,0,4,2,4,6,2,6,7,2,7,3,2,5,7,4,7,6,4};
    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);

    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].colors = colors;
}

void ModelViewer::generateTest(){
    makeCurrent();
    float rec[] = {-1,-1,-1,1,-1,-1,-1,1,-1,1,1,-1,-1,-1,1,1,-1,1,-1,1,1,1,1,1};
    for(int i=0;i<24;i++){rec[i]*=25;}
    vector<float> empty;
    vector<float> positions(rec, rec + 24);
    vector<float> normals(rec, rec + 24);
    vector<float> colors = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    vector<unsigned int> indices = {0,1,2,2,1,3,3,1,7,7,1,5,5,1,4,4,1,0,0,2,4,4,2,6,6,2,7,7,2,3,5,4,7,7,4,6};

    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);

    for(int i=0;i<24;i++){positions[i]-=25;}
    for(int i=0;i<24;i++){colors[i]/=2;}
    vertexAttribute[tarnum++].Create();
    vertexAttribute[tarnum-1].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum-1,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    tarnum++;
    viewMgr->modelMgr[tarnum-1].Reset();
    viewMgr->modelMgr[tarnum-1].vertices = positions;
    viewMgr->modelMgr[tarnum-1].indices = indices;
    viewMgr->modelMgr[tarnum-1].vertices_ori = positions;
    viewMgr->modelMgr[tarnum-1].normals = normals;
    viewMgr->modelMgr[tarnum-1].colors = colors;
}
void ModelViewer::generateAssitPillar(QVector3D c, QVector3D n, float r, float l){
    loadPillar();
    int tarIdx_ass=2;
    int tar =  tarIdx_ass;
    viewMgr->modelMgr_ass[tar].scaleDepend_ori(QVector3D(0,0,0),r/50,r/50,l/100);
    viewMgr->modelMgr_ass[tar].rotateTo(n);
    viewMgr->modelMgr_ass[tar].translate_pure(c);
    viewMgr->modelMgr_ass[tar].translate_pure(l/2*n.normalized());
    viewMgr->modelMgr_ass[tar].setColors(0.5f, 0.5f, 1.0f);
    viewMgr->modelMgr_ass[tar].applyModelMatrix_force();
    reBuffer_ass(tar);
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
    setVis_ass(tarIdx_ass, assistMode);
}


void ModelViewer::generateAssitDisc(int tarObj, QVector3D &c, float &r){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
        c = center;
        r = radii;
    }
}
void ModelViewer::generateAssitDisc(QVector3D center, QVector3D platnorm, float radii){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    generateDisc(center,platnorm,radii,tarIdx_ass);
    setVis_ass(tarIdx_ass, assistMode);
    viewMgr->modelMgr_ass[tarIdx_ass].setColors(0.5f, 1.0f, 0.5f);
    reBuffer_ass(tarIdx_ass);
}
void ModelViewer::generateAssitDisc(QVector3D center, QVector3D platnorm, float radii, int tar){
    int tarIdx_ass=tar;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    generateDisc(center,platnorm,radii,tarIdx_ass);
    setVis_ass(tarIdx_ass, assistMode);
    viewMgr->modelMgr_ass[tarIdx_ass].setColors(1.0f, 0.5f, 0.5f);
    reBuffer_ass(tarIdx_ass);
}

void ModelViewer::generateAssitDisc(int tarObj){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
    }
}
/*
void ModelViewer::generateAssitDisc(int tarObj){
    int tarIdx_ass=0;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        QVector3D platnorm = QVector3D::crossProduct(v1-v3,v2-v3);
        generateDisc(center,platnorm,radii,tarIdx_ass);
        setVis_ass(tarIdx_ass, assistMode);
    }
}
*/
//int DIV = 100;

void ModelViewer::genTrack(QVector3D center, QVector3D platenorm, float radii, bool minerr){

    float rate = radii / 1.0f;
    //std::cout << "rate : " << rate << std::endl;

    float h = radii/2.0f, r = radii/1.3f, r2 = radii/1.1f, l = r2*rate;
    if(minerr){
        r-=0.5f;
        r2-=0.5f;
    }
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;

    vertices.push_back(QVector3D(l, 0, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(0, r, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-l, 0, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(0, -r, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(l, 0, h)*rotationMat+center);
    vertices.push_back(QVector3D(0, r2, h)*rotationMat+center);
    vertices.push_back(QVector3D(-l, 0, h)*rotationMat+center);
    vertices.push_back(QVector3D(0, -r2, h)*rotationMat+center);

    indices.push_back(0);indices.push_back(2);indices.push_back(1);
    indices.push_back(0);indices.push_back(3);indices.push_back(2);
    indices.push_back(4);indices.push_back(5);indices.push_back(6);
    indices.push_back(4);indices.push_back(6);indices.push_back(7);

    indices.push_back(0);indices.push_back(1);indices.push_back(5);
    indices.push_back(1);indices.push_back(2);indices.push_back(6);
    indices.push_back(2);indices.push_back(3);indices.push_back(7);
    indices.push_back(3);indices.push_back(0);indices.push_back(4);

    indices.push_back(0);indices.push_back(5);indices.push_back(4);
    indices.push_back(1);indices.push_back(6);indices.push_back(5);
    indices.push_back(2);indices.push_back(7);indices.push_back(6);
    indices.push_back(3);indices.push_back(4);indices.push_back(7);

    /**********************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

void ModelViewer::genSpiralCav(QVector3D center, QVector3D platenorm, float radii,float hrate){

    float h = radii * hrate, r = radii;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;

    vertices.push_back(QVector3D(radii*2, -radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, -radii/4, -0.001f)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, -radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(radii*2, radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, radii/4, h*2)*rotationMat+center);
    vertices.push_back(QVector3D(-radii*2, -radii/4, h*2)*rotationMat+center);

    indices.push_back(0);indices.push_back(2);indices.push_back(1);
    indices.push_back(0);indices.push_back(3);indices.push_back(2);
    indices.push_back(4);indices.push_back(5);indices.push_back(6);
    indices.push_back(4);indices.push_back(6);indices.push_back(7);

    indices.push_back(0);indices.push_back(1);indices.push_back(5);
    indices.push_back(1);indices.push_back(2);indices.push_back(6);
    indices.push_back(2);indices.push_back(3);indices.push_back(7);
    indices.push_back(3);indices.push_back(0);indices.push_back(4);

    indices.push_back(0);indices.push_back(5);indices.push_back(4);
    indices.push_back(1);indices.push_back(6);indices.push_back(5);
    indices.push_back(2);indices.push_back(7);indices.push_back(6);
    indices.push_back(3);indices.push_back(4);indices.push_back(7);

    /**********************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

int cornNum = 4;
void ModelViewer::genSpiral_withhead(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float dec, float decu, float headAngle){

    float h = radii * hrate, r = radii;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;
    for(int i=0;i<=div;i++){
        //float rr = std::abs((float)(i-(div/2))/(div/2));
        float rr = (float)(i-(div))/div;
        r = radii * (1-((1-dec)* rr));
        float curangle = (angle / div * i) / 360 * 2 *M_PI;
        for(int j=0;j<cornNum;j++){
            vertices.push_back(QVector3D(r*cos(curangle+M_PI*2*j/cornNum), r*sin(curangle+M_PI*2*j/cornNum), h / div * i));
        }
        /*
        vertices.push_back(QVector3D(r*cos(curangle), r*sin(curangle), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2), r*sin(curangle+M_PI/2), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI), r*sin(curangle+M_PI), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2*3), r*sin(curangle+M_PI/2*3), h / div * i));
        */
    }
    for(int i=0;i<cornNum;i++){
        vertices[i]-=QVector3D(0,0,0.01f);
    }
    /*
    vertices[0]-=QVector3D(0,0,0.01f);
    vertices[1]-=QVector3D(0,0,0.01f);
    vertices[2]-=QVector3D(0,0,0.01f);
    vertices[3]-=QVector3D(0,0,0.01f);
    */
    float hp = (tan(M_PI/4 + (headAngle*M_PI/180)) * r * dec) + h;
    vertices.push_back(QVector3D(0,0,hp)*rotationMat+center);

    div+=1;
    for(int i=0;i<div;i++){
        for(int j=0;j<cornNum;j++){
            vertices[i*cornNum+j] = vertices[i*cornNum+j]*rotationMat+center;
        }
        /*
        vertices[i*cornum]   = vertices[i*4]*rotationMat+center;
        vertices[i*4+1] =  vertices[i*4+1]*rotationMat+center;
        vertices[i*4+2] =  vertices[i*4+2]*rotationMat+center;
        vertices[i*4+3] =  vertices[i*4+3]*rotationMat+center;
        */
    }

    for(int i=0;i<div-1;i++){
        for(int j=0;j<cornNum;j++){
            indices.push_back(i*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);
            indices.push_back((i+1)*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);
        }
        /*
        indices.push_back(i*4);indices.push_back(i*4+1);indices.push_back((i+1)*4);
        indices.push_back(i*4+1);indices.push_back(i*4+2);indices.push_back((i+1)*4+1);
        indices.push_back(i*4+2);indices.push_back(i*4+3);indices.push_back((i+1)*4+2);
        indices.push_back(i*4+3);indices.push_back(i*4);indices.push_back((i+1)*4+3);
        indices.push_back((i+1)*4+1);indices.push_back((i+1)*4);indices.push_back(i*4+1);
        indices.push_back((i+1)*4+2);indices.push_back((i+1)*4+1);indices.push_back(i*4+2);
        indices.push_back((i+1)*4+3);indices.push_back((i+1)*4+2);indices.push_back(i*4+3);
        indices.push_back((i+1)*4);indices.push_back((i+1)*4+3);indices.push_back(i*4);
        */
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(0);indices.push_back(i+2);indices.push_back(i+1);
    }
    for(int i=0;i<cornNum;i++){
        indices.push_back(div*cornNum-cornNum+i);indices.push_back(div*cornNum-cornNum+(1+i)%cornNum);indices.push_back(div*cornNum);
    }
    /*
    indices.push_back(0);indices.push_back(3);indices.push_back(1);
    indices.push_back(1);indices.push_back(3);indices.push_back(2);
    indices.push_back(div*4-4);indices.push_back(div*4-3);indices.push_back(div*4);
    indices.push_back(div*4-3);indices.push_back(div*4-2);indices.push_back(div*4);
    indices.push_back(div*4-2);indices.push_back(div*4-1);indices.push_back(div*4);
    indices.push_back(div*4-1);indices.push_back(div*4-4);indices.push_back(div*4);
    */
    /****************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}

void ModelViewer::genSpiral(QVector3D center, QVector3D platenorm, float radii,float hrate, int div, float angle, float dec, float decu){
    float innerFix = 0.25f;
    //float angle = 90;
    float h = radii * hrate, r = radii;
    //int div = DIV;
    platenorm.normalize();
    platenorm*=-1;
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));

    std::vector<QVector3D> vertices;
    std::vector<unsigned int> indices;
    vector<float> normals;
    for(int i=0;i<=div;i++){
        //float rr = std::abs((float)(i-(div/2))/(div/2));
        float rr = (float)(i-(div))/div;
        r = radii * (1-((1-dec)* rr)) - innerFix;
        float curangle = (angle / div * i) / 360 * 2 *M_PI;
        for(int j=0;j<cornNum;j++){
            vertices.push_back(QVector3D(r*cos(curangle+M_PI*2*j/cornNum), r*sin(curangle+M_PI*2*j/cornNum), h / div * i));
        }
        /*
        vertices.push_back(QVector3D(r*cos(curangle), r*sin(curangle), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2), r*sin(curangle+M_PI/2), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI), r*sin(curangle+M_PI), h / div * i));
        vertices.push_back(QVector3D(r*cos(curangle+M_PI/2*3), r*sin(curangle+M_PI/2*3), h / div * i));
        */
    }
    for(int i=0;i<cornNum;i++){
        vertices[i]-=QVector3D(0,0,0.01f);
    }
    /*
    vertices[0]-=QVector3D(0,0,0.01f);
    vertices[1]-=QVector3D(0,0,0.01f);
    vertices[2]-=QVector3D(0,0,0.01f);
    vertices[3]-=QVector3D(0,0,0.01f);
    */

    div+=1;
    for(int i=0;i<div;i++){
        for(int j=0;j<cornNum;j++){
            vertices[i*cornNum+j] = vertices[i*cornNum+j]*rotationMat+center;
        }
        /*
        vertices[i*4]   = vertices[i*4]*rotationMat+center;
        vertices[i*4+1] =  vertices[i*4+1]*rotationMat+center;
        vertices[i*4+2] =  vertices[i*4+2]*rotationMat+center;
        vertices[i*4+3] =  vertices[i*4+3]*rotationMat+center;
        */
    }
    for(int i=0;i<div-1;i++){
        for(int j=0;j<cornNum;j++){
            indices.push_back(i*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);
            indices.push_back((i+1)*cornNum+(1+j)%cornNum);indices.push_back((i+1)*cornNum+j);indices.push_back(i*cornNum+(1+j)%cornNum);
        }
        /*
        indices.push_back(i*4);indices.push_back(i*4+1);indices.push_back((i+1)*4);
        indices.push_back(i*4+1);indices.push_back(i*4+2);indices.push_back((i+1)*4+1);
        indices.push_back(i*4+2);indices.push_back(i*4+3);indices.push_back((i+1)*4+2);
        indices.push_back(i*4+3);indices.push_back(i*4);indices.push_back((i+1)*4+3);
        indices.push_back((i+1)*4+1);indices.push_back((i+1)*4);indices.push_back(i*4+1);
        indices.push_back((i+1)*4+2);indices.push_back((i+1)*4+1);indices.push_back(i*4+2);
        indices.push_back((i+1)*4+3);indices.push_back((i+1)*4+2);indices.push_back(i*4+3);
        indices.push_back((i+1)*4);indices.push_back((i+1)*4+3);indices.push_back(i*4);
        */
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(0);indices.push_back(i+2);indices.push_back(i+1);
    }
    for(int i=0;i<cornNum-2;i++){
        indices.push_back(div*cornNum-cornNum);indices.push_back(div*cornNum-cornNum+1+i);indices.push_back(div*cornNum-cornNum+2+i);
    }
    /*
    indices.push_back(0);indices.push_back(3);indices.push_back(1);
    indices.push_back(1);indices.push_back(3);indices.push_back(2);
    indices.push_back(div*4-4);indices.push_back(div*4-3);indices.push_back(div*4-1);
    indices.push_back(div*4-1);indices.push_back(div*4-3);indices.push_back(div*4-2);
    */
    /****************************************/
    vector<float> empty;
    vector<float> positions;
    vector<float> colors;
    QVector3D c;
    for(int i=0;i<vertices.size();i++){
        c+=vertices[i];
    }
    c/=vertices.size();
    for(int i=0;i<vertices.size();i++){
        positions.push_back(vertices[i].x());
        positions.push_back(vertices[i].y());
        positions.push_back(vertices[i].z());
        normals.push_back(vertices[i].x()-c.x());
        normals.push_back(vertices[i].y()-c.y());
        normals.push_back(vertices[i].z()-c.z());
        colors.push_back(0.5f);
        colors.push_back(0.5f);
        colors.push_back(1);
    }
    makeCurrent();
    vertexAttribute[tarnum].Destroy();
    vertexAttribute[tarnum].Create();
    vertexAttribute[tarnum].BufferData(positions, normals, empty, colors, indices);
    setVis(tarnum,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr[tarnum].Reset();
    viewMgr->modelMgr[tarnum].vertices = positions;
    viewMgr->modelMgr[tarnum].indices = indices;
    viewMgr->modelMgr[tarnum].vertices_ori = positions;
    viewMgr->modelMgr[tarnum].normals = normals;
    viewMgr->modelMgr[tarnum].colors = colors;
    tarnum++;
}
void ModelViewer::generateDisc(QVector3D center, QVector3D platenorm, float radii, int tarIdx_ass){
    makeCurrent();
    vector<float> empty;
    vector<float> positions;
    vector<float> normals;
    vector<float> colors;
    vector<unsigned int> indices;
    platenorm.normalize();
    QMatrix4x4 rotationMat;
    rotationMat.rotate( acosf(QVector3D::dotProduct(platenorm,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(platenorm,QVector3D(0,0,1)));
    positions.push_back(center.x());positions.push_back(center.y());positions.push_back(center.z());
    colors.push_back(0);colors.push_back(1);colors.push_back(0);
    int divnum=24;
    for(int i=0;i<divnum;i++){
        QVector3D curpoint = (QVector4D(radii*cos(2*M_PI*i/divnum), radii*sin(2*M_PI*i/divnum), 0 ,1)*rotationMat).toVector3DAffine();
        curpoint += center;
        positions.push_back(curpoint.x());positions.push_back(curpoint.y());positions.push_back(curpoint.z());
        colors.push_back(0);colors.push_back(1);colors.push_back(0);
        indices.push_back(0);indices.push_back(i+1);
        if(i+2<divnum+1)indices.push_back(i+2);else indices.push_back(1);
    }

    vertexAttribute_ass[tarIdx_ass].Destroy();
    vertexAttribute_ass[tarIdx_ass].Create();
    vertexAttribute_ass[tarIdx_ass].BufferData(positions, normals, empty, colors, indices);
    setVis_ass(tarIdx_ass,1);
    //vertexAttribute[curtar].Color3f(0.5f, 1.0f, 0.5f);
    update();
    viewMgr->modelMgr_ass[tarIdx_ass].Reset();
    viewMgr->modelMgr_ass[tarIdx_ass].vertices = positions;
    viewMgr->modelMgr_ass[tarIdx_ass].indices = indices;
    viewMgr->modelMgr_ass[tarIdx_ass].vertices_ori = positions;
    viewMgr->modelMgr_ass[tarIdx_ass].normals = normals;
    viewMgr->modelMgr_ass[tarIdx_ass].colors = colors;
}
