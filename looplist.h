#ifndef LOOPLIST_H
#define LOOPLIST_H
#include "pch.h"
class LoopList{
public:
    struct ListNode{
        unsigned int idx;
        struct ListNode * prev;
        struct ListNode * next;
    }*curNode;
    int cnt=0;
    int size(){
        return cnt;
    }
    void clear(){
        while(curNode!=NULL){
            deleteNode();
        }
    }
    void insertNode(unsigned int idx){
        if(curNode==NULL){
            struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
            newNode->idx=idx;
            newNode->next=newNode;
            newNode->prev=newNode;
            curNode = newNode;
            return;
        }
        struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
        newNode->idx=idx;
        newNode->prev=curNode;
        newNode->next=curNode->next;
        curNode->next->prev=newNode;
        curNode->next = newNode;
        forward();
        cnt++;
    }
    void deleteNode(){
        if(curNode->next==curNode){
            struct ListNode * temp=curNode;
            curNode=NULL;
            free(temp);
            return;
        }
        backward();
        struct ListNode * temp=curNode->next;
        curNode->next = curNode->next->next;
        curNode->next->prev = curNode;
        free(temp);
        cnt--;
    }
    void forward(){
        curNode = curNode->next;
    }
    void backward(){
        curNode = curNode->prev;
    }
    unsigned int getCurIdx(){
        return curNode->idx;
    }
    unsigned int getNextIdx(){
        return curNode->next->idx;
    }
    unsigned int getPrevIdx(){
        return curNode->prev->idx;
    }
    void assign(std::vector<unsigned int> vec){
        for(int i=0;i<(int)vec.size();i++){
            insertNode(vec[i]);
        }
        forward();
    }
    void assign(std::vector<int> vec){
        for(int i=0;i<(int)vec.size();i++){
            insertNode((unsigned int)vec[i]);
        }
        forward();
    }
    std::vector<int> getIdxs(){
        struct ListNode * endi = curNode;
        std::vector<int> idxs;
        do{
            idxs.push_back(getCurIdx());
            forward();
        }while(curNode!=endi);
        return idxs;
    }
    LoopList(){
        curNode=NULL;
    }
    ~LoopList(){
        clear();
    }

};
#endif // LOOPLIST_H
