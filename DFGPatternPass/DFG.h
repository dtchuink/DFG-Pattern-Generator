#ifndef DFG_H
#define DFG_H
#include "DFGEdge.h"
#include "DFGNode.h"
#include "TypeDefine.h"
#include <string>
/**
 * @brief The DFG struct
 * @author Danielle Tchuinkou Kwadjo
 */
class DFG {
  public :
  std::vector<DFGNode*> nodes;
  std::vector<DFGEdge> edges;
  std::string basicBlockName;
  std::size_t basicBlockID;
  bool hasFrequentInstruction= false;

  SourcesToDestinations listSrcToDest;//this allow us to store a map between source Node and destination Node
  DestinationsToSources listDestToSrc;//this allow us to store a map between destination Node and source Node
  PtrIDToNodeID listPtrIDToNodeID;//this allow us to store a map between ptrID and nodeID
  NodeIDToPtrID listNodeIDToPrtID;//this allow us to store a map between nodeID and ptrID

  DFG(){
    ;
  }

  DFG(std::size_t BB_Id,String BB_Name, String functionName, const BasicBlock* basicBlock, bool isload=false){
      this->DFGID = BB_Id;
      if(isload)
        this->DFGName=BB_Name + "_load";
      else
          this->DFGName=BB_Name;
      this->functionName=functionName;
      this->basicBlock=basicBlock;
  }

  //Pattern owns the DFGNodes
  ~DFG(){
     std::vector<DFGNode*>().swap(nodes);

    /*while(!edges.empty()){
        delete edges.back();
        edges.pop_back();
    }*/
   }


  DFGNode* findNodeById(Integer id){
      for(unsigned int i = 0; i<nodes.size(); i++){
          if(nodes.at(i)->getNodeID() == id){
              return nodes.at(i);
          }
      }
      return NULL;
  }

  DFGEdge* findEdgeById(int edge_id){
      for(std::vector<DFGEdge>::iterator iter=edges.begin(),end=edges.end();iter!=end;iter++){
          if(iter->edge_id == edge_id){
              return &(*iter);
          }
      }
      return NULL;
  }

  int size() const {
    return nodes.size();
  }
  Integer sizeOfNode() const{
      return nodes.size();
  }
  Integer sizeOfEdge() const{
      return edges.size();
  }

  DFGNode* createNode(Integer ptrID,Integer ID,Integer opCode,const Instruction* i,constString label) {
    DFGNode* newnode = new DFGNode(ptrID,ID,opCode,i,label);
    listPtrIDToNodeID.insert(PtrIDToNodeID::value_type(ptrID,ID));
    listNodeIDToPrtID.insert(NodeIDToPtrID::value_type(ID,ptrID));
    nodes.push_back(newnode);
    return newnode;
  }

  DFGNode* createNode(DFGNode* newNode){
      listPtrIDToNodeID.insert(PtrIDToNodeID::value_type(newNode->ptrID,newNode->NodeID));
      listNodeIDToPrtID.insert(NodeIDToPtrID::value_type(newNode->NodeID,newNode->ptrID));
      nodes.push_back(newNode);
      return newNode;
  }

  void addEdge(Integer i, Integer j,String label, int edge_rank) {
   // edges.insert(edges.end(),std::make_pair(i,j));
      DFGEdge edge(std::make_pair(i,j),label);
      edge.edge_id = edge_rank;
      //errs() <<" ---- Adding edge ------>Edge ID: "<<edge_rank<<" \n ";
      edges.push_back(edge);
      //edges.insert(edges.end(),edge);
      listSrcToDest.insert(SourcesToDestinations::value_type(i,j));
      listDestToSrc.insert(DestinationsToSources::value_type(j,i));
  }


  bool isThisNodeInGraph(DFGNode* node){
      PtrIDToNodeID::iterator objectIter=this->listPtrIDToNodeID.find(node->ptrID);
      PtrIDToNodeID::iterator endIter=this->listPtrIDToNodeID.end();
      return objectIter!=endIter;
  }
  bool isThisNodeInGraph(Integer nodePtrID){
      PtrIDToNodeID::iterator objectIter=this->listPtrIDToNodeID.find(nodePtrID);
      PtrIDToNodeID::iterator endIter=this->listPtrIDToNodeID.end();
      return objectIter!=endIter;
  }
  bool hasThisNodeAParent(DFGNode* node){
     DestinationsToSources::iterator objectIter=this->listDestToSrc.find(node->ptrID);
     DestinationsToSources::iterator endIter=this->listDestToSrc.end();
     return objectIter!=endIter;
  }
  bool hasThisNodeAParent(Integer nodePtrID){
     DestinationsToSources::iterator objectIter=this->listDestToSrc.find(nodePtrID);
     DestinationsToSources::iterator endIter=this->listDestToSrc.end();
     return objectIter!=endIter;
  }
  std::vector<DFGNode*> getListOfParent(DFGNode* node){
        std::vector<DFGNode*> parents;
        pair<DestinationsToSources::iterator,DestinationsToSources::iterator> borne=this->listDestToSrc.equal_range(node->ptrID);
        for(DestinationsToSources::iterator i=borne.first,end=borne.second;i!=end;i++){
            Integer srcPtrID=i->second;
            PtrIDToNodeID::iterator objectIter=this->listPtrIDToNodeID.find(srcPtrID);
            Integer parentIndex=objectIter->second-1;
            DFGNode* parent=nodes[parentIndex];
            parents.push_back(parent);
        }
        return parents;
  }
  std::vector<DFGNode*> getListOfParent(Integer nodePtrID){
        std::vector<DFGNode*> parents;
        pair<DestinationsToSources::iterator,DestinationsToSources::iterator> borne=this->listDestToSrc.equal_range(nodePtrID);
        for(DestinationsToSources::iterator i=borne.first,end=borne.second;i!=end;i++){
            Integer srcPtrID=i->second;
            PtrIDToNodeID::iterator objectIter=this->listPtrIDToNodeID.find(srcPtrID);
            Integer parentIndex=objectIter->second-1;
            DFGNode* parent=nodes[parentIndex];
            parents.push_back(parent);
        }
       return parents;
  }
  String getLabelEdge(Integer srcIndex,Integer destIndex){
      bool find=false;
      String label="";
      for(std::vector<DFGEdge>::iterator iter=edges.begin(),end=edges.end();!find && (iter!=end);iter++){
          if((iter->edge.first==srcIndex) && (iter->edge.first==destIndex)){
              label=iter->getLabelEdge();
              find=true;
          }
      }
      return label;

  }

  DFGEdge* getEdge(Integer srcIndex,Integer destIndex){
      bool find=false;
      DFGEdge* edge=NULL;
      for(std::vector<DFGEdge>::iterator iter=edges.begin(),end=edges.end();!find && (iter!=end);iter++){
          if((iter->edge.first==srcIndex) && (iter->edge.first==destIndex)){
              edge=&(*iter);
              find=true;
          }
      }
      return edge;
  }
  std::vector<DFGNode*>::iterator beginNode(){
      return nodes.begin();
  }
  std::vector<DFGNode*>::iterator endNode(){
      return nodes.end();
  }

  std::vector<DFGEdge>::iterator beginEdge(){
      return edges.begin();
  }
  std::vector<DFGEdge>::iterator endEdge(){
      return edges.end();
  }
  void printOnLlvmOut(llvm::raw_ostream& O) const {
    O << this << ": nodes[";
    for(std::vector<DFGNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
  (*i)->printOnLlvmOut(O);
  O << ", ";
    }
    O << "]\n";
  }
  void printOnFile(File& F) const {
    F << this << ": nodes[";
    for(std::vector<DFGNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
  (*i)->printOnFile(F);
  F << ", ";
    }
    F << "]\n";
  }

  void printDotOnLllvmOut(LLvmOut& O) const {
      O << "digraph \""<< "Pattern" <<"\" {\n";
    for(std::vector<DFGNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
        O << "\t" << (*i)->ptrID << " [shape=circle,label=\""<< (*i)->getLabelNode();//<<"\\n nodeID="<<(*i)->NodeID<<"\"]";
         O << "\"];\n";
    }
    for(std::vector<DFGEdge>::const_iterator i = edges.begin(), e=edges.end(); i!=e; ++i) {
        O << "\t" << i->edge.first << " -> " << i->edge.second <<"[label=\""<<i->getLabelEdge()<<";\n";
    }
                O<<"}\n";
  }
  void printDotOnFile(File& F) const {
      F << "digraph \""<< "Pattern" <<"\" {\n";
    for(std::vector<DFGNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
        if((*i)->is_frequent)
            F << "\t" << (*i)->ptrID << " [shape=circle, color=red, label=\""<< (*i)->getLabelNode();//<<"\\n nodeID="<<(*i)->NodeID;
        else
            F << "\t" << (*i)->ptrID << " [shape=circle, label=\""<< (*i)->getLabelNode();//<<"\\n nodeID="<<(*i)->NodeID;
         F << "\"];\n";
    }
    for(std::vector<DFGEdge>::const_iterator i = edges.begin(), e=edges.end(); i!=e; ++i) {
        if(i->is_frequent)
            F << "\t" << i->edge.first << " -> " << i->edge.second <<"[color=red, label=\""<<i->getLabelEdge()<<"\"]"<<";\n";
        else
            F << "\t" << i->edge.first << " -> " << i->edge.second <<"[label=\""<<i->getLabelEdge()<<"\"]"<<";\n";
    }
                F<<"}\n";
  }

  size_t getDFGID(){
    return DFGID;
  }
  String getDFGName(){
     return this->DFGName;
  }
 const BasicBlock* getBasicBlock(){
    return this->basicBlock;
  }
 String getFunctionName(){
   return this->functionName;
 }
  size_t DFGID;
  String DFGName;
  String functionName;
 const BasicBlock* basicBlock;

 /*bool isInPattern(int n, const Instruction* ii) const {
  for(std::vector<DFGNode*>::const_iterator i = nodes.begin(), e=nodes.end(); i!=e; ++i) {
    if(((*i)->instances).at(n) == ii)
  return true;
  }
  return false;
}*/
};
#endif // DFG_H
