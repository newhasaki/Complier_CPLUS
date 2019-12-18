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
    void syn_fundef();      //函数定义
    void syn_fundeclare();  //函数声明
    void syn_statement();   //函数体;
    void syn_program();
    Symbols* syn_paralist();
    SymDeclare* syn_parameters(std::string varname,TAG datatype);
    SymDeclare* syn_parameters_init(std::string varname,TAG datatype);
    SymDeclare* syn_vardefine(std::string varname,TAG datatype);
    SymDeclare* syn_vardeclare(std::string varname,TAG datatype);
    void syn_datalist();
    void syn_ifstat();
    ExpNode* syn_exp();
    void syn_block();
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
    vector<Token*> mTokens;
    Token* curToken;
    size_t mindex;              //向前看指针
    vector<Symbols*> symbolStack;
    Symbols* curSymbol;
};



#endif /* parse_hpp */
