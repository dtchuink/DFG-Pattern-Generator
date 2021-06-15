#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/PassAnalysisSupport.h"

#include "DataDependence.h"
#include "ControlDependence.h"
#include "DFGNode.h"

using namespace llvm;

class Utils {
  public:

    std::map< std::string, int> opcodeCounter;
    std::map< std::string, int> functionStats;
    std::map< std::string, int> frequentInstruction;
    float numberInstr = 0;

    Utils() {
      opcodeCounter.erase (opcodeCounter.begin(), opcodeCounter.end() );

    }

    //SizeKDFGset owns the DFGs
    ~Utils() {
         opcodeCounter.clear();
    }

    /**
     * Count the number of occurence of an instruction
     * @brief registerOpCodeInfo
     * @param II
     */
    void registerOpCodeInfo(Instruction &II){
        if(opcodeCounter.find(II.getOpcodeName()) == opcodeCounter.end()) {
               opcodeCounter[II.getOpcodeName()] = 1;
         } else {
               opcodeCounter[II.getOpcodeName()] += 1;
         }
        numberInstr++;
    }

    /**
     * Print the instruction name and the number of occurence of that instruction
     * @brief printOpCodeInfo
     */
    void printOpCodeInfo(){
        std::map< std::string, int>::iterator i = opcodeCounter.begin();
        std::map< std::string, int>::iterator e =opcodeCounter.end();
        while (i != e) {
           errs() << i->first << ": " << i->second << " ( "<<(float)(i->second/numberInstr)<<"% ) for "<<numberInstr<<"\n";
           i++;
           }
        numberInstr = 0;
    }

    void registerFunctionStats(Function &FI){
        if(functionStats.find(FI.getName()) == functionStats.end()) {
               functionStats[FI.getName()] = FI.getEntryCount();
         } /*else {
               functionStats[FI.getName()] += 1;
         }*/
    }

    void registerFrequentInstructions(DFGNode &node){
        if(frequentInstruction.find(node.getLabelNode()) == frequentInstruction.end()) {
               frequentInstruction[node.getLabelNode()] = 1;
         } else {
               frequentInstruction[node.getLabelNode()] += 1;
         }
        numberInstr++;
    }

    void printInfo(std::map< std::string, int> structData){
        std::map< std::string, int>::iterator i = structData.begin();
        std::map< std::string, int>::iterator e =structData.end();

        while (i != e) {
           errs() << i->first << ": " << i->second << "\n";
           i++;
           }
        errs() <<"For a total of "<< numberInstr<<"\n";
    }

    void printAnalysis(raw_ostream &OS, DataDependence DataDep, ControlDependence  ControlDep) {
      // dump the contents of the local Deps_ map
      OS << "\n ####### Local Dependence map size: " << DataDep.LocalDeps_.size() << '\n';
      for (auto i = DataDep.LocalDeps_.begin(); i != DataDep.LocalDeps_.end(); ++i) {
        assert(i->first && "NULL inst found");
        OS << "========= Instruction: " << *(i->first);
        OS << "\n    has dependence\n";
        if(i->second.DepInst_ !=NULL)
            OS << "    with instruction " << (i->second.DepInst_->getName()) << '\n';
        OS << "    of type " << DataDependence::depTypeToString(i->second.Type_) << '\n';
      }
      OS << "\n ######## Non-Local Dependence map size: " << DataDep.NonLocalDeps_.size() << '\n';
      for (auto i = DataDep.NonLocalDeps_.begin(); i != DataDep.NonLocalDeps_.end(); ++i) {
        assert(i->first && "NULL inst found");
        OS << "-------- Instruction: " << *(i->first)
           << "\n    has non local dependence(s) with:\n";
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
          //assert(j->getAddress() && " Dependannce instruction failed!!!");
          if(j->getAddress()!=NULL)
            OS << "    Address: " << *(j->getAddress()) << '\n';
        }
      }

      // create a dot file for the CDG
      ControlDep.toDot(std::string(""));

      // dump the contents of the control dependencies
      for (auto i = ControlDep.controlDeps_.begin(),
          ei = ControlDep.controlDeps_.end(); i != ei; ++i) {
        std::set<BasicBlock *> depSet;
        depSet = i->second;

        OS << "BasicBlock: " << *(i->first)
           << "Is dependent on:\n";
        for (auto j = depSet.begin(), ej = depSet.end(); j != ej; ++j) {
          // *j is a pointer to a BasicBlock, **j is a BasicBlock
          OS << **j << '\n';
        }
      }
    }
};
