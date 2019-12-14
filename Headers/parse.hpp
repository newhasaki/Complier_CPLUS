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
    void syn_localdef(vector<VarDeclare*>*,TAG datatype);//变量定义
    void syn_type(vector<VarDeclare*>*);
    void syn_id(vector<VarDeclare*>*,TAG datatype);
    void syn_fundef();      //函数定义
    void syn_fundeclare();  //函数声明
    void syn_statement(vector<VarDeclare*>*);   //函数体;
    void syn_program();
    void syn_paralist(vector<VarDeclare*>*);
    void syn_varinit(vector<VarDeclare*>*,std::string varname,TAG datatype);
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
    std::vector<vector<Symbols*>*> symbolStack;
    vector<Symbols*>* curSymbol;
    
    
};



#endif /* parse_hpp */
