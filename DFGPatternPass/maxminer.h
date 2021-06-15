#ifndef MAXMINER_H
#define MAXMINER_H

#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include "DFGSet.h"

using namespace std;

/**
 * @brief Perform the max miner algorithm
 * @author Danielle Tchuinkou Kwadjo
 */
class MaxMiner {
  public :

      MaxMiner(){
          ;
      }

      std::map<string,int> support_elt;
      int MIN_SUPPOT = 2;

      void insert(string instr_name){

          if(support_elt.count(instr_name) > 0){
                auto it = support_elt.find(instr_name.c_str());
                it->second = it->second +1;
          }
          else {
              support_elt.insert(pair<string, int>(instr_name, 1));
          }
      }

      int getSupport(string instr_name){
          auto it = support_elt.find(instr_name.c_str());
          return it->second;
      }

      /**
       * @brief getTailSupport
       * @param tails
       * @return
       */
      int getTailSupport(std::vector<Instruction*> tails){
          int support = 0;

          for(int i = 0; i<tails.size(); i++){
              support = support + getSupport(tails.at(i)->getOpcodeName());
          }
      }

      void candidate_group(DFGSet DFGs, DFGSet frequentGraph){
          for(DFG*  dfg : DFGs){
              Instruction* head;
              int k;
              for (int i = 0; i< dfg->nodes.size(); i++){
//                   if (i == 0){
//                       //&head = &dfg->nodes.at(i)->instruction;
//                       dfg->nodes.at(i)->head = dfg->nodes.at(i)->instruction;
//                   }
//                   else{
//                       dfg->nodes.at(i)->head = dfg->nodes.at(i)->instruction;

//                       for(k=i; k>0; k--){
//                           dfg->nodes.at(k-1)->tails.push_back(dfg->nodes.at(k)).instruction;
//                       }
//                   }
                   //head ++;
              }

              //count support
              for (int j = 0; j< dfg->nodes.size(); j++){
                    dfg->nodes.at(j)->support = getSupport(dfg->nodes.at(j)->head->getOpcodeName()) + getTailSupport(dfg->nodes.at(j).tails);

                    if(dfg->nodes.at(j)->support  > MIN_SUPPOT){

                    }
                    else{

                       DFG* graph = new DFG(dfg->DFGID, dfg->DFGName, dfg->functionName, dfg->basicBlock);
                    }

              }
          }
      }


      /**
       * @brief isGraphConnected
       * @param subgraph
       * @param newNode
       * @return
       */
      bool isGraphConnected(DFG* subgraph, DFGNode* node){
          bool connected=false;
          std::pair<DestinationsToSources::iterator,DestinationsToSources::iterator> list = subgraph->listDestToSrc.equal_range(node->ptrID);
          for (DestinationsToSources::iterator iter = list.first;(!connected && (iter != list.second)); ++iter)
          {
              if(subgraph->isThisNodeInGraph(iter->second)){
                  connected=true;
             }

          }
          return connected;
      }

      ~MaxMiner(){
          support_elt.clear();
       }

};
#endif // DFG_H
