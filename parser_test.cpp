#include "parser.h"
int main(){
    try{
        string script = "a = b+3;";
        Parser parser(script);
        parser.dump();
    }catch(const char *e){
        cout << "Error: " << e << endl;
    }
}