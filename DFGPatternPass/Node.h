#ifndef NODE_H
#define NODE_H
#include "TypeDefine.h"
/**
 * @brief The Node struct
 * @abstract This Node class define the structure of Graph Node.
 * @author Danielle Tchuinkou Kwadjo
 */
class Node {
  public :
  Integer NodeID;
  Integer NodeType; //as OpCode (cf. Instruction.h)
  String labelNode;
  bool is_frequent = false;

  String getLabelNode() const{
       return this->labelNode;
  }
  void setLabelNode(const char* label){
      this->labelNode=label;
  }

  Integer getNodeID(){
    return this->NodeID;
  }
  void setNodeID(const unsigned long int nodeID){
      this->NodeID=nodeID;
  }

  Integer getNodeType(){
     return this->NodeType;
  }

  void setNodeType(unsigned nodeType){
        this->NodeType=nodeType;
  }


  void print(llvm::raw_ostream& O) const {
    O <<" NodeType : "<< NodeType;
    O <<" labelNode : "<<labelNode<<"\n";
  }


  Node(Integer i = 0, Integer opcode = 0,constString label="") : NodeID(i), NodeType(opcode),labelNode(label) { }

};

#endif // NODE_H
