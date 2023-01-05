#include "lexer.h"
std::ostream& operator<<(std::ostream& out, const TokenType value){
    const char* s = 0;
    // # in preprocessor will stringify 
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(value){
        PROCESS_VAL(INIT);     
        PROCESS_VAL(ID);     
        PROCESS_VAL(INT_LITERAL);
        PROCESS_VAL(GT);
        PROCESS_VAL(GE);
        PROCESS_VAL(PLUS);
        PROCESS_VAL(MINUS);
        PROCESS_VAL(STAR);
        PROCESS_VAL(SLASH);
        PROCESS_VAL(SEMI_COLON);
        PROCESS_VAL(RIGHT_PAR);
        PROCESS_VAL(LEFT_PAR);
        PROCESS_VAL(ASSIGN);
    }
#undef PROCESS_VAL
    return out << s;
}
bool isAlpha(char ch){
    return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z';
}
bool isDigit(char ch){
    return ch >= '0' && ch <= '9';
}
bool isBlank(char ch){
    return ch == ' ' || ch == '\t' || ch == '\n';
}
State Lexer::initState(char c){
    // previous token is complete
    cout << token.text << endl;
    if(token.text.size() > 0){
        // previous token is complete
        tokens.push_back(token);
        token.init();
    }
    State newState = s_INIT; // new state
    if(isDigit(c)){
        newState = s_INT_LITERAL;
        token.type = INT_LITERAL;
        token.text.push_back(c);
    }else if (isAlpha(c)){
        newState = s_ID;
        token.type = ID;
        token.text.push_back(c);
    }else if(c == '>'){
        newState = s_GT;
        token.type = GT;
        token.text.push_back(c);
    }else if(c == '+'){
        newState = s_PLUS;
        token.type = PLUS;
        token.text.push_back(c);
    }else if(c == '-'){
        newState = s_MINUS;
        token.type = MINUS;
        token.text.push_back(c);
    }else if(c == '*'){
        newState = s_STAR;
        token.type = STAR;
        token.text.push_back(c);
    }else if(c == '/'){
        newState = s_SLASH;
        token.type = SLASH;
        token.text.push_back(c);
    }else if(c == ';'){
        newState = s_SEMI_COLON;
        token.type = SEMI_COLON;
        token.text.push_back(c);
    }else if(c == '('){
        newState = s_LEFT_PAR;
        token.type = LEFT_PAR;
        token.text.push_back(c);
    }else if(c == ')'){
        newState = s_RIGHT_PAR;
        token.type = RIGHT_PAR;
        token.text.push_back(c);
    }else if(c == '='){
        newState = s_ASSIGN;
        token.type = ASSIGN;
        token.text.push_back(c);
    }else{
        newState = s_INIT;
    }
    return newState;

}

vector<Token> Lexer::tokenize(string code){
    State state = s_INIT;
    for(char c : code){
        cout << "state:" << state << endl;
        switch(state){
            case s_INIT: {
                state = initState(c);
                break;
            }
            case s_ID:{
                if(isAlpha(c) || isDigit(c)){
                    // still in the identifier mode
                    token.text.push_back(c);
                }else{
                    state = initState(c);
                }
                break;
            }
            case s_INT_LITERAL:{
                if(isDigit(c)){
                    token.text.push_back(c);
                }else{
                    state = initState(c);
                    cout << "now state is" << state << endl;
                }
                break;
            }
            case s_GT:{
                if(c == '='){
                    token.type = GE;
                    state = s_GE;
                    token.text.push_back(c);        
                }else{
                    state = initState(c);
                }
                break;
            }
            default:{
                state = initState(c);
                break;
            }

            // case s_ASSIGN:
            // case s_PLUS:
            // case s_MINUS:
            // case s_STAR:
            // case s_SLASH:
            // case s_SEMI_COLON:
            // case s_LEFT_PAR:
            // case s_RIGHT_PAR:
            
            
        }
    }
    if(token.text.size() > 0){
        // last token in buffer
        tokens.push_back(token);
    }
    return tokens;
}
void Lexer::dump(){
    for(Token t: tokens){
        cout << setw(10) << t.type << setw(20)  <<  t.text << endl; 
    }
}

