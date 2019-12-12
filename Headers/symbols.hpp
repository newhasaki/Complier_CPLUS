//
//  symbols.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/3.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef symbols_hpp
#define symbols_hpp

#include <map>
#include <string>
#include "Tokens.h"
#include <vector>

enum PARSETYPE{
    VARDEFINE,VARDECLARE,
    FUNDEFINE,FUNDECLARE,LABEL
};

class ExpNode;

class SymDeclare{
public:
    SymDeclare(){}
    SymDeclare(const std::string name, PARSETYPE type);
    virtual ~SymDeclare(){}
private:
    void setName(std::string name);
    void setParseType(PARSETYPE type);
public:
    std::string getName();
    PARSETYPE getParseType();
private:
    std::string name;
    PARSETYPE parseType;
};

class VarDef: public SymDeclare{
public:
    VarDef(const std::string name, PARSETYPE type,TAG datatype,ExpNode* value);
    VarDef(){}
    ~VarDef(){}
private:
    void setValue(ExpNode* value);
private:
    ExpNode* value;
    TAG datatype;
};

class VarDeclare: public SymDeclare{
public:
    VarDeclare(const std::string name, PARSETYPE type,TAG tag);
    VarDeclare(){}
    ~VarDeclare(){}
private:
    TAG datatype;
};

class FunDef: public SymDeclare{
public:
    FunDef(const std::string name,PARSETYPE type,TAG retvalue);
    FunDef(){}
    ~FunDef(){}
public:
    std::vector<VarDeclare*>* paralist;
private:
    TAG retValue;
};

class FunDeclare: public SymDeclare{
public:
    FunDeclare(const std::string name,PARSETYPE type);
    FunDeclare(){}
    ~FunDeclare(){}
public:
    bool checkRepeatDeclare();
public:
    std::vector<VarDeclare*>* paralist;
};

class Label: public SymDeclare{
public:
    Label();
    ~Label(){}
private:
    size_t getLable();
private:
    size_t curLabel;
    static size_t labelNum;
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


class Symbols{
public:
    Symbols(){}
    ~Symbols();
public:
    void insert(std::string sym_name,SymDeclare* data);
    SymDeclare* find(const std::string&);
private:
    std::map<std::string,SymDeclare*> syms;
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
    void setVarName(std::string str);
    void createParentNode(ExpNode* left,ExpNode* right,TAG tag);
private:
    VarDataDef value;
    TAG nodetype;
    std::string varname;
};



#endif /* symbols_hpp */
