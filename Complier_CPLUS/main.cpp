//
//  main.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/1.
//  Copyright © 2019 mac. All rights reserved.
//


//准备错误处理
#include "scan.hpp"
#include "parse.hpp"
int main(int argc, const char * argv[]) {
    Scan scan;
    scan.readFile("/Users/mac/Desktop/testbuild/test.c");
    vector<Token*> tokens = scan.startScan();
    
    Token* endToken = new  Token(ERR);
    tokens.push_back(endToken);
    
    Parse parse(tokens);
    parse.startParse();
    
    
    return 0;
}   
