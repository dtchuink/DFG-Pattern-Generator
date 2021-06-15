#ifndef PATTERN_H
#define PATTERN_H
#include "PatternEdge.h"
#include "PatternNode.h"
#include "TypeDefine.h"
/**
 * @brief The Pattern struct
 * @author Danielle Tchuinkou Kwadjo
 */

struct Pattern {
  std::vector<PatternNode*> nodes;
  std::vector<PatternEdge> edges;

  //Pattern owns the PatternNodes
  ~Pattern(){
    while(!nodes.empty()) {
       delete nodes.back();
       nodes.pop_back();
    };
    /*while(!edges.empty()){
        delete edges.back();
        edges.pop_back();
    }*/
   }

  int size() const {
    return nodes.size();
  }

  PatternNode* createNode(Integer op,constString label) {
    PatternNode* newnode = new PatternNode(nodes.size()+1,op,label);
    nodes.push_back(newnode);
    return newnode;
  }

  void addEdge(Integer i, Integer j,constString label) {
   // edges.insert(edges.end(),std::make_pair(i,j));
      PatternEdge edge(std::make_pair(i,j),label);
      edges.insert(edges.end(),edge);
  }

  void printOnLlvmOut(llvm::raw_ostream& O) const {
    O << this << ": nodes[";
    for(std::vector<PatternNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
  (*i)->printOnLlvmOut(O);
  O << ", ";
    }
    O << "]\n";
  }
  void printOnFile(File& F) const {
    F << this << ": nodes[";
    for(std::vector<PatternNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
  (*i)->printOnFile(F);
  F << ", ";
    }
    F << "]\n";
  }

  void printDotOnLllvmOut(LLvmOut& O) const {
      O << "digraph \""<< "Pattern" <<"\" {\n";
    for(std::vector<PatternNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
        O << "\t" << (*i)->NodeID << " [shape=circle,label=\""<< (*i)->getLabelNode() << " (" << *i <<"); Instances:";
    //for(std::vector<const Instruction*>::const_iterator j = (*i)->instances.begin(), je = (*i)->instances.end();j!=je;++j){
            // O << " " << *j;
        //}
         O << "\"];\n";
    }
    for(std::vector<PatternEdge>::const_iterator i = edges.begin(), e=edges.end(); i!=e; ++i) {
            O << "\t" << i->edge.first << " -> " << i->edge.second << ";\n";
    }
                O<<"}\n";
  }
  void printDotOnFile(File& F) const {
      F << "digraph \""<< "Pattern" <<"\" {\n";
    for(std::vector<PatternNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
        F << "\t" << (*i)->NodeID << " [shape=circle,label=\""<< (*i)->getLabelNode() << " (" << *i <<"); Instances:";
    //for(std::vector<const Instruction*>::const_iterator j = (*i)->instances.begin(), je = (*i)->instances.end();j!=je;++j){
            // F << " " << *j;
        //}
         F << "\"];\n";
    }
    for(std::vector<PatternEdge>::const_iterator i = edges.begin(), e=edges.end(); i!=e; ++i) {
            F << "\t" << i->edge.first << " -> " << i->edge.second << ";\n";
    }
                F<<"}\n";
  }

 /*bool isInPattern(int n, const Instruction* ii) const {
  for(std::vector<PatternNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
    if(((*i)->instances).at(n) == ii)
  return true;
  }
  return false;
}*/
};
#endif // PATTERN_H
