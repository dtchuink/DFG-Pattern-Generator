#ifndef PATTERNEDGE_H
#define PATTERNEDGE_H
#include "TypeDefine.h"
#include "Edge.h"
/**
 * @abstract this class define pattern Edge
 * @author Danielle Tchuinkou Kwadjo
 */
class PatternEdge: public Edge{
  public :
    PatternEdge(EdgeType edge,String label):Edge(edge,label){}
};
#endif // PATTERNEDGE_H
