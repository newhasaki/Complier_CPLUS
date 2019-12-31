//
//  main.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/1.
//  Copyright © 2019 mac. All rights reserved.
//

#include "scan.hpp"
#include "parse.hpp"
#include "semantics.hpp"

int main(int argc, const char * argv[]) {
    Scan scan;
    scan.readFile("/Users/mac/Desktop/testbuild/mas.cpp");
    vector<Token*> tokens = scan.startScan();
    
    Token* endToken = new  Token(ERR);
    tokens.push_back(endToken);
    
    Parse parse(tokens);
    parse.startParse();
    //parse.printAST_BFS();
    parse.printAST_DFS();
    vector<Symbols*> symbolStack = parse.getSymbolStack();


    Semantics semantic(parse.getFunSymTab());
    semantic.start();
    
    return 0;
}   
