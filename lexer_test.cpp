#include "lexer.h"
using namespace std;
int main(){
    Lexer lexer;
    string script = "age = 45;";
    cout << "script:" << script << endl;
    lexer.tokenize(script);
    cout << "result:\n";
    lexer.dump();
    return 0;
}