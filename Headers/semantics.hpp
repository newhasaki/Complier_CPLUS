//
//  semantics.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/26.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef semantics_hpp
#define semantics_hpp

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "symbols.hpp"
#include "tools.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::map;

typedef struct{
    string datatype;
    string align;
}allocas;

class Semantics{
public:
    Semantics(map<string,SymDeclare*>*);
    Semantics();
    ~Semantics();
    void setSymbolStack(vector<Symbols*> symbolStack);
    void genVarDefine(SymDeclare* symdeclare);
    void genFunDefine(SymDeclare* symdeclare);
    void genFunEntry(SymDeclare* symdeclare);
    void genReturnDefine(SymDeclare* symdeclare);
    
public:
    void start();
    ExpNode* postorder_traversal(ExpNode*);
private:
    map<string,SymDeclare*>* funSymbolTab;
    map<TAG,allocas> memory_alloca;
    map<string,string>* cur_nmi;
    size_t ass_index;
    vector<Symbols*> m_symbolStack;
    
    vector<map<string,string>*> nmiStack;
};

#endif /* semantics_hpp */
