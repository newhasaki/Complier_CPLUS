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
    
    Symbols* rootSymbol = new Symbols(); //创建符号表
    symbolStack.push_back(rootSymbol);
    
    SymDeclare* rootBlock = new SymDeclare();
    rootBlock->setName("TranslationUnitDecl");
    rootBlock->m_symbols = rootSymbol;
    blockStack.push_back(rootBlock);
    
    curFundef = nullptr;
}

void Parse::setTokens(vector<Token *> tokens){
    mTokens = tokens;
}

vector<Symbols*> Parse::getSymbolStack(){
    return symbolStack;
}

vector<SymDeclare*>  Parse::startParse(){
    syn_program();
    return  symDeclares;
}

void Parse::syn_program(){
    while(curToken->getTag()!=ERR){
        syn_type();
    }
}

void Parse::syn_localvar(TAG datatype){
    nextToken();
    while(curToken->getTag()!=SEMICON){
        if(match(ID)){
            Id* id = dynamic_cast<Id*>(curToken);
            nextToken();
            if(match(ASSIGN)){
                if(match(ID)||match_const()){
                    if(!syn_isRedefinition(id->getName())){     //检测变量是否重定义
                        
                        VarDef* vardef = syn_vardefine(id->getName(),datatype);
                        SymDeclare* curBlock = getCurBlock();      //收集块内语句信息
                        curBlock->m_symbols->insert(id->getName(),vardef);        //收集符号信息
                        curBlock->m_action.push_back(vardef);
//                        if(curBlock->getParseType()==FUNDEFINE){
//
//                            FunDef* fundef = dynamic_cast<FunDef*>(curBlock);
//                            fundef->m_action.push_back(vardef);
//
//                        }else if(curBlock->getParseType()==IFSTAT){
//                            IfStat* ifstat = dynamic_cast<IfStat*>(curBlock);
//                            ifstat->m_action.push_back(ifstat);
//                        }
                        curFundef->m_allLocal.push_back(vardef);          //收集本函数内所有变量信息(包括不同作用域)
                        
                    }else{
                        printf("%s is Redefinition\n",id->getName().c_str());
                    }
                }
                if(match(SEMICON))
                    break;
                if(match(COMMA))
                    continue;
            }else if(match(SEMICON)){
                if(!syn_isRedefinition(id->getName())){
                    VarDef* vardef = syn_vardeclare(id->getName(),datatype);
                    
                    SymDeclare* curBlock = getCurBlock();      //收集块内语句信息
                    curBlock->m_symbols->insert(id->getName(),vardef);        //收集符号信息
                    if(curBlock->getParseType()==FUNDEFINE){
                        FunDef* fundef = dynamic_cast<FunDef*>(curBlock);
                        fundef->m_action.push_back(vardef);
                            
                    }else if(curBlock->getParseType()==IFSTAT){
                        IfStat* ifstat = dynamic_cast<IfStat*>(curBlock);
                        ifstat->m_action.push_back(ifstat);
                    }
                    curFundef->m_allLocal.push_back(vardef);
                }else{
                    printf("%s is Redefinition\n",id->getName().c_str());
                }
                break;
            }else if(match(COMMA)){
                if(!syn_isRedefinition(id->getName())){
                    VarDef* vardef = syn_vardeclare(id->getName(),datatype);
                    SymDeclare* curBlock = getCurBlock();      //收集块内语句信息
                    curBlock->m_symbols->insert(id->getName(),vardef);        //收集符号信息
                    if(curBlock->getParseType()==FUNDEFINE){
                        FunDef* fundef = dynamic_cast<FunDef*>(curBlock);
                        fundef->m_action.push_back(vardef);
                            
                    }else if(curBlock->getParseType()==IFSTAT){
                        IfStat* ifstat = dynamic_cast<IfStat*>(curBlock);
                        ifstat->m_action.push_back(ifstat);
                    }
                    curFundef->m_allLocal.push_back(vardef);
                }else{
                    printf("%s is Redefinition\n",id->getName().c_str());
                }
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

vector<SymDeclare*>* Parse::syn_paralist(){
    vector<SymDeclare*>* paralist = new vector<SymDeclare*>();
    while(true){
        TAG datatype = curToken->getTag();
        if(match_type()){
            if(match(ID)){
                Id* id = dynamic_cast<Id*>(curToken);
                SymDeclare* symbol = nullptr;
                nextToken();
                if(match(COMMA)){
                    symbol = syn_parameters(id->getName(),datatype);
                    paralist->push_back(symbol);
                }else if(match(RPAREN)){
                    symbol = syn_parameters(id->getName(),datatype);
                    paralist->push_back(symbol);
                    return paralist;
                }else if(match(ASSIGN)){
                    if(match_const()){
                        symbol = syn_parameters_init(id->getName(),datatype);
                        paralist->push_back(symbol);
                        while(true){
                            if(match(COMMA)){
                                TAG datatype = curToken->getTag();
                                if(match_type()){
                                    if(match(ID)){
                                        Id* id = dynamic_cast<Id*>(curToken);
                                        if(match(ASSIGN)){
                                            if(match_const()){
                                                symbol = syn_parameters_init(id->getName(), datatype);
                                                paralist->push_back(symbol);
                                            }
                                        }
                                    }
                                }
                            }else if(match(RPAREN)){
                                return paralist;
                            }
                        }
                    }
                }
            }else if(match(COMMA)){
                VarDef* var_declare = new VarDef("undefine_varname",VARDECLARE,datatype,nullptr);
                paralist->push_back(var_declare);
            }else if(match(RPAREN)){
                return paralist;
            }
        }else{
            printf("ERROR\n");
        }
    }
    return paralist;
}

void Parse::syn_id(TAG datatype){
    if(curToken->getTag() == ID ){
        Id* id = dynamic_cast<Id*>(curToken);
        nextToken();
        if(match(EQU)){
            syn_vardefine(id->getName(),datatype);
        }else if(match(LPAREN)){
            vector<SymDeclare*>* paralist = syn_paralist();
            
            if(match(LBRACE)){
                if(!syn_isRedefinition(id->getName())){
                    FunDef* fundef = syn_fundef(id->getName(),datatype,paralist);
                    funSymbolTab.insert(std::make_pair(id->getName(),fundef));
                    
                    
                    SymDeclare* curBlock = getCurBlock();
                    
                    //curBlock->m_symbols->insert(id->getName(), fundef);
                    curBlock->m_action.push_back(fundef);
                    
                    curFundef = fundef;
                    //fundef->sym_print();    //打印函数定义信息
                    
                    //SymDeclare* curBlock = getCurBlock();
                    //fundef->symbols->insert(id->getName(),fundef);
                    
                    ready_entry_funblock(paralist,fundef);     //将函数参数加入当前作用域符号表
                    syn_block();
                    ready_leave_funblock();

                }else{
                    printf("%s is Redefinition\n",id->getName().c_str());
                }
            }else if(match(SEMICON)){
                if(!syn_isRedefinition(id->getName())){
                    FunDeclare* fundeclare = syn_fundeclare(id->getName(), datatype, paralist);
                    funSymbolTab.insert(std::make_pair(id->getName(), fundeclare));
                    
                    //fundeclare->sym_print();    //打印函数声明信息
                    //fundeclare->m_symbols->insert(id->getName(), fundeclare);
                    //symDeclares.push_back(fundeclare);
                }else{
                    printf("%s is Redefinition\n",id->getName().c_str());
                }
            }
        }else{
            VarDef* var_declare  = new VarDef(id->getName(),VARDECLARE,datatype,nullptr);
            SymDeclare* curBlock = getCurBlock();
            curBlock->m_symbols->insert(id->getName(),var_declare);     //加入当前符号表，因为参数也在当前作用域内
            
            curFundef->m_allLocal.push_back(var_declare);
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
VarDef* Parse::syn_vardefine(std::string varname,TAG datatype){
    ExpNode* rootNode = syn_exp();
    VarDef* var_define = new VarDef(varname, VARDEFINE,datatype,rootNode);
    return var_define;
}

/*
变量声明
*/
VarDef* Parse::syn_vardeclare(std::string varname,TAG datatype){
    VarDef* var_declare = new VarDef(varname,VARDEFINE,datatype,nullptr);
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

void Parse::syn_case(){
    if(!while_switch_Stacks.empty()){
        SymDeclare* symdeclare = while_switch_Stacks.at(while_switch_Stacks.size()-1);
        if(symdeclare->getParseType() == SWITCHDECLARE){
        Switch* sch = dynamic_cast<Switch*>(symdeclare);
            if(match_const()){
                NUM* num = dynamic_cast<NUM*>(curToken);
                if(match(COLON)){
                    if(match(LBRACE)){
//                        entry_block();
//                        syn_block();
//                        leave_block();
                    }
                }
            }
        }
    }
}

void Parse::syn_default(){
//    Label* default_label = new Label();
//    symDeclares.push_back(default_label);
    if(!while_switch_Stacks.empty()){
          SymDeclare* symdeclare = while_switch_Stacks.at(while_switch_Stacks.size()-1);
          if(symdeclare->getParseType() == SWITCHDECLARE){
          Switch* sch = dynamic_cast<Switch*>(symdeclare);
              if(match(COLON)){
//                  if(sch->getDefaultLabel()==nullptr){
//                            sch->createDefaultLabel(default_label);
//                  }else{
//                      printf("Redefinition default in switch\n");
//                  }
              }
          }
    }
}

void Parse::syn_switch(){
    nextToken();
    
    Switch* sch = new Switch();
    
    while_switch_Stacks.push_back(sch);
    //Label* endLabel = new Label();
    
    if(match(LPAREN)){
        
    }
    
    sch->setExp(syn_exp());
    
    if(match(RPAREN)){
        
    }
    
    if(match(LBRACE)){
        
    }
    entry_block(sch);
    syn_block();
    leave_block();
    
    //symDeclares.push_back(endLabel);
}

void Parse::syn_do_while(){
    if(match(KW_DO)){
        DoWhile* dowhile = new DoWhile();
        //curFundef->m_symdeclare.push_back(dowhile);
        while_switch_Stacks.push_back(dowhile);
        
        if(match(LBRACE)){
            entry_block(dowhile);
            syn_block();
            leave_block();
            if(match(KW_WHILE)){
                
            }
            
            if(match(LPAREN)){
               dowhile->setExp(syn_exp());
            }
            
            if(match(RPAREN)){
                
                //symDeclares.push_back(endLabel);
            }
            
            if(match(SEMICON)){
                
            }
        }
    }
}

void Parse::syn_if_else_stat(){
    nextToken();
    if(match(LPAREN)){
        
    }else{
        printf("except (\n");
    }
    
    IfStat* ifstat = new IfStat();
    
    SymDeclare* curBlock = getCurBlock();
    
    curBlock->m_action.push_back(ifstat);
    
    //判断当前块
    
    ifstat->setExp(syn_exp());
    
    if(match(RPAREN))
    {
        
    }else{
        printf("except )\n");
    }
    
    if(match(LBRACE)){
        entry_block(ifstat);
        syn_block();
        leave_block();
        
        if(match(RBRACE)){
            nextToken();
        }
        //else
        if(match(KW_ELSE)){
            ElseStat* elsestat = new ElseStat();
            curBlock = getCurBlock();
            curBlock->m_action.push_back(elsestat);
            
            if(match(LBRACE)){
                entry_block(elsestat);
                syn_block();
                leave_block();
            }
            
            if(match(RBRACE)){
                nextToken();
            }
            
        }else{
             //symDeclares.push_back(first_label);   //记录标签
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
        Id* id = dynamic_cast<Id*>(curToken);
        if(!syn_isUndefinition(id->getName(),VARDEFINE)){
            if(syn_isUnInitialization(id->getName()))
                printf("warning %s unInitialization\n",id->getName().c_str());      //检测变量是否初始化
                          
            ExpNode* id_node = new ExpNode();
            id_node->left = nullptr;
            id_node->right = nullptr;
            id_node->setNodeType(curToken->getTag());
            
            id_node->setVarName(id->getName());
            nextToken();
            return id_node;
        }else{
            printf("%s undefinition\n",id->getName().c_str());
        }
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

/*
 进入函数之前需要将参数传进当前作用域的符号表
 */
void Parse::ready_entry_funblock(vector<SymDeclare*>* paralist,SymDeclare* symdeclare){
    entry_block(symdeclare);
    FunDef* fundef = dynamic_cast<FunDef*>(symdeclare);
    
    for(size_t i=0;i<paralist->size();++i){
        fundef->m_symbols->insert(paralist->at(i)->getName(), paralist->at(i));
    }
}

void Parse::entry_block(SymDeclare* symdeclare){
    blockStack.push_back(symdeclare);
    //symdeclare->symbols = new Symbols(); //创建符号表
    symbolStack.push_back(symdeclare->m_symbols);
}

void Parse::leave_block(){
    blockStack.pop_back();
    if(symbolStack.size()!=1)
        symbolStack.pop_back();
}

void Parse::syn_block(){
    do{
        syn_statement();
    }while(!match(RBRACE));
}

void Parse::ready_leave_funblock(){
    leave_block();
}

FunDef* Parse::syn_fundef(string name,TAG datatype,vector<SymDeclare*>* paralist){
    FunDef* fundef = new FunDef(name,FUNDEFINE,datatype);  //函数定义
    fundef->paralist = paralist;
    fundef->m_symbols = new Symbols();
    return fundef;
}

FunDeclare*  Parse::syn_fundeclare(string name, TAG datatype,vector<SymDeclare*>* paralist){
    FunDeclare* fundeclare = new FunDeclare(name,FUNDECLARE,datatype); //函数声明
    fundeclare->paralist = paralist;
    return fundeclare;
}

FunCall* Parse::syn_funcall(string name,TAG datatype,vector<SymDeclare*>* paralist){
    FunCall* funcall = new FunCall(name,FUNCALL,datatype);
    funcall->paralist = paralist;
    return funcall;
}

DoWhile* Parse::syn_genDoWhile(){
    return nullptr;
}

void Parse::syn_genReturn(){
    
    Return* retObject = new Return();
    //curFundef->m_symdeclare.push_back(retObject);
    
    nextToken();
    retObject->setDataType(curFundef->getDataType());
    retObject->setRetValue(syn_exp());
 
    if(match(SEMICON)){
        
    }
}

void Parse::syn_genBreak(){
    if(!while_switch_Stacks.empty()){
        SymDeclare* symdeclare = while_switch_Stacks.at(while_switch_Stacks.size()-1);
        Break* brk = new Break();
        if(symdeclare->getParseType() == DOWHILE){
            DoWhile* dowhile = dynamic_cast<DoWhile*>(symdeclare);
            //brk->setGotoEndLabel(dowhile->getEndLabel());
        }else if(symdeclare->getParseType() == CASEDECLARE){
            Switch* sch = dynamic_cast<Switch*>(symdeclare);
            //brk->setGotoEndLabel(sch->getEndLabel());
        }
        symDeclares.push_back(brk);
    }else{
        printf("break not in loop or switch\n");
    }
    nextToken();
    if(match(SEMICON)){
        
    }
}

void Parse::syn_genContinue(){
    if(!while_switch_Stacks.empty()){
        for(size_t i = 0;i < while_switch_Stacks.size();++i){
            SymDeclare* symdeclare = while_switch_Stacks.at(i);
            if(symdeclare->getParseType() == DOWHILE){
                DoWhile* dowhile = dynamic_cast<DoWhile*>(symdeclare);
                Continue* contie = new Continue();
                //contie->setGotoHeadLabel(dowhile->getHeadLabel());
                //symDeclares.push_back(contie);
                break;
            }
        }
    }else{
        printf("continue not in loop\n");
    }
    nextToken();
    if(match(SEMICON)){
        
    }
}

void Parse::syn_genCallVar(Id* id){
    string name = id->getName();
    
    if(!syn_isUndefinition(name,VARDEFINE)){
        SymDeclare*  sym_declare = syn_getTargetInfo(name,VARDEFINE);
        ExpNode* rootNode = syn_exp();
        VarCall* varcall = new VarCall(name,VARCALL,sym_declare->getDataType(),rootNode);
        //symDeclares.push_back(varcall);
    }else{
        printf("%s undefinition\n",name.c_str());
    }
}

void Parse::syn_genCallFun(Id* funid){
    SymDeclare* sym_declare = syn_getTargetInfo(funid->getName(),FUNDECLARE);
    Fun* fun = dynamic_cast<Fun*>(sym_declare);
    vector<SymDeclare*>* paraInfos = fun->getParaInfo();
    
    vector<SymDeclare*>* paralist = new vector<SymDeclare*>();
    
    string argvNameFlag = "X";
    int argvindex = 0;
    string argvName;
    if(match(ID)||match_const()){
        argvName = argvNameFlag + itos(argvindex);
        SymDeclare* argv_first = paraInfos->at(argvindex);
        VarDef* argument = syn_vardefine(argvName, argv_first->getDataType());  //参数的构造只需要获取函数参数类型就行
        paralist->push_back(argument);
        argvindex++;
        
        while(!match(RPAREN)){
            if(match(COMMA)){
                if(match(ID)||match_const()){
                    argvName = argvNameFlag + itos(argvindex);
                    SymDeclare* argv_next = paraInfos->at(argvindex);
                    argument = syn_vardefine(argvName, argv_next->getDataType());
                    paralist->push_back(argument);
                    argvindex++;
                }
            }
        }
        
        FunCall* funcall = new FunCall(funid->getName(),FUNCALL,fun->getDataType());
        funcall->paralist = paralist;
    }else if(match(RPAREN)){
        if(paraInfos->size()!=0){
            printf("%s except %ld argv\n", funid->getName().c_str(),paraInfos->size());
        }
    }
}

void Parse::syn_statement(){
    
    switch (curToken->getTag()) {               //在函数体中中以数据类型开始的关键字只有变量定义和变量声明
        case KW_INT:
        case KW_FLOAT:
        case KW_VOID:
        case KW_CHAR:
        case KW_BOOL:syn_localvar(curToken->getTag());break;
        case KW_IF:syn_if_else_stat();break;
        case KW_SWITCH:syn_switch(); break;
        case KW_CASE:syn_case();break;
        case KW_DEFAULT:syn_default(); break;
        case KW_DO:syn_do_while();break;
        case KW_BREAK:syn_genBreak(); break;
        case KW_CONTINUE:syn_genContinue(); break;
        case KW_FOR:break;
        case KW_WHILE:break;
        case KW_RETURN:syn_genReturn(); break;
        
       default:
           break;
   }
    
    if(match(ID)){
        Id* id = dynamic_cast<Id*>(curToken);
        nextToken();
        if(match(LPAREN)){
            if(!syn_isUndefinition(id->getName(),VARDEFINE)){
                syn_genCallFun(id);      //函数调用
            }
        }else if(match(ASSIGN)){
            syn_genCallVar(id);        //变量调用
        }
        if(!match(SEMICON)){
            printf("except ;\n");
        }
    }
}

SymDeclare* Parse::syn_getTargetInfo(const string& name,PARSETYPE parsetype){
    for(size_t i = 0;i<symbolStack.size();++i){
         
         Symbols* sym = symbolStack.at(i);
         SymDeclare* symdeclare = sym->find(name);
         
         if(symdeclare!=nullptr){
             if(symdeclare->getParseType()==FUNDECLARE){
                 FunDeclare* fundeclare = dynamic_cast<FunDeclare*>(symdeclare);
                 readyDeclareFunNames.push_back(name);
                 return fundeclare;
             }else if(symdeclare->getParseType()==FUNDEFINE){
                 FunDef* fundef = dynamic_cast<FunDef*>(symdeclare);
                 return fundef;
             }else if(symdeclare->getParseType()==VARDEFINE){
                 VarDef* vardef = dynamic_cast<VarDef*>(symdeclare);
                 return vardef;
             }
         }
     }
    return nullptr;
}

bool Parse::syn_isUndefinition(const string& name,PARSETYPE parsetype){
    SymDeclare* sym_fundeclare = syn_getTargetInfo(name,parsetype);
    return  sym_fundeclare == nullptr?true:false;
}

bool Parse::syn_paralist_check(const string& name){
    
//    vector<TAG> callfun_paralist;       //获取函数调用所填参数
//    if(match(ID)){
//        Id* id = dynamic_cast<Id*>(curToken);
//        SymDeclare* argsym = syn_getTargetInfo(id->getName(),VARDEFINE);
//        TAG argtype = argsym->getDataType();
//        callfun_paralist.push_back(argtype);
//        while(!match(RPAREN)){
//            if(match(COMMA)){
//                if(match(ID)){
//                    id = dynamic_cast<Id*>(curToken);
//                    argsym = syn_getTargetInfo(id->getName(),VARDEFINE);
//                    argtype = argsym->getDataType();
//                    callfun_paralist.push_back(argtype);
//                }
//            }
//        }
//    }
//
//    SymDeclare* symdeclare = syn_getTargetInfo(name,VARDEFINE);   //获取声明的函数的参数
//    Fun* fun = dynamic_cast<Fun*>(symdeclare);
//    vector<SymDeclare*> paralist = fun->paralist;
//
//
//    for(unordered_map<string,SymDeclare*>::iterator it = parainfo->begin();it!=parainfo->end();it++){
//        fundef_paralist.push_back(it->second->getDataType());
//    }
//
//    bool isFitPara = false;
//    if(callfun_paralist.size()==fundef_paralist.size()){            //比较
//        for(size_t i = 0; i<callfun_paralist.size();++i){
//            if(callfun_paralist.at(i) == fundef_paralist.at(i)){
//                isFitPara = true;
//            }else{
//                isFitPara = false;
//                break;
//            }
//        }
//    }
    return true;
}

bool Parse::syn_symbols_check(const std::string& name,PARSETYPE parsetype){
    return true;
}

bool Parse::syn_isRedefinition(const string& name){
    Symbols* curSymbol = getCurSymbol();
    return curSymbol->find(name)==nullptr?false:true;
}

bool Parse::syn_isUnInitialization(const string& name){
    Symbols* curSymbol = getCurSymbol();
    SymDeclare* symdeclare = curSymbol->find(name);
    VarDef* vardef = dynamic_cast<VarDef*>(symdeclare);
    return vardef->getValue() == nullptr?true:false;
}

bool Parse::match(TAG tag){
    
    if(curToken->getTag()==tag){
        if(curToken->getTag()==ID ||match_const()||curToken->getTag()==RBRACE){
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

SymDeclare* Parse::getCurBlock(){
    return blockStack.at(blockStack.size()-1);
}

Symbols* Parse::getCurSymbol(){
    return symbolStack.at(symbolStack.size()-1);
}

void Parse::printAST_BFS(){
    for(map<string,SymDeclare*>::iterator it = funSymbolTab.begin(); it!=funSymbolTab.end();++it){
        FunDef*  fundef = dynamic_cast<FunDef*>(it->second);
        cout<<fundef->getName()<<endl;
        
        queue<vector<SymDeclare*>> qe_list;         //广度遍历 BFS
        
        if(!fundef->m_action.empty()){
            vector<SymDeclare*> temp = fundef->m_action;
            qe_list.push(temp);
            
            while(!qe_list.empty()){
                vector<SymDeclare*> nodes =qe_list.front();
                qe_list.pop();
                for(size_t i=0;i<nodes.size();++i){
                    cout<<nodes.at(i)->getName()<<endl;
                    if(nodes.at(i)->getParseType()==IFSTAT
                       ||nodes.at(i)->getParseType()==ELSESTAT){
                        qe_list.push(nodes.at(i)->m_action);
                    }
                }
            }
        }
    }
}

void Parse::printAST_DFS(){
     for(map<string,SymDeclare*>::iterator it = funSymbolTab.begin(); it!=funSymbolTab.end();++it){
         FunDef*  fundef = dynamic_cast<FunDef*>(it->second);
         cout<<fundef->getName()<<endl;
         if(!fundef->m_action.empty()){
             vector<SymDeclare*> temp = fundef->m_action;
             DFS(temp);
         }
         
     }
}

void Parse::DFS(vector<SymDeclare *> ves){
    for(size_t i = 0;i<ves.size();++i){
        if(ves.at(i)->getParseType()== VARDEFINE){
            cout<<ves.at(i)->getName()<<endl;
        }else if(ves.at(i)->getParseType()==FUNDEFINE
                 ||ves.at(i)->getParseType()==IFSTAT
                 ||ves.at(i)->getParseType()==ELSESTAT){
            cout<<ves.at(i)->getName()<<endl;
            DFS(ves.at(i)->m_action);
        }
    }
}

void Parse::nextToken(){
    curToken = mTokens.at(mindex++);
}

map<string,SymDeclare*>* Parse::getFunSymTab(){
    return &funSymbolTab;
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
