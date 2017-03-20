#include "modelviewer.h"

using namespace std;

ModelViewer::ModelViewer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    viewMgr = ViewManager::GetInstance();
    memset(vis,0,sizeof(int)*tarlimit);
    memset(vis_ass,0,sizeof(int)*tarlimit);
}

int ModelViewer::getTarnum(){
    return tarnum;
}
int ModelViewer::getTarnum_ass(){
    return tarnum_ass;
}

void ModelViewer::deleteTar(int tar){
    makeCurrent();
    tarnum--;
    for(int i=tar;i<tarnum;i++){
        viewMgr->modelMgr[i] = viewMgr->modelMgr[i+1];
        vis[i]=vis[i+1];
        reBuffer(i);
    }
    vertexAttribute[tarnum].Destroy();
    viewMgr->modelMgr[tarnum].Reset();

    update();
}
void ModelViewer::deleteTar_ass(int tar){
    makeCurrent();
    vertexAttribute_ass[tar].Destroy();
    tarnum_ass--;
    for(int i=tar;i<tarnum_ass;i++){
        vertexAttribute_ass[i] = vertexAttribute_ass[i+1];
        vis_ass[i]=vis_ass[i+1];
    }
}

void ModelViewer::setAllVis(int val){
    for(int i=0;i<tarnum;i++){
        vis[i]=val;
    }
}

void ModelViewer::setColor_allrandom(){
    for(int i=0;i<tarnum;i++){
        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        setColor(i,r1, r2, r3);
    }
}

void ModelViewer::setColor_mySet(){
    for(int i=0;i<tarnum;i++){
        float r1 = i%2 == 0?0.5f:1;
        float r2 = i/2%2 == 0?0.5f:1;
        float r3 = i/4%2 == 0?0.5f:1;

        setColor(i,r1, r2, r3);
    }
}

void ModelViewer::setColor(int tar, float r,float g,float b){
    makeCurrent();
    colors.clear();
    for(int i=0;i<(int)viewMgr->modelMgr[tar].vertices_ori.size();i+=3){colors.push_back(r);colors.push_back(g);colors.push_back(b);}
    viewMgr->modelMgr[tar].colors = colors;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::setColor(int tar, std::vector<float> colors){//no error size handle
    makeCurrent();
    viewMgr->modelMgr[tar].colors = colors;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);

}
void ModelViewer::setColor_ass(int tar, float r,float g,float b){
    makeCurrent();
    colors.clear();
    for(int i=0;i<(int)viewMgr->modelMgr_ass[tar].vertices_ori.size();i+=3){colors.push_back(r);colors.push_back(g);colors.push_back(b);}
    viewMgr->modelMgr_ass[tar].colors = colors;
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}
void ModelViewer::reBuffer(int tar){
    makeCurrent();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::reBuffer_ass(int tar){
    makeCurrent();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}

void ModelViewer::reBuffer(){
    makeCurrent();
    for(int tar=0;tar<tarnum;tar++){
        vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    }
}

void ModelViewer::reBuffer_ass(){
    makeCurrent();
    for(int tar=0;tar<tarnum_ass;tar++){
        vector<float> vertices = viewMgr->modelMgr_ass[tar].vertices_ori;
        vector<float> normals = viewMgr->modelMgr_ass[tar].normals;
        vector<unsigned int> indices = viewMgr->modelMgr_ass[tar].indices;
        vector<float> colors = viewMgr->modelMgr_ass[tar].colors;
        vector<float> empty;
        vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    }
    update();
}

void ModelViewer::load_rc(QFile * qfile){
    makeCurrent();
    loadObj_rc(qfile, positions, normals, indices);
    colors.clear();
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum++;
    int tar = tarnum-1;
    viewMgr->modelMgr[tar].Reset();
    viewMgr->modelMgr[tar].vertices = positions;
    viewMgr->modelMgr[tar].colors = colors;
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].normals = normals;
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].saveColors();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::load_rc_ass(QFile * qfile){
    makeCurrent();
    colors.clear();
    loadObj_rc(qfile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum_ass++;
    int tar = tarnum_ass-1;
    viewMgr->modelMgr_ass[tar].Reset();
    viewMgr->modelMgr_ass[tar].vertices = positions;
    viewMgr->modelMgr_ass[tar].colors = colors;
    viewMgr->modelMgr_ass[tar].vertices_ori = positions;
    viewMgr->modelMgr_ass[tar].normals = normals;
    viewMgr->modelMgr_ass[tar].indices = indices;
    viewMgr->modelMgr_ass[tar].saveColors();
    vertexAttribute_ass[tar].Create();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}

void ModelViewer::loadPillar(){
    makeCurrent();
    colors.clear();
    int tarIdx_ass = 2;
    while(tarnum_ass<=tarIdx_ass)tarnum_ass++;
    QFile stdObjFile(":/object/cylinder10X10.obj");
    loadObj_rc(&stdObjFile, positions, normals, indices);
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(1.0f);colors.push_back(0.5f);}
    int tar = tarIdx_ass;
    viewMgr->modelMgr_ass[tar].Reset();
    viewMgr->modelMgr_ass[tar].vertices = positions;
    viewMgr->modelMgr_ass[tar].colors = colors;
    viewMgr->modelMgr_ass[tar].vertices_ori = positions;
    viewMgr->modelMgr_ass[tar].normals = normals;
    viewMgr->modelMgr_ass[tar].indices = indices;
    viewMgr->modelMgr_ass[tar].saveColors();
    vertexAttribute_ass[tar].Create();
    vertexAttribute_ass[tar].BufferData(viewMgr->modelMgr_ass[tar].vertices_ori, viewMgr->modelMgr_ass[tar].normals, empty, viewMgr->modelMgr_ass[tar].colors, viewMgr->modelMgr_ass[tar].indices);
    update();
}


void ModelViewer::load(const char* filename){
    makeCurrent();
    vector<float> empty;
    loadObj(filename, positions, normals, indices);
    colors.clear();
    for(int i=0;i<(int)positions.size();i+=3){colors.push_back(0.5f);colors.push_back(0.5f);colors.push_back(0.5f);}
    tarnum++;
    int tar = tarnum-1;
    viewMgr->modelMgr[tar].Reset();
    viewMgr->modelMgr[tar].vertices = positions;
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].normals = normals;
    viewMgr->modelMgr[tar].regenNormals();
    viewMgr->modelMgr[tar].colors = colors;
    viewMgr->modelMgr[tar].saveColors();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}
void ModelViewer::reload(const char* filename,int tar){
    makeCurrent();
    if(tar>=tarnum)return;
    loadObj(filename, positions, normals, indices);
    viewMgr->modelMgr[tar].vertices_ori = positions;
    viewMgr->modelMgr[tar].applyModelMatrix_force();
    viewMgr->modelMgr[tar].indices = indices;
    viewMgr->modelMgr[tar].regenNormals();
    viewMgr->modelMgr[tar].setColors(0.5f,0.5f,0.5f);
    viewMgr->modelMgr[tar].saveColors();

    vertexAttribute[tar].Destroy();
    vertexAttribute[tar].Create();
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
    update();
}

void ModelViewer::copyObj(int tar){
    makeCurrent();
    vertexAttribute[tarnum++].Create();
    viewMgr->modelMgr[tarnum-1] = viewMgr->modelMgr[tar];
    tar = tarnum-1;
    vertexAttribute[tar].BufferData(viewMgr->modelMgr[tar].vertices_ori, viewMgr->modelMgr[tar].normals, empty, viewMgr->modelMgr[tar].colors, viewMgr->modelMgr[tar].indices);
}

void ModelViewer::exchangeObj(int tar1, int tar2){
    makeCurrent();
    copyObj(tar1);
    viewMgr->modelMgr[tar1] = viewMgr->modelMgr[tar2];
    vertexAttribute[tar1].BufferData(viewMgr->modelMgr[tar2].vertices_ori, viewMgr->modelMgr[tar2].normals, empty, viewMgr->modelMgr[tar2].colors, viewMgr->modelMgr[tar2].indices);
    viewMgr->modelMgr[tar2] = viewMgr->modelMgr[tarnum-1];
    vertexAttribute[tar2].BufferData(viewMgr->modelMgr[tarnum-1].vertices_ori, viewMgr->modelMgr[tarnum-1].normals, empty, viewMgr->modelMgr[tarnum-1].colors, viewMgr->modelMgr[tarnum-1].indices);
    deleteTar(tarnum-1);
}

int ModelViewer::getPlaneParaBySelec(int tarObj, QVector3D &c, float &r){
    if(viewMgr->modelMgr[tarObj].selecPoints.size()==3){
        QVector3D v1=viewMgr->modelMgr[tarObj].selecPoints[0];
        QVector3D v2=viewMgr->modelMgr[tarObj].selecPoints[1];
        QVector3D v3=viewMgr->modelMgr[tarObj].selecPoints[2];
        QVector3D center=(v1+v2+v3)/3;
        float radii = ((v1-center).length()+(v2-center).length()+(v3-center).length())/3*2;
        c = center;
        r = radii;
        return 0;
    }
    return -1;
}



void ModelViewer::assignModel(){
    for(int i=0;i<tarnum;i++){
        viewMgr->modelMgr[i].renewByMatrix();
        QVector3D m = viewMgr->modelMgr[i].minZVertice_ori();
        QVector3D a = viewMgr->modelMgr[i].averageVertice_ori();
        viewMgr->modelMgr[i].translate_pure(-QVector3D(a.x()-200+40*i,a.y(),m.z()));
        reBuffer(i);
        viewMgr->modelMgr[i].applyModelMatrix_force();
    }
}



void ModelViewer::initializeGL()
{

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    standardShader.Create();
    //generateTest();

}

void ModelViewer::paintGL()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //standardShader.SetRenderMode(RENDER_COLORED);
    standardShader.SetRenderMode(RENDER_PHONG_COLORED);
    for(int i=0;i<tarnum;i++){
        standardShader.SetLightDirection(QVector3D(0,0,1)*viewMgr->GetViewRotationMatrix());
        standardShader.SetModelMatrix(viewMgr->GetModelMatrix(i));
        standardShader.SetMVPMatrix(viewMgr->GetModelViewProjectionMatrix(viewportAspect, i));
        if(vis[i]){
            standardShader.Draw(GL_TRIANGLES, vertexAttribute[i]);
        }
    }
    for(int i=0;i<tarnum_ass;i++){
        standardShader.SetLightDirection(QVector3D(0,0,1)*viewMgr->GetViewRotationMatrix());
        standardShader.SetModelMatrix(viewMgr->GetModelMatrix_ass(i));
        standardShader.SetMVPMatrix(viewMgr->GetModelViewProjectionMatrix_ass(viewportAspect, i));
        if(vis_ass[i]){
            standardShader.Draw(GL_TRIANGLES, vertexAttribute_ass[i]);
        }
    }
}

void ModelViewer::resizeGL(int w, int h)
{
    dpr = QPaintDevice::devicePixelRatio();

    viewportSize = QSize(w*dpr, h*dpr);
    viewportAspect = (float)w / (float)h;
}


QVector3D ModelViewer::rayIntersec(Ray3 ray,std::vector<float> verts, std::vector<unsigned int> indices, int &resultnum){
    std::vector<QVector3D> vertices;
    for(int i=0;i<(int)verts.size();i+=3)vertices.push_back(QVector3D(verts[i],verts[i+1],verts[i+2]));
    std::vector<QVector3D> candidateVert;
    std::vector<unsigned int> candidateIdx;
    for(int i=0;i<(int)indices.size();i+=3){
        QVector3D p1 = vertices[indices[i]];
        QVector3D p2 = vertices[indices[i+1]];
        QVector3D p3 = vertices[indices[i+2]];
        QVector3D n  = QVector3D::normal((p2-p1),(p3-p1));
        QVector3D q  = ray.origin;
        QVector3D v  = ray.direction;
        QVector3D r  = (p1-q);

        if(QVector3D::dotProduct(v,n)==0)continue;
        float t = QVector3D::dotProduct(r,n)/QVector3D::dotProduct(v,n);
        if(t<0)continue;
        QVector3D c = q + t*v;
        c = c - QVector3D::dotProduct((c-p1), n.normalized()) * n.normalized();

        float d1 = QVector3D::dotProduct(QVector3D::crossProduct(p1-c,p2-c),n);
        float d2 = QVector3D::dotProduct(QVector3D::crossProduct(p2-c,p3-c),n);
        float d3 = QVector3D::dotProduct(QVector3D::crossProduct(p3-c,p1-c),n);
        if((d1>=0&&d2>=0&&d3>=0)||(d1<=0&&d2<=0&&d3<=0)){
            candidateVert.push_back(c);
            candidateIdx.push_back(i/3);
        }
    }
    resultnum = (int)candidateVert.size();
    QVector3D bestCandidateVert;
    unsigned int bestCandidateIdx;
    float bestCandidateDepth = FLT_MAX;
    for(int i=0;i<(int)candidateVert.size();i++){
        float dis = (ray.origin-candidateVert[i]).length();
        if(dis<bestCandidateDepth){
            bestCandidateDepth = dis;
            bestCandidateVert = candidateVert[i];
            bestCandidateIdx = candidateIdx[i];
        }
    }
    return bestCandidateVert;
}

void ModelViewer::mousePressEvent(QMouseEvent *event)
{
    if(selectMode){
        if(viewMgr->getControlType()==0){
            winw=this->geometry().width();
            winh=this->geometry().height();
            float planeh = (viewMgr->eyelookdis-viewMgr->clipdis)*2*tan(viewMgr->perspangle*viewMgr->getZoom()/2 * M_PI / 180.0 );
            float planew = planeh * winw / winh;
            Ray3 ray;
            ray.origin.setX( (event->x()-winw/2)        *planew/winw );
            ray.origin.setY( (-(event->y()-winh/2))     *planeh/winh );
            ray.origin.setZ( viewMgr->eyeLookPosition.z() + viewMgr->clipdis );
            ray.direction = ray.origin - viewMgr->eyePosition;
            QMatrix4x4 mat = viewMgr->GetViewTranslationMatrix().inverted() * viewMgr->GetViewRotationMatrix().inverted();
            ray.origin = mat * ray.origin;
            mat = viewMgr->GetViewRotationMatrix().inverted();
            ray.direction = mat * ray.direction;

            viewMgr->modelMgr[tarObj].applyModelMatrix();
            int internum;
            QVector3D ri = rayIntersec(ray, viewMgr->modelMgr[tarObj].vertices, viewMgr->modelMgr[tarObj].indices, internum);
            if(internum>0){
                for(int i=(int)viewMgr->modelMgr[tarObj].selecPoints.size();i<=curSelecTar;i++){
                    viewMgr->modelMgr[tarObj].selecPoints.push_back(QVector3D(viewMgr->modelMgr[tarObj].vertices[0],viewMgr->modelMgr[tarObj].vertices[1],viewMgr->modelMgr[tarObj].vertices[2]));
                    viewMgr->modelMgr[tarObj].selecIdxs.push_back(0);
                }
                viewMgr->modelMgr[tarObj].selecPoints[curSelecTar] = ri;
                viewMgr->modelMgr[tarObj].fitSelecIdxs(curSelecTar);
                viewMgr->modelMgr[tarObj].loadColors();
                viewMgr->modelMgr[tarObj].paintSelecIdxs();
                reBuffer(tarObj);
                //viewMgr->modelMgr[tarObj].loadColors();
                generateAssitDisc(tarObj);
            }
        }
    }else{
        viewMgr->mousePressEvent(event);
    }
}
void ModelViewer::mouseReleaseEvent(QMouseEvent *event)
{
    viewMgr->mouseReleaseEvent(event);

    update();
}

void ModelViewer::mouseMoveEvent(QMouseEvent *event)
{
    viewMgr->mouseMoveEvent(event);
    update();
}

void ModelViewer::wheelEvent(QWheelEvent *event)
{
    viewMgr->wheelEvent(event);
    update();
}
void ModelViewer::keyReleaseEvent(QKeyEvent *){//QKeyEvent *event
    viewMgr->setFixAxis(QVector2D(1,1));
    this->setFocus();
}
void ModelViewer::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Z){
         viewMgr->setFixAxis(QVector2D(1,0));
    }else if(event->key() == Qt::Key_X){
        viewMgr->setFixAxis(QVector2D(0,1));
    }else if(event->key() == Qt::Key_Q){
         viewMgr->ResetView();
    }else if(event->key() == Qt::Key_W){
        viewMgr->ResetModel();
        //viewMgr->modelMgr[0].normalize(50.0f);
    }else if(event->key() == Qt::Key_Left){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Right){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Up){
        viewMgr->keyPressEvent(event);
    }else if(event->key() == Qt::Key_Down){
        viewMgr->keyPressEvent(event);
    }
    update();
}
