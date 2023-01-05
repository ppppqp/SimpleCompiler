CXX = g++
CXXFLAGS = -std=c++11


lexer: 
	$(CXX) $(CXXFLAGS) lexer_test.cpp lexer.cpp -o lexer_test

test_lexer:
	make clean
	make lexer
	./lexer_test

parser:
	$(CXX) $(CXXFLAGS) parser_test.cpp parser.cpp lexer.cpp -o parser_test

test_parser:
	make clean
	make parser
	./parser_test


clean:
	rm -f *.o
	rm -f lexer_test
	rm -f parser_test