#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#include <queue>

enum TokenType
{
  // TODO: Add keywords. Currently classed as IDENTIFIER
  IDENTIFIER,         // 1
  NUMBER,             // 2
  BINARYOPERATOR,     // 3
  ASSIGNMENTOPERATOR, // 4
  OPENPARENTHESIS,    // 5
  CLOSEPARENTHESIS,   // 6
  INVALID,            // 7
  SKIPPABLE,          // 8
  ENDOFFILE,          // 9
};

const std::string TokenTypeArray[] = {"IDENTIFIER", "NUMBER", "BINARYOPERATOR", "ASSIGNMENTOPERATOR", "OPENPARENTHESIS", "CLOSEPARENTHESIS", "INVALID", "SKIPPABLE", "ENDOFFILE"};

struct Token
{
  TokenType type;
  std::string value;
  Token(TokenType type, std::string str) : type(type), value(str) {}
  void print()
  {
    std::cout << "Token Type: " << TokenTypeArray[type];
    std::cout << std::setw(30 - TokenTypeArray[type].size()) << "Value: " << value << std::endl;
  }
};

class Lexer
{
public:
  std::string input;
  std::deque<char> dq;
  Lexer(std::string &inputString) : input(inputString)
  {
    for (size_t i = 0; i < input.size(); i++)
    {
      dq.push_back(input[i]);
    }
  };
  std::queue<Token> tokenise();
};

std::queue<Token> Lexer::tokenise()
{
  std::queue<Token> tokens;
  while (!dq.empty())
  {
    char front = dq.front();
    dq.pop_front();
    if (front == ' ' || front == '\n' || front == '\0')
    {
      continue;
    }
    if (front == '+' || front == '-' || front == '*' || front == '/')
    {
      tokens.push(Token(TokenType::BINARYOPERATOR, std::string(1, front)));
    }
    else if (front == '(')
    {
      tokens.push(Token(TokenType::OPENPARENTHESIS, std::string(1, front)));
    }
    else if (front == ')')
    {
      tokens.push(Token(TokenType::CLOSEPARENTHESIS, std::string(1, front)));
    }
    else if (front == '=')
    {
      tokens.push(Token(TokenType::ASSIGNMENTOPERATOR, std::string(1, front)));
    }
    else
    {
      if (isdigit(front))
      {
        std::string num = "";
        num += front;
        while (isdigit(dq.front()))
        {
          num += dq.front();
          dq.pop_front();
        }
        tokens.push(Token(TokenType::NUMBER, num));
      }
      else if (isalpha(front))
      {
        std::string alpha = "";
        alpha += front;
        while (isalpha(dq.front()))
        {
          alpha += dq.front();
          dq.pop_front();
        }
        tokens.push(Token(TokenType::IDENTIFIER, alpha));
      }
      else
      {
        tokens.push(Token(TokenType::INVALID, std::string(1, front)));
      }
    }
  }
  tokens.push(Token(TokenType::ENDOFFILE, "ENDOFFILE"));
  return tokens;
}