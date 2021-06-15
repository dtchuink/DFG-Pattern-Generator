#ifndef DFGSET_H
#define DFGSET_H
#include "llvm/IR/BasicBlock.h"
#include "DFG.h"
#include "TypeDefine.h"
class DFGSet {
  public:

    std::map <std::string, int> label_id;
    std::map <Integer, int> vertexes_id;
    int id = 0;

    DFGSet() {
      ;
    }

    //SizeKDFGset owns the DFGs
    ~DFGSet() {
        std::vector<DFG*>().swap(DFGs);
    }

    void insert(DFG* p){        
        //errs()<<"++++++++++++++++++ Insert sub graph\n";
        DFGs.push_back(p);
    }

    DFG* findSingletonByOpcode(unsigned op) {
      for(std::vector<DFG*>::iterator i = DFGs.begin(), e=DFGs.end();i!=e;++i){
        DFG* p = *i;
        if(p->size() == 1) {
          if ((*(p->nodes.begin()))->NodeType == op)
            return p;
        }
      }
      return NULL;
    }

// Functions to assign an id to instructions ID and labels
    void add_or_update_label_ID(std::string instr_name){
        //errs()<<" ------ label_id--- "<<instr_name<<" --count(instr_name)= "<<label_id.count(instr_name)<<"\n";
        if(label_id.count(instr_name) <= 0){
            if(instr_name.compare("")==0)
                label_id.insert(pair<string, int>(instr_name, 0));
            else
                label_id.insert(pair<string, int>(instr_name, id));
            id++;
        }
    }

    void add_or_update_vertex_ID(Integer v_id, int vertex_id){
        vertexes_id.insert(pair<Integer, int>(v_id, vertex_id));
        vertex_id++;
    }

    int getLabelID(string instr_name){
        auto it = label_id.find(instr_name.c_str());
        return it->second;
    }

    int getVertexID(Integer v_id){
        auto it = vertexes_id.find(v_id);
        return it->second;
    }

    Integer getVertexName(int id){
         for (std::map<Integer,int>::iterator it=vertexes_id.begin(); it!=vertexes_id.end(); ++it){
             if(it->second == id){
                 return it->first;
             }
         }
     }

   std::string getLabelName(int id){
        for (std::map<std::string,int>::iterator it=label_id.begin(); it!=label_id.end(); ++it){
            if(it->second == id){
                return it->first;
            }
        }
    }

//    DFG* findGraphByID(size_t id) {
//      for(std::vector<DFG*>::iterator it = DFGs.begin(), end=DFGs.end();it!=end;++it){
//        DFG* graph = *it;
//        errs()<<"findGraphByID "<< graph->DFGName<<" \n";
//        if(graph->DFGID == id) {
//            return *it;
//        }
//      }
//      return NULL;
//    }

    DFG* findGraphByID(size_t id) {
      for(unsigned int i=0; i<DFGs.size(); i++){
        //errs()<<"findGraphByID current ID"<< DFGs.at(i)->getDFGID()<<" input ID"<< id<<"\n";
        if(DFGs.at(i)->DFGID == id) {
            //errs()<<"findGraphByID size "<< DFGs.size()<<" \n";
            return DFGs.at(i);
        }
      }
      return NULL;
    }

    std::vector<DFG*>::iterator begin() {
        return DFGs.begin();
    }

    std::vector<DFG*>::iterator end() {
        return DFGs.end();
    }

    int numDFGs() {
        return DFGs.size();
    }

    void erase(std::vector<DFG*>::iterator pos, std::vector<DFG*>::iterator end) {
        if((!DFGs.empty()) && (pos<end))
            DFGs.erase(pos);
    }

    bool empty() {
        return DFGs.empty();
    }

    int getDFGSetID(){
      return DFGSetID;
    }

  public:
    Integer DFGSetID;
    std::vector<DFG*> DFGs;
};
#endif // DFGSET_H
