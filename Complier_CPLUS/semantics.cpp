//
//  semantics.cpp
//  Complier_CPLUS
//
//  Created by mac on 2019/12/26.
//  Copyright © 2019 mac. All rights reserved.
//

#include "semantics.hpp"

Semantics::Semantics(){
    
}

Semantics::~Semantics(){
    
}

void Semantics::setSymbolStack(vector<Symbols*> symbolStack){
    m_symbolStack = symbolStack;
}

Semantics::Semantics(map<string,SymDeclare*>* funSymbolTab,vector<Symbols*> symbolStack){
    this->funSymbolTab = funSymbolTab;
    this->m_symbolStack = symbolStack;
    
    memory_alloca[KW_INT] = allocas{"i32","4"};
    memory_alloca[CONST_INT] = allocas{"i32","4"};
    
    memory_alloca[KW_FLOAT] = allocas{"float","4"};
    memory_alloca[CONST_FLOAT] = allocas{"float","4"};
    
    memory_alloca[KW_CHAR] = allocas{"i8","1"};
    memory_alloca[KW_BOOL] = allocas{"i8","1"};
    
    typeToStr[KW_INT] = "int";
    typeToStr[KW_FLOAT] = "float";
    typeToStr[KW_BOOL] = "bool";
    typeToStr[KW_CHAR] = "char";
    
    typeToStr[CONST_INT] = "IntegerLiteral 'int' ";
    typeToStr[CONST_FLOAT] = "FloatingLiteral 'float' ";
    typeToStr[CONST_STR] = "StrLiteral 'string' ";
    typeToStr[CONST_CHAR] = "CharLiteral 'char' ";
    typeToStr[ID] = "DeclRefExpr ";
    typeToStr[ADD] = "+";
    typeToStr[SUB] = "-";
    typeToStr[MUL] = "*";
    typeToStr[DIV] = "/";
    //typeToStr[ADD] = "BinaryOperator"
}

void Semantics::genReturnDefine(SymDeclare* symdeclare){
    Return* retobject = dynamic_cast<Return*>(symdeclare);
    
}

void Semantics::genVarDefine(SymDeclare* symdeclare){
    VarDef* vardef = dynamic_cast<VarDef*>(symdeclare);
    string getAssName = cur_nmi->find(vardef->getName())->second;
    if(vardef->getParseType() == VARDEFINE){        //初始化
        ExpNode* value = postorder_traversal(vardef->getValue());
        if(value->getNodeType() == CONST_INT){
            if(vardef->getDataType() == KW_INT){
                cout<<"store i32 "+itos(value->getValue().idata)
                +", i32* "+cur_nmi->find(vardef->getName())->second+", align 4"<<endl;
            }
        }
    }
}

void Semantics::genFunEntry(SymDeclare *symdeclare){
    
    map<string,string>* name_mapping_index = new map<string,string>();
    cur_nmi = name_mapping_index;
    nmiStack.push_back(name_mapping_index);
    
    FunDef* fundef = dynamic_cast<FunDef*>(symdeclare);
    vector<SymDeclare*>* paralist = fundef->paralist;
    TAG retValue = fundef->getDataType();
    cout<<"define " + memory_alloca[retValue].datatype +" @" + fundef->getName();
    cout<<"(";
    cout<< memory_alloca[dynamic_cast<VarDef*>(paralist->at(0))->getDataType()].datatype;
    for(size_t i=1;i< paralist->size();++i){
        VarDef* para = dynamic_cast<VarDef*>(paralist->at(i));
        cout<<", " + memory_alloca[para->getDataType()].datatype;
    }
    cout<<"){"<<endl;
    
    //参数入栈分配内存
    size_t count = paralist->size();
    for(size_t i=0;i< paralist->size();++i){
        VarDef* para = dynamic_cast<VarDef*>(paralist->at(i));
        cout<<"%"+itos(++count)+" = "
        +"alloca "+memory_alloca[para->getDataType()].datatype
        +", align "+memory_alloca[para->getDataType()].align<<endl;
        
        name_mapping_index->insert(std::make_pair(para->getName(),"%"+itos(count)));
    }
    
    //局部变量内存分配
    for(size_t i =0;i<fundef->m_allLocal.size();++i){
        VarDef* local = dynamic_cast<VarDef*>(fundef->m_allLocal.at(i));
        cout<<"%"+itos(++count)+" = "
        +"alloca "+ memory_alloca[local->getDataType()].datatype
        +", align " + memory_alloca[local->getDataType()].align<<endl;
        
        name_mapping_index->insert(std::make_pair(local->getName(),"%"+itos(count)));
    }
    ass_index = count;
}

void Semantics::genFunDefine(SymDeclare* symdeclare){
    
    genFunEntry(symdeclare);
    
    FunDef* fundef = dynamic_cast<FunDef*>(symdeclare);
    
    for(size_t i=0; i<fundef->m_action.size();++i){
        switch(fundef->m_action.at(i)->getParseType()){
            case VARDEFINE:
                genVarDefine(fundef->m_action.at(i));
                break;
            case IFSTAT:
                
                break;
            case DOWHILE:
                
                break;
            default:
                
                break;
        }
    }
}

Symbols* Semantics::getCurSymbols(){
    return m_symbolStack.at(m_symbolStack.size()-1);
}

void Semantics::deal_AST(vector<SymDeclare *> ves){
    for(vector<SymDeclare*>::iterator it=ves.begin();it!=ves.end();++it){
        if((*it)->getParseType()==VARDEFINE){
        VarDef* vardef = dynamic_cast<VarDef*>((*it));
            if(vardef->getValue()!=nullptr){
                ExpNode* expNode = vardef->getValue();
                ExpNode* value = postorder_traversal(expNode);
                switch (value->getNodeType()) {
                    case CONST_INT:
                        cout<<"store "+memory_alloca[vardef->getDataType()].datatype+" "
                        +itos(value->getValue().idata) + " "
                        +memory_alloca[vardef->getDataType()].datatype+"* "+ (*cur_nmi)[vardef->getName()]<<endl;
                        break;
                    case CONST_FLOAT:
                        cout<<"store "+memory_alloca[value->getNodeType()].datatype
                        +itos(value->getValue().vdata)+" "
                        +memory_alloca[vardef->getDataType()].datatype+"* "+ (*cur_nmi)[vardef->getName()]<<endl;
                        break;
                    default:
                        break;
                }
            }
        }else if((*it)->getParseType()==IFSTAT){
            IfStat* ifstat = dynamic_cast<IfStat*>(*it);
            
            m_symbolStack.push_back(ifstat->m_symbols);
            
            if(ifstat->getValue()!=nullptr){
                ExpNode* expNode = ifstat->getValue();
                

                //ExpNode* value = postorder_traversal(expNode);
                
            }
        }
    }
}

void Semantics::start(){
    for(map<string,SymDeclare*>::iterator it = funSymbolTab->begin(); it!=funSymbolTab->end();++it){
        
        FunDef* fundef = dynamic_cast<FunDef*>(it->second);
        
        m_symbolStack.push_back(fundef->m_symbols);
        
        cout<<"fun Name: "<<it->second->getName()<<endl;
        genFunEntry(it->second);
        vector<SymDeclare*> funves = fundef->m_action;
        deal_AST(funves);
    }
}

ExpNode* Semantics::preorder_traversal(ExpNode* root){
    
    if(root->getNodeType()==CONST_INT
       ||root->getNodeType()==CONST_FLOAT
       ||root->getNodeType()==ID){
        switch (root->getNodeType()) {
            case CONST_INT:
                 cout<<typeToStr[root->getNodeType()]<<root->getValue().idata<<endl;
                break;
            case CONST_FLOAT:
                cout<<typeToStr[root->getNodeType()]<<root->getValue().vdata<<endl;
                break;
            case ID:
                cout<<typeToStr[root->getNodeType()]<<root->getVarName()<<endl;
            default:
                break;
        }
        return nullptr;
    }
    
    cout<<"BinaryOperator "<< typeToStr[root->getNodeType()]<<endl;
   
    preorder_traversal(root->left);
    preorder_traversal(root->right);
    return nullptr;
}

ExpNode* Semantics::postorder_traversal(ExpNode* root){
    
    if(root->getNodeType()==CONST_INT||root->getNodeType()==CONST_FLOAT||
          root->getNodeType()==CONST_STR||root->getNodeType()==CONST_CHAR){
         return root;
    }else if(root->getNodeType()==ID){
        ass_index++;
        
        Symbols* symbols=getCurSymbols();
        SymDeclare* symdeclare = symbols->find(root->getVarName());
        
        cout<<"%"+itos(ass_index)+" = load "+ memory_alloca[symdeclare->getDataType()].datatype+", "
        <<memory_alloca[symdeclare->getDataType()].datatype+"* "+memory_alloca[symdeclare->getDataType()].align<<endl;
    }

    ExpNode* leftNode = postorder_traversal(root->left);
    ExpNode* rightNode = postorder_traversal(root->right);
    
    /*可转成模板*/
    switch (root->getNodeType()) {
        case ADD:
                if(leftNode->getNodeType()==CONST_INT){
                    if(rightNode->getNodeType()==CONST_INT){
                        VarDataDef vardatadef;
                        vardatadef.idata = leftNode->getValue().idata + rightNode->getValue().idata;
                        
                        root->setNodeType(CONST_INT);
                        root->setValue(vardatadef);
                    }
                }
            break;
        case SUB:
                if(leftNode->getNodeType()==CONST_INT){
                    if(rightNode->getNodeType()==CONST_INT){
                        VarDataDef vardatadef;
                        vardatadef.idata = leftNode->getValue().idata - rightNode->getValue().idata;
                       
                        root->setNodeType(CONST_INT);
                        root->setValue(vardatadef);
                    }
                }
            break;
        case MUL:
                if(leftNode->getNodeType()==CONST_INT){
                    if(rightNode->getNodeType()==CONST_INT){
                        VarDataDef vardatadef;
                        vardatadef.idata = leftNode->getValue().idata * rightNode->getValue().idata;
                        root->setNodeType(CONST_INT);
                        root->setValue(vardatadef);
                    }
                }
            break;
        case DIV:
            if(leftNode->getNodeType()==CONST_INT){
                if(rightNode->getNodeType()==CONST_INT){
                    VarDataDef vardatadef;
                    vardatadef.vdata = leftNode->getValue().idata / rightNode->getValue().idata;
                    root->setNodeType(CONST_INT);
                    root->setValue(vardatadef);
                }
            }
            break;
        default:
            break;
    }
    return root;
}
