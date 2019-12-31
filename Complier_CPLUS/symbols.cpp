//
//  symbols.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/3.
//  Copyright © 2019 mac. All rights reserved.
//
#include "symbols.hpp"

Symbols::~Symbols(){
    for(unordered_map<std::string,SymDeclare*>::iterator it; it!=syms.end();++it){
        delete it->second;
        it->second = nullptr;
    }
}

void Symbols::insert(std::string sym_name, SymDeclare* data){
    if(sym_name.empty()||data == nullptr){
        printf("sym_name is empty or data is nullptr !\n");
        return;
    }
    
    syms.insert(std::make_pair(sym_name, data));
}

SymDeclare* Symbols::find(const std::string& name){
    SymDeclare* symdec = nullptr;
    
    if(syms.find(name)!=syms.end()){
        symdec = syms.find(name)->second;
        return symdec;
    }
    return nullptr;
}

unordered_map<std::string,SymDeclare*>* Symbols::getSyms(){
    return &syms;
}

std::string SymDeclare::getName(){
    return name;
}

SymDeclare::SymDeclare(const std::string name, PARSETYPE type,TAG datatype){
    setName(name);
    setParseType(type);
    setDataType(datatype);
}

SymDeclare::SymDeclare(const std::string name, PARSETYPE type){
    setName(name);
    setParseType(type);
}

void SymDeclare::setDataType(TAG datatype){
    this->datatype = datatype;
}

void SymDeclare::setName(std::string name){
    this->name = name;
}

void SymDeclare::setParseType(PARSETYPE type){
    this->parseType = type;
}

PARSETYPE SymDeclare::getParseType(){
    return this->parseType;
}

TAG SymDeclare::getDataType(){
    return datatype;
}

void VarDef::setValue(ExpNode* value){
    this->value = value;
}

ExpNode* VarDef::getValue(){
    return value;
}

VarDef::VarDef(string name ,PARSETYPE type,TAG datatype,ExpNode* value):SymDeclare(name,type,datatype){
    setValue(value);
}

vector<SymDeclare*>* Fun::getParaInfo(){
//    vector<SymDeclare*> paraInfos;
//    unordered_map<std::string,SymDeclare*>* parainfo = this->paralist->getSyms();
//    for(unordered_map<string,SymDeclare*>::iterator it = parainfo->begin();it!=parainfo->end();it++){
//        paraInfos.push_back(it->second);
//    }
    
    return paralist;
}

void Fun::sym_print(){
    printf("function name:%s\n",getName().c_str());     //打印函数名
   
    size_t argc = 0;
    for(vector<SymDeclare*>::iterator it=paralist->begin(); it!=paralist->end();++it){
    
        switch ((*it)->getDataType()){
            case KW_INT:printf("%ld.Type: int\n",argc);break;       //打印函数参数类型
            case KW_BOOL:printf("%ld.Type: bool\n",argc);break;
            case KW_CHAR:printf("%ld.Type: char\n",argc);break;
            case KW_VOID:printf("%ld.Type: void\n",argc);break;
            case KW_FLOAT:printf("%ld.Type: float\n",argc);break;
            default:break;
        }
        argc++;
    }
    printf("argc: %ld\n",argc);     //打印函数个数
    
    switch (getDataType()) {
        case KW_INT:printf("retValue Type: int\n");break;       //打印函数返回类型
        case KW_BOOL:printf("retValue Type: bool\n");break;
        case KW_CHAR:printf("retValue Type: char\n");break;
        case KW_VOID:printf("retValue Type: void\n");break;
        case KW_FLOAT:printf("retValue Type: float\n");break;
        default:break;
    }
}

Fun::Fun(const string name,PARSETYPE type,TAG retvalue):SymDeclare(name,type,retvalue){
}

FunDef::FunDef(const string name ,PARSETYPE type,TAG retvalue):Fun(name,type,retvalue){
}

FunDeclare::FunDeclare(const string name ,PARSETYPE type,TAG retValue):Fun(name,type,retValue){
}

FunCall::FunCall(const string name,PARSETYPE type,TAG retValue):Fun(name,type,retValue){
}

VarCall::VarCall(const string name,PARSETYPE type,TAG datatype,ExpNode* value):SymDeclare(name,type,datatype){
    setValue(value);
}

void VarCall::setValue(ExpNode *value){
    this->value = value;
}

IfStat::IfStat(){
    setParseType(IFSTAT);
    setName("IfStat");
    this->m_symbols = new Symbols();
}

void IfStat::setExp(ExpNode* exp){
    this->exp = exp;
}

Case::Case(){
    setParseType(CASEDECLARE);
}

Switch::Switch(){
    setParseType(SWITCHDECLARE);
}

DoWhile::DoWhile(){
    setParseType(DOWHILE);
}

void DoWhile::setExp(ExpNode* exp){
    exp = exp;
}

void Switch::setExp(ExpNode* exp){
    this->exp = exp;
}

Return::Return(){
    setParseType(RETURNDEFINE);
}

void Return::setRetValue(ExpNode* retValue){
    this->retValue = retValue;
}

ElseStat::ElseStat(){
    setParseType(ELSESTAT);
    setName("ElseStat");
    this->m_symbols = new Symbols();
}

TAG ExpNode::getNodeType(){
    return nodetype;
}

void ExpNode::setNodeType(TAG tag){
    this->nodetype = tag;
}

void ExpNode::setValue(VarDataDef value){
    this->value = value;
}

VarDataDef ExpNode::getValue(){
    return value;
}

void ExpNode::setVarName(std::string varname){
    this->varname = varname;
}

void ExpNode::createParentNode(ExpNode* left, ExpNode* right, TAG tag){
    this->left = left;
    this->right = right;
    this->setNodeType(tag);
}
