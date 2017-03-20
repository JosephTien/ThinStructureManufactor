#include "modelmanager.h"
#include "pch.h"

void print(std::string str){
    std::cout << str << std::endl;
}

bool ModelManager::checkDetour(){
    std::vector<std::set<unsigned int>> neighborset;
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        std::set<unsigned int> pushor;
        neighborset.push_back(pushor);
    }
    int vn = (int)vertices_ori.size()/3;
    bool * exsit;exsit = (bool*)malloc(sizeof(bool)*vn);memset(exsit,false,vn);
    for(int i=0;i<(int)detourIdxs.size();i++){
        exsit[detourIdxs[i]]=true;
    }
    //vecq3d s = getSelecPointsByIdxs();
    for(int i=0;i<(int)indices.size();i+=3){
        if(exsit[indices[i]] && exsit[indices[i+1]]){
            neighborset[indices[i]].insert(indices[i+1]);neighborset[indices[i+1]].insert(indices[i]);
        }
        if(exsit[indices[i+2]] && exsit[indices[i+1]]){
            neighborset[indices[i+1]].insert(indices[i+2]);neighborset[indices[i+2]].insert(indices[i+1]);
        }
        if(exsit[indices[i+2]] && exsit[indices[i]]){
            neighborset[indices[i]].insert(indices[i+2]);neighborset[indices[i+2]].insert(indices[i]);
        }
    }
    int begin = detourSPIdx;
    int cur = begin;
    std::vector<int> detourIdxs_old = detourIdxs;
    detourIdxs.clear();
    detourIdxs_all.clear();
    memset(exsit,false,vn);
    while(true){
        if(neighborset[cur].empty())break;
        int tar = *neighborset[cur].begin();
        neighborset[cur].erase(tar);
        neighborset[tar].erase(cur);
        detourIdxs.push_back(cur);
        detourIdxs_all.push_back(cur);
        exsit[cur]=true;
        cur = tar;
        if(cur == begin)break;
    }
    if(cur == begin){
        QVector3D c = cuttingPlane.center = detourCenter_ori();
        float maxLen = 0;
        for(int i=0;i<(int)detourIdxs.size();i++){
            maxLen = std::max(maxLen, (getVertice_ori(detourIdxs[i])-c).length());
        }
        for(int i=0;i<(int)detourIdxs_old.size();i++){
            if(!exsit[detourIdxs_old[i]]){
                if((getVertice_ori(detourIdxs_old[i])-c).length()<=maxLen){
                       detourIdxs_all.push_back(detourIdxs_old[i]);
                }
            }

        }
        return true;
    }
    return false;
}

void ModelManager::sortDetour(){
    QVector3D c = cuttingPlane.center;
    QVector3D nv(0,0,0);
    for(int i=0;i<(int)indices.size();i+=3){
        for(int j=0;j<3;j++){
            int vai = indices[i+j];
            int vbi = indices[i+(j+1)%3];
            QVector3D va = getVertice(vai);
            QVector3D vb = getVertice(vbi);
            if(std::fabs(cuttingPlane.distanceToPoint(va))<minThre && std::fabs(cuttingPlane.distanceToPoint(vb))<minThre){
                nv+=QVector3D::crossProduct(va-c,vb-c);
            }
        }
    }
    nv.normalize();
    QVector3D nd = detourNormal();
    if(dotProduct(nv,nd) > 0){
        reverseDetours();
    }
}

void ModelManager::extractPiece(int state){
    if(neighbor.size()==0)calNeighbor();
    std::vector<int> queue;

    int vn = (int)vertices_ori.size()/3;
    bool * visited;
    visited = (bool*)malloc(sizeof(bool)*vn);
    memset(visited,false,vn);

    int rootIdx = 0;
    queue.push_back(rootIdx);
    int flag = 0;
    while(flag < queue.size()){
        int cur = queue[flag];
        visited[cur]=true;
        for(int i=0;i<neighbor[cur].size();i++){
            int tar = neighbor[cur][i];
            if(!visited[tar]){
                queue.push_back(tar);
                visited[tar] = true;
            }
        }
        flag++;
    }
    std::vector<unsigned int> indices_new;
    for(int i=0;i<indices.size();i+=3){
        if(state>0){
            if(visited[indices[i]]||visited[indices[i+1]]||visited[indices[i+2]]){
                indices_new.push_back(indices[i]);
                indices_new.push_back(indices[i+1]);
                indices_new.push_back(indices[i+2]);
            }
        }
        else if(state<0){
            if(!visited[indices[i]]||!visited[indices[i+1]]||!visited[indices[i+2]]){
                indices_new.push_back(indices[i]);
                indices_new.push_back(indices[i+1]);
                indices_new.push_back(indices[i+2]);
            }
        }
    }
    indices = indices_new;
    fix();
}

void ModelManager::cutByDetour(int state){
    if(detourIdxs.size()==0)return;
    if(neighbor.size()==0)calNeighbor();
    int vn = (int)vertices_ori.size()/3;
    std::vector<int> queue;
    bool * visited;
    visited = (bool*)malloc(sizeof(bool)*vn);
    memset(visited,false,vn);
    for(int i=0;i<(int)detourIdxs_all.size();i++){
        visited[detourIdxs_all[i]]=true;
    }
    int rootIdx = -1;
    /*QVector3D c = detourCenter();
    QVector3D n = detourNormal();
    */
    /************************************************/
    QVector3D c = cuttingPlane.center;
    QVector3D n = cuttingPlane.normal;
    float minDis = FLT_MAX;
    /*
    for(int i=0;i<(int)vertices.size()/3;i++){
        QVector3D v = getVertice(i);
        float dis = (v-c).length();
        if(state>0){
            if(!visited[i] && dotProduct(v-c,n) < 0){
                if(dis<minDis){
                    rootIdx=i;
                    minDis = dis;
                }
            }
        }else if (state<0){
            if(!visited[i] && dotProduct(v-c,n) > 0){
                if(dis<minDis){
                    rootIdx=i;
                    minDis = dis;
                }
            }
        }
    }
    */
    if(state>0)n*=-1;
    for(int i=0;i<indices.size()/3;i++){
        Triangle tri = Triangle(getVertice_ori(indices[i*3]), getVertice_ori(indices[i*3+1]), getVertice_ori(indices[i*3+2]));
        if(tri.intersecByRay(c, n)){
            float dis = tri.disTo(c);
            if(dis<minDis){
                float maxDis = FLT_MIN;
                for(int j=0;j<3;j++){
                    float temp = (getVertice_ori(indices[i*3+j])-c).length();
                    if(temp>maxDis){
                        rootIdx = indices[i*3+j];
                        maxDis = temp;
                    }
                }
                rootIdx = indices[i*3];
                minDis = dis;
            }
        }
    }
    /************************************************/
    queue.clear();
    queue.push_back(rootIdx);
    visited[rootIdx]=true;
    int head = 0;
    while(head<(int)queue.size()){
        int qs = (int)queue.size();
        for(int k=head;k<qs;k++){
            int a = queue[k];
            for(int l=0;l<(int)neighbor[a].size();l++){
                int b = neighbor[a][l];
                if(!visited[b]){
                    visited[b] = true;
                    queue.push_back(b);
                }
            }
        }
        head = qs;
    }
    std::vector<unsigned int> indices_new;
    for(int i=0;i<(int)indices.size();i+=3){
        if(visited[indices[i]]&&visited[indices[i+1]]&&visited[indices[i+2]]){
            indices_new.push_back(indices[i]);
            indices_new.push_back(indices[i+1]);
            indices_new.push_back(indices[i+2]);
        }
    }
    indices = indices_new;
    fix();
    neighbor.clear();
    curvures.clear();
    sortDetour();
    //gendetourPlane();//gendetourPlane_convex();//gendetourPlane_patch();//fillByDetour();
    //regenNormals();
    connectorNormal_ori = detourNormal_ori();
    connectorCenter_ori = detourCenter_ori();
    connectorRadii_ori=FLT_MAX;
    for(int i=0;i<(int)detourIdxs.size();i++){
        connectorRadii_ori = std::min(connectorRadii_ori,(getVertice_ori(detourIdxs[i])-connectorCenter_ori).length());
    }
    connectorRadii2_ori=0;
    for(int i=0;i<(int)detourIdxs.size();i++){
        connectorRadii2_ori = std::max(connectorRadii2_ori,(getVertice_ori(detourIdxs[i])-connectorCenter_ori).length());
    }
    if(state<0)connectReverse = true;
    connectorReady = true;
    connectorFaceReady = false;
    setColors(0.5f,0.5f,0.5f);
    applyed=false;applyModelMatrix();
}

void ModelManager::calCurvures(int rings){
    applyModelMatrix();
    vecq3d verts;
    vecq3d norms;
    for(int i=0;i<(int)vertices.size();i+=3){
        verts.push_back(QVector3D(vertices[i],vertices[i+1],vertices[i+2]));
    }
    for(int i=0;i<(int)normals.size();i+=3){
        norms.push_back(QVector3D(normals[i],normals[i+1],normals[i+2]).normalized());
    }
    curvures.clear();
    int vn = (int)verts.size();
    for(int i=0;i<vn;i++){
        std::vector<int> queue;
        bool * visited;
        visited = (bool*)malloc(sizeof(bool)*vn);
        queue.clear();
        memset(visited,false,vn);
        queue.push_back(i);
        visited[i]=true;
        float cal = 0;
        int num = 0;
        int head = 0;
        for(int j=0;j<rings;j++){
            int qs = (int)queue.size();
            for(int k=head;k<qs;k++){
                int a = queue[k];
                for(int l=0;l<(int)neighbor[a].size();l++){
                    int b = neighbor[a][l];
                    if(!visited[b]){
                        cal += dotProduct(norms[a]-norms[b],verts[a]-verts[b])/(verts[a]-verts[b]).lengthSquared();
                        num++;
                        visited[b] = true;
                        queue.push_back(b);
                    }
                }
            }
            head = qs;
        }
        curvures.push_back(cal/num);
    }
}

void ModelManager::calNeighbor(){

    //fix();
    std::vector<std::set<unsigned int>> neighborset;
    for(int i=0;i<(int)vertices_ori.size()/3;i++){
        std::set<unsigned int> pushor;
        neighborset.push_back(pushor);
    }
    for (int i = 0; i<(int) indices.size(); i+=3)
    {
        unsigned int ia = indices[i];
        unsigned int ib = indices[i+1];
        unsigned int ic = indices[i+2];
        neighborset[ia].insert(ib);neighborset[ia].insert(ic);
        neighborset[ib].insert(ia);neighborset[ib].insert(ic);
        neighborset[ic].insert(ia);neighborset[ic].insert(ib);

    }
    neighbor.clear();
    for (int i = 0; i<(int) neighborset.size(); i++){
        std::vector<unsigned int> pushor;
        //for (std::set<int>::iterator itrt = neighborset[i].begin(); itrt != neighborset[i].end(); itrt++){
        for(auto itrt : neighborset[i]){
            pushor.push_back(itrt);
        }
        neighbor.push_back(pushor);
    }
}

std::vector<int> ModelManager::nRingNeighbor(int rootIdx, int rings){
    if(neighbor.size()==0)calNeighbor();
    int vn = (int)vertices_ori.size()/3;
    std::vector<int> queue;
    bool * visited;
    visited = (bool*)malloc(sizeof(bool)*vn);
    queue.clear();
    memset(visited,false,vn);
    queue.push_back(rootIdx);
    visited[rootIdx]=true;
    int head = 0;
    for(int j=0;j<rings;j++){
        int qs = (int)queue.size();
        for(int k=head;k<qs;k++){
            int a = queue[k];
            for(int l=0;l<(int)neighbor[a].size();l++){
                int b = neighbor[a][l];
                if(!visited[b]){
                    visited[b] = true;
                    queue.push_back(b);
                }
            }
        }
        head = qs;
    }
    return queue;
}



bool ModelManager::checkColumnBound(QVector3D c,QVector3D n, float r, float l){
/*
    Column clmn = Column(QVector3D(0,0,0), QVector3D(0,1,0),1,1);
    if(clmn.faceIn(QVector3D(0,0.5f,0), QVector3D(2,0.5f,0), QVector3D(0,0.5f,2)))print("    test : true");
    else print("    test : false");
    return true;
*/
    Column clmn = Column(c,n,r,l);
    int cnt = 0;
    //loadColors();
    for(int i=0;i<indices.size()/3;i++){
        std::vector<QVector3D> faces = getIndicesVertice_ori(i);
        Triangle tri = Triangle(faces[0], faces[1], faces[2]);
        if(tri.intersecWithCircle(c, n, r)){
            //colors[indices[i*3]*3+2] = colors[indices[i*3+1]*3+2] = colors[indices[i*3+2]*3+2] = 1;
        }
        if(clmn.faceIn(faces[0], faces[1], faces[2])){
            return true;
            //colors[indices[i*3]*3+2] = colors[indices[i*3+1]*3+2] = colors[indices[i*3+2]*3+2] = 1;
            //cnt++;
        }
    }\
    //std::cout << "inersec face : " << cnt << std::endl;
    return (cnt>0);


}

float ModelManager::calCutArea(QVector3D &c,QVector3D &n, float &radii, bool autofix, float &angleCal){
    Plane plane = Plane(n, c);
    int ids = (int)indices.size();
    std::vector<Pair> pairs;
    int cnt = 0;
    float paraAngle = 1;
    int paraCnt = 0;
    angleCal = 0;
    float lengthCal = 0;
    for(int j=0;j<ids;j+=3){
        bool interseced[3];memset(interseced,false,sizeof(interseced));
        QVector3D v[5];
        v[0] = getVertice_ori(indices[j]);
        v[1] = getVertice_ori(indices[j+1]);
        v[2] = getVertice_ori(indices[j+2]);
        int flag=3;
        for(int i=0;i<3;i++){
            float d1 = std::fabs(plane.distanceToPoint(v[i]));
            float d2 = std::fabs(plane.distanceToPoint(v[(i+1)%3]));
            if(plane.isCrossBy(v[i], v[(i+1)%3])){//isCrossPlane(v[i], v[(i+1)%3], s[0], s[1], s[2])
                v[flag]=(v[(i+1)%3]-v[i])*d1/(d1+d2) + v[i];
                interseced[i]=true;
                flag++;
            }
        }
        if(flag == 5){
            pairs.push_back(Pair(v[3], v[4]));
            cnt++;
        }else if(flag == 4){
            if(interseced[0])pairs.push_back(Pair(v[2], v[4]));
            if(interseced[1])pairs.push_back(Pair(v[0], v[4]));
            if(interseced[2])pairs.push_back(Pair(v[1], v[4]));
        }else{
            std::vector<QVector3D> tempv;
            if(plane.distanceToPoint(v[0])<minThre)tempv.push_back(v[0]);
            if(plane.distanceToPoint(v[1])<minThre)tempv.push_back(v[1]);
            if(plane.distanceToPoint(v[2])<minThre)tempv.push_back(v[2]);
            if(tempv.size()==2)pairs.push_back(Pair(tempv[0], tempv[1]));
        }

        if(flag>3){
            float angleDif = acos(QVector3D::dotProduct(n.normalized(), Triangle(v[0],v[1],v[2]).norm()));//normalized()
            float pairLength = pairs[(int)pairs.size()-1].length();
            angleCal+= angleDif*angleDif*pairLength;
            lengthCal+=pairLength;
        }
    }
    angleCal/=lengthCal;
    /*clear multi*/
    bool * toDelete;toDelete = (bool*)malloc(sizeof(bool)*pairs.size());memset(toDelete,false,pairs.size());
    for(int i=0;i<pairs.size();i++){
        for(int j=i+1;j<pairs.size();j++){
            if(pairs[i].isLinked(pairs[j]))continue;
            if(pairs[i].projIntsec(c, pairs[j])){
                float disi = (pairs[i].mid()-c).length();
                float disj = (pairs[j].mid()-c).length();
                if(disi<disj)toDelete[j]=true;
                else toDelete[i]=true;
            }
        }
    }
    int tail = 0;
    for(int i=0;i<pairs.size();i++){
        if(!toDelete[i]){
            pairs[tail++] = pairs[i];
        }
    }
    while(pairs.size()>tail)pairs.pop_back();
    /*find minDis*//*may cause problem because we use mid to judge distance*/
    std::vector<QVector3D> cand;
    QVector3D minv;
    float minl = FLT_MAX;
    for(int i=0;i<pairs.size();i++){
        QVector3D v = pairs[i].mid();
        //QVector3D v = pairs[i].proj(c);
        float dis = (v-c).length();
        if(dis < minl){
            minl = dis;
            minv = v;
        }
    }
    float maxminl = minl;
    QVector3D maxc = c;
    int maxiter = 3;
    for(int iter=0;iter<maxiter&&autofix;iter++){
        /*find intersec*/
        QVector3D vec = c - minv;
        cand.clear();
        for(int i=0;i<pairs.size();i++){
            QVector3D crossa = QVector3D::crossProduct(pairs[i].a-minv, vec);
            QVector3D crossb = QVector3D::crossProduct(vec, pairs[i].b-minv);
            if(QVector3D::dotProduct(crossa,crossb)>=0){
                float ara = crossa.length();
                float arb = crossb.length();
                QVector3D ab = (pairs[i].b -  pairs[i].a);
                QVector3D itsc = pairs[i].a + ab * ara/(ara+arb);
                if(QVector3D::dotProduct(itsc-minv,vec)>0)cand.push_back(itsc);
            }

        }
        /*find min inter sec and move(usesort)*/
        for(int n=0;n<cand.size();n++){
            for(int i=n;i<cand.size();i++){
                float minval = QVector3D::dotProduct(cand[n]-minv, vec.normalized());
                float val = QVector3D::dotProduct(cand[i]-minv, vec.normalized());
                if(val<minval){
                    QVector3D temp;
                    temp = cand[n];
                    cand[n] = cand[i];
                    cand[i] = temp;
                }
            }
        }

        QVector3D secminv;
        if(cand.size()>1&&(cand[0]-minv).length()<minThre)secminv = cand[1];
        else if(cand.size()>0&&(cand[0]-minv).length()>minThre)secminv = cand[0];
        else secminv = c + (minv-c)*2;
        if(cand.size()>0)c = (secminv + minv)/2;

        /*find vert intersec*/
        vec = QVector3D::crossProduct(vec, n).normalized();
        cand.clear();
        for(int i=0;i<pairs.size();i++){
            QVector3D crossa = QVector3D::crossProduct(pairs[i].a-c, vec);
            QVector3D crossb = QVector3D::crossProduct(vec, pairs[i].b-c);
            if(QVector3D::dotProduct(crossa,crossb)>=0){
                float ara = crossa.length();
                float arb = crossb.length();
                QVector3D ab = (pairs[i].b -  pairs[i].a);
                QVector3D itsc = pairs[i].a + ab * ara/(ara+arb);
                cand.push_back(itsc);
            }
        }
        /*find mininter vert two(use sort)*/
        for(int n=0;n<cand.size();n++){
            for(int i=n;i<cand.size();i++){
                float minval = QVector3D::dotProduct(cand[n]-c, vec.normalized());
                float val = QVector3D::dotProduct(cand[i]-c, vec.normalized());
                if(val<minval){
                    QVector3D temp;
                    temp = cand[n];
                    cand[n] = cand[i];
                    cand[i] = temp;
                }
            }
        }
        QVector3D ct = c;
        for(int i=0;i<(int)cand.size()-1;i++){
            float val = QVector3D::dotProduct(cand[i]-ct, vec.normalized());
            float valnext = QVector3D::dotProduct(cand[i+1]-ct, vec.normalized());
            if(val<0 && valnext>0){
                c = (cand[i]+cand[i+1])/2;
                break;
            }
        }
        /*find minDis*/
        minl = FLT_MAX;
        for(int i=0;i<(int)pairs.size();i++){
            QVector3D v = pairs[i].mid();
            //QVector3D v = pairs[i].proj(c);
            float dis = (v-c).length();
            if(dis < minl){
                minl = dis;
                minv = v;
            }
        }
        if(minl>maxminl){
            maxc = c;
            maxminl = minl;
        }
    }
    radii = maxminl;
    c = maxc;
    /*cal area*/
    QVector3D v;
    float l = 0;
    for(int i=0;i<pairs.size();i++){
        v+=pairs[i].mid() * pairs[i].distance();
        l += pairs[i].distance();
    }
    v/=l;
    float rtn = 0;
    for(int i=0;i<pairs.size();i++){
        rtn += QVector3D::crossProduct(pairs[i].a-v, pairs[i].b-v).length();
    }
    return rtn;
}
float ModelManager::calCutArea(){
    float area=0;
    QVector3D c = detourCenter_ori();
    for(int i=0;i<detourIdxs.size()-1;i++){
        QVector3D a = getVertice(detourIdxs[i]);
        QVector3D b = getVertice(detourIdxs[i+1]);
        area += QVector3D::crossProduct(a-c,b-c).length();
    }
    if(detourIdxs.size()==0)return FLT_MAX;
    return area;
}
int ModelManager::calDetourByPlane(QVector3D c,QVector3D n){
    cuttingPlane = Plane(n, c);
    regenByPlateIntersec();//contain "detourIdxs.clear();"
    calNeighbor();
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    detourIdxs.clear();
    for(int i=0;i<(int)vertices.size()/3;i++){
        if(std::fabs(cuttingPlane.distanceToPoint(getVertice_ori(i)))<minThre){//pointDistanceToPlane(getVertice(i),s[0],s[1],s[2])
            visited[i]=true;
            detourIdxs.push_back(i);
        }
    }
    float mindis = FLT_MAX;
    for(int i=0;i<(int)detourIdxs.size();i++){
        QVector3D v = getVertice_ori(detourIdxs[i]);
        if(dotProduct(v-c,getNormal_ori(detourIdxs[i]))>0){
            float vcl = std::fabs((v-c).length());
            if(mindis > vcl){
                mindis = vcl;
                detourSPIdx = detourIdxs[i];
            }
        }
    }
    if(checkDetour()){
        return 1;
    }else {
        detourIdxs.clear();
        return 0;
    }
}

int ModelManager::calDetourByPlane(){
    if(selecIdxs.size()<3)return 0;
    vecq3d s;
    s = getSelecPointsByIdxs_ori();
    cuttingPlane = Plane(s[0],s[1],s[2]);
    regenByPlateIntersec();//contain "detourIdxs.clear();"
    calNeighbor();
    QVector3D c  = cuttingPlane.center;
    bool * visited;visited = (bool*)malloc(sizeof(bool)*vertices.size());memset(visited,false,vertices.size());
    detourIdxs.clear();
    for(int i=0;i<(int)vertices.size()/3;i++){
        if(std::fabs(cuttingPlane.distanceToPoint(getVertice_ori(i)))<minThre){//pointDistanceToPlane(getVertice(i),s[0],s[1],s[2])
            visited[i]=true;
            detourIdxs.push_back(i);
        }
    }
    float mindis = FLT_MAX;
    for(int i=0;i<(int)detourIdxs.size();i++){
        QVector3D v = getVertice_ori(detourIdxs[i]);
        if(dotProduct(v-c,getNormal_ori(detourIdxs[i]))>0){
            float vcl = std::fabs((v-c).length());
            if(mindis > vcl){
                mindis = vcl;
                detourSPIdx = detourIdxs[i];
            }
        }
    }
    if(checkDetour()){
        return 1;
    }else {
        detourIdxs.clear();
        return 0;
    }
}

void ModelManager::regenByPlateIntersec(){//must use model vertices, check all vertices;
    if(!cuttingPlane.isExist())return;
    edges.clear();
    int ids = (int)indices.size();
    for(int j=0;j<ids;j+=3){
        bool interseced[3];memset(interseced,false,sizeof(interseced));
        QVector3D v[5];
        v[0] = getVertice_ori(indices[j]);
        v[1] = getVertice_ori(indices[j+1]);
        v[2] = getVertice_ori(indices[j+2]);
        QVector3D cl[2];
        QVector3D nm[2];
        int flag=3;
        for(int i=0;i<3;i++){
            float d1 = std::fabs(cuttingPlane.distanceToPoint(v[i]));      //pointDistanceToPlane(v[i],s[0],s[1],s[2])
            float d2 = std::fabs(cuttingPlane.distanceToPoint(v[(i+1)%3]));//pointDistanceToPlane(v[(i+1)%3],s[0],s[1],s[2])


            if(cuttingPlane.isCrossBy(v[i], v[(i+1)%3])){//isCrossPlane(v[i], v[(i+1)%3], s[0], s[1], s[2])
                v[flag]=(v[(i+1)%3]-v[i])*d1/(d1+d2) + v[i];
                cl[flag-3] = (getColor(indices[j+i])+getColor(indices[j+(i+1)%3]))/2;
                nm[flag-3] = (getNormal_ori(indices[j+i])+getNormal_ori(indices[j+(i+1)%3]))/2;
                interseced[i]=true;
                Edge edge(indices[j+i], indices[j+(i+1)%3]);
                if(edges.count(edge)==0){
                    edge.mid=(int)vertices_ori.size()/3;
                    edges.insert(edge);
                    vertices_ori.push_back(v[flag].x());vertices_ori.push_back(v[flag].y());vertices_ori.push_back(v[flag].z());
                    colors.push_back(cl[flag-3].x());colors.push_back(cl[flag-3].y());colors.push_back(cl[flag-3].z());
                    normals.push_back(nm[flag-3].x());normals.push_back(nm[flag-3].y());normals.push_back(nm[flag-3].z());
                }
                flag++;
            }
        }
        if(flag==5){
            int idx[5];
            idx[0]=indices[j];idx[1]=indices[j+1];idx[2]=indices[j+2];
            if(interseced[0]&&interseced[1]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                idx[4]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                indices.push_back(idx[2]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
            }else if(interseced[1]&&interseced[2]){
                idx[3]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                idx[4]=edges.find(Edge(indices[j+2],indices[j]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[3]);indices.push_back(idx[4]);indices.push_back(idx[0]);
                indices.push_back(idx[2]);indices.push_back(idx[4]);indices.push_back(idx[3]);
            }else if(interseced[2]&&interseced[0]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                idx[4]=edges.find(Edge(indices[j+2],indices[j]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
                indices.push_back(idx[1]);indices.push_back(idx[2]);indices.push_back(idx[4]);
            }
            indices[j]=indices[j+1]=indices[j+2]=0xffffffff;
        }else if(flag==4){
            int idx[5];
            idx[0]=indices[j];idx[1]=indices[j+1];idx[2]=indices[j+2];
            if(interseced[0]){
                idx[3]=edges.find(Edge(indices[j],indices[j+1]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
            }else if(interseced[1]){
                idx[3]=edges.find(Edge(indices[j+1],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
            }else if(interseced[2]){
                idx[3]=edges.find(Edge(indices[j],indices[j+2]))->mid;
                indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
            }
            indices[j]=indices[j+1]=indices[j+2]=0xffffffff;
        }
    }

    std::vector<unsigned int> indices_new;
    for(int j=0;j<(int)indices.size();j++){
        if(indices[j]!=0xffffffff){
            indices_new.push_back(indices[j]);
        }
    }
    indices.clear();
    for(int j=0;j<(int)indices_new.size();j++){
        indices.push_back(indices_new[j]);
    }
    neighbor.clear();
    detourIdxs.clear();
    applyed=false;applyModelMatrix();
    saveColors();
}

void ModelManager::pullConnect(float val){
    if(connectorFaceIdxs.size()==0)return;
    QVector3D n = connectorNormal_ori;
    for(int i=0;i<(int)connectorFaceIdxs.size();i++){
        QVector3D v = getVertice_ori(connectorFaceIdxs[i]);
        v = v + n * val / scalex;
        vertices_ori[connectorFaceIdxs[i]*3]  = v.x();
        vertices_ori[connectorFaceIdxs[i]*3+1]= v.y();
        vertices_ori[connectorFaceIdxs[i]*3+2]= v.z();
    }
    for(int i=0;i<(int)connectorFaceIdxs_sup.size();i++){
        putVertice_ori(connectorFaceIdxs_sup[i],getVertice_ori(connectorFaceIdxs_sup[i]) + n * val / scalex / 2);
    }
    regenNormals();
    applyed=false;applyModelMatrix();
}

void ModelManager::pushConnect(float val){
    if(connectorFaceIdxs.size()==0)return;
    if(connectorFaceIdxs.size()==0)return;
    QVector3D n = connectorNormal_ori;
    for(int i=0;i<(int)connectorFaceIdxs.size();i++){
        QVector3D v = getVertice_ori(connectorFaceIdxs[i]);
        v = v - n * val / scalex;
        vertices_ori[connectorFaceIdxs[i]*3]  = v.x();
        vertices_ori[connectorFaceIdxs[i]*3+1]= v.y();
        vertices_ori[connectorFaceIdxs[i]*3+2]= v.z();
    }

    for(int i=0;i<(int)connectorFaceIdxs_sup.size();i++){
        putVertice_ori(connectorFaceIdxs_sup[i],getVertice_ori(connectorFaceIdxs_sup[i]) - n * val / scalex / 2);
    }

    regenNormals();
    applyed=false;applyModelMatrix();

}

void ModelManager::circleOnPlane(QVector3D c, QVector3D n, float radii, int div){
    circleOnPlane(c,n,radii,div,0);
}

void ModelManager::circleOnPlane(QVector3D c, QVector3D n, float radii, int div, int cmp){
    std::vector<int> facetsOnPlane;
    std::set<int> verticesOnPlane;
    Plane plane = Plane(n,c);
    for(int i=0;i<indices.size()/3;i++){
        vecq3d v=getIndicesVertice_ori(i);
        if(std::fabs(plane.distanceToPoint((v[0]+v[1]+v[2])/3)) <= minThre){
            facetsOnPlane.push_back(i);
            verticesOnPlane.insert(indices[i*3]);
            verticesOnPlane.insert(indices[i*3+1]);
            verticesOnPlane.insert(indices[i*3+2]);
        }
    }
    std::vector<int> contourIdxs;
    QMatrix4x4 rotationMat;
    QMatrix4x4 rotationMat_fix;
    rotationMat.rotate( acosf(QVector3D::dotProduct(n,QVector3D(0,0,1)))/(2*M_PI)*360, QVector3D::crossProduct(n,QVector3D(0,0,1)));
    rotationMat_fix.setToIdentity();
    if(cmp!=0){
        QMatrix4x4 rotationMat_fix_best;
        QVector3D firstPoint_best;
        if(cmp>0)firstPoint_best = QVector3D(0,FLT_MIN,0);
        else firstPoint_best = QVector3D(0,FLT_MAX,0);
        for(int i=0;i<360;i++){
            QMatrix4x4 rotationMat_fix_this;
            rotationMat_fix_this.rotate(i,n);
            QVector3D firstPoint = (QVector4D(1,0,0,1)*rotationMat*rotationMat_fix_this).toVector3DAffine();
            bool cmpstate=false;
            if(cmp==1&&firstPoint.x()>firstPoint_best.x())cmpstate=true;
            else if(cmp==2&&firstPoint.y()>firstPoint_best.y())cmpstate=true;
            else if(cmp==3&&firstPoint.z()>firstPoint_best.z())cmpstate=true;
            else if(cmp==-1&&firstPoint.x()<firstPoint_best.x())cmpstate=true;
            else if(cmp==-2&&firstPoint.y()<firstPoint_best.y())cmpstate=true;
            else if(cmp==-3&&firstPoint.z()<firstPoint_best.z())cmpstate=true;
            if(cmpstate){
                rotationMat_fix_best = rotationMat_fix_this;
                firstPoint_best = firstPoint;
            }
        }
        rotationMat_fix = rotationMat_fix_best;
    }
    float conf = connectReverse?-1.0f:1.0f;
    for(int i=0;i<div;i++){
        QVector3D p = (QVector4D(radii*cos(2*M_PI*conf*i/div),radii*sin(2*M_PI*conf*i/div),0,1)*rotationMat*rotationMat_fix).toVector3DAffine() + c;
        bool finded = false;
        int fops = (int)facetsOnPlane.size();
        for(int i=0;i<fops;i++){
            vecq3d v = getIndicesVertice_ori(facetsOnPlane[i]);
            float d1 = dotProduct(QVector3D::crossProduct(v[0]-p,v[1]-p),n);
            float d2 = dotProduct(QVector3D::crossProduct(v[1]-p,v[2]-p),n);
            float d3 = dotProduct(QVector3D::crossProduct(v[2]-p,v[0]-p),n);
            if((d1>0&&d2>0&&d3>0)||(d1<0&&d2<0&&d3<0)){
                pushNormal(n);
                pushColor(QVector3D(0.5f,0.5f,0.5f));
                int pi = pushVertice_ori(p);
                indices.push_back(pi);indices.push_back(indices[facetsOnPlane[i]*3]);indices.push_back(indices[facetsOnPlane[i]*3+1]);
                indices.push_back(pi);indices.push_back(indices[facetsOnPlane[i]*3+1]);indices.push_back(indices[facetsOnPlane[i]*3+2]);
                //indices.push_back(pi);indices.push_back(indices[facetsOnPlane[i]*3+2]);indices.push_back(indices[facetsOnPlane[i]*3]);
                indices[facetsOnPlane[i]*3+1]=indices[facetsOnPlane[i]*3+2];
                indices[facetsOnPlane[i]*3+2]=indices[facetsOnPlane[i]*3];
                indices[facetsOnPlane[i]*3]=pi;
                facetsOnPlane.push_back((int)indices.size()/3-2);
                facetsOnPlane.push_back((int)indices.size()/3-1);
                contourIdxs.push_back(pi);
                verticesOnPlane.insert(pi);
                finded = true;
                break;
            }
        }
        if (!finded){
            float minVal=FLT_MAX;
            int minIdx=0;
            for(auto idx : verticesOnPlane){
                QVector3D v = getVertice_ori(idx);
                float d = (v-p).length();
                if(d<minVal){
                    minIdx = idx;
                    minVal = d;
                }
            }
            contourIdxs.push_back(minIdx);
        }
    }
    applyed = false;applyModelMatrix();

//    for(int i=0;i<contourIdxs.size();i++){
//        putColor(contourIdxs[i],QVector3D(1.0f,0.5f,0.5f));
//        std::cout << contourIdxs[i] << std::endl;
//    }
    /****************************************************/
    plane.radii = radii;
    linkContour(plane, contourIdxs);
}
std::vector<int> ModelManager::bfs(std::vector<bool> &visited, int rootIdx){
    calNeighbor();
    int vn = (int)visited.size();
    std::vector<int> queue;
    queue.push_back(rootIdx);
    visited[rootIdx]=true;
    int head = 0;
    while(head<(int)queue.size()){
        int qs = (int)queue.size();
        for(int k=head;k<qs;k++){
            int a = queue[k];
            for(int l=0;l<(int)neighbor[a].size();l++){
                int b = neighbor[a][l];
                if(!visited[b]){
                    visited[b] = true;
                    queue.push_back(b);
                }
            }
        }
        head = qs;
    }
    std::vector<int> out;
    for(int i=0;i<vn;i++){
        if(visited[i])out.push_back(i);
    }
    return out;
}
void ModelManager::linkContour(Plane mainPlane, std::vector<int> contourIdxs){
    int div = (int)contourIdxs.size();
    std::vector<Plane> bound;
    std::vector<int> facetsOnPlane;
    facetsOnPlane.clear();
    int ids = (int)indices.size();
    for(int i=0;i<ids/3;i++){
        vecq3d v=getIndicesVertice_ori(i);
        QVector3D mid = (v[0]+v[1]+v[2])/3;
        if(std::fabs(mainPlane.distanceToPoint(mid)) <= minThre){
//        if(std::fabs(mainPlane.distanceToPoint(v[0])) <= minThre
//        && std::fabs(mainPlane.distanceToPoint(v[1])) <= minThre
//        && std::fabs(mainPlane.distanceToPoint(v[2])) <= minThre){
            facetsOnPlane.push_back(i);
//            mid += mainPlane.normal*2;
//            int pi = pushVertice_ori(mid);
//            pushColor(QVector3D(0.5f,0.5f,0.5f));
//            pushNormal(mainPlane.normal);
//            indices.push_back(pi);indices.push_back(indices[i*3]);indices.push_back(indices[i*3+1]);
//            indices.push_back(pi);indices.push_back(indices[i*3+1]);indices.push_back(indices[i*3+2]);
//            indices[i*3+1] = indices[i*3+2];
//            indices[i*3+2] = indices[i*3];
//            indices[i*3] = pi;
            }
    }
    std::vector<bool> exist;exist.resize(vertices_ori.size()/3,false);
    for(int k=0;k<div;k++){
        edges.clear();
        QVector3D p1 = getVertice_ori(contourIdxs[k]);
        QVector3D p2 = getVertice_ori(contourIdxs[(k+1)%div]);
        QVector3D p12 = p2-p1;
        float conf = connectReverse?-1.0f:1.0f;
        QVector3D pn = QVector3D::crossProduct(mainPlane.normal,p12)*conf;
        Plane plane(pn,p1);
        bound.push_back(plane);
        int fops = (int)facetsOnPlane.size();
        for(int j=0;j<fops;j++){
            bool interseced[3];memset(interseced,false,sizeof(interseced));
            QVector3D v[5];
            int jj = facetsOnPlane[j]*3;
            v[0] = getVertice_ori(indices[jj]);
            v[1] = getVertice_ori(indices[jj+1]);
            v[2] = getVertice_ori(indices[jj+2]);
            QVector3D cl[2];
            QVector3D nm[2];
            int flag=3;
            for(int i=0;i<3;i++){
                float d1 = std::fabs(plane.distanceToPoint(v[i]));
                float d2 = std::fabs(plane.distanceToPoint(v[(i+1)%3]));
                int i1 = indices[jj+i];
                int i2 = indices[jj+(i+1)%3];
                if(plane.isCrossBy(v[i], v[(i+1)%3])){//isCrossPlane(v[i], v[(i+1)%3], s[0], s[1], s[2])
                    v[flag]=(v[(i+1)%3]-v[i])*d1/(d1+d2) + v[i];
                    cl[flag-3] = (getColor(i1)+getColor(i2))/2;
                    nm[flag-3] = (getNormal_ori(i1)+getNormal_ori(i2))/2;
                    interseced[i]=true;
                    Edge edge(i1, i2);
                    if(edges.count(edge)==0){
                        edge.mid=(int)vertices_ori.size()/3;
                        edges.insert(edge);
                        vertices_ori.push_back(v[flag].x());vertices_ori.push_back(v[flag].y());vertices_ori.push_back(v[flag].z());
                        colors.push_back(cl[flag-3].x());colors.push_back(cl[flag-3].y());colors.push_back(cl[flag-3].z());
                        normals.push_back(nm[flag-3].x());normals.push_back(nm[flag-3].y());normals.push_back(nm[flag-3].z());
                        exist.push_back(true);
                    }
                    flag++;
                }else{
                    if(d1<minThre)exist[indices[jj+i]]=true;
                    if(d2<minThre)exist[indices[jj+(i+1)%3]]=true;
                }
            }
            if(flag==5){
                int idx[5];
                idx[0]=indices[jj];idx[1]=indices[jj+1];idx[2]=indices[jj+2];
                if(interseced[0]&&interseced[1]){
                    idx[3]=edges.find(Edge(idx[0],idx[1]))->mid;
                    idx[4]=edges.find(Edge(idx[1],idx[2]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                    indices.push_back(idx[2]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                    //indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
                    indices[jj+2] = idx[1]; indices[jj] = idx[4]; indices[jj+1] = idx[3];
                }else if(interseced[1]&&interseced[2]){
                    idx[3]=edges.find(Edge(idx[1],idx[2]))->mid;
                    idx[4]=edges.find(Edge(idx[2],idx[0]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                    indices.push_back(idx[3]);indices.push_back(idx[4]);indices.push_back(idx[0]);
                    //indices.push_back(idx[2]);indices.push_back(idx[4]);indices.push_back(idx[3]);
                    indices[jj] = idx[2]; indices[jj+1] = idx[4]; indices[jj+2] = idx[3];
                }else if(interseced[2]&&interseced[0]){
                    idx[3]=edges.find(Edge(idx[0],idx[1]))->mid;
                    idx[4]=edges.find(Edge(idx[2],idx[0]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[4]);
                    indices.push_back(idx[4]);indices.push_back(idx[3]);indices.push_back(idx[1]);
                    //indices.push_back(idx[1]);indices.push_back(idx[2]);indices.push_back(idx[4]);
                    indices[jj] = idx[1]; indices[jj+1] = idx[2]; indices[jj+2] = idx[4];
                }
                facetsOnPlane.push_back((int)indices.size()/3-2);
                facetsOnPlane.push_back((int)indices.size()/3-1);
            }else if(flag==4){
                int idx[5];
                idx[0]=indices[jj];idx[1]=indices[jj+1];idx[2]=indices[jj+2];
                if(interseced[0]){
                    idx[3]=edges.find(Edge(idx[0],idx[1]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                    //indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
                    indices[jj+1] = idx[1];indices[jj+2] = idx[2];indices[jj] = idx[3];
                    exist[idx[2]] = true;
                }else if(interseced[1]){
                    idx[3]=edges.find(Edge(idx[1],idx[2]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                    //indices.push_back(idx[0]);indices.push_back(idx[3]);indices.push_back(idx[2]);
                    indices[jj] = idx[0]; indices[jj+1] = idx[3]; indices[jj+2] = idx[2];
                    exist[idx[0]] = true;
                }else if(interseced[2]){
                    idx[3]=edges.find(Edge(idx[0],idx[2]))->mid;
                    indices.push_back(idx[0]);indices.push_back(idx[1]);indices.push_back(idx[3]);
                    //indices.push_back(idx[3]);indices.push_back(idx[1]);indices.push_back(idx[2]);
                    indices[jj] = idx[3]; indices[jj+1] = idx[1]; indices[jj+2] = idx[2];
                   exist[idx[1]] = true;
                }
                facetsOnPlane.push_back((int)indices.size()/3-1);
            }
        }
    }
    for(int i=0;i<exist.size();i++){
        if(exist[i]){
            if((getVertice_ori(i)-mainPlane.center).length()>mainPlane.radii){
                exist[i] = false;
            }
        }
    }
    for(int i=0;i<contourIdxs.size();i++){
        exist[contourIdxs[i]]=true;
    }

    //fix();error?
    /************************************************/
    std::set<int> verticesOnPlane;
    for(int i=0;i<facetsOnPlane.size();i++){
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3]);
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3+1]);
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3+2]);
    }
    /************************************************/
    std::vector<int> boundry,contain;
    for(auto idx : verticesOnPlane){
        if(exist[idx]){
            boundry.push_back(idx);
        }
    }
//    for(auto idx : verticesOnPlane){
//        QVector3D v = getVertice_ori(idx);
//        int in=0,out=0,on=0;
//        for(int i=0;i<bound.size();i++){
//            if(abs(bound[i].distanceToPoint(v))<minThre){
//                on++;
//            }else if(bound[i].distanceToPoint(v)>0){
//                in++;
//            }else{
//                out++;
//            }
//        }
//        if(on>0&&out==0){
//            boundry.push_back(idx);
//            exist[idx] = true;
//        }else if(on==0&&out==0){
//            contain.push_back(idx);
//            exist[idx] = true;
//        }
//    }
    for(auto idx : verticesOnPlane){
        QVector3D v = getVertice_ori(idx);
        int out=0;
        for(int i=0;i<bound.size();i++){
            if(bound[i].distanceToPoint(v)<0){
                out++;
            }
        }
        if(out==0){
            boundry.push_back(idx);
            if(!exist[idx])contain.push_back(idx);
            exist[idx] = true;
        }
    }
    std::vector<int> replace;
    for(int i=0;i<exist.size();i++){
        replace.push_back(i);
    }
    for(int i=0;i<boundry.size();i++){
        replace[boundry[i]]=pushVertice_ori(getVertice_ori(boundry[i]));
        pushColor(QVector3D(0.5f,0.5f,0.5f));
        pushNormal(mainPlane.normal);
        verticesOnPlane.insert(replace[boundry[i]]);
        exist.push_back(true);
    }
    /************************************************/
    for(int i=0;i<facetsOnPlane.size();i++){
        int jj = facetsOnPlane[i]*3;
        if(exist[indices[jj]]&&exist[indices[jj+1]]&&exist[indices[jj+2]]){
            indices[jj]   = replace[indices[jj]];
            indices[jj+1] = replace[indices[jj+1]];
            indices[jj+2] = replace[indices[jj+2]];
        }
    }
    /************************************************/
    connectorFaceIdxs_sup.clear();
    for(int i=0;i<div;i++){
        QVector3D va1 = getVertice_ori(        contourIdxs[i        ] );
        QVector3D va2 = getVertice_ori(replace[contourIdxs[i        ]]);
        QVector3D vb1 = getVertice_ori(        contourIdxs[(i+1)%div] );
        QVector3D vb2 = getVertice_ori(replace[contourIdxs[(i+1)%div]]);
        QVector3D vn = QVector3D::crossProduct(vb1-va1,va2-va1);
        QVector3D mid1 = (va1+va2)/2;
        QVector3D mid2 = (vb1+vb2)/2;
        QVector3D mid = (mid1+mid2)/2;
        int pi = pushVertice_ori(mid);
        pushNormal(vn);
        pushColor(QVector3D(0.5f,0.5f,0.5f));
        if(!connectReverse){
            pushIndice(contourIdxs[i],pi,replace[contourIdxs[i]]);
            pushIndice(replace[contourIdxs[(i+1)%div]],pi,contourIdxs[(i+1)%div]);
        }else{
            pushIndice(replace[contourIdxs[i]],pi,contourIdxs[i]);
            pushIndice(contourIdxs[(i+1)%div],pi,replace[contourIdxs[(i+1)%div]]);
        }
        connectorFaceIdxs_sup.push_back(pi);
    }
    /************************************************/
    connectorFaceIdxs.clear();

    for(int i=0;i<boundry.size();i++){
        exist[boundry[i]] = false;
//        connectorFaceIdxs.push_back(replace[boundry[i]]);
    }
//    for(int i=0;i<contain.size();i++){
//        connectorFaceIdxs.push_back(contain[i]);
//    }

    for(auto idx : verticesOnPlane){
        if(exist[idx]){
            connectorFaceIdxs.push_back(idx);
        }
    }
    connectorFaceReady = true;
    /************************************************/
    applyed = false;applyModelMatrix();
}

void ModelManager::produceFace(){
    Plane mainPlane = cuttingPlane;
    std::vector<bool> exist;exist.resize(vertices_ori.size()/3,false);
    std::vector<int> facetsOnPlane;
    facetsOnPlane.clear();
    int ids = (int)indices.size();
    for(int i=0;i<ids/3;i++){
        vecq3d v=getIndicesVertice_ori(i);
        bool on[3]={false,false,false};
        int cnt=0;
        if(std::fabs(mainPlane.distanceToPoint(v[0])) <= minThre){on[0]=true;cnt++;}
        if(std::fabs(mainPlane.distanceToPoint(v[1])) <= minThre){on[1]=true;cnt++;}
        if(std::fabs(mainPlane.distanceToPoint(v[2])) <= minThre){on[2]=true;cnt++;}
        if(cnt==3){
            facetsOnPlane.push_back(i);
        }else if(cnt>0){
            if(on[0]){
                exist[indices[i*3]]=true;
            }
            if(on[1]){
                exist[indices[i*3+1]]=true;
            }
            if(on[2]){
                exist[indices[i*3+2]]=true;
            }
        }
    }
    std::set<int> verticesOnPlane;
    for(int i=0;i<facetsOnPlane.size();i++){
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3]);
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3+1]);
        verticesOnPlane.insert(indices[facetsOnPlane[i]*3+2]);
    }
    /************************************************/
    std::vector<int> boundry;
    std::vector<int> replace;
    int rootIdx,curIdx;
    for(int i=0;i<vertices_ori.size()/3;i++){
        replace.push_back(i);
        if(exist[i])rootIdx = i;
    }
    std::vector<bool> visited;visited.resize(vertices_ori.size()/3,false);
    calNeighbor();
    curIdx=rootIdx;
    do{
        bool has=false;
        for(int i=0;i<neighbor[curIdx].size();i++){
            if(exist[neighbor[curIdx][i]] && !visited[neighbor[curIdx][i]]){
                boundry.push_back(curIdx);
                curIdx = neighbor[curIdx][i];
                visited[curIdx] = true;
                has = true;
                break;
            }
        }
        if(!has)break;
    }while(curIdx!=rootIdx);
    /**/
    int bs = (int)boundry.size();
    QVector3D c(0,0,0);
    for(int i=0;i<boundry.size();i++){
        c+=getVertice_ori(boundry[i]);
    }c/=boundry.size();
    QVector3D n(0,0,0);
    for(int i=0;i<boundry.size();i++){
        n+=QVector3D::crossProduct(getVertice_ori(boundry[i])-c,getVertice_ori(boundry[(i+1)%bs])-c);
    }
    if(dotProduct(n, mainPlane.normal)<0){
        for(int i=1;i<(bs+1)/2;i++){
            unsigned int t = boundry[i];
            boundry[i] = boundry[bs-i];
            boundry[bs-i] = t;
        }
    }
    /**/

    for(auto idx : verticesOnPlane){
        exist[idx] = true;
    }
    for(int i=0;i<boundry.size();i++){
        replace[boundry[i]]=pushVertice_ori(getVertice_ori(boundry[i]));
        pushColor(QVector3D(0.5f,0.5f,0.5f));
        pushNormal(mainPlane.normal);
        verticesOnPlane.insert(replace[boundry[i]]);
        exist.push_back(true);
    }
    /************************************************/
    for(int i=0;i<facetsOnPlane.size();i++){
        int jj = facetsOnPlane[i]*3;
        if(exist[indices[jj]]&&exist[indices[jj+1]]&&exist[indices[jj+2]]){
            indices[jj]   = replace[indices[jj]];
            indices[jj+1] = replace[indices[jj+1]];
            indices[jj+2] = replace[indices[jj+2]];
        }
    }
    for(int i=0;i<boundry.size();i++){
        exist[boundry[i]] = false;
    }
    /************************************************/
    for(int i=0;i<boundry.size();i++){
        pushIndice(boundry[i],replace[boundry[(i+1)%bs]],replace[boundry[i]]);
        pushIndice(replace[boundry[(i+1)%bs]],boundry[i],boundry[(i+1)%bs]);
    }
    /************************************************/
    connectorFaceIdxs.clear();
    for(auto idx : verticesOnPlane){
        if(exist[idx]){
            connectorFaceIdxs.push_back(idx);
        }
    }
    connectorFaceReady = true;
    /************************************************/
    applyed = false;applyModelMatrix();
}
