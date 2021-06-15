#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <map>
/**
 * @abstract this file is use to define some used types
 * @author Danielle Tchuinkou Kwadjo
 */

typedef std::string String;
typedef unsigned long int Integer;
typedef const char* constString;
typedef std::pair<Integer,Integer> EdgeType;
typedef char*  charString;
typedef std::ofstream File;
typedef llvm::raw_ostream LLvmOut;
typedef std::map<Integer,Integer> PtrIDToNodeID;
typedef std::map<Integer,Integer> NodeIDToPtrID;
typedef std::multimap<Integer,Integer> SourcesToDestinations;
typedef std::multimap<Integer,Integer> DestinationsToSources;
#endif // TYPEDEFINE_H
