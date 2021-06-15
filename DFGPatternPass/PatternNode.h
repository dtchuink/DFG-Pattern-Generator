#ifndef PATTERNNODE_H
#define PATTERNNODE_H
#include <iostream>
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "Node.h"
#include "TypeDefine.h"
#include "llvm/Support/Debug.h"

/**
 * @abstract Node of a pattern
 * @author Danielle Tchuinkou Kwadjo
 */
using namespace llvm;

 // PATTERNNODE_H


class PatternNode :public Node{
public :
      Integer NodeID;
      Integer NodeType; //as OpCode (cf. Instruction.h)
      String labelNode;
      std::vector<const Instruction*> instances;

      /**
       * @brief addInstance
       * @param i it is Instance of this Node
       */
      void addInstance(const Instruction* i) {
//        DEBUG(dbgs() << "***Adding instance "<< i->getName().str() << " to node "<< this <<"***\n");
        instances.push_back(i);
      }

      void printOnLlvmOut(LLvmOut& O) const {
        O <<" NodeType : "<< NodeType;
        O <<" labelNode : "<<labelNode<<"\n";
      }
      void printOnFile(File& F) const {
        F <<" NodeType : "<< NodeType;
        F <<" labelNode : "<<labelNode<<"\n";
      }


      PatternNode(Integer i = 0, Integer opcode = 0,constString label="") : Node(i,opcode,label) { }

};
#endif
