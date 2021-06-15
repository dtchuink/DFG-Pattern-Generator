#ifndef SUPPORT_H
#define SUPPORT_H

#include <iostream>
#include <map>
#include <iterator>
#include <string>

using namespace std;

/**
 * @brief Count support of every instruction
 * @author Danielle Tchuinkou Kwadjo
 */
class Support {
  public :

      map <string, int> support_elt;

      Support(){
          ;
      }

      void insert(string instr_name){

          if(support_elt.count(instr_name) > 0){
                auto it = support_elt.find(instr_name.c_str());
                it->second = it->second +1;
          }
          else {
              support_elt.insert(pair<string, int>(instr_name, 1));
          }

      }

      ~Support(){
          support_elt.clear();
       }

};
#endif // DFG_H
