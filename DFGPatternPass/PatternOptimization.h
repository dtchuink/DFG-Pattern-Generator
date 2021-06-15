#ifndef PATTERNOPTIMIZATION_H
#define PATTERNOPTIMIZATION_H

#include "DFG.h"


class PatternOptimization
{
public:
    PatternOptimization();

    void extendPatterns(DFG *dfgs);

    std::vector<std::string> patternConv1;
    std::vector<std::string> patternConv2;
    std::vector<std::string> patternConv3;
    std::vector<std::string> patternConv4;
    std::vector<std::string> patternConv5;
    std::vector<std::string> patternConv6;


    std::vector<std::string> patternPooling1;
    std::vector<std::string> patternPooling2;
    std::vector<std::string> patternPooling3;
    std::vector<std::string> patternPooling4;


};

#endif // PATTERNOPTIMIZATION_H
