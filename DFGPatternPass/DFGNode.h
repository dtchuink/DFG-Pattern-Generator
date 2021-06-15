#ifndef DFGNODE_H
#define DFGNODE_H
#include "Node.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "TypeDefine.h"
#include <map>
#include <iterator>

using namespace llvm;
/**
 * @abstract Node of a DFG
 * @author Danielle Tchuinkou Kwadjo
 */

class DFGNode : public Node {
  public :
      const Instruction* instruction;
      Integer ptrID;

      Instruction* head;
      std::vector<Instruction*> tails;
      int support=0;


      void setInstruction(const Instruction* i){
          this->instruction=i;
      }

      void printOnLlvmOut(LLvmOut& O) const {
        O <<"NodeID  : "<<this->NodeID;
        O <<" NodeType : "<< NodeType;
        O <<" labelNode : "<<labelNode<<"\n";
      }

      void printOnFile(File& F) const {
          F <<"NodeID  : "<<this->NodeID;
        F <<" NodeType : "<< NodeType;
        F <<" labelNode : "<<labelNode<<"\n";
      }

      DFGNode(Integer ptrID, Integer i = 0, Integer opcode = 0,const Instruction* ii=NULL,constString label="") : Node(ptrID,opcode,label) {
           this->ptrID=ptrID;
           instruction=ii;
      }

};
#endif // DFGNODE_H
