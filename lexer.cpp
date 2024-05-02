// USELESS FILE FOR TESTING

#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#include <queue>
#include <unordered_map>

enum TokenType
{
  // TODO: Add more keywords. Currently classed as IDENTIFIER except let
  IDENTIFIER, // 0
  NUMBER,     // 1

  BINARYOPERATOR,     // 2
  ASSIGNMENTOPERATOR, // 3

  OPENPARENTHESIS,  // 4
  CLOSEPARENTHESIS, // 5

  // Keywords
  LET,      // 6
  NULLTOKEN, // 7    NULL is a reserved token in C++

  INVALID,
  SKIPPABLE,
  ENDOFFILE,
};

const std::unordered_map<std::string, TokenType> Keywords{
    {"let", TokenType::LET},
    {"null", TokenType::NULLTOKEN},
};

const std::string TokenTypeArray[] = {"IDENTIFIER", "NUMBER", "BINARYOPERATOR", "ASSIGNMENTOPERATOR", "OPENPARENTHESIS", "CLOSEPARENTHESIS", "LET", "NULLTOKEN", "INVALID", "SKIPPABLE", "ENDOFFILE"};

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
        if (Keywords.find(alpha) != Keywords.end())
        {
          tokens.push(Token(Keywords.at(alpha), alpha));
        }
        else
        {
          tokens.push(Token(TokenType::IDENTIFIER, alpha));
        }
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
  std::string input = "let x = null";
  Lexer lx = Lexer(input);
  std::queue<Token> tokens = lx.tokenise();
  while (!tokens.empty())
  {
    tokens.front().print();
    tokens.pop();
  }
  return 0;
}