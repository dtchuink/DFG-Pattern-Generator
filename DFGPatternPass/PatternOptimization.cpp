#include "PatternOptimization.h"

PatternOptimization::PatternOptimization()
{
    patternConv1 = {"load", "fmul", "fadd"};
    patternConv2 = {"load", "add", "stofp"};
    patternConv3 = {"mul", "mul", "sext"};
    patternConv4 = {"mul", "add", "mul", "add", "sext"};
    patternConv5 = {"load", "fext", "fmul", "fptrunc", "store"};\
    patternConv6 = {"load", "fpext", "fadd"};
    patternConv7 = {"add", "mul", "add", "sext"};

    patternPooling1 = {"add", "icmp"};
    patternPooling2 = {"load", "icmp", "select", "store"};
    patternPooling3;
    patternPooling4;
}

PatternOptimization::extendPatterns(DFG *dfgs, std::vector<std::string> pattern){
    if(dfgs->hasFrequentInstruction && dfgs->nodes.size()>=3)
        int pos = 0;
        bool match= false;
        std::string  head = pattern.at(pos);
        for(std::vector<DFGNode *>::iterator it = dfgs->nodes.begin()+1, end=edges.end()-1; it!=end; it++){
            if((*it)->instruction->getOpcodeName() == head){
                match = true;
                pos++;
                break;
            }
            else if(match){
                if((*it)->instruction->getOpcodeName() == pattern.at(pos)){
                    (*it)->is_frequent = true;
                }
                else
                    match = false;
            }

        }
}
