#ifndef SIZEKGRAPHSSET_H
#define SIZEKGRAPHSSET_H
#include "SizeKGRAPH.h"
#include "TypeDefine.h"
class SizeKGRAPHsSet {
  public:
    SizeKGRAPHsSet() {
      ;
    }

    //SizeKSizeKGRAPHsSet owns the sizeKGRAPHs
    ~SizeKGRAPHsSet() {
  while(!sizeKGRAPHs.empty()) {
    delete sizeKGRAPHs.back();
    sizeKGRAPHs.pop_back();
  }
    }

    void insert(SizeKGRAPH* p){
  sizeKGRAPHs.push_back(p);
    }

    SizeKGRAPH* findSingletonByOpcode(unsigned op) {
  for(std::vector<SizeKGRAPH*>::iterator i = sizeKGRAPHs.begin(), e=sizeKGRAPHs.end();i!=e;++i){
    SizeKGRAPH* p = *i;
    if(p->size() == 1) {
      if ((*(p->nodes.begin()))->NodeType == op)
        return p;
    }
  }
  return NULL;
    }

    std::vector<SizeKGRAPH*>::iterator begin() {
  return sizeKGRAPHs.begin();
    }

    std::vector<SizeKGRAPH*>::iterator end() {
  return sizeKGRAPHs.end();
    }

    int numsizeKGRAPHs() {
  return sizeKGRAPHs.size();
    }

    void erase(std::vector<SizeKGRAPH*>::iterator pos) {
  sizeKGRAPHs.erase(pos);
    }

    bool empty() {
  return sizeKGRAPHs.empty();
    }

    int getSizeKGRAPHsSetID(){
      return SizeKGRAPHsSetID;
    }
  private:
    Integer SizeKGRAPHsSetID;
    std::vector<SizeKGRAPH*> sizeKGRAPHs;
};
#endif // SIZEKGRAPHSSET_H
