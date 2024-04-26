// USELESS FILE FOR TESTING

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
      if (isdigit(front) || (front == '.' && isdigit(dq.front())))
      {
        std::string num = front == '.' ? "0" : "";
        num += front;
        front = dq.front();
        while (!dq.empty() && (isdigit(front) || front == '.'))
        {
          if (front == '.' && num.find('.') != std::string::npos)
          {
            return std::queue<Token>({Token(TokenType::INVALID, "Decimal error")});
          }
          num += front;
          dq.pop_front();
          front = dq.front();
        }
        tokens.push(Token(TokenType::NUMBER, num));
      }
      else if (isalpha(front))
      {
        std::string alpha = "";
        alpha += front;
        while (!dq.empty() && isalpha(dq.front()))
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

int main()
{
  std::string input = "int x 1.66 y";
  Lexer lx = Lexer(input);
  std::queue<Token> tokens = lx.tokenise();
  while (!tokens.empty())
  {
    tokens.front().print();
    tokens.pop();
  }
  return 0;
}