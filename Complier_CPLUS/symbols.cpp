//
//  symbols.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/3.
//  Copyright © 2019 mac. All rights reserved.
//
#include "symbols.hpp"

Symbols::~Symbols(){
    for(std::map<std::string,SymDeclare*>::iterator it; it!=syms.end();++it){
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

std::map<std::string,SymDeclare*>* Symbols::getSyms(){
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

VarDeclare::VarDeclare(std::string name ,PARSETYPE type,TAG datatype):SymDeclare(name,type,datatype){
}

VarDef::VarDef(std::string name ,PARSETYPE type,TAG datatype,ExpNode* value):SymDeclare(name,type,datatype){
    setValue(value);
}

FunDef::FunDef(std::string name ,PARSETYPE type,TAG retvalue):SymDeclare(name,type,retvalue){
}

void FunDef::sym_print(){
    printf("function name:%s\n",getName().c_str());     //打印函数名
   
    map<std::string,SymDeclare*>* syms = paralist->getSyms();
    size_t argc = 0;
    for(map<std::string,SymDeclare*>::iterator it = syms->begin();it!=syms->end();it++){
        switch (it->second->getDataType()){
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

void FunDeclare::sym_print(){
    printf("function name:%s\n",getName().c_str());     //打印函数名
   
    map<std::string,SymDeclare*>* syms = paralist->getSyms();
    size_t argc = 0;
    for(map<std::string,SymDeclare*>::iterator it = syms->begin();it!=syms->end();it++){
        switch (it->second->getDataType()) {
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

FunDeclare::FunDeclare(std::string name ,PARSETYPE type,TAG retValue):SymDeclare(name,type,retValue){
}

size_t Label::labelNum = 0;

Label::Label():SymDeclare("LABEL", LABEL){
    curLabel = labelNum;
    labelNum++;
}

size_t Label::getLable(){
    return curLabel;
}

void IfStat::setGotoLabel(Label* label){
    this->gotolabel = label;
}

void IfStat::setExp(ExpNode* exp){
    this->exp = exp;
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

void ExpNode::setVarName(std::string varname){
    this->varname = varname;
}

void ExpNode::createParentNode(ExpNode* left, ExpNode* right, TAG tag){
    this->left = left;
    this->right = right;
    this->setNodeType(tag);
}
