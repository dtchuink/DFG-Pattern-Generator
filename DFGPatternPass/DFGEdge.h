#ifndef DFGEDGE_H
#define DFGEDGE_H
#include "TypeDefine.h"
#include "Edge.h"
/**
 * @abstract this class define DFG Edge
 * @author Danielle Tchuinkou Kwadjo
 */
class DFGEdge: public Edge{
  public :
    DFGEdge(EdgeType edge, String label):Edge(edge,label){}
};
#endif // DFGEDGE_H
