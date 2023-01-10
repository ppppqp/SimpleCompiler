#include "parser.h"
int main(){
    try{
        string script = "int a = b+3;";
        Parser parser(script);
        parser.dump();
        
        script = "a=45;";
        parser.parse(script);
        parser.dump();

        script = "a=45; a=a+1+2*3+4;";
        parser.parse(script);
        parser.dump();
    }catch(const char *e){
        cout << "Error: " << e << endl;
    }
}