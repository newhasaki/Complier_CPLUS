//
//  symbols.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/3.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef symbols_hpp
#define symbols_hpp

#include <unordered_map>
#include <string>
#include "Tokens.h"
#include <vector>


using std::vector;
using std::unordered_map;
using std::string;

enum PARSETYPE{
    VARDEFINE,VARDECLARE,VARCALL,
    FUNDEFINE,FUNDECLARE,FUNCALL,
    LABEL
};

class ExpNode;
class Symbols;
class ParaList;
class IfStat;

class SymDeclare{
public:
    SymDeclare(){}
    SymDeclare(const std::string name, PARSETYPE type,TAG datatype);
    SymDeclare(const std::string name, PARSETYPE type);
    virtual ~SymDeclare(){}
private:
    void setName(std::string name);
    void setParseType(PARSETYPE type);
public:
    std::string getName();
    PARSETYPE getParseType();
    TAG getDataType();
    void setDataType(TAG datatype);
private:
    TAG datatype;
    std::string name;
    PARSETYPE parseType;
};

class VarDef: public SymDeclare{
public:
    VarDef(const string name, PARSETYPE type,TAG datatype,ExpNode* value);
    VarDef(){}
    ~VarDef(){}
public:
    ExpNode* getValue();
private:
    void setValue(ExpNode* value);
    ExpNode* value;
};

//class VarDeclare: public SymDeclare{
//public:
//    VarDeclare(const string name, PARSETYPE type,TAG tag);
//    VarDeclare(){}
//    ~VarDeclare(){}
//};

class Fun: public SymDeclare{
public:
    Fun(const string name,PARSETYPE type,TAG retvalue);
    Fun(){}
    ~Fun(){}
public:
    void sym_print();
    vector<SymDeclare*>* getParaInfo();
public:
    vector<SymDeclare*>* paralist;
};

class FunDef: public Fun{
public:
    FunDef(const string name,PARSETYPE type,TAG retvalue);
    FunDef(){}
    ~FunDef(){}

};

class FunDeclare: public Fun{
public:
    FunDeclare(const string name,PARSETYPE type,TAG retValue);
    FunDeclare(){}
    ~FunDeclare(){}
};

class FunCall:public Fun{
public:
    FunCall(const string name,PARSETYPE type,TAG retValue);
    FunCall(){}
    ~FunCall(){}
};

class VarCall: public SymDeclare{
public:
    VarCall(const string name,PARSETYPE type,TAG datatype,ExpNode* value);
    VarCall(){}
    ~VarCall(){}
private:
    void setValue(ExpNode* value);
private:
    ExpNode* value;
};


class Label: public SymDeclare{
public:
    Label();
    Label(PARSETYPE);
    ~Label(){}
private:
    size_t getLable();
private:
    size_t curLabel;
    static size_t labelNum;
};

class DoWhile:public SymDeclare{
public:
    DoWhile(){}
    ~DoWhile(){}
private:
    IfStat* determine;
};

class IfStat: public SymDeclare{
public:
    IfStat(){}
    ~IfStat(){}
public:
    void setGotoLabel(Label* label);
    void setExp(ExpNode*);
private:
    ExpNode* exp;
    Label* gotolabel;
};

class ElseStat: public SymDeclare{
public:
    ElseStat(){}
    ~ElseStat(){}
public:
    void setGotoLabel(Label* label);
private:
    ExpNode* exp;
    Label* gotolabel;
};

class Symbols{
public:
    Symbols(){}
    ~Symbols();
public:
    void insert(string sym_name,SymDeclare* data);
    SymDeclare* find(const string&);
    unordered_map<string,SymDeclare*>* getSyms();
private:
    unordered_map<string,SymDeclare*> syms;
};



class ExpNode{
public:
    ExpNode(){}
    ~ExpNode(){}
public:
    ExpNode* left;
    ExpNode* right;
    
public:
    void setNodeType(TAG tag);
    TAG getNodeType();
    void setValue(VarDataDef value);
    void setVarName(string str);
    void createParentNode(ExpNode* left,ExpNode* right,TAG tag);
private:
    VarDataDef value;
    TAG nodetype;
    std::string varname;
};



#endif /* symbols_hpp */
