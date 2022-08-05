#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <math.h>

using namespace std;

class Function
{
public:
    Function(std::string function_definition, std::vector<std::string> variables);

    double get(map<string, double> variable_values);
    double Integrate(string variable_name, double a, double b, int n);
    double PartialDerivative(map<string, double> variable_values, string variable_name, double limit);

private:
    std::string _function_definition;
    std::vector<std::string> _variables;
    std::vector<std::string> _definition;
    std::map<string, double> _variable_values;
    int _definition_length;

    void Preprocess();

    void CheckVariableValues();
    template <typename type> bool FindInVector(vector<type> vec, type value);
    double EvaluateVariable(string type, string value);

    vector<double> OperatorsBasic(int position, string oper);
    vector<double> OperatorsFunction(int position, string function);

    double Basic(double val1, double val2, string oper);
    double Func(double value, string oper);

protected:
    vector<string> _operators_function{ "sin(", "cos(", "tan(", "asin(", "acos(", "atan(",
        "cosh(", "sinh(", "tanh(", "log(", "abs(" };
    vector<string> _operators_basic{ "sum(", "dec(", "mul(", "div(", "pow(" };
    vector<string> _operators_variable{ "con(", "var(" };
};

#endif
