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
    curSymbol = new Symbols(); //创建符号表
}

void Parse::startParse(){
    syn_program();
}

void Parse::syn_program(){
    while(curToken->getTag()!=ERR){
        syn_type();
    }
}

/*
 
 */
void Parse::syn_localvar(TAG datatype){
    nextToken();
    while(curToken->getTag()!=SEMICON){
        if(match(ID)){
            Id* id = dynamic_cast<Id*>(curToken);
            nextToken();
            if(match(ASSIGN)){
                if(match(ID)||match_const()){
                    SymDeclare* symbol = syn_vardefine(id->getName(),datatype);
                    curSymbol->insert(id->getName(),symbol);
                    symDeclares.push_back(symbol);
                }
                if(match(SEMICON))
                    break;
                if(match(COMMA))
                    continue;
            }else if(match(SEMICON)){
                SymDeclare* symbol = syn_vardeclare(id->getName(),datatype);
                curSymbol->insert(id->getName(), symbol);
                symDeclares.push_back(symbol);
                break;
            }else if(match(COMMA)){
                SymDeclare* symbol = syn_vardeclare(id->getName(),datatype);
                curSymbol->insert(id->getName(), symbol);
                symDeclares.push_back(symbol);
            }
        }
    }
}

void Parse::syn_type(){
    TAG datatype = curToken->getTag();
    if(match_type()){
        syn_id(datatype);
    }
}

SymDeclare* Parse::syn_parameters(std::string varname,TAG datatype){
    return syn_vardeclare(varname,datatype);
}

SymDeclare* Parse::syn_parameters_init(std::string varname,TAG datatype){
    return syn_vardefine(varname,datatype);
}

Symbols* Parse::syn_paralist(){
    Symbols* paralist = new Symbols();
    while(true){
        TAG datatype = curToken->getTag();
        if(match_type()){
            if(match(ID)){
                Id* id = dynamic_cast<Id*>(curToken);
                SymDeclare* symbol = nullptr;
                nextToken();
                if(match(COMMA)){
                    symbol = syn_parameters(id->getName(),datatype);
                    paralist->insert(id->getName(), symbol);
                }else if(match(RPAREN)){
                    symbol = syn_parameters(id->getName(),datatype);
                    paralist->insert(id->getName(),symbol);
                    return paralist;
                }else if(match(ASSIGN)){
                    if(match_const()){
                        symbol = syn_parameters_init(id->getName(),datatype);
                        while(true){
                            if(match(COMMA)){
                                TAG datatype = curToken->getTag();
                                if(match_type()){
                                    if(match(ID)){
                                        Id* id = dynamic_cast<Id*>(curToken);
                                        if(match(ASSIGN)){
                                            if(match_const()){
                                                symbol = syn_parameters_init(id->getName(), datatype);
                                                paralist->insert(id->getName(),symbol);        //参数值
                                            }
                                        }
                                    }
                                }
                            }else if(match(RPAREN)){
                                symbol = syn_parameters_init(id->getName(), datatype);
                                paralist->insert(id->getName(),symbol);        //参数值
                                return paralist;
                            }
                        }
                    }
                }
            }else if(match(COMMA)){
                VarDeclare* var_declare = new VarDeclare("undefine_varname",VARDECLARE,datatype);
                paralist->insert("undefine_varname",var_declare);
            }else if(match(RPAREN)){
                return paralist;
            }
        }else{
            printf("ERROR\n");
        }
    }
    return nullptr;
}
    
void Parse::syn_id(TAG datatype){
    if(curToken->getTag() == ID ){
        Id* id = dynamic_cast<Id*>(curToken);
        nextToken();
        if(match(EQU)){
            syn_vardefine(id->getName(),datatype);
        }else if(match(LPAREN)){
            Symbols* paralist = syn_paralist();
            
            if(match(LBRACE)){
                FunDef* fundef = new FunDef(id->getName(),FUNDEFINE,datatype);  //函数定义
                fundef->paralist = paralist;
                
                fundef->sym_print();    //打印函数定义信息
                
                curSymbol->insert(id->getName(),fundef);
                symDeclares.push_back(fundef);
                symbolStack.push_back(curSymbol);
                
                syn_block();
                
            }else if(match(SEMICON)){
                FunDeclare* fundeclare = new FunDeclare(id->getName(),FUNDECLARE,datatype); //函数声明
                fundeclare->paralist = paralist;
                
                fundeclare->sym_print();    //打印函数声明信息

                curSymbol->insert(id->getName(), fundeclare);
                symDeclares.push_back(fundeclare);
                printf("fun decal\n");
            }
            
        }else{
            
            VarDeclare* var_declare  = new VarDeclare(id->getName(),VARDECLARE,datatype);
            Symbols* symbols = new Symbols();
            symbols->insert(id->getName(),var_declare);     //加入当前符号表，因为参数也在当前作用域内
            symDeclares.push_back(var_declare);
            if(match(SEMICON))
                printf("var def\n");
            else if(match(COMMA))
                syn_id(datatype);
            else if(match(LPAREN)){
                syn_type();
                if(match(RPAREN)){
                    
                    if(match(SEMICON)){
                        printf("fun declare");
                    }else if(match(LBRACE)){
                        
                        syn_block();
                        
                        if(match(RBRACE)){
                                printf("fun define\n");
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
SymDeclare* Parse::syn_vardefine(std::string varname,TAG datatype){
    ExpNode* rootNode = syn_exp();
    VarDef* var_define = new VarDef(varname, VARDEFINE,datatype,rootNode);
    return var_define;
}

/*
变量声明
*/
SymDeclare* Parse::syn_vardeclare(std::string varname,TAG datatype){
    VarDeclare* var_declare = new VarDeclare(varname,VARDECLARE,datatype);
    return var_declare;
}

/*
    if else语句
    if(){
 
    }
    first_label
 
    if(){
        goto first_label
    }else{
        second_label
    }
    first_label
*/
void Parse::syn_if_else_stat(){
    nextToken();
    if(match(LPAREN)){
        
    }else{
        printf("except (\n");
    }
        
    Label* first_label = new Label();
    
    IfStat* ifstat = new IfStat();
    ifstat->setGotoLabel(first_label);
    ifstat->setExp(syn_exp());
    symDeclares.push_back(ifstat);
    
    if(match(RPAREN))
    {
        
    }else{
        printf("except )\n");
    }
    
    if(match(LBRACE)){
        
        curSymbol = new Symbols(); //创建符号表
        symbolStack.push_back(curSymbol);

        syn_statement();
        if(match(RBRACE)){
            symbolStack.pop_back();     //离开if语句
            
        }else{
            printf("except }\n");
        }
        
        
        //else
        if(match(KW_ELSE)){
            ElseStat* elsestat = new ElseStat();
            elsestat->setGotoLabel(first_label);
            symDeclares.push_back(elsestat);
            
            Label* second_label = new Label();
            ifstat->setGotoLabel(second_label);
            symDeclares.push_back(second_label);
            
                if(match(LBRACE)){
                    syn_statement();
                    if(match(RBRACE)){
                        symDeclares.push_back(first_label);
                    }
                }
        }else{
             symDeclares.push_back(first_label);   //记录标签
        }
    }
    else{
        /*
         if(boolexp):
            i = 1;
         else
            i = 0;
         */
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
    ExpNode* and_node = left_node;
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
    
    ExpNode* or_node = left_node;
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
    ExpNode* cmp_node = left_node;
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
        return syn_factor();  //非
    }else if(match(LPAREN)){
        ExpNode* expNode = syn_exp();
        if(match(RPAREN)){
             return expNode;
        }else{
            printf("except )\n");
        }
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
    
    curSymbol = new Symbols(); //创建符号表
    symbolStack.push_back(curSymbol);
    do{
        syn_statement();
    }while(curToken->getTag()!=RBRACE);
}

void Parse::syn_statement(){
    
    switch (curToken->getTag()) {               //在函数体中中以数据类型开始的关键字只有变量定义和变量声明
       case KW_INT:
       case KW_FLOAT:
       case KW_VOID:
       case KW_CHAR:
       case KW_BOOL:
           syn_localvar(curToken->getTag());
           break;
       case KW_IF:
           syn_if_else_stat();
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
    if(curToken->getTag()==KW_FALSE_CONST||curToken->getTag()== KW_TRUE_CONST
       ||curToken->getTag()== CONST_INT||curToken->getTag()== CONST_FLOAT
       ||curToken->getTag()==CONST_STR||curToken->getTag()==CONST_CHAR){
        return true;
    }
    return false;
}

bool Parse::match_type(){
    if(curToken->getTag()==KW_INT||curToken->getTag()== KW_FLOAT
          ||curToken->getTag()== KW_VOID||curToken->getTag()== KW_BOOL
          ||curToken->getTag()== KW_CHAR||curToken->getTag()){
           nextToken();
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
    return true;
}
