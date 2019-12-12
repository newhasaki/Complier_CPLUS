//
//  parse.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/29.
//  Copyright © 2019 mac. All rights reserved.
//

#include "parse.hpp"


Parse::Parse(vector<Token*>& tokens){
    mTokens = tokens;
    mindex = 0;
    curToken = tokens.at(mindex++);
}

void Parse::startParse(){
    syn_program();
}

void Parse::syn_program(){
    vector<VarDeclare*>* vlist = new vector<VarDeclare*>();
    syn_type(vlist);
}

void Parse::syn_localdef(vector<VarDeclare*>* vlist,TAG datatype){
    nextToken();
    if(match(ID)){
        Id* id = dynamic_cast<Id*>(curToken);
        nextToken();
        if(match(ASSIGN)){
            syn_varinit(vlist,id->getName(),datatype);
        }else{
            if(match(SEMICON)){
                printf("local var def\n");
            }
        }
    }
}

void Parse::syn_type(vector<VarDeclare*>* vlist){
    if(curToken->getTag() == KW_FLOAT
       ||curToken->getTag()== KW_INT
       ||curToken->getTag() == KW_VOID){
        
       
        syn_id(vlist,curToken->getTag());
        
    }else{
        printf("ERROR\n");
    }
}

void Parse::syn_paralist(vector<VarDeclare*>* paralist){
    syn_type(paralist);
}

void Parse::syn_id(vector<VarDeclare*>* vlist,TAG datatype){
    nextToken();
    if(curToken->getTag() == ID ){
        
        Id* id = dynamic_cast<Id*>(curToken);
        nextToken();
        if(match(EQU)){
            syn_varinit(vlist,id->getName(),datatype);
        }else if(match(LPAREN)){
            
            printf("fun\n");
            
            vector<VarDeclare*>* paralist = new vector<VarDeclare*>();
            syn_paralist(paralist);
           
            if (match(RPAREN)){
                
                if(match(LBRACE)){
                    
                    curSymbol = new vector<Symbols*>(); //创建符号表
                    symbolStack.push_back(curSymbol);
                    
                    FunDef* fundef = new FunDef(id->getName(),FUNDEFINE,datatype);
                    fundef->paralist = paralist;
                    Symbols* symbols = new Symbols();
                    symbols->insert(id->getName(), fundef);
                    
                    syn_block();
                    if(match(RPAREN)){
                                       
                    }
                }else if(match(SEMICON)){
                   
                    FunDeclare* fundeclare = new FunDeclare();
                    fundeclare->paralist = paralist;
                    
                    Symbols* symbols = new Symbols();
                    symbols->insert(id->getName(), fundeclare);
                    
                    printf("fun decal\n");
                }
            }
            
        }else{
            //Id* id = dynamic_cast<Id*>(curToken);           //变量声明
            
            VarDeclare* var_declare  = new VarDeclare(id->getName(),VARDECLARE,datatype);
            Symbols* symbols = new Symbols();
            symbols->insert(id->getName(),var_declare);     //加入当前符号表，因为参数也在当前作用域内
            
            //curSymbol->push_back(symbols);
           
            vlist->push_back(var_declare);     //参数列表
            
            if(match(SEMICON))
                printf("var def\n");
            else if(match(COMMA))
                syn_id(vlist,datatype);
            else if(match(LPAREN)){
                syn_type(vlist);
                if(!match(RPAREN)){
                    
                    if(match(SEMICON)){
                        printf("fun define");
                    }else if(match(LBRACE)){
                        
                        syn_block();
                        
                        if(match(RBRACE)){
                                printf("fun declare\n");
                            }
                        }
                    }
                }
            }
    }
    else{
        printf("ERROR\n");
    }
}

/*
 变量定义
 */

void Parse::syn_varinit(vector<VarDeclare*>* vlist,std::string varname,TAG datatype){
    
    if(match(ID)||match(CONST_INT)
       ||match(CONST_CHAR)||match(CONST_FLOAT)
       ||match(CONST_STR)){
        
        ExpNode* rootNode = syn_exp();
        VarDef* var_define = new VarDef(varname, VARDEFINE,datatype,rootNode);
        Symbols* symbols = new Symbols();
        symbols->insert(varname,var_define);
        curSymbol->push_back(symbols);
        
        nextToken();
        
        if(match(SEMICON))
            printf("init var\n");
        if(match(COMMA))
            syn_id(vlist,datatype);
    }
}

void Parse::syn_ifstat(){
    if(!match(LPAREN))
        printf("except (\n");
    
    syn_exp();
    
    Label* label = new Label();
    
    IfStat* ifstat = new IfStat();
    ifstat->setGotoLabel(label);
    ifstat->setExp(syn_bool_exp());
                    
    if(!match(RPAREN))
        printf("except )\n");
    
    if(match(LBRACE)){
        
        curSymbol = new vector<Symbols*>(); //创建符号表
        symbolStack.push_back(curSymbol);
        syn_statement();
        if(!match(RBRACE)){
            printf("except }\n");
        }else{
            
        }
        
        //if else
        if(match(KW_ELSE)){
            
        }
    }
    else{
        syn_statement();
        if(match(SEMICON))
            printf("if singel statement\n");
    }
}

ExpNode* Parse::syn_exp(){
    return syn_bool_exp();
}

ExpNode* Parse::syn_bool_and(){
    
    ExpNode* left_node = syn_add_sub();
    ExpNode* and_node = nullptr;
    while (curToken->getTag() == AND) {
        TAG op = curToken->getTag();
        nextToken();
        and_node = new ExpNode();
        and_node->createParentNode(left_node, syn_bool_and(), op);
        left_node = and_node;
    }
    return and_node;
}

ExpNode* Parse::syn_bool_or(){
    
    ExpNode* left_node = syn_bool_and();
    
    ExpNode* or_node = nullptr;
    while (curToken->getTag() == OR) {
        TAG op = curToken->getTag();
        nextToken();
        or_node = new ExpNode();
        or_node->createParentNode(left_node, syn_bool_and(), op);
        left_node = or_node;
    }
    return or_node;
}

ExpNode* Parse::syn_bool_compare(){
    
    ExpNode* left_node = syn_bool_or();
    ExpNode* cmp_node = nullptr;
    while(curToken->getTag()==EQU||curToken->getTag()==GE
          ||curToken->getTag()==LE||curToken->getTag()==LT
          ||curToken->getTag()==GT||curToken->getTag()==NEQU){
        TAG op = curToken->getTag();
        nextToken();
        cmp_node = new ExpNode();
        cmp_node->createParentNode(left_node, syn_bool_or(), op);
        left_node = cmp_node;
    }
          
    return cmp_node;
}

ExpNode* Parse::syn_bool_exp(){
    
    ExpNode* boolexp_node = syn_bool_compare();
    
    return boolexp_node;
}

ExpNode* Parse::syn_factor(){
    if(match(ID)){
        //标识符
        ExpNode* id_node = new ExpNode();
        id_node->left = nullptr;
        id_node->right = nullptr;
        id_node->setNodeType(curToken->getTag());
        Id* id = dynamic_cast<Id*>(curToken);
        id_node->setVarName(id->getName());
        nextToken();
        return id_node;
        
    }else if(match_const()){
        //常量
        ExpNode* const_node = new ExpNode();
        const_node->left = nullptr;
        const_node->right = nullptr;
        const_node->setNodeType(curToken->getTag());
        NUM* num = dynamic_cast<NUM*>(curToken);
        const_node->setValue(num->getVarDataDef());
        nextToken();
        return const_node;
        
    }else if(match(NOT)){
        syn_factor();  //非
    }else if(match(LPAREN)){
        syn_exp();
    }
    
    return nullptr;
}

ExpNode* Parse::syn_add_sub(){
    
    ExpNode* left_node = syn_mul_div();
    ExpNode* as_node = left_node;
    
    while (curToken->getTag() == ADD
           ||curToken->getTag() == SUB) {
        
        as_node = new ExpNode();
        TAG op = curToken->getTag();
        nextToken();
        switch(op){
            case ADD:
            case SUB:
                as_node->createParentNode(left_node, syn_mul_div(), op);
                left_node = as_node;
                break;
            default:
                break;
        }
    }
    return as_node;
}

ExpNode* Parse::syn_mul_div(){
    
    ExpNode* left_node = syn_factor();
    ExpNode* md_node = left_node;
    
    while (curToken->getTag() == MUL
           ||curToken->getTag() == DIV) {
        
        md_node = new ExpNode();
        TAG op = curToken->getTag();
        nextToken();
        switch (op) {
            case MUL:
            case DIV:
                md_node->createParentNode(left_node, syn_factor(), op);
                left_node = md_node;
                break;
            default:
                break;
        }
    }
    return md_node;
}


void Parse::syn_block(){
    do{
        syn_statement();
    }while(curToken->getTag()!=RBRACE);
}

void Parse::syn_statement(){
    
    switch (curToken->getTag()) {
       case KW_INT:
       case KW_FLOAT:
       case KW_VOID:
       case KW_CHAR:
        {
           vector<VarDeclare*>* vlist = new vector<VarDeclare*>();
           syn_localdef(vlist,curToken->getTag());
        }
           break;
       case KW_IF:
           syn_ifstat();
           break;
       case KW_SWITCH:
           break;
       case KW_DO:
           break;
       case KW_FOR:
           break;
       case KW_WHILE:
           break;
       case KW_RETURN:
           break;
       default:
           break;
   }
}

bool Parse::match(TAG tag){
    
    if(curToken->getTag()==tag){
        if(curToken->getTag()==ID ||match_const()){
            return true;
        }
        nextToken();
        return true;
    }
    
    return false;
}

bool Parse::match_const(){
    if(curToken->getTag()==KW_FALSE||curToken->getTag()== KW_TRUE
       ||curToken->getTag()== CONST_INT||curToken->getTag()== CONST_FLOAT
       ||curToken->getTag()==CONST_STR||curToken->getTag()==CONST_CHAR){
        return true;
    }
    return false;
}

void Parse::nextToken(){
    curToken = mTokens.at(mindex++);
}

void Parse::add_type_value(VarDataDef* vdd){
    NUM* num = dynamic_cast<NUM*>(curToken);
    switch(curToken->getTag()){
        case CONST_INT:
            vdd->idata = num->getInt();
            break;
        case CONST_STR:
            //可以重写string类操作符 =
            strcpy(vdd->sstr, num->getStr());
            break;
        case CONST_CHAR:
            vdd->cdata = num->getChar();
            break;
        case CONST_FLOAT:
            vdd->vdata = num->getFloat();
            break;
        default:break;

    }
}

bool Parse::checkRepeatFunDeclare(SymDeclare* sym){

    vector<Symbols*> symbols = (*symbolStack.at(0));
    for(vector<Symbols*>::iterator it = symbols.begin();it!=symbols.end();it++){
        SymDeclare* symdeclare = (*it)->find(sym->getName());
        if(symdeclare!=nullptr){
            FunDeclare* sym_fundeclare = dynamic_cast<FunDeclare*>(symdeclare);
            size_t sym_paraCount = sym_fundeclare->paralist->size();
            FunDeclare* fundeclare = dynamic_cast<FunDeclare*>(sym);
            size_t cur_paraCount = fundeclare->paralist->size();
            if(sym_paraCount == cur_paraCount){
                printf("参数个数相同\n");
                bool checktype = false;
                for(size_t i = 0;i<cur_paraCount;++i){
                    if(fundeclare->paralist->at(i)->getParseType() == sym_fundeclare->paralist->at(i)->getParseType()){
                        checktype = true;
                    }
                }
                
                if(checktype){
                    printf("参数类型符合!");
                    return true;
                }
            }
            
        }
    }
    return false;
}
