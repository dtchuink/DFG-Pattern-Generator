//===- PatternsExtractor.cpp - Find frequent computational patterns --------===//
//
//                          An LLVM Compiler Pass
//
//		Author: Danielle Tchuinkou daniellekwadjo@gmail.com
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/Debug.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm//IR/User.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
//#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/Vectorize/SLPVectorizer.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include "PatternNode.h"
#include "PatternsExtractor.h"
#include "TypeDefine.h"
#include "DFGSet.h"
#include "DFGNode.h"
#include "DFGEdge.h"
#include "DFG.h"
#include "SizeKGRAPH.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "patternExtractor"

/**
 * @brief PatternsExtractor::extractDFG() methode is the first part of this tool, it extract DFGs of a module iterating through basick bloc
 * @param M
 * @abstract here i don't consider phi instructions
 */

void PatternsExtractor::extractDFG(Module &M){


    for (Function &FI : M){
        String functionName = FI.getName();
        if (FI.isDeclaration())
            continue;
        AliasAnalysis &AA = getAnalysis<AAResultsWrapperPass>(FI).getAAResults();
        // Since MemoryDependenceAnalysis is a function pass, we need to pass the current function we are examining to the getAnalysis() call.
        MemoryDependenceResults &MDA = getAnalysis<MemoryDependenceWrapperPass>(FI).getMemDep();
        PostDominatorTree &PDT = getAnalysis<PostDominatorTreeWrapperPass>(FI).getPostDomTree();

        DataDep.getDataDependencies(FI, MDA, AA);
        ControlDep.getControlDependencies(FI, PDT);


        //Count the occurence of the number of time we going throw a function
        utils.registerFunctionStats(FI);

        for (BasicBlock &BB : FI){
            DFG* graph = new DFG((std::size_t)&BB, BB.getName(), functionName, &BB);// set of module DFG.
           // DFG* load_graph = new DFG((Integer)&BB, BB.getName(), functionName, &BB, true);// set of module DFG.
            Integer vertex_id=0; int edge_id = 0;

            for (Instruction &II : BB){
              const Instruction* inst = &II;
              utils.registerOpCodeInfo(II);

              if(isAcceptableInstruction(inst)){
                   DFGNode* node = new DFGNode((Integer)inst, vertex_id, inst->getOpcode(), inst , inst->getOpcodeName());
                   DFGs.add_or_update_label_ID(inst->getOpcodeName());
                   DFGs.add_or_update_vertex_ID((Integer)inst, vertex_id);
                   graph->createNode(node);

                    for(llvm::Instruction::op_iterator op = II.op_begin(), ope = II.op_end(); op != ope; ++op){
                        if(llvm::dyn_cast<llvm::Instruction>(*op)||llvm::dyn_cast<llvm::Argument>(*op)) {
                             const Instruction* ii = dyn_cast<Instruction>(*op);
                             //errs() <<"ii instruction (from) "<<(Integer)ii<<" -----inst instruction (to) "<<(Integer)inst<<" \n ";
                             graph->addEdge((Integer)ii,(Integer)inst, op->get()->getName(), edge_id);
                             DFGs.add_or_update_label_ID(op->get()->getName());
                             edge_id++;
                        }
                        // Increment the edge ID for the next edge
                    }                    
                    vertex_id++;
              }
            }// end for loop to iterate over instruction in a basicBlock
            DFGs.insert(graph);
            errs() <<" \n";
        }
    }// end for loop to iterate through the functions

    //Function to print results of the dependency
    std::error_code EC; sys::fs::OpenFlags flags;
    raw_ostream *OS = new raw_fd_ostream(dependencyFilename, EC, flags);
    utils.printAnalysis(*OS, DataDep, ControlDep);

    utils.printOpCodeInfo();
    utils.printInfo(utils.frequentInstruction);
}

std::string PatternsExtractor::LLVMInstructionAsString(llvm::Instruction* I){
    std::string instString;
    llvm::raw_string_ostream N(instString);
    I->print(N);
    return N.str();
}

std::string PatternsExtractor::getvaluestaticname(llvm::Value* val){
    static unsigned int num = 0;
    std::string ret_val = "val";
    if(val->getName().empty()) {ret_val += std::to_string(num);num++;}
    else ret_val = val->getName().str();

    if(llvm::isa<llvm::Instruction>(val))ret_val += ":"+LLVMInstructionAsString(llvm::dyn_cast<llvm::Instruction>(val));

    return ret_val;
}

void PatternsExtractor::gSpan(){
    //errs()<<"\n------ In gSpan -------\n";
    printGraphDetails(DFGs);
    //Delete data structures for printing the graph
    DFGs.label_id.erase( DFGs.label_id.begin(), DFGs.label_id.end() );
    DFGs.vertexes_id.erase( DFGs.vertexes_id.begin(), DFGs.vertexes_id.end() );
    system("./run_pattern_detection.sh");
    getSubgraphsDetails(output_file, separator);
    utils.printInfo(utils.frequentInstruction);
}

/**
 * @brief PatternsExtractor::printGraph
 * @param graph
 * Ouput the graph as a text for gSPAN
 */
void PatternsExtractor::printGraphDetails(DFGSet graphs){
    ofstream fout(input_file.c_str());
    for(unsigned int i=0; i<graphs.DFGs.size(); i++){
        fout << "t " << graphs.DFGs.at(i)->getDFGID() << std::endl;
        //list the vertex in the file

        for(unsigned int j=0; j<graphs.DFGs.at(i)->nodes.size(); j++){
            fout << "v " << j << " "<< graphs.getLabelID(graphs.DFGs.at(i)->nodes.at(j)->getLabelNode())<<std::endl;
        }

        //list the edges in the file
        int k = 0;
         for(std::vector<DFGEdge>::const_iterator it = graphs.DFGs.at(i)->edges.begin(), end=graphs.DFGs.at(i)->edges.end(); it!=end; ++it) {
             if(graphs.DFGs.at(i)->isThisNodeInGraph(it->edge.first)){
                //errs()<<"############# node "<<it->edge.first<<" to_node_id "<<it->edge.second<<"\n";
                fout << "e " << k<<" "<<graphs.getVertexID(it->edge.first) << " " << graphs.getVertexID(it->edge.second) <<" "<<graphs.getLabelID(it->getLabelEdge())<<std::endl;
                k++;
             }
         }
    }
    fout.close();
}

void PatternsExtractor::getSubgraphsDetails(string file, string separator){

    //Contruct Subgraphs size_t graph_line;
     bool isSubGraphNotExist = true;
     bool isMapping = false;
     std::vector<DFG*>().swap(frequentGraphs.DFGs);//frequentGraphs.DFGs.clear();

     ifstream fin(file.c_str(), ifstream::in);
     errs() << "Output File read!!\n";
     vector<vector<string> > input_;
     //size_t num_graph_;
     char line[FILE_MAX_LINE];

    if (!fin.is_open()) {
         errs() << "Open file: " << file << " error!\n";
      }
      size_t num_line = 0;

      errs() << "\nContruct Subgraphs\n";
      DFG* subgraph; DFG* graph;
      while (fin.getline(line, FILE_MAX_LINE)) {
        isSubGraphNotExist = false;
        char *pch = NULL;
        pch = strtok(line, separator.c_str());
        input_.resize(num_line + 1);
        while (pch != NULL) {
          input_[num_line].emplace_back(pch);
          //errs() << " pch = " <<pch<<"\n";
          pch = strtok(NULL, separator.c_str());
        }

        //errs()<<"frequentGraphs.DFGs = "<< frequentGraphs.DFGs.capacity()<<" \n";
        if (!input_[num_line][0].compare("m")){
            isMapping = true; subgraph = NULL; graph = NULL;
            size_t graph_id = stoul(input_[num_line][4].c_str());
            //Before adding the graph in the new set of frequent graphs, we check if it already exist.
            if(frequentGraphs.DFGs.capacity()>0)
                subgraph = frequentGraphs.findGraphByID(graph_id);

            if(subgraph==NULL){
                //errs()<<"SubGraph ID does not exist!! \n";
                graph = DFGs.findGraphByID(graph_id);
                if(graph!=NULL){
                      isSubGraphNotExist = true;
                      //errs()<<"Graph ID Found!! =  "<<graph_id<<"\n";
                      subgraph = new DFG(graph->basicBlockID, graph->basicBlockName, graph->functionName, graph->basicBlock);
                }
                else
                     errs()<<"Could not Find graph "<<graph_id<<"\n";
            } else{
               //errs()<<"SubGraph ID exist!! \n";
               graph = DFGs.findGraphByID(graph_id);
            }
        }
        else if (!input_[num_line][0].compare("v") && isMapping) {

                int node_id = atoi(input_[num_line][2].c_str());
                //errs() << "Node id = "<<node_id<<"   Number of nodes in  the graph="<<graph->nodes.size()<<"\n";
                if(graph!=NULL){
                    DFGNode* node = graph->nodes.at(node_id);
                    if(!graph->nodes.at(node_id)->is_frequent)
                        utils.registerFrequentInstructions(*node);
                    graph->nodes.at(node_id)->is_frequent = true;
                    graph->hasFrequentInstruction = true;
                    subgraph->createNode(node);
                }

            } else if (!input_[num_line][0].compare("e") && isMapping) {
                int edge_id = atoi(input_[num_line][2].c_str());
                // Add an edge
                // Forward direction
                if(graph!=NULL){
                    DFGEdge* edge =  &graph->edges.at(edge_id);
                    graph->edges.at(edge_id).is_frequent = true;
                    if(edge!=NULL)
                            subgraph->addEdge(edge->edge.first, edge->edge.second, edge->getLabelEdge(), edge->edge_id);
                    else
                            errs()<<"Could not Find the Edge \n";
                    }
                 else {
                        errs()<<"Could not Find Edge in the graph "<<edge_id<<"\n";
                    }
              } else if(!input_[num_line][0].compare("p")){
                isMapping = false;
                cout << "Reading input error!"<<endl;
              }

        if(isSubGraphNotExist)
            frequentGraphs.insert(subgraph);

        ++num_line;
      }
      fin.close();
}

/* Indicates if the Instruction i is a valid or useful DFG node.
 * there are memory instruction, call instruction and some phi instuction
 */
/**
 * @brief PatternsExtractor::isAcceptableInstruction
 * @param i
 * @return
 */
bool PatternsExtractor::isAcceptableInstruction (const Instruction* i){
  //TODO: trawl through Instruction.h and see which other Instructions to consider
  //TODO: possible candidates: integer/float comparison, casts

    bool result =  //(i && (i->getOpcode()!=Instruction::Call)
            ((i->getOpcode()!=Instruction::GetElementPtr)
//            && !(i->mayReadOrWriteMemory() || i->mayHaveSideEffects())
            && !(i->isTerminator()) && (isPhiInst(i)?!hasThisPhiInstAoutSideLabel(i):true));
//    errs()<<"Instruction : "<<(Integer)i<<"\n";

    return result;
}

/**
 * @brief PatternsExtractor::isPhiInst
 * @param i
 * @return
 */
bool PatternsExtractor::isPhiInst(const Instruction *i){
   // errs()<<"\n code :"<<i->getOpcodeName();
     return (i->getOpcode()==Instruction::PHI);
}

/**
 * @brief PatternsExtractor::hasThisPhiInstAoutSideLabel, here i test if this phi instruction is implement here to manage SSA form.
 * @param i
 * @return
 */
bool PatternsExtractor::hasThisPhiInstAoutSideLabel(const Instruction *i){
    bool find=false;
    const BasicBlock* bb=i->getParent();
    for(User::const_op_iterator op = i->op_begin(), ope = i->op_end();op!=ope;op++){
        if(*op){
            const Instruction* ii = dyn_cast<Instruction>(*op);

            if(ii!=NULL){
                const BasicBlock* parent = ii->getParent();
                if(!ii || (ii && (bb!=parent))){//if the precedent pointer is and instruction and have not same parent or if there is not and instruction
                    find=true;
                    break;
                }
            }
        }
      }
    //errs()<<"  test == "<<(find?"vraie":"faux")<<"\n\n";
    return find;
}

void PatternsExtractor::delDir(constString repertoire){
    DIR * dir;
    struct dirent * entree;
    dir = opendir (repertoire);
    if (dir == NULL)
    return;
    while ((entree = readdir (dir)) != NULL){
       errs()<<" File = "<<entree->d_name<<"\n";
       remove(entree->d_name);

      }
    closedir (dir);
    remove(repertoire);
}

/**
 * @brief print all DFGs
 */
void PatternsExtractor::printDFG(DFGSet DFGs){
    //delDir("./DFG");
    mkdir("DFG",00777);
    for(std::vector<DFG*>::iterator i = DFGs.begin(), end=DFGs.end();i!=end;++i){
        DFG* graph=*i;
        String fileName="./DFG/FUNCTION_"+graph->getFunctionName()+"_of_basicblock_"+graph->getDFGName()+".dot";
        File F(fileName.data());
        graph->printDotOnFile(F);
        F.close();
     }
}

/**
 * @brief PatternsExtractor::constructSize1GraphsSet
 * Remove DFG with a small support
 */
void PatternsExtractor::constructSize1GraphsSet(){
    //for(std::vector<DFG*>::iterator i=DFGs.begin(),end=DFGs.end();i!=end;++i){
    for(std::vector<DFG*>::iterator i=DFGs.begin(),end=DFGs.end();i!=end;++i){
        DFG* graph=*i;
        if(graph->nodes.size()<K){
            end = DFGs.end();
            DFGs.erase(i, end);
            end = DFGs.end();
        }
        //Build the graph with bigger support
        else{

        }
    }

    errs()<<"Remove all graph of size less than K\n";
}

void PatternsExtractor::constructSize_K_GraphsSet(){
    /*Implement a algorithm here, consist to go DFG per DFG and use those DFGs to construct size k pattern*/
    Integer ID=0;
    for(std::vector<DFG*>::iterator DFGIterator = DFGs.begin(), endDFG=DFGs.end();DFGIterator!=endDFG;++DFGIterator){
        DFG* graph=*DFGIterator;
        errs()<<"In loop --- graph nodes"<<graph->nodes.size()<<"\n";
        //intialize size 1 graph with nodes in a DFG.it can be do until it remaind (K-1) Node.
        Integer nodeSize=graph->sizeOfNode();

        if(nodeSize>=K){
            errs()<<"In loop --- if(nodeSize\n";
            Integer nbOfNodeToUse=(nodeSize-(K-1));
            Integer nbOfUsedNode=1;
            cout<<"\n\n graph : "<<graph->getDFGName()<<"\n";
            errs()<<" Size : "<<nodeSize<<"----- SizeToUse :"<<nbOfNodeToUse<<"\n";

            for(std::vector<DFGNode*>::iterator nodeIterator=graph->nodes.begin(),endNode=graph->nodes.end();(nbOfUsedNode<=nbOfNodeToUse) && (nodeIterator!=endNode);++nodeIterator,++nbOfUsedNode){
               DFGNode* node=*nodeIterator;
               //node->printOnLlvmOut(errs()<<"\n---Noeud----\n");
               SizeKGRAPH* sizeKGRAPH=new SizeKGRAPH(++ID,graph->DFGName,graph->functionName,graph);
               sizeKGRAPH->createNode(node);

              // for(std::vector<DFGNode*>::iterator be=sizeKGRAPH->nodesSet.begin(),en=sizeKGRAPH->nodesSet.end();be!=en;be++)
                   //(*be)->printOnLlvmOut(errs());
               sizeKGRAPH->currentNodeID=node->getNodeID();
               setOfGraphs.push_back(sizeKGRAPH);
            }
           cout<<"\n\n";
        }
    }
}
/**
 * @brief PatternsExtractor::printSizeKGRAPH print all graph of size K
 */
void PatternsExtractor::printSizeKGRAPH(){
    //delDir("./GRAPH");
    mkdir("GRAPH",00777);
    errs()<<"----printSizeKGRAPH----\n";
    for(std::vector<SizeKGRAPH*>::iterator i = setOfGraphs.begin(), end=setOfGraphs.end();i!=end;++i){
        SizeKGRAPH* graph=*i;
        ostringstream strID;
        strID<<graph->DFGID;
        errs()<<graph->currentNodeID<<"\n";
        String fileName="./GRAPH/graph_"+graph->getDFGName()+"_OF_FUNCTION_"+graph->getFunctionName()+"_Nbr_"+strID.str()+".dot";
        File F(fileName.data());
        graph->printDotOnFile(F);
        F.close();
     }
}
/**
 * @brief PatternsExtractor::isConnected
 * @param subgraph
 * @param newNode
 * @return
 */
bool PatternsExtractor::isConnected(SizeKGRAPH* subgraph,DFGNode* newNode){
    bool connected=false;
    std::pair<DestinationsToSources::iterator,DestinationsToSources::iterator> list=subgraph->originDFG->listDestToSrc.equal_range(newNode->ptrID);
    for (DestinationsToSources::iterator iter = list.first;(!connected && (iter != list.second)); ++iter)
    {
        //errs()<<"----dest ="<<iter->first<<" src ="<<iter->second<<"----";
        if(subgraph->isThisNodeInGraph(iter->second))
            connected=true;
        errs()<<" connected="<<(connected?"true":"false")<<"\n";
    }
    errs()<<" In general connected="<<(connected?"true":"false")<<"\n";
    return connected;
}


/**
 * @brief PatternsExtractor::isConvex, check if the size j graph genarated by producing a size (j-1) graph with newNode is convex.
 * @param graph, is the size (j-1) graph
 * @param newNode, is the node to be added.
 * @return boolean is the extending produce a convex graph
 */
bool PatternsExtractor::isConvex(SizeKGRAPH* graph,DFGNode* newNode){
      bool ok=true;
       std::vector<DFGNode*> list=graph->nodes;
       Integer maxlength=list.size();
       //maxlength++;
       errs()<<"maxLength = "<<maxlength<<"\n";
       for(std::vector<DFGNode*>::iterator nodeIter=list.begin(),endIter=list.end();nodeIter!=endIter;++nodeIter){
           DFGNode* srcNode=*nodeIter;
           ok=(ok && hasOnlyInternalPath(srcNode,newNode,graph,maxlength-1));
       }
       errs()<<(ok?"Convexity = true \n":"convexity = false\n");
    return ok;
}
/**
 * @brief PatternsExtractor::hasOnlyInternalPath
 * @param srcNode
 * @param destNode
 * @param graph
 * @param maxlength
 * @return
 */
bool PatternsExtractor::hasOnlyInternalPath(DFGNode* srcNode,DFGNode* destNode,SizeKGRAPH* graph,Integer maxlength){
    if(maxlength==0){
        if((srcNode->ptrID==destNode->ptrID) || (graph->isThisNodeInGraph(destNode))){
            //errs()<<"First cond if \n";
            return true;
         }
        else{
            //errs()<<"First cond else\n";
            return false;
         }
    }
    else{
        if(!graph->isThisNodeInGraph(destNode))
            return false;
        else
            if((srcNode->ptrID==destNode->ptrID) || (!graph->originDFG->hasThisNodeAParent(destNode)))
                return true;
            else{
                bool ok=true;
                std::vector<DFGNode*> list=graph->originDFG->getListOfParent(destNode);
                for(std::vector<DFGNode*>::iterator nodeIter=list.begin(),endIter=list.end();nodeIter!=endIter;++nodeIter){
                    DFGNode* node=*nodeIter;
                    //errs()<<"parent :  ";
                    //node->printOnLlvmOut(errs());
                    ok=(ok && hasOnlyInternalPath(srcNode,node,graph,maxlength-1));
                }
                return ok;

            }

    }
    return true;
}

/**
 * @brief PatternsExtractor::addThisNode
 * @param graph
 * @param destNode
 * @return
 */
bool PatternsExtractor::addThisNode(SizeKGRAPH* graph,DFGNode* destNode){
    bool ok=false;
    std::vector<DFGNode*> list=graph->originDFG->getListOfParent(destNode);
    for(std::vector<DFGNode*>::iterator nodeIter=list.begin(),endIter=list.end();((!ok)&&(nodeIter!=endIter));++nodeIter){
        DFGNode* srcNode=*nodeIter;
        if(graph->isThisNodeInGraph(srcNode)){
              ok=true;
        }
     }
    if(ok){
      graph->createNode(destNode);
      for(std::vector<DFGNode*>::iterator nodeIter=list.begin(),endIter=list.end();nodeIter!=endIter;++nodeIter){
          DFGNode* srcNode=*nodeIter;
          if(graph->isThisNodeInGraph(srcNode)){
                String edgeLabel=graph->getLabelEdge(srcNode->ptrID,destNode->ptrID);
                graph->addEdge(srcNode->ptrID,destNode->ptrID,edgeLabel, 0);
          }
       }
    }
    return ok;
}

/**
 * @brief PatternsExtractor::removeNodeOnTheGraph
 * @param graph
 * @param nodeToRemove
 */
void PatternsExtractor::removeNodeOnTheGraph(SizeKGRAPH* graph,DFGNode* nodeToRemove){
    //errs()<<"===Dans remove avant edge===\n";
    for(std::vector<DFGEdge>::iterator iter=graph->edges.begin(), end=graph->edges.end(); iter!=end; iter++){
        if(iter->edge.second==nodeToRemove->ptrID)
            graph->edges.erase(iter);
    }
    //errs()<<"===Dans remove apres edge===\n";
    bool remove=false;
    for(std::vector<DFGNode*>::iterator iter=graph->nodes.begin(), end=graph->nodes.end(); !remove && iter!=end; iter++){
        DFGNode* node=*iter;
        if(node->ptrID==nodeToRemove->ptrID)
          graph->nodes.erase(iter);
    }
    errs()<<"====apres nodes remove===\n";
}

/**
 * @brief PatternsExtractor::constructSizeKGraphsSet This methode extract all size K graph.
 */
void PatternsExtractor::constructSizeKGraphsSet(){
    for(std::vector<SizeKGRAPH*>::iterator i=setOfGraphs.begin(),end=setOfGraphs.end();i!=end;++i){
        SizeKGRAPH* graph=*i;
        Integer nextIndex=graph->currentNodeID;
        Integer graphSize=graph->nodes.size();
        Integer maximumSize=graph->originDFG->nodes.size();
        while((nextIndex<maximumSize) && (graphSize < K)){

            DFGNode* nodeToAdd=graph->originDFG->nodes[nextIndex];
            //errs()<<" currentNodeID ="<<graph->currentNodeID<<"---- nextNodeID ="<<nodeToAdd->getNodeID()<<"\n";
            //NodeIDToPtrID::iterator objectIter=graph->listNodeIDToPrtID.find(graph->currentNodeID);
             //errs()<<" currentNodePtrID ="<<objectIter->second<<"---- nextNodePtrID="<<nodeToAdd->ptrID<<"\n";
            if(isConnected(graph, nodeToAdd)){
               addThisNode(graph, nodeToAdd);
               if(isConvex(graph, nodeToAdd)){
                    graph->currentNodeID=nodeToAdd->NodeID;
                    nextIndex=graph->currentNodeID;
               }
               else{
                   removeNodeOnTheGraph(graph,nodeToAdd);
                   nextIndex++;
               }
            }
            else{
                //errs()<<"------------------Mauvais------------------\n";
                nextIndex++;
            }
            errs()<<"----MaximumSize ="<<maximumSize<<"\n\n";
            graphSize = graph->nodes.size();
        }
        if(graphSize<K){
            setOfGraphs.erase(i);
            errs()<<"Graph size smaller than k"<<"\n";
         }
    }
}



