#ifndef SIZEKGRAPH_H
#define SIZEKGRAPH_H
#include "DFG.h"
#include "DFGNode.h"
#include "DFGEdge.h"
#include "TypeDefine.h"
class SizeKGRAPH : public DFG{
public :
    SizeKGRAPH(Integer ID,String DFGName,String functionName,DFG* dfg){
        this->DFGID = ID;
        this->DFGName=DFGName;
        this->functionName=functionName;
        this->originDFG=dfg;
    }
    DFG* originDFG;//that is origin DFG
    Integer currentNodeID;
   /* void addEdge(DFGEdge* edge) {
        edges.push_back(*edge);
    }*/
    bool isInGraph(Integer ptrID){
      bool find=false;
      for(std::vector<DFGNode*>::iterator bg=nodes.begin(),ed=nodes.end();((bg!=ed) && (!find));bg++){
        DFGNode* node=*bg;
        if(node->ptrID==ptrID)
            find=true;
      }
      return find;
    }
};
#endif // SIZEKGRAPH_H

