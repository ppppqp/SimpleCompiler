#ifndef SCRIPT_H
#define SCRIPT_H
#include<unordered_map>
#include <iostream>
#include <string>
#include "parser.h"
#include "lexer.h"
using namespace std;
class Scripter{
private:
    unordered_map<string, int> variables;
    Parser parser;
public:
    void repl();
    int evaluate(ASTNode* node);
};
#endif