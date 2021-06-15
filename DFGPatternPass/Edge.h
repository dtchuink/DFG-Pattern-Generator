#ifndef EDGE_H
#define EDGE_H
#include "TypeDefine.h"
/**
 * @abstract this class define structure of a Edge
 */

class Edge {
 public :
  EdgeType edge;
  String labelEdge;
  int edge_id;
  bool is_frequent = false;

  void setEdge(EdgeType edge){
      this->edge=edge;
  }
  EdgeType getEdge(){
      return this->edge;
  }
  void setLabelEdge(String e){
      labelEdge=e;
  }
  String getLabelEdge() const{
     return labelEdge;
  }
  Edge(EdgeType e,String label=""){
     edge=e;
     labelEdge=label;
  }

};

#endif // EDGE_H
