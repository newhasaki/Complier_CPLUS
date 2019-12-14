//
//  scanning.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/1.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef scanning_hpp
#define scanning_hpp
#include "tools.hpp"
#include <string>
#include <map>
#include <vector>
#include "Tokens.h"

using std::vector;
using std::string;
using std::map;

class Token
{
public:
    Token();
    Token(TAG t);
    virtual ~Token();
public:
    void setTag(TAG tag);
    TAG getTag();
    virtual void toPrintf();
private:
    TAG tag;
};

class Id:public Token
{
private:
    Id();
public:
    Id(string n);
    ~Id();
public:
    void setName(string name);
    string getName();
    virtual void toPrintf();
private:
    string name;
};

class NUM:public Token
{
private:
    NUM();
public:
    NUM(int v);
    NUM(float v);
    NUM(char v);
    NUM(const char* v);
    NUM(bool v);
    ~NUM();
public:
    void setInt(int v);
    void setFloat(float v);
    void setChar(char v);
    void setStr(char* v);
    void setBool(bool v);
    bool getBool();
    int getInt();
    float getFloat();
    char getChar();
    char* getStr();
    virtual void toPrintf();
    VarDataDef getVarDataDef();
private:
    VarDataDef val;
};

//class Char:public Token
//{
//private:
//    Char();
//public:
//    Char(char c);
//    ~Char();
//public:
//    virtual void toPrintf();
//    char getChar();
//private:
//    char ch;
//};

//class Str:public Token
//{
//private:
//     Str();
//public:
//    Str(string s);
//    ~Str();
//public:
//    string getStr();
//    virtual void toPrintf();
//private:
//    string str;
//};

class Keywords
{
public:
    Keywords();
    ~Keywords();
private:
    map<string,TAG> keywords;
public:
    TAG getTag(string name);
};

class Scan{
public:
    Scan();
    ~Scan();
public:
    vector<Token*> startScan();
    void readFile(string path);
    Token* tokenize();
private:
    bool isbinopr(char ch);
    TAG dstfloat(string str);   
    void nextChar();
    void backChar();
    char getCurChar();
    void setFileSize(long size);
    long getFileSize();
private:
    char* source;
    long filesize;
    Keywords keywords;
};



#endif /* scanning_hpp */
