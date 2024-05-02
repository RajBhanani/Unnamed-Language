#include "ast.h"
#include "lexer.h"
#include <queue>

class Parser
{
private:
  std::queue<Token> tokens;
  Program program;
  bool notEOF();
  Token shiftParser();
  Statement *parseStatemet();
  Expression *parseExpression();
  Expression *parseAdditiveExpression();
  Expression *parseMultiplicativeExpression();
  Expression *parsePrimaryExpression();

public:
  Program produceAST(std::string sourceCode);
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
  case TokenType::NULLTOKEN:
    shiftParser();
    return new NullLiteral();

  case TokenType::IDENTIFIER:
    return new Identifier(shiftParser().value);

  case TokenType::NUMBER:
    return new NumericLiteral(stod(shiftParser().value));

  case TokenType::OPENPARENTHESIS:
  {
    shiftParser();
    if (tokens.front().type == TokenType::CLOSEPARENTHESIS)
    {
      std::cout << "Expected expression before ')'" << std::endl;
      exit(1);
    }
    Expression *expr = parseExpression();
    Token temp = shiftParser();
    if (temp.type != TokenType::CLOSEPARENTHESIS)
    {
      std::cout << "Unexpected token. Expected ')' or an expression. Instead, found:" << std::endl;
      temp.print();
      program.body.back()->print();
      exit(1);
    }
    return expr;
  }

  default:
    std::cout << "Error in parsing token:" << std::endl;
    tokens.front().print();
    exit(1);
  }
}

Program Parser::produceAST(std::string sourceCode)
{
  Lexer lx = Lexer(sourceCode);
  tokens = lx.tokenise();
  while (notEOF())
  {
    program.body.push_back(parseStatemet());
  }
  return program;
}