//
//  parse.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/29.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include "scan.hpp"
#include "symbols.hpp"
#include <stack>
#include "Tokens.h"

class Parse{
private:
    Parse(){}
public:
    Parse(vector<Token*>&);
    void startParse();
private:
    void syn_localvar(TAG datatype);//变量定义
    void syn_type();
    void syn_id(TAG datatype);
    FunDef*  syn_fundef(string name,TAG datatype,Symbols* paralist);      //函数定义
    FunDeclare*  syn_fundeclare(string name,TAG datatype,Symbols* paralist);  //函数声明
    void syn_statement();   //函数体;
    void syn_program();
    Symbols* syn_paralist();
    SymDeclare* syn_parameters(string varname,TAG datatype);
    SymDeclare* syn_parameters_init(string varname,TAG datatype);
    SymDeclare* syn_vardefine(string varname,TAG datatype);
    SymDeclare* syn_vardeclare(string varname,TAG datatype);
    void syn_datalist();
    void syn_if_else_stat();
    ExpNode* syn_exp();
    void ready_entry_funblock(Symbols* paralist);
    void syn_block();
    void ready_leave_funblock();
    bool syn_isRedefinition(const string& name);
    bool syn_symbols_check(const string& name,PARSETYPE parsetype);
    bool syn_varcheck();
    bool syn_funcheck();
    
    ExpNode* syn_bool_or();
    ExpNode* syn_bool_and();
    ExpNode* syn_bool_compare();
    ExpNode* syn_factor();
    ExpNode* syn_bool_exp();
    ExpNode* syn_add_sub();
    ExpNode* syn_mul_div();
    bool checkRepeatFunDeclare(SymDeclare* sym);
    void add_type_value(VarDataDef* vdd);
    bool match(TAG tag);
    bool match_const();
    bool match_type();
private:
    void nextToken();
private:
    vector<SymDeclare*> symDeclares;
    vector<Token*> mTokens;
    Token* curToken;
    size_t mindex;              //向前看指针
    vector<Symbols*> symbolStack;
    Symbols* curSymbol;
};



#endif /* parse_hpp */
