#ifndef SIZEKPATTERNSET_H
#define SIZEKPATTERNSET_H

#endif // SIZEKPATTERNSET_H
class SizeKPatternset {
  public:
    SizeKPatternset(int k) {
        size = k;
    }

    //SizeKPatternset owns the patterns
    ~SizeKPatternset() {
      while(!patterns.empty()) {
        delete patterns.back();
        patterns.pop_back();
      }
    }

    void insert(Pattern* p){
  patterns.push_back(p);
    }

    Pattern* findSingletonByOpcode(unsigned op) {
          for(std::vector<Pattern*>::iterator i = patterns.begin(), e=patterns.end();i!=e;++i){
            Pattern* p = *i;
            if(p->size() == 1) {
              if ((*(p->nodes.begin()))->NodeType == op)
                return p;
            }
  }
  return NULL;
    }

    std::vector<Pattern*>::iterator begin() {
  return patterns.begin();
    }

    std::vector<Pattern*>::iterator end() {
  return patterns.end();
    }

    int numPatterns() {
  return patterns.size();
    }

    void erase(std::vector<Pattern*>::iterator pos) {
  patterns.erase(pos);
    }

    bool empty() {
  return patterns.empty();
    }

  private:
    int size;
    std::vector<Pattern*> patterns;
};
