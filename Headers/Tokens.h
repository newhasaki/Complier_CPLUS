//
//  Tokens.h
//  Complier_CPLUS
//
//  Created by mac on 2019/12/3.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef Tokens_h
#define Tokens_h

#include <stdio.h>

#define MAX 256

union VarDataDef{
    int idata;
    float vdata;
    char cdata;
    char sstr[MAX];
    bool bdata;
};

enum TAG{ERR,
    END,
    ID,
    KW_INT,KW_CHAR,KW_VOID,
    KW_EXTERN,KW_FLOAT,
    KW_TRUE_CONST,KW_FALSE_CONST,KW_BOOL,
    CH,STR,
    NOT,LEA,
    ADD,SUB,MUL,DIV,MOD,
    INC,DEC,
    GT,GE,LT,LE,EQU,NEQU,
    AND,OR,
    LPAREN,RPAREN,
    LBRACK,RBRACK,
    LBRACE,RBRACE,
    COMMA,COLON,SEMICON,
    ASSIGN,
    KW_IF,KW_ELSE,
    KW_SWITCH,KW_CASE,KW_DEFAULT,
    KW_WHILE,KW_DO,KW_FOR,
    KW_BREAK,KW_CONTINUE,KW_RETURN,
    KW_MAIN,
    CONST_INT,CONST_FLOAT,CONST_CHAR,CONST_STR
};

#endif /* Tokens_h */
