#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include "lexer.h"
using namespace std;
enum ASTType{
    ast_ROOT,
    ast_DECLARATION,
    ast_EXPRESSION,
    ast_ASSIGNMENT,
    ast_PRIMARY,
    ast_MULTIPLICATIVE,
    ast_ADDITIVE,
    ast_ID,
    ast_INT_LITERAL
};



class ASTNode{
public:
    ASTNode* parent;
    vector<ASTNode*> children;
    ASTType type;
    string text;
    ASTNode():parent(nullptr),type(ast_ROOT),text(""),children(vector<ASTNode*>()){}
    ASTNode(ASTType _type, string _text):parent(nullptr),type(_type),text(_text),children(vector<ASTNode*>()){}
};


class Parser{
public:
    Lexer lexer;
    vector<Token> tokens;
    vector<Token>::iterator it;
    ASTNode* root;
    Parser():root(nullptr){}
    Parser(string code):root(nullptr){
        parse(code);
    }
    ~Parser(){
        recursiveDelete(root);
    }
    void recursiveDelete(ASTNode* node){
        if(node == nullptr) return;
        for(ASTNode* child: node->children){
            recursiveDelete(child);
        }
        delete node;
    }
    void init(){
        recursiveDelete(root);
        root = nullptr;
        tokens.clear();
        it = tokens.begin();
    }
    ASTNode* parse(string code);
    ASTNode* primary();
    ASTNode* multiplicative();
    ASTNode* additive();
    ASTNode* expressionStmt();
    ASTNode* assignmentStmt();
    ASTNode* programRoot();


    void checkStatementEnd();
    ASTNode* declare();

    void dump();

};
#endif

