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

std::string SymDeclare::getName(){
    return name;
}

SymDeclare::SymDeclare(const std::string name, PARSETYPE type){
    setName(name);
    setParseType(type);
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

void VarDef::setValue(ExpNode* value){
    this->value = value;
}

VarDeclare::VarDeclare(std::string name ,PARSETYPE type,TAG datatype):SymDeclare(name,type){
    this->datatype = datatype;
}

VarDef::VarDef(std::string name ,PARSETYPE type,TAG datatype,ExpNode* value):SymDeclare(name,type){
    setValue(value);
    this->datatype = datatype;
}

FunDef::FunDef(std::string name ,PARSETYPE type,TAG retvalue):SymDeclare(name,type){
    this->retValue = retvalue;
}

FunDeclare::FunDeclare(std::string name ,PARSETYPE type):SymDeclare(name,type){
    
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
