#ifndef OUTPUT_H
#define OUTPUT_H
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include "TypeDefine.h"
using namespace llvm;
using namespace std;
/**
 * @brief This class is use as output on the scream or on a file
 */
class Out{
private:
     raw_ostream* llvmOut;
     ofstream* file;
public :
     Out(raw_ostream& o){
       this->llvmOut=&o;
     }
     bool createFile(constString name) {
         ofstream file(name);
         this->file=&file;
         return file.is_open();
     }
     raw_ostream& getLlvmOut(){
          return *(this->llvmOut);
     }
     ofstream& getFile(){
         return *(this->file);
     }
};

#endif // OUTPUT_H
