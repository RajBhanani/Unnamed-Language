#include "parser.h"
#include "values.h"
#include <iostream>

RuntimeValue *evaluateProgram(Program *ProgramNode);
RuntimeValue *evaluateBinaryExpression(BinaryExpression *BinaryExprNode);
NumberValue *evaluateNumericBinaryExpression(NumberValue *leftSide, NumberValue *rightSide, std::string op);
RuntimeValue *evaluate(Statement *Node);

RuntimeValue *evaluateProgram(Program *ProgramNode)
{
  // RuntimeValue *lastEvaluated = new NullValue();
  std::cout << 1 << std::endl;
  RuntimeValue *lastEvaluated;
  for (std::vector<Statement *>::const_iterator itr = ProgramNode->body.begin(); itr != ProgramNode->body.end(); ++itr)
  {
    (*itr)->print();
    lastEvaluated = evaluate(*itr);
  }
  return lastEvaluated;
}

RuntimeValue *evaluateBinaryExpression(BinaryExpression *BinaryExprNode)
{
  RuntimeValue *leftSide = evaluate(BinaryExprNode->left);
  RuntimeValue *rightSide = evaluate(BinaryExprNode->right);
  if (leftSide->type == ValueType::NUMBER && rightSide->type == ValueType::NUMBER)
  {
    NumberValue *leftSideNumberValue = dynamic_cast<NumberValue *>(leftSide);
    NumberValue *rightSideNumberValue = dynamic_cast<NumberValue *>(rightSide);
    return evaluateNumericBinaryExpression(leftSideNumberValue, rightSideNumberValue, BinaryExprNode->op);
  }
  return new NullValue();
}

NumberValue *evaluateNumericBinaryExpression(NumberValue *leftSideNumberValue, NumberValue *rightSideNumberValue, std::string op)
{
  double result = 0;
  switch (op[0])
  {
  case '+':
    result = leftSideNumberValue->value + rightSideNumberValue->value;
    break;

  case '-':
    result = leftSideNumberValue->value - rightSideNumberValue->value;
    break;
  case '*':
    result = leftSideNumberValue->value * rightSideNumberValue->value;
    break;
  case '/':
    if (rightSideNumberValue->value != 0)
    {
      result = leftSideNumberValue->value / rightSideNumberValue->value;
    }
    else
    {
      std::cout << "Can't divide by 0" << std::endl;
    }
    break;
  default:
    std::cout << "Error." << std::endl;
  }
  return new NumberValue(result);
}

RuntimeValue *evaluate(Statement *Node)
{
  switch (Node->type)
  {
  case NodeType::NUMERICLITERAL:
  {
    NumericLiteral *NumericNode = dynamic_cast<NumericLiteral *>(Node);
    return new NumberValue(NumericNode->value);
  }

  case NodeType::NULLLITERAL:
    return new NullValue();

  case NodeType::BINARYEXPRESSION:
  {
    BinaryExpression *BinaryExprNode = dynamic_cast<BinaryExpression *>(Node);
    return evaluateBinaryExpression(BinaryExprNode);
  }

  case NodeType::PROGRAM:
  {
    Program *ProgramNode = dynamic_cast<Program *>(Node);
    return evaluateProgram(ProgramNode);
  }

  default:
    std::cout << "Unrecognised node" << std::endl;
    Node->print();
    exit(0);
  }
}

int main()
{
  Parser p;
  Program prog = p.produceAST("10 - 5 + 15");
  prog.print();
  RuntimeValue *rv = evaluate(prog.body[0]);
  rv->print();
  // std::cout << rv->type << std::endl;
  // delete rv;
  return 0;
}