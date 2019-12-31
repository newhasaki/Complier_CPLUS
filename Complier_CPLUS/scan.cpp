//
//  scanning.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/1.
//  Copyright © 2019 mac. All rights reserved.
//

#include "scan.hpp"
#include <iostream>
using std::cout;
using std::endl;

Token::Token(TAG tag){
    this->tag = tag;
}

Token::Token(){
}

Token::~Token(){
}

void Token::setTag(TAG tag){
    this->tag = tag;
}

TAG Token::getTag(){
    return tag;
}


//enum TAG{ERR,
//    END,
//    ID,
//    KW_INT,KW_CHAR,KW_VOID,
//    KW_EXTERN,KW_FLOAT,
//    CH,STR,
//    NOT,LEA,
//    ADD,SUB,MUL,DIV,MOD,
//    INC,DEC,CONST,
//    GT,GE,LT,LE,EQU,NEQU,
//    AND,OR,
//    LPAREN,RPAREN,
//    LBRACK,RBRACK,
//    LBRACE,RBRACE,
//    COMMA,COLON,SEMICON,
//    ASSIGN,
//    KW_IF,KW_ELSE,
//    KW_SWITCH,KW_CASE,KW_DEFAULT,
//    KW_WHILE,KW_DO,KW_FOR,
//    KW_BREAK,KW_CONTINUE,KW_RETURN,
//    KW_MAIN
//};

void Token::toPrintf(){
    switch (getTag()) {
        case ID:break;
        case KW_INT:cout<<"KW_INT"<<endl;break;
        case KW_CHAR:cout<<"KW_CHAR"<<endl;break;
        case KW_VOID:cout<<"KW_VOID"<<endl;break;
        case KW_EXTERN:cout<<"KW_EXTERN"<<endl;break;
        case KW_FLOAT:cout<<"KW_FLOAT"<<endl;break;
        case KW_IF:cout<<"KW_IF"<<endl;break;
        case KW_ELSE:cout<<"KW_ELSE"<<endl;break;
        case KW_SWITCH:cout<<"KW_SWITCH"<<endl;break;
        case KW_CASE:cout<<"KW_CASE"<<endl;break;
        case KW_DEFAULT:cout<<"KW_DEFAULT"<<endl;break;
        case KW_WHILE:cout<<"KW_WHILE"<<endl;break;
        case KW_DO:cout<<"KW_DO"<<endl;break;
        case KW_FOR:cout<<"KW_FOR"<<endl;break;
        case KW_BREAK:cout<<"KW_BREAK"<<endl;break;
        case KW_CONTINUE:cout<<"KW_CONTINUE"<<endl;break;
        case KW_RETURN:cout<<"KW_RETURN"<<endl;break;
        case KW_MAIN:cout<<"KW_MAIN"<<endl;break;
        case KW_TRUE_CONST:cout<<"KW_TRUE"<<endl;break;
        case KW_FALSE_CONST:cout<<"KW_FALSE"<<endl;break;
        case CH:break;
        case STR:break;
        case NOT:cout<<"NOT"<<endl;break;
        case LEA:cout<<"LEA"<<endl;break;
        case ADD:cout<<"ADD"<<endl;break;
        case SUB:cout<<"SUB"<<endl;break;
        case MUL:cout<<"MUL"<<endl;break;
        case DIV:cout<<"DIV"<<endl;break;
        case MOD:cout<<"MOD"<<endl;break;
        case INC:cout<<"INC"<<endl;break;
        case DEC:cout<<"DEC"<<endl;break;
        case GT:cout<<"GT"<<endl;break;
        case GE:cout<<"GE"<<endl;break;
        case LT:cout<<"LT"<<endl;break;
        case EQU:cout<<"EQU"<<endl;break;
        case NEQU:cout<<"NEQU"<<endl;break;
        case AND:cout<<"AND"<<endl;break;
        case OR:cout<<"OR"<<endl;break;
        case LPAREN:cout<<"LPAREN"<<endl;break;
        case RPAREN:cout<<"RPAREN"<<endl;break;
        case LBRACE:cout<<"LBRACE"<<endl;break;
        case RBRACE:cout<<"RBRACE"<<endl;break;
        case LBRACK:cout<<"LBRACK"<<endl;break;
        case RBRACK:cout<<"RBRACK"<<endl;break;
        case COMMA:cout<<"COMMA"<<endl;break;
        case COLON:cout<<"COLON"<<endl;break;
        case SEMICON:cout<<"SEMICON"<<endl;break;
        case ASSIGN:cout<<"ASSIGN"<<endl;break;
        default:
            break;
    }
}


Id::Id(){
}

Id::Id(string str):Token(ID){
  
    this->name = str;
}

Id::~Id(){
    
}

void Id::setName(string name){
    this->name = name;
}

string Id::getName(){
    return name;
}

void Id::toPrintf(){
    cout<<"ID :"<<getName()<<endl;
}

Keywords::Keywords(){
    keywords["int"] = KW_INT;
    keywords["char"] = KW_CHAR;
    keywords["void"] = KW_VOID;
    keywords["extern"] = KW_EXTERN;
    keywords["if"] = KW_IF;
    keywords["else"] = KW_ELSE;
    keywords["switch"] = KW_SWITCH;
    keywords["case"] = KW_CASE;
    keywords["default"] = KW_DEFAULT;
    keywords["while"] = KW_WHILE;
    keywords["do"] = KW_DO;
    keywords["for"] = KW_BREAK;
    keywords["continue"] = KW_CONTINUE;
    keywords["return"] = KW_RETURN;
    //keywords["main"] = KW_MAIN;
    keywords["float"] = KW_FLOAT;
    keywords["break"] = KW_BREAK;
    keywords["true"] = KW_TRUE_CONST;
    keywords["false"] = KW_FALSE_CONST;
    keywords["bool"] = KW_BOOL;
}

TAG Keywords::getTag(string name){
    return keywords.find(name)!=keywords.end()?keywords[name]:ID;
}

Keywords::~Keywords(){
}

NUM::NUM(){
    this->val = {0};
}

NUM::NUM(int v):Token(CONST_INT){
    this->val.idata = v;
}

NUM::NUM(float v):Token(CONST_FLOAT){
    this->val.vdata = v;
}

NUM::NUM(char v):Token(CONST_CHAR){
    this->val.cdata = v;
}

NUM::NUM(const char* v):Token(CONST_STR){
    strcpy(this->val.sstr, v);
}

NUM::NUM(bool v){
    if(v)
        setTag(KW_TRUE_CONST);
    else
        setTag(KW_FALSE_CONST);
    
    
    this->val.bdata = v;
}

NUM::~NUM(){
}

void NUM::setInt(int v){
    this->val.idata = v;
}

void NUM::setFloat(float v){
    this->val.vdata = v;
}

int NUM::getInt(){
    return this->val.idata;
}

float NUM::getFloat(){
    return this->val.vdata;
}

void NUM::setChar(char v){
    this->val.cdata = v;
}

void NUM::setStr(char* v){
    strcpy(this->val.sstr, v);
}

char NUM::getChar(){
    return this->val.cdata;
}

char* NUM::getStr(){
    return this->val.sstr;
}

void NUM::setBool(bool v){
    this->val.bdata = v;
}

bool NUM::getBool(){
    return this->val.bdata;
}

VarDataDef NUM::getVarDataDef(){
    return this->val;
}


void NUM::toPrintf(){
    switch (getTag()) {
        case CONST_INT:
            cout<<"CONST_INT :"<< getInt()<<endl;
            break;
        case CONST_FLOAT:
            cout<<"CONST_FLOAT :"<<getFloat()<<endl;
            break;
        default:
            break;
    }
}


//Char::Char(){
//}
//
//Char::Char(char ch):Token(CONST_CHAR){
//    this->ch = ch;
//}
//
//void Char::toPrintf(){
//    cout<<"Char :"<<getChar()<<endl;
//}
//
//char Char::getChar(){
//    return ch;
//}
//
//Char::~Char(){
//}
//
//Str::Str(){
//}
//
//string Str::getStr(){
//    return this->str;
//}
//
//void Str::toPrintf(){
//    cout<<"Str :"<<getStr()<<endl;
//}
//
//Str::~Str(){
//}
//
//Str::Str(string str):Token(CONST_STR){
//    this->str = str;
//}

Scan::Scan(){
}

Scan::~Scan(){
}

bool Scan::isbinopr(char ch){   //检测是否为双目运算符
    nextChar();
    if(*source == ch)
        return true;
    backChar();
    return false;
}

void Scan::setFileSize(long size){
    this->filesize = size;
}

long Scan::getFileSize(){
    return this->filesize;
}

void Scan::nextChar(){
    source++;
}

void Scan::backChar(){
    source--;
}

char Scan::getCurChar(){
    return *source;
}

vector<Token*> Scan::startScan(){
    char* fileend = source + getFileSize();
    vector<Token*> tokens;
    while(source!=fileend){
        Token* token = tokenize();
        if(token!=nullptr)
            tokens.push_back(token);
        nextChar();
    }
    return tokens;
}

void Scan::readFile(string path){
    FILE* fp = fopen(path.c_str(), "r");
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    setFileSize(filesize);
    source = new char[filesize]{0};
    fseek(fp, 0, SEEK_SET);
    fread(source, filesize, 1, fp);
    fclose(fp);
}

//区分浮点和整型
TAG Scan::dstfloat(string str){
    int status = 0;
    
    for(size_t n = 0;n<str.length();n++) {
        if(isdigit(str.at(n)))
        {
            if(status == 0||status == 2){
                status++;
            }
        }
        else if(str.at(n) == '.'){
            if(status == 1)
                status ++;
            else
                return ERR;
        }
    }
    if(status == 1 || status == 2)  return CONST_INT;
    else if(status == 3) return CONST_FLOAT;
    else          return ERR;
}

Token* Scan::tokenize(){
    Token*  ptoken = nullptr;
    while(getCurChar() == ' '||getCurChar()=='\t'||getCurChar() == '\n'){   //去除空格,制表符,换行符
        return ptoken;
    }
    
    //界符
    switch (getCurChar()) {
        case '+': ptoken = new Token(ADD);break;  //ADD
        case '-': ptoken = new Token(SUB);break;  //SU
        case '*': ptoken = new Token(MUL);break;  //MUL
        case '/': ptoken = new Token(DIV);break;  //DIV
        case '%': ptoken = new Token(MOD);break;  //MOD
        case '=':
            ptoken = new Token(ASSIGN);
            ptoken->setTag(isbinopr('=')?EQU:ASSIGN);
            ;break;  //EQU
        case '>':
            ptoken = new Token(GT); //GT
            ptoken->setTag(isbinopr('=')?GE:GT);
            break;
        case '<':
            ptoken = new Token(LT); //LT
            ptoken->setTag(isbinopr('=')?LE:LT);
            break;
        case '!':
            ptoken = new Token(NOT);  //Not
            ptoken->setTag(isbinopr('=')?NEQU:NOT);
            break;
        case '|':
            ptoken = new Token(ERR);
            ptoken->setTag(isbinopr('|')?OR:ERR);   //OR
            break;
        case '&':
            ptoken = new Token(LEA);
            ptoken->setTag(isbinopr('&')?AND:ERR);  //AND
            break;
        case ':': ptoken = new Token(COLON);break;
        case ',': ptoken = new Token(COMMA);break;   //Comma
        case ';': ptoken = new Token(SEMICON);break; //Semicon
        case '(': ptoken = new Token(LPAREN);break;  //Lparen
        case ')': ptoken = new Token(RPAREN);break;  //Rparen
        case '{': ptoken = new Token(LBRACE);break;  //Lbrac
        case '}': ptoken = new Token(RBRACE);break;  //Rbrac
        default: ptoken = new Token(ERR);break;
    }
    
    if(ptoken->getTag() != ERR){
        return ptoken;
    }
    
    //自定义符号
    if(getCurChar() == '_'||
       isalpha(getCurChar())){
        string str;
        do{
            str.push_back(getCurChar());
            nextChar();
        }while(isalnum(getCurChar())|| getCurChar()=='_');
        backChar();
        TAG curTag = keywords.getTag(str);
        
        switch (curTag) {
            case ID:
                ptoken = new Id(str);       //自定义符号
                break;
            case KW_FALSE_CONST:            //bool 常量
                ptoken = new NUM(false);
                break;
            case KW_TRUE_CONST:
                ptoken = new NUM(true);
                break;
            default:
                ptoken = new Token(keywords.getTag(str));   //关键字
                break;
        }
        return ptoken;
    }
    
    //常量数值
    if(isdigit(getCurChar())){
        string str;
        do{
            str.push_back(getCurChar());
            nextChar();
        }while (isdigit(getCurChar())||getCurChar() == '.');
        backChar();
        if(dstfloat(str) == ERR){
            printf("NUM Error!\n");
        }else if(dstfloat(str)==CONST_FLOAT){
            ptoken = new NUM(stringToNum<float>(str));
        }else if(dstfloat(str)== CONST_INT){
            ptoken = new NUM(stringToNum<int>(str));
        }
        return ptoken;
    }
    
    //常量字符
    if(getCurChar() == '\''){
        nextChar();
        char ch = getCurChar();
        if(isalnum(ch)){
            nextChar();
            if(getCurChar() == '\''){
                ptoken = new NUM(ch);
            }else{
                printf("expected \'");
            }
        }
        return ptoken;
    }
    
    //常量字符串
    if(getCurChar() == '\"'){
        nextChar();
        string str;
        if(isalpha(getCurChar())){
            do{
                str.push_back(getCurChar());
                nextChar();
            }while(isalpha(getCurChar()));
            backChar();
            if(getCurChar() == '\"'){
                ptoken = new NUM(str.c_str());
                return ptoken;
            }else{
                printf("expected \"\n");
            }
        }
    }
    
    return ptoken;
}


