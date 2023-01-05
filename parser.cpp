#include <string>
#include <vector>
#include "parser.h"
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
  Lexer lexer;
  tokens = lexer.tokenize(code);
  lexer.dump();
  it = tokens.begin();
  root = programRoot();
  return root;
}

ASTNode* Parser::primary() {
  ASTNode* node = nullptr;
  if (it != tokens.end()) {
    if (it->type == TokenType::INT_LITERAL) {
      it++;
      node = new ASTNode(ASTType::ast_INT_LITERAL, it->text);
    } else if (it->type == TokenType::ID) {
      it++;
      node = new ASTNode(ASTType::ast_ID, it->text);
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
  if (child1 != nullptr && it != tokens.end()) {
    if (it->type == TokenType::STAR || it->type == TokenType::SLASH) {
      it++;
    }
    ASTNode* child2 = multiplicative();
    if (child2 != nullptr) {
      node = new ASTNode(ASTType::ast_MULTIPLICATIVE, it->text);
      node->children.push_back(child1);
      node->children.push_back(child2);
    } else {
      throw "Invalid multiplicative expression";
    }
  }
  return node;
}
ASTNode* Parser::additive() {
  ASTNode* child1 = multiplicative();
  ASTNode* node = child1;
  if (child1 != nullptr && it != tokens.end()) {
    // first part is a valid multiplicative

    // read the operator
    if (it->type == TokenType::PLUS || it->type == TokenType::MINUS) {
      it++;
    }

    // second part
    ASTNode* child2 = additive();
    if (child2 != nullptr) {
      // second part is valid additive
      child1->children.push_back(child2);
      node = new ASTNode(ASTType::ast_ADDITIVE, it->text);
      node->children.push_back(child1);
      node->children.push_back(child2);
    } else {
      throw "Invalid additive expression.";
    }
  }
  return node;
}

ASTNode* Parser::programRoot() {
  ASTNode* node = new ASTNode();
  ASTNode* child = additive();  // see if it is additive
  if (child != nullptr) {
    node->children.push_back(child);
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

  if (it->type == TokenType::ID) {
    // identifier
    it++;
    checkStatementEnd();
    node = new ASTNode(ASTType::ast_DECLARATION, it->text);
    if (it->type == TokenType::ASSIGN) {
      // assignment =
      it++;
      checkStatementEnd();
    }
    ASTNode* child = additive();
    // expression

    if (child == nullptr) {
      throw "Expecting expression in assignment.";
    }
    node->children.push_back(child);
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
void dumpHelper(ASTNode* node, string indent){
    cout << indent << node->type << node->text << endl;
    indent += "\t";
    for (ASTNode* child : node->children) {
        dumpHelper(child, indent);
    }
}
void Parser::dump() {
    dumpHelper(root, "");
}

