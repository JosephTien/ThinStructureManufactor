#include "modelmanager.h"
#include "pch.h"

ModelManager::ModelManager()
{
    Reset();
}
QMatrix4x4 ModelManager::GetScaleMatrix() {
    scaleMatrix.setToIdentity();
    scaleMatrix.scale(scalex,scaley,scalez);
    return scaleMatrix;
}
void ModelManager::normalize(float val){
    float transx,transy,transz;
    float max=vertices_ori[0],min=vertices_ori[0];
    for(int i=0;i<(int)vertices_ori.size();i+=3){
        if (max < vertices_ori[i]){
            max = vertices_ori[i];
        }
        if (min > vertices_ori[i]){
            min = vertices_ori[i];
        }
        transx += vertices_ori[i]/(vertices_ori.size()/3);
    }
    float mul = val / (max - min);

    max=min=vertices_ori[1];
    for(int i=1;i<(int)vertices_ori.size();i+=3){
        if (max < vertices_ori[i]){
            max = vertices_ori[i];
        }
        if (min > vertices_ori[i]){
            min = vertices_ori[i];
        }
        transy += vertices_ori[i]/(vertices_ori.size()/3);
    }
    mul = val / (max - min) < mul ? val / (max - min) : mul;

    max=min=vertices_ori[2];
    for(int i=2;i<(int)vertices_ori.size();i+=3){
        if (max < vertices_ori[i]){
            max = vertices_ori[i];
        }
        if (min > vertices_ori[i]){
            min = vertices_ori[i];
        }
        transz += vertices_ori[i]/(vertices_ori.size()/3);
    }
    mul = val / (max - min) < mul ? val / (max - min) : mul;

    SetScale(mul,mul,mul);
    transx*=mul;transy*=mul;transz*=mul;
    //translate(QVector3D(-transx,-transy,-transz));
    //printf("%f/%f/%f",-transx,-transy,-transz);std::cout<<std::endl;
    applyed=false;applyModelMatrix();
}

void ModelManager::genRandomColor(){
    colors.clear();
    for(int i=0;i<(int)vertices_ori.size();i++){
        colors.push_back(rand() / (RAND_MAX + 1.0));
    }
}

void ModelManager::setColors(float r,float g,float b){
    colors.clear();
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
    }
    saveColors();
}

void ModelManager::paintCurvures(){
    float min,max;
//    max=min=curvures[0];
//    for(int i=1;i<(int)curvures.size();i++){
//        max = max>=curvures[i]?max:curvures[i];
//        min = min<=curvures[i]?min:curvures[i];
//    }
    std::vector<float> curvures_cpy;
    curvures_cpy = curvures;
    std::sort(curvures_cpy.begin(), curvures_cpy.end());
    int num = (int)curvures_cpy.size();
    min = curvures_cpy[num/100]; max = curvures_cpy[(num*99+99)/100];
    colors.clear();
    for(int i=0;i<(int)curvures.size();i++){
        float c = (curvures[i]-min)/(max-min);
        if(c<0)c=0;
        if(c>1)c=1;
        colors.push_back(c);
        colors.push_back(c);
        colors.push_back(1-c);
    }
}
void ModelManager::paintSelecIdxs(){
    int ring = 1;
    applyModelMatrix();
    //saveColors();
    for(int j=0;j<(int)selecIdxs.size();j++){
        std::vector<int> pl = nRingNeighbor(selecIdxs[j],ring);
        for(int i=0;i<(int)pl.size();i++){
            colors[pl[i]*3]= 1.0f;
            colors[pl[i]*3+1]= 0.0f;
            colors[pl[i]*3+2]= 0.0f;
        }
    }
}

void ModelManager::paintDetour(){
    int ring = 1;
    if(neighbor.size()==0)calNeighbor();
    loadColors();
    for(int i=0;i<(int)detourIdxs.size();i++){
        int curIdx = detourIdxs[i];
        std::vector<int> ne = nRingNeighbor(curIdx,ring);
        for(int j=0;j<(int)ne.size();j++){
            colors[ne[j]*3]=1.0f;
            colors[ne[j]*3+1]=1.0f;
            colors[ne[j]*3+2]=1.0f;
        }
    }
}

void ModelManager::SetScale(float x, float y, float z){
    applyed = false;
    scalex = x;
    scaley = y;
    scalez = z;
}

void ModelManager::SetRotation(QVector3D from,QVector3D to){
    applyed = false;
    from.normalize();
    to.normalize();
    double angle = acos(dotProduct(from, to));
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate(qRadiansToDegrees(angle), QVector3D::crossProduct(from, to));
}

void ModelManager::rotateX(float angle){
    applyed = false;
    rotationMatrix.rotate(angle, rotateYAxis);
}

void ModelManager::rotateY(float angle){
    applyed = false;
    rotationMatrix.rotate(angle, rotateXAxis);
}
void ModelManager::rotate(QMatrix4x4 rot){
    rotationMatrix = rot * rotationMatrix;
}

void ModelManager::rotateTo(QVector3D vec){
    applyed = false;
    vec.normalize();
    float dot = dotProduct(QVector3D(0,0,1),vec);
    QVector3D axis = QVector3D::crossProduct(QVector3D(0,0,1),vec);
    if(dot>0.95f||dot<-0.95)axis = QVector3D(vec.x(), vec.z(), vec.y());
    rotationMatrix.rotate( qRadiansToDegrees(acos(dot)), axis);
}
void ModelManager::rotateTo_u(QVector3D vec){
    applyed = false;
    vec.normalize();float dot = dotProduct(QVector3D(0,1,0),vec);
    QVector3D axis = QVector3D::crossProduct(QVector3D(0,1,0),vec);
    if(dot>0.95f||dot<-0.95)axis = QVector3D(vec.x(), vec.z(), vec.y());
    rotationMatrix.rotate( qRadiansToDegrees(acos(dot)), axis);
}
void ModelManager::rotateFrom(QVector3D vec){//unrevise
    applyed = false;
    vec.normalize();
    rotationMatrix.rotate( -qRadiansToDegrees(acos(dotProduct(QVector3D(0,0,1),vec))), QVector3D::crossProduct(QVector3D(0,0,1),vec));
}
void ModelManager::rotateFrom_u(QVector3D vec){//unrevise
    applyed = false;
    vec.normalize();
    rotationMatrix.rotate( -qRadiansToDegrees(acos(dotProduct(QVector3D(0,1,0),vec))), QVector3D::crossProduct(QVector3D(0,1,0),vec));
}
void ModelManager::translate(QVector3D movement){
    applyed = false;
    translationMatrix.translate(movement*viewRotationMatrix);
}

void ModelManager::translate_pure(QVector3D movement){
    applyed = false;
    translationMatrix.translate(movement);
}

void ModelManager::setRotationAxis(){
    QMatrix4x4 invrtRot = (viewRotationMatrix*GetModelMatrix()).inverted();
    rotateYAxis = (invrtRot * QVector4D(0, 1, 0, 0)).toVector3D();
    rotateXAxis = (invrtRot * QVector4D(1, 0, 0, 0)).toVector3D();
}

void ModelManager::setViewRotation(QMatrix4x4 m){
    viewRotationMatrix = m;
}

void ModelManager::ResetView(){
    viewRotationMatrix.setToIdentity();
}

void ModelManager::Reset(){
    ResetView();
    ResetModel();
    clearSupportData();
    ResetConnector();
    ResetMainData();
}
void ModelManager::ResetMainData(){
    normals.clear();
    vertices.clear();
    vertices_ori.clear();
    colors_ori.clear();
    colors.clear();
    indices.clear();
}
void ModelManager::ResetConnector(){
    connectorFaceIdxs.clear();
    connectorFaceIdxs_sup.clear();
    connectorReady = false;
    connectorFaceReady = false;
    connectReverse = false;
//    connectorNormal_ori;
//    connectorCenter_ori;
//    connectorRadii_ori;
//    cuttingPlane.exist=false;
}

void ModelManager::ResetModel(){
    applyed = false;
    scaleMatrix.setToIdentity();
    rotationMatrix.setToIdentity();
    translationMatrix.setToIdentity();
    scalex=scaley=scalez=1;
    rotateYAxis=QVector3D(0, 1, 0);
    rotateXAxis=QVector3D(1, 0, 0);
}

void ModelManager::applyModelMatrix_force(){
    vertices.resize(vertices_ori.size());
    QMatrix4x4 mat = GetModelMatrix();
    for(int i=0;i<(int)vertices_ori.size();i+=3){
        QVector4D vert = {vertices_ori[i], vertices_ori[i+1], vertices_ori[i+2], 1};
        vert = mat * vert;
        vertices[i] = vert.x()/vert.w();
        vertices[i+1] = vert.y()/vert.w();
        vertices[i+2] = vert.z()/vert.w();
    }
    applyed = true;
}

void ModelManager::applyModelMatrix(){
    if(applyed)return;
    vertices.resize(vertices_ori.size());
    QMatrix4x4 mat = GetModelMatrix();
    for(int i=0;i<(int)vertices_ori.size();i+=3){
        QVector4D vert = {vertices_ori[i], vertices_ori[i+1], vertices_ori[i+2], 1};
        vert = mat * vert;
        vertices[i] = vert.x()/vert.w();
        vertices[i+1] = vert.y()/vert.w();
        vertices[i+2] = vert.z()/vert.w();
    }
    applyed = true;
}

void ModelManager::renewByMatrix(){
    applyModelMatrix_force();
    vertices_ori.resize(vertices.size());
    for(int i=0;i<(int)vertices.size();i++){
        vertices_ori[i] = vertices[i];
    }
    scaleMatrix.setToIdentity();
    translationMatrix.setToIdentity();
    rotationMatrix.setToIdentity();
    scalex = scaley = scalez = 1;
}

void ModelManager::regenNormals(){
    vecq3d verts;
    vecq3d norms;
    for(int i=0;i<(int)vertices_ori.size();i+=3){
        verts.push_back(QVector3D(vertices_ori[i],vertices_ori[i+1],vertices_ori[i+2]));
    }
    norms.resize(verts.size(), QVector3D(0,0,0));
    for (int i = 0; i<(int) indices.size(); i+=3)
    {
        int ia = (int)indices[i];
        int ib = (int)indices[i+1];
        int ic = (int)indices[i+2];
        QVector3D normal = QVector3D::crossProduct((verts[ib] - verts[ia]),(verts[ic] - verts[ia])).normalized();
        norms[ia] += normal;
        norms[ib] += normal;
        norms[ic] += normal;
    }
    for (int i = 0; i<(int) norms.size(); i++){
        norms[i].normalize();
    }
    normals.clear();
    for (int i = 0; i<(int) norms.size(); i++){
        normals.push_back(norms[i].x());
        normals.push_back(norms[i].y());
        normals.push_back(norms[i].z());
    }
}

void ModelManager::scaleDepend_ori(QVector3D c, float valx, float valy, float valz){//scale vertices_ori depend on a certain point
    for(int i=0;i<vertices_ori.size()/3;i++){
        QVector3D v = (getVertice_ori(i)-c);
        v = QVector3D(v.x()*valx,v.y()*valy,v.z()*valz);
        v+=c;
        putVertice_ori(i,v);
    }
}
void ModelManager::putColor(unsigned int idx,QVector3D c){
    colors[idx*3]=c.x();
    colors[idx*3+1]=c.y();
    colors[idx*3+2]=c.z();
}

void ModelManager::putVertice_ori(unsigned int idx,QVector3D v){
    vertices_ori[idx*3]=v.x();
    vertices_ori[idx*3+1]=v.y();
    vertices_ori[idx*3+2]=v.z();
}
unsigned int ModelManager::pushVertice_ori(QVector3D v){
    vertices_ori.push_back(v.x());vertices_ori.push_back(v.y());vertices_ori.push_back(v.z());
    return vertices_ori.size()/3-1;
}
void ModelManager::pushColor(QVector3D c){
    colors.push_back(c.x());colors.push_back(c.y());colors.push_back(c.z());
}
void ModelManager::pushNormal(QVector3D n){
    normals.push_back(n.x());normals.push_back(n.y());normals.push_back(n.z());
}
void ModelManager::pushIndice(unsigned int a, unsigned int b, unsigned int c){
    indices.push_back(a);indices.push_back(b);indices.push_back(c);
}
QVector3D ModelManager::getVertice_ori(int idx){
    return QVector3D(vertices_ori[idx*3],vertices_ori[idx*3+1],vertices_ori[idx*3+2]);

}
vecq3d ModelManager::getIndicesVertice_ori(int idx){
    vecq3d o;
    o.push_back(getVertice_ori(indices[idx*3]));
    o.push_back(getVertice_ori(indices[idx*3+1]));
    o.push_back(getVertice_ori(indices[idx*3+2]));
    return o;

}

QVector3D ModelManager::getCenter_ori(){
    QVector3D c(0,0,0);
    for(int i=0;i<vertices_ori.size()/3;i++){
        c+=getVertice_ori(i);
    }
    c/=vertices_ori.size()/3;
    return c;
}

QVector3D ModelManager::getMassCenter_ori(){
    QVector3D c(0,0,0);
    float sum=0;
    for(int i=0;i<indices.size()/3;i++){
        vecq3d s=getIndicesVertice_ori(i);
        float area = QVector3D::crossProduct(s[1]-s[0],s[2]-s[0]).length();
        c += area * (s[0]+s[1]+s[2])/3;
        sum+=area;
    }
    c/=sum;
    return c;
}

QVector3D ModelManager::getVertice(int idx){
    return QVector3D(vertices[idx*3],vertices[idx*3+1],vertices[idx*3+2]);

}

QVector3D ModelManager::getColor(int idx){
    return QVector3D(colors[idx*3],colors[idx*3+1],colors[idx*3+2]);

}

QVector3D ModelManager::getNormal_ori(int idx){
    return QVector3D(normals[idx*3],normals[idx*3+1],normals[idx*3+2]);

}
vecq3d ModelManager::getSelecPointsByIdxs_ori(){
    vecq3d sp;
    for(int i=0;i<(int)selecIdxs.size();i++){
        sp.push_back(getVertice_ori(selecIdxs[i]));
    }
    return sp;
}
vecq3d ModelManager::getSelecPointsByIdxs(){
    vecq3d sp;
    for(int i=0;i<(int)selecIdxs.size();i++){
        sp.push_back(getVertice(selecIdxs[i]));
    }
    return sp;
}

void ModelManager::fitAllSelecIdxs(){
    applyModelMatrix();
    selecIdxs.clear();
    for(int j=0;j<(int)selecPoints.size();j++){
        QVector3D tarPoint = selecPoints[j];
        float minDis=9999.0f;int minIdx=0;
        for(int i=0;i<(int)vertices.size();i+=3){
            float curDis = (tarPoint-QVector3D(vertices[i],vertices[i+1],vertices[i+2])).length();
            if(curDis<minDis){
                minDis=curDis;
                minIdx=i/3;
            }
        }
        selecIdxs.push_back(minIdx);
    }
}
void ModelManager::fitSelecIdxs(int tar){
    QVector3D tarPoint = selecPoints[tar];
    float minDis=9999.0f;int minIdx=0;
    for(int i=0;i<(int)vertices.size();i+=3){
        float curDis = (tarPoint-QVector3D(vertices[i],vertices[i+1],vertices[i+2])).length();
        if(curDis<minDis){
            minDis=curDis;
            minIdx=i/3;
        }
    }
    selecIdxs[tar] = minIdx;
}
void ModelManager::clearSupportData(){
    selecIdxs.clear();
    selecPoints.clear();
    detourIdxs.clear();
    detourIdxs_all.clear();
    edges.clear();
    curvures.clear();
    neighbor.clear();
}
void ModelManager::refresh(){
    clearSupportData();
    //ResetConnector();
    regenNormals();
    setColors(0.5f,0.5f,0.5f);
    applyModelMatrix_force();
}

void ModelManager::refresh_with_normalize(float scale){
    selecIdxs.clear();
    selecPoints.clear();
    detourIdxs.clear();
    curvures.clear();
    neighbor.clear();
    regenNormals();
    setColors(0.5f,0.5f,0.5f);
    normalize(scale);
    applyModelMatrix_force();

}
void ModelManager::fix(){ // clear uncovered point
    std::vector<bool>  vertices_covered;
    vertices_covered.resize(vertices_ori.size()/3,false);
    for (int i = 0; i<(int) indices.size(); i++)
    {
        vertices_covered[indices[i]]=true;
    }
    std::vector<float> vertices_new;
    std::vector<float> normals_new;
    std::vector<float> colors_new;
    std::vector<int> vertices_index;
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        if(vertices_covered[i]){
            vertices_new.push_back(vertices_ori[i*3]);
            vertices_new.push_back(vertices_ori[i*3+1]);
            vertices_new.push_back(vertices_ori[i*3+2]);
            normals_new.push_back(normals[i*3]);
            normals_new.push_back(normals[i*3+1]);
            normals_new.push_back(normals[i*3+2]);
            colors_new.push_back(colors[i*3]);
            colors_new.push_back(colors[i*3+1]);
            colors_new.push_back(colors[i*3+2]);
            vertices_index.push_back((int)vertices_new.size()/3-1);
        }else{
            vertices_index.push_back(0xffffffff);
        }
    }
    for (int i = 0; i<(int) indices.size(); i++)
    {
        indices[i] = vertices_index[indices[i]];
    }
    for (int i = 0; i<(int) detourIdxs.size(); i++)
    {
        detourIdxs[i] = vertices_index[detourIdxs[i]];
    }
    for (int i = 0; i<(int) selecIdxs.size(); i++)
    {
        selecIdxs[i] = vertices_index[selecIdxs[i]];
    }
    vertices.clear();vertices_ori.clear();normals.clear();colors.clear();
    for(int i=0;i<(int)vertices_new.size();i++){
        vertices.push_back(vertices_new[i]);
        vertices_ori.push_back(vertices_new[i]);
        normals.push_back(normals_new[i]);
        colors.push_back(colors_new[i]);
    }
    applyed=false;applyModelMatrix();
    neighbor.clear();
}
QVector3D ModelManager::minYVertice_ori(){
    int idx;
    int min = FLT_MAX;
    for(int i=0;i<vertices_ori.size()/3;i++){
        if(vertices_ori[i*3+1]<min){
            min = vertices_ori[i*3+1];
            idx = i;
        }
    }
    return getVertice_ori(idx);
}
QVector3D ModelManager::minZVertice_ori(){
    int idx;
    int min = FLT_MAX;
    for(int i=0;i<vertices_ori.size()/3;i++){
        if(vertices_ori[i*3+2]<min){
            min = vertices_ori[i*3+2];
            idx = i;
        }
    }
    return getVertice_ori(idx);
}
QVector3D ModelManager::averageVertice_ori(){
    QVector3D average;
    float areaSum;
    for(int i=0;i<indices.size()/3;i++){
        Triangle tri = Triangle(getVertice_ori(indices[i*3]),getVertice_ori(indices[i*3+1]),getVertice_ori(indices[i*3+2]));
        average+=tri.cent()*tri.area();
        areaSum+=tri.area();
    }
    average/=areaSum;
    return average;
}
float ModelManager::minY(){
    int idx;
    int min = FLT_MAX;
    for(int i=0;i<vertices.size()/3;i++){
        if(vertices[i*3+1]<min){
            min = vertices[i*3+1];
            idx = i;
        }
    }
    return min;
}

//maintainable mesh
//to maintain update apply
//color

//must no hole
//normal (make finer can be ok, but how to correct regen)
//modelmatrix of selectpoint
//calneighbor applyed maintain
