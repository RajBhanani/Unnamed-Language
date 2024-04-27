#include "ast.h"
#include "lexer.h"
#include <queue>

class Parser
{
private:
  std::queue<Token> tokens;
  bool notEOF();
  Token shiftParser();
  Statement *parseStatemet();
  Expression *parseExpression();
  Expression *parseAdditiveExpression();
  Expression *parseMultiplicativeExpression();
  Expression *parsePrimaryExpression();

public:
  void produceAST(std::string sourceCode);
};

bool Parser::notEOF()
{
  return tokens.front().type != TokenType::ENDOFFILE;
}

Token Parser::shiftParser()
{
  Token temp = tokens.front();
  tokens.pop();
  return temp;
}

Statement *Parser::parseStatemet()
{
  return parseExpression();
}

Expression *Parser::parseExpression()
{
  return parseAdditiveExpression();
}

Expression *Parser::parseAdditiveExpression()
{
  Expression *left = parseMultiplicativeExpression();
  while (tokens.front().value == "+" || tokens.front().value == "-")
  {
    std::string op = shiftParser().value;
    Expression *right = parseMultiplicativeExpression();
    left = new BinaryExpression(op, left, right);
  }
  return left;
}

Expression *Parser::parseMultiplicativeExpression()
{
  Expression *left = parsePrimaryExpression();
  while (tokens.front().value == "/" || tokens.front().value == "*")
  {
    std::string op = shiftParser().value;
    Expression *right = parsePrimaryExpression();
    left = new BinaryExpression(op, left, right);
  }
  return left;
}

Expression *Parser::parsePrimaryExpression()
{
  TokenType token = tokens.front().type;
  switch (token)
  {
  case TokenType::IDENTIFIER:
    return new Identifier(shiftParser().value);
    break;

  case TokenType::NUMBER:
    return new NumericLiteral(stod(shiftParser().value));
    break;

  default:
    std::cout << "Error in parsing on token:" << std::endl;
    tokens.front().print();
    exit(1);
  }
}

void Parser::produceAST(std::string sourceCode)
{
  Lexer lx = Lexer(sourceCode);
  tokens = lx.tokenise();
  Program program;

  while (notEOF())
  {
    program.body.push_back(parseStatemet());
  }
  program.print();
}

int main()
{
  Parser p;
  p.produceAST("x 10 + 12 * 13 + 20");
  return 0;
}