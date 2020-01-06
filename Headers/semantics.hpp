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

typedef struct{
    string name;
    TAG datatype;
}ass_register_info;

class Semantics{
public:
    Semantics(map<string,SymDeclare*>*,vector<Symbols*>);
    Semantics();
    ~Semantics();
    void setSymbolStack(vector<Symbols*> symbolStack);
    void genVarDefine(SymDeclare* symdeclare);
    void genFunDefine(SymDeclare* symdeclare);
    void genFunEntry(SymDeclare* symdeclare);
    void genReturnDefine(SymDeclare* symdeclare);
    void deal_AST(vector<SymDeclare*>);
    void deal_block();
    Symbols* getCurSymbols();
public:
    void start();
    ExpNode* postorder_traversal(ExpNode*); //后序遍历
    ExpNode* preorder_traversal(ExpNode*);             //先序遍历
private:
    ass_register_info nmi_find(const string& name);
private:
    map<string,SymDeclare*>* funSymbolTab;
    map<TAG,allocas> memory_alloca;
    vector<std::pair<string,ass_register_info>>* name_mapping_index;
    size_t ass_index;
    vector<Symbols*> m_symbolStack;
    //vector<map<string,string>*> nmiStack;
    map<TAG,string> typeToStr;
    size_t iftrue;
    size_t iffalse;
};

#endif /* semantics_hpp */
