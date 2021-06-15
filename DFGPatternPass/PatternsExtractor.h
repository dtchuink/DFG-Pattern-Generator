#ifndef PATTERNSEXTRACTOR_H
#define PATTERNSEXTRACTOR_H
#include "llvm/Support/raw_ostream.h"
#include <string>
#include "Out.h"
#include "Pattern.h"
#include "DFG.h"
#include "DFGSet.h"
#include "SizeKGRAPH.h"
#include "TypeDefine.h"
#include "Utils.h"

/**
 * @abstract main class that define tools
 * @author Danielle Tchuinkou Kwadjo
 */

namespace {
class PatternsExtractor : public ModulePass {
public :
        static char ID; // Pass identification, replacement for typeid
        #define FILE_MAX_LINE 10024
       // std::vector<DFG*> basicblockDFGs;// list of DFGs of each basicblock.
        DFGSet DFGs;
        DFGSet frequentGraphs;
        std::string input_file = "DFG/patterns/input.txt";
        std::string output_file = "DFG/patterns/output.txt";
        std::string dependencyFilename = "DFG/patterns/dependency.txt";
        std::string separator = " ";
        DFGSet LaodDFGs;
        std::vector<SizeKGRAPH*> setOfGraphs;
        Integer K;// that is size of patterns. this value can be initialize at the begin of runtime.
        Utils utils;

        // DataDependence Object. Holds and calculates the data dependency information
        DataDependence DataDep;
        // ControlDependence object. Holds and calculates the control dependency information
        ControlDependence ControlDep;

        PatternsExtractor() : ModulePass(ID){}

        /* run the pass */
        bool runOnModule(Module& M) override{
                    K=1;
                    extractDFG(M);
                    //printDFG(DFGs);
                    //constructSize1GraphsSet();
                    errs()<<"###########################################################"
                            "\n ################### DFG generated #######################\n";
                    gSpan();
                    errs()<<"#################  Pattern mining Done############\n";
                    //printDFG(frequentGraphs);
                    printDFG(DFGs);
                    //constructSizeKGraphsSet();
                    //printSizeKGRAPH();
                   return false;
        }

        /**
         * @brief extractDFG
         * @param M : it is the module representing source file
         */
       virtual void extractDFG(Module& M);
       virtual bool isAcceptableInstruction (const Instruction* i);
       virtual bool isPhiInst(const Instruction* i);
       virtual bool hasThisPhiInstAoutSideLabel(const Instruction* i);
       virtual void printDFG(DFGSet DFGs);
       virtual void delDir(constString repertoire);
       virtual void constructSize1GraphsSet();
       virtual void printSizeKGRAPH();

       virtual void printGraphDetails(DFGSet graphs);
       virtual void getSubgraphsDetails(string file, string separator);
       virtual void gSpan();
       virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
                 //AU.addRequired<AAResultsWrapperPass>();
                 AU.addRequired<MemoryDependenceWrapperPass>();
                 AU.addRequired<PostDominatorTreeWrapperPass>();
          }

       virtual void constructSizeKGraphsSet();
       virtual void constructSize_K_GraphsSet();
       virtual bool hasOnlyInternalPath(DFGNode* srcNode,DFGNode* destNode,SizeKGRAPH* graph,Integer maxlength);
       virtual bool isConvex(SizeKGRAPH* graph,DFGNode* newNode);
       virtual bool isConnected(SizeKGRAPH* subgraph,DFGNode* newNode);
       virtual bool addThisNode(SizeKGRAPH* graph,DFGNode* destNode);
       virtual void removeNodeOnTheGraph(SizeKGRAPH* graph,DFGNode* nodeToRemove);
       virtual std::string getvaluestaticname(llvm::Value* val);
       virtual std::string LLVMInstructionAsString(llvm::Instruction* I);

};
char PatternsExtractor::ID = 0;
static llvm::RegisterPass<PatternsExtractor> X("patternExtractor", //command line option name
                    "Print data flow graph and detect pattern in the graph"); // is an analysis pass
}
#endif // PATTERNSEXTRACTOR_H
