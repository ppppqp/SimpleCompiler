#include "script.h"

void Scripter::repl(){
    while(true){
        cout << "> ";
        string code;
        getline(cin, code);
        if(code == "exit();"){
            cout << "Good bye!" << endl;
            return;
        }
        try{
            ASTNode* root = parser.parse(code);
            cout << evaluate(root) << endl;
        }catch(const char* e){
            cout << e << endl;
        }
    }
}

int Scripter::evaluate(ASTNode* node){
    int result = 0;
    switch(node->type){
        case ast_ROOT:{
            for(auto child : node->children){
                result = evaluate(child);
            }
            break;
        }
        case ast_ADDITIVE:{
            if(node->text == "+"){
                result = evaluate(node->children[0]) + evaluate(node->children[1]);
            }else{
                result = evaluate(node->children[0]) - evaluate(node->children[1]);
            }
            break;
        }
        case ast_MULTIPLICATIVE:{
            if(node->text == "*"){
                result = evaluate(node->children[0]) * evaluate(node->children[1]);
            }else{
                result = evaluate(node->children[0]) / evaluate(node->children[1]);
            }
            break;
        }
        case ast_INT_LITERAL:{
            return stoi(node->text);
        }
        case ast_ID:{
            if(variables.find(node->text) != variables.end()){
                result = variables[node->text];
            }else{
                throw "variable "+node->text+" not found";
            }
            break;
        }
        case ast_ASSIGNMENT:{
            if(variables.find(node->text) != variables.end()){
                result = evaluate(node->children[0]);
                
                variables[node->text] = result;
            }else{
                throw "variable "+node->text+" not found";
            }
            break;
        }
        case ast_DECLARATION:{
            if(variables.find(node->text)!=variables.end()){
                throw "variable "+node->text+" is already declared";
            }
            if(node->children.size() > 0){
                // contain initialization
                result = evaluate(node->children[0]);
                variables[node->text] = result;
            } else{
                // default
                variables[node->text] = 0;
            }
            break;
        }
        default:{}
    }
    return result;
}
