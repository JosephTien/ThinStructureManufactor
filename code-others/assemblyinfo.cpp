#include "assemblyinfo.h"

AsmTerm AsmTerm::operator* (const AsmTerm& asmTerm) const
{
    AsmTerm rtn;
    for(int i=0;i<size();i++){
        for(int j=0;j<asmTerm.size();j++){
            AsmTermExp thisExp = this->_[i];
            AsmTermExp thatExp = asmTerm._[j];
            AsmTermExp temp;
            if(thisExp.asmType>thatExp.asmType)temp = thisExp;thisExp = thatExp;thatExp = temp;
            if(thisExp.asmType==0 && thatExp.asmType==0){
                if(dot(thisExp.asmUp, thatExp.asmUp)>ALMOSTONE)rtn._.push_back(thisExp);
            }
            if(thisExp.asmType==0 && thatExp.asmType==1){
                if(std::abs(dot(thisExp.asmUp, thatExp.asmRight))<ALMOSTZERO){
                    float angle = acosf(dot(thisExp.asmUp,thatExp.asmUp))/(M_PI)*180;
                    if(dot(cross(thatExp.asmUp, thisExp.asmUp),thatExp.asmRight)<0)angle*=-1;
                    if(thatExp.asmForwRLB < angle && angle < thatExp.asmForwRUB)rtn._.push_back(thisExp);
                }
            }
            if(thisExp.asmType==0 && thatExp.asmType==2){
                QVector3D cc = cross(cross(thatExp.asmForw, thisExp.asmUp),thatExp.asmForw).normalized();
                float ccangle = acosf(dot(thisExp.asmUp,cc))/(M_PI)*180;
                if(dot(cross(thisExp.asmUp, cc),thatExp.asmUp)<0)ccangle*=-1;
                if(thatExp.asmForwRLB>ccangle||ccangle>thatExp.asmForwRUB)continue;
                float upangle = acosf(dot(thatExp.asmUp,cc))/(M_PI)*180;
                if(dot(cross(thatExp.asmUp, cc),thatExp.asmForw)<0)upangle*=-1;
                if(thatExp.asmRightRLB>upangle||upangle>thatExp.asmRightRUB)continue;
                rtn._.push_back(thisExp);
            }
            if(thisExp.asmType==1 && thatExp.asmType==1){

            }
            if(thisExp.asmType==1 && thatExp.asmType==2){

            }
            if(thisExp.asmType==2 && thatExp.asmType==2){

            }
        }
    }
    return rtn;
}

AsmSet AsmSet::operator+ (const AsmSet& asmSet) const
{

}

AssemblyInfo::AssemblyInfo()
{

}

AssemblyInfo::AssemblyInfo(std::vector<float> vertices, std::vector<int> edges){
    this->vertices = vertices;
    this->edges = edges;
}

void AssemblyInfo::calAsmSets(){

}

void AssemblyInfo::calCombination(){

}
