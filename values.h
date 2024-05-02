#include <iostream>
#include <string>

enum class ValueType
{
  NULLTYPE,
  NUMBER
};

struct RuntimeValue
{
  ValueType type;
  virtual void print() const = 0;
};

struct NullValue : RuntimeValue
{
  std::string const value = "null";
  NullValue() { type = ValueType::NULLTYPE; }
  void print() const override
  {
    std::cout << "NULL VALUE" << std::endl;
  }
};

struct NumberValue : RuntimeValue
{
  double value;
  NumberValue(double val) : value(val) { type = ValueType::NUMBER; }
  void print() const override
  {
    std::cout << "Numeric Value: " << value << std::endl;
  }
};