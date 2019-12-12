//
//  tools.hpp
//  Complier_CPLUS
//
//  Created by mac on 2019/11/6.
//  Copyright © 2019 mac. All rights reserved.
//

#ifndef tools_hpp
#define tools_hpp

#include <string>
#include <sstream>
#include "Tokens.h"
using std::string;
using std::istringstream;

//外部模板函数不支持头文件和源文件分离
//字符串转数值类型
template<typename Type>
Type stringToNum(const string& str){
    istringstream iss(str);
    Type num;
    iss >>num;
    return num;
}

#endif /* tools_hpp */
