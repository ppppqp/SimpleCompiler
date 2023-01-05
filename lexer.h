
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <iomanip>
#include<iostream>
using namespace std;

enum TokenType{
    INIT,
    ID,
    INT_LITERAL,
    GT,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    SEMI_COLON,
    RIGHT_PAR,
    LEFT_PAR,
    ASSIGN,
};




class Token{
public:
    TokenType type;
    string text;
    void init(){
        type = INIT;
        text = "";
    }
    Token(){init();};
};

enum State{
    s_INIT,
    s_ID,
    s_INT_LITERAL,
    s_GT,
    s_GE,
    s_LT,
    s_PLUS,
    s_MINUS,
    s_STAR,
    s_SLASH,
    s_SEMI_COLON,
    s_RIGHT_PAR,
    s_LEFT_PAR,
    s_ASSIGN,
};

class Lexer{
private:
    Token token; // current token
    vector<Token> tokens;

public:

    void init(){
        // initialize the lexer
        
    }
    State initState(char c);

    vector<Token> tokenize(string code);
    void dump();
};

#endif