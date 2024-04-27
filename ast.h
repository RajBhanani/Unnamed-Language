#include <iostream>
#include <string>
#include <vector>

// Using enum class here as the print functions are handled by individual structs of the corresponding type.
enum class NodeType
{
  PROGRAM,
  NUMBERICLITERAL,
  IDENTIFIER,
  BINARYEXPRESSION
};

struct Statement
{
  NodeType kind;
  virtual void print() const = 0; // Pure virual function as there will be no nodes of this type.
};

struct Program : Statement
{
  /* Array of pointers of type Statement that can point to all of its derived classes.
    In essence, the logic is the same as:
    parent *p = new child();
    p->methodFromChild() calls methods from the children.
  */
  std::vector<Statement *> body;
  Program() { kind = NodeType::PROGRAM; }
  void print() const override
  {
    std::cout << "Program:" << std::endl;
    for (std::vector<Statement *>::const_iterator itr = body.begin(); itr != body.end(); ++itr)
    {
      (*itr)->print();
      std::cout << std::endl;
    }
  }
  ~Program()
  {
    for (std::vector<Statement *>::const_iterator itr = body.begin(); itr != body.end(); ++itr)
    {
      delete *itr;
    }
  }
};

struct Expression : Statement
{
  virtual void print() const = 0;
};

struct BinaryExpression : Expression
{
  Expression *left;
  Expression *right;
  std::string op;
  BinaryExpression(std::string operatorString, Expression *leftExpression, Expression *rightExpression) : left(leftExpression), right(rightExpression), op(operatorString) { kind = NodeType::BINARYEXPRESSION; }
  void print() const override
  {
    std::cout << "Binary Expression: {";
    left->print();
    std::cout << ", Operator: " << op << ", ";
    right->print();
    std::cout << "}";
  }
};

struct Identifier : Expression
{
  std::string symbol;
  Identifier(std::string sym) : symbol(sym) { kind = NodeType::IDENTIFIER; }
  void print() const override
  {
    std::cout << "Identifier: " << symbol;
  }
};

struct NumericLiteral : Expression
{
  double value;
  NumericLiteral(double val) : value(val) { kind = NodeType::NUMBERICLITERAL; }
  void print() const override
  {
    std::cout << "Numeric Literal: " << value;
  }
};
