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
#include "Tokens.hpp"
#include <vector>


using std::vector;
using std::unordered_map;
using std::string;

enum PARSETYPE{
    RETURNDEFINE,
    VARDEFINE,VARDECLARE,VARCALL,
    FUNDEFINE,FUNDECLARE,FUNCALL,
    LABEL,
    DOWHILE,
    SWITCHDECLARE,
    CASEDECLARE,
    IFSTAT,
    ELSESTAT,
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
public:
    void setName(std::string name);
    void setParseType(PARSETYPE type);
public:
    std::string getName();
    PARSETYPE getParseType();
    TAG getDataType();
    void setDataType(TAG datatype);
public:
    Symbols* m_symbols;
    vector<SymDeclare*> m_action;
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
    //vector<SymDeclare*> localVars;
};

class FunDef: public Fun{
public:
    FunDef(const string name,PARSETYPE type,TAG retvalue);
    FunDef(){}
    ~FunDef(){}
public:
    vector<SymDeclare*> m_allLocal;
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

class DoWhile:public SymDeclare{
public:
    DoWhile();
    ~DoWhile(){}
public:
    void setExp(ExpNode*);
public:
    Symbols* m_symbols;
    ExpNode* exp;
};

class Switch:public SymDeclare{
public:
    Switch();
    ~Switch(){}
    void setExp(ExpNode* exp);
private:
    ExpNode* exp;
};

class Case:public SymDeclare{
public:
    Case();
    ~Case(){}
};

class Return:public SymDeclare{
public:
    Return();
    ~Return(){}
public:
    void setRetValue(ExpNode* retValue);
private:
    ExpNode* retValue;
};

class Default:public SymDeclare{
public:
    Default();
    ~Default();
};

class IfStat: public SymDeclare{
public:
    IfStat();
    ~IfStat(){}
public:
    void setExp(ExpNode*);
    ExpNode* getValue();
private:
    ExpNode* exp;
};

class ElseStat: public SymDeclare{
public:
    ElseStat();
    ~ElseStat(){}
public:
    vector<SymDeclare*> m_symdeclare;
    //void setGotoLabel(Label* label);
private:
    ExpNode* exp;
    //Label* gotolabel;
};

class Break:public SymDeclare{
public:
    Break(){}
    ~Break(){}
};

class Continue:public SymDeclare{
public:
    Continue(){}
    ~Continue(){}
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
    VarDataDef getValue();
    void setVarName(string str);
    string getVarName();
    void createParentNode(ExpNode* left,ExpNode* right,TAG tag);
private:
    VarDataDef value;
    TAG nodetype;
    string varname;
};

#endif /* symbols_hpp */
