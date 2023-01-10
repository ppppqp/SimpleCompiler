#include "parser.h"

#include <string>
#include <vector>

#include "lexer.h"
using namespace std;

std::ostream& operator<<(std::ostream& out, const ASTType value) {
  const char* s = 0;
  // # in preprocessor will stringify
#define PROCESS_VAL(p) \
  case (p):            \
    s = #p;            \
    break;
  switch (value) {
    PROCESS_VAL(ast_ROOT);
    PROCESS_VAL(ast_ID);
    PROCESS_VAL(ast_INT_LITERAL);
    PROCESS_VAL(ast_DECLARATION);
    PROCESS_VAL(ast_EXPRESSION);
    PROCESS_VAL(ast_PRIMARY);
    PROCESS_VAL(ast_MULTIPLICATIVE);
    PROCESS_VAL(ast_ADDITIVE);
    PROCESS_VAL(ast_ASSIGNMENT);
  }
#undef PROCESS_VAL
  return out << s;
}

ASTNode* Parser::parse(string code) {
  init();
  tokens = lexer.tokenize(code);
  it = tokens.begin();
  root = programRoot();
  return root;
}

ASTNode* Parser::primary() {
  ASTNode* node = nullptr;
  if (it != tokens.end()) {
    if (it->type == TokenType::INT_LITERAL) {
      it++;
      node = new ASTNode(ASTType::ast_INT_LITERAL, (it-1)->text);
    } else if (it->type == TokenType::ID) {
      it++;
      node = new ASTNode(ASTType::ast_ID, (it-1)->text);
    } else if (it->type == TokenType::LEFT_PAR) {
      it++;
      node = additive();
      if (node != nullptr) {
        if (it != tokens.end() && it->type == TokenType::RIGHT_PAR) {
          it++;
        } else {
          throw "Expecting right parenthesis.";
        }
      } else {
        throw "Expecting expression inside parenthesis.";
      }
    }
  }
  return node;
}
ASTNode* Parser::multiplicative() {
  ASTNode* child1 = primary();
  ASTNode* node = child1;
  while (true) {
    if (child1 != nullptr && it != tokens.end()) {
			bool isStar = false;
      if (it->type == TokenType::STAR || it->type == TokenType::SLASH) {
				isStar = it->type == TokenType::STAR;
        it++;
      } else {
				// multiplicative ends
				break;
      }
      ASTNode* child2 = multiplicative();
      if (child2 != nullptr) {
        node = new ASTNode(ASTType::ast_MULTIPLICATIVE, (isStar) ? "*" : "/");
        node->children.push_back(child1);
        node->children.push_back(child2);
      } else {
        throw "Invalid multiplicative expression.";
      }
    } else{
			return node;
		}
  }

  return node;
}
ASTNode* Parser::additive() {
  ASTNode* child1 = multiplicative();
  ASTNode* node = child1;
  while (child1 != nullptr && it != tokens.end()) {
    // first part is a valid multiplicative

    // read the operator
		bool isPlus = false;
    if (it->type == TokenType::PLUS || it->type == TokenType::MINUS) {
			isPlus = it->type == TokenType::PLUS;
      it++;
    }else{
			break;
		}
    // second part
    ASTNode* child2 = multiplicative(); // left associative
    if (child2 != nullptr) {
      // second part is valid additive
      node = new ASTNode(ASTType::ast_ADDITIVE, (isPlus) ? "+" : "-");
      node->children.push_back(child1);
      node->children.push_back(child2);
			child1 = node; // left associative
    } else {
      throw "Invalid additive expression.";
    }
  }
  return node;
}

ASTNode* Parser::expressionStmt() {
	vector<Token>::iterator pos = it;
  ASTNode* node = additive();
  if (node) {
    if (it != tokens.end() && it->type == TokenType::SEMI_COLON) {
      it++;
    } else {
      node = nullptr;  // error: this is not an expression statement!
      it = pos;        // rewind
    }
  }
  return node;
}

ASTNode* Parser::assignmentStmt() {
  ASTNode* node = nullptr;
  if (it != tokens.end() && it->type == TokenType::ID) {
		it++;
    node = new ASTNode(ASTType::ast_ASSIGNMENT, (it-1)->text);

    if (it != tokens.end() && it->type == TokenType::ASSIGN) {
      it++;
      ASTNode* child = additive();
      if (child == nullptr) {
        throw "Invalid assignment statement, expecting an expression.";
      } else {
        node->children.push_back(child);
        if (it != tokens.end() && it->type == TokenType::SEMI_COLON) {
          it++;
        } else {
          throw "Invalid statement, expecting semicolon";
        }
      }
    } else {
      // QUESTION: When to rewind and when to throw an error?
      // ANSWER: when encounter an identifier, we don't know if it is an
      // assignment or an expression, so rewind However, if we have identifier
      // with an ASSIGN, we know the second part must be an expression, so error
      it--;
      node = nullptr;
    }
  }
  return node;
}

ASTNode* Parser::programRoot() {
  ASTNode* node = new ASTNode(ASTType::ast_ROOT, "root");
  while (it != tokens.end()) {
    ASTNode* child = declare();
    if (child == nullptr) {
      child = expressionStmt();
    }
    if (child == nullptr) {
      child = assignmentStmt();
    }
    if (child != nullptr) {
      node->children.push_back(child);
    } else {
      throw "Unknown statement.";
    }
  }
  return node;
}

void Parser::checkStatementEnd() {
  if (it == tokens.end()) {
    throw "Expecting semi-colon in declaration.";
  }
}
ASTNode* Parser::declare() {
  ASTNode* node = nullptr;
  if (it == tokens.end()) return node;
	if(it->type == TokenType::ID_INT){
		it++;
		checkStatementEnd();
	}else{
		return node;
	}
  if (it->type == TokenType::ID) {
    // identifier
    it++;
    checkStatementEnd();
    node = new ASTNode(ASTType::ast_DECLARATION, (it-1)->text);
    if (it->type == TokenType::ASSIGN) {
			// assignment =
			it++;
			ASTNode* child = additive();
			// expression

			if (child == nullptr) {
				throw "Expecting expression in assignment.";
			}
			node->children.push_back(child);
		}
    // semi colon
    checkStatementEnd();
    if (it->type == TokenType::SEMI_COLON) {
      it++;
    } else {
      throw "Expecting semi-colon in declaration.";
    }
  }
  return node;
}
void dumpHelper(ASTNode* node, string indent) {
  cout << indent << node->type << '(' << node->text  << ')' << endl;
  indent += "  ";
	if(node->type == ast_ASSIGNMENT) cout << "assign in dump:" << node << endl;
  for (ASTNode* child : node->children) {
    dumpHelper(child, indent);
  }
}
void Parser::dump() {
	lexer.dump();
	cout << "=====================PARSER RESULT====================" << endl;
	dumpHelper(root, ""); 
}
