#include "function.h"

//Initializes the class and preprocesses the string into string vector
Function::Function(std::string function_definition, std::vector<std::string> variables)
{
	//Initializes private members of Function class
	_function_definition = function_definition;
	_variables = variables;
	//Checks validity of given function and adds it to vector for easier evaluation
	Preprocess();
};

//Gets the value of function for given variables
double Function::get(map<string, double> variable_values)
{
	_variable_values = variable_values;

	CheckVariableValues();

	string value = _definition[0];
	//If starts with basic's operator pass it and wait for result
	if (FindInVector <string>(_operators_basic, value)) {
		vector<double> ret = OperatorsBasic(0, value);
		return ret[1];
	}
	//If starts with function's operator pass it and wait for result
	else if (FindInVector <string>(_operators_function, value)) {
		vector<double> ret = OperatorsFunction(0, value);
		return ret[1];
	}
	//If only constant or variable, returns its value
	else {
		double ret = EvaluateVariable(_definition[1], _definition[2]);
		return ret;
	}

	return 0;
};

/* Works only on single variabe function */
//Integrates the function with definite integral by Simpson's rule
double Function::Integrate(string variable_name, double a, double b, int n)
{
	if (n % 2 == 0) { n += 1; }

	double h = (b - a) / (n - 1);
	double s1 = 0;
	double s2 = 0;

	for (int i = 2; i < n - 2; i += 2) {
		map < string, double> values = { {variable_name, a + i * h} };
		s1 += this->get(values);
	}
	for (int i = 1; i < n - 1; i += 2) {
		map < string, double> values = { {variable_name, a + i * h} };
		s2 += this->get(values);
	}

	map < string, double> values = { {variable_name, a} };

	return (h / 3) * (this->get(values) + 4 * s2 + 2 * s1 + this->get(values));
}

//gives partial derivate for limit, variable and values specified
double Function::PartialDerivative(map<string, double> variable_values, string variable_name, double limit)
{
	_variable_values = variable_values;

	CheckVariableValues();
	//Checks, that limit (in math mainly defined as h) is higher than zero
	if (limit > 0) {
		//Implements the function as pd/dx = (f(n0, ..., nk+h, ..., nn) - f(n0, ..., nn))/h)
		map<string, double> run_values = _variable_values;
		map<string, double>::iterator itr = run_values.find(variable_name);
		itr->second += limit;
		double derivate = (this->get(run_values) - this->get(_variable_values)) / limit;
		return derivate;
	}
	return 0;
}

//Preprocess the string into vector to bve then compiled
void Function::Preprocess()
{
	//Checks that variables are not equal to operators
	for (string variable : _variables)
	{
		if (FindInVector <string>(_operators_basic, variable)
			|| FindInVector <string>(_operators_function, variable)
			|| FindInVector <string>(_operators_variable, variable))
		{
			throw invalid_argument("Variables cannot be same as operators!");
		}
	}

	string running_string = "";
	int separator_counter = 0;

	// Read definition character by character and check validity
	for (char character : _function_definition)
	{
		if (character == ';') {
			//Firstly tries to find if it is constant
			try { double val = std::stof(running_string); }
			catch (std::exception& e)
			{
				//Find if given element is valid
				if (!FindInVector <string>(_operators_basic, running_string)
					&& !FindInVector <string>(_operators_function, running_string)
					&& !FindInVector <string>(_operators_variable, running_string)
					&& !FindInVector <string>(_variables, running_string)
					&& running_string != ")") {
					cout << running_string;
					throw invalid_argument("Unknown element encountred at: " + separator_counter);
				}
			}
			//Add defined part of equation to vector
			_definition.push_back(running_string);
			// reset the running string and increment counter
			separator_counter += 1;
			running_string = "";
		}
		else {
			//Add element to running string
			running_string += character;
		}
	}
	// Check for number of separators, at least separator must always be present
	if (separator_counter == 0) { throw invalid_argument("No separator encountred!"); }
	_definition_length = _definition.size();
};
// Checks if all variables get their values upon calling public methods
void Function::CheckVariableValues()
{
	bool found = false;
	for (string run_check : _variables)
	{
		for (auto map_var : _variable_values)
		{
			if (map_var.first == run_check) { found = true; }
		}
		//If variable value not present, exception is throwned
		if (found == false) { throw invalid_argument("Some variables are undefined!"); }
	}
}
//Handler for better finding of elements in vectors, used when finding defined functions
template <typename type> bool Function::FindInVector(vector<type> vec, type value)
{
	if (std::find(vec.begin(), vec.end(), value) != vec.end()) {
		return true;
	}
	return false;
}
//Checks if value is constant or variable and returns its value
double Function::EvaluateVariable(string type, string value)
{
	//If constant, convert to string
	if (type == "con(") { return std::stof(value); }
	//If variable retrieve the value from private variable initiazed by public methods by user
	else { return _variable_values[value]; }
	return 0;
}
//Evaluates and returns value of function if of basic type (two sides matter)
vector<double> Function::OperatorsBasic(int position, string oper)
{
	//Check if first value is variable/constant
	if (FindInVector<string>(_operators_variable, _definition[position + 1])) {
		//If so, get value and continue to second value
		double value1 = EvaluateVariable(_definition[position + 1], _definition[position + 2]);

		if (FindInVector<string>(_operators_variable, _definition[position + 4])) {
			//If also constant/variable return the value of basic operator
			double value2 = EvaluateVariable(_definition[position + 4], _definition[position + 5]);
			//Length is here always 8, as operator( + 3 for constant/variable + 3 for constant/variable + ) = 8
			vector<double> return_array = { 8, Basic(value1, value2, oper) };
			return return_array;
		}

		else {
			//If not, check if it is function or basic operator and nest them together until constant/variable is found and values are returned
			vector<double> arr;
			if (FindInVector<string>(_operators_basic, _definition[position + 4])) {
				arr = OperatorsBasic(position + 4, _definition[position + 4]);
			}
			else { arr = OperatorsFunction(position + 4, _definition[position + 4]); }
			//Length is given for orientation and for value of nested + constant/variable 
			vector<double> return_array = { arr[0] + 5, Basic(value1, arr[1], oper) };
			return return_array;
		}
	}
	else {
		//Same process as above, but here the first element is basic or function operator, with the second being decided
		vector<double> arr1;
		if (FindInVector<string>(_operators_basic, _definition[position + 1])) {
			arr1 = OperatorsBasic(position + 1, _definition[position + 1]);
		}
		else { arr1 = OperatorsFunction(position + 1, _definition[position + 1]); }

		if (FindInVector<string>(_operators_variable, _definition[position + arr1[0] + 1])) {
			double value2 = EvaluateVariable(_definition[position + arr1[0] + 1], _definition[position + arr1[0] + 2]);
			vector<double> return_array = { arr1[0] + 5, Basic(arr1[1], value2, oper) };
			return return_array;
		}
		else {

			vector<double> arr2;
			if (FindInVector<string>(_operators_basic, _definition[position + arr1[0] + 1])) {
				arr2 = OperatorsBasic(position + arr1[0] + 1, _definition[position + arr1[0] + 1]);
			}
			else { arr2 = OperatorsFunction(position + arr1[0] + 1, _definition[position + arr1[0] + 1]); }

			vector<double> return_array = { arr1[0] + arr2[0] + 2, Basic(arr1[1], arr2[1], oper) };
			return return_array;
		}
	}
}
//Evaluates and returns value of function if of function type (only one value)
vector<double> Function::OperatorsFunction(int position, string function)
{
	//Is element constant/variable
	if (FindInVector<string>(_operators_variable, _definition[position + 1])) {
		// If so return the length and value of function upon the value
		double value = EvaluateVariable(_definition[position + 1], _definition[position + 2]);
		vector<double> return_array = { 5, Func(value, function) };
		return return_array;
	}
	else {
		//If not, find if the operator is of basic type or of function type
		vector<double> arr;
		//Pass then to given function, nesting them, until constant/variable is found and values are returned
		if (FindInVector<string>(_operators_basic, _definition[position + 1])) {
			arr = OperatorsBasic(position + 1, _definition[position + 1]);
		}
		else { arr = OperatorsFunction(position + 1, _definition[position + 1]); }
		//Return of length for orientation and nested value of function
		vector<double> return_array = { arr[0] + 2, Func(arr[1], function) };
		return return_array;
	}
}
//Returns value to operator of basic type
double Function::Basic(double val1, double val2, string oper)
{
	//switch not used as it does not accept string
	if (oper == "sum(") { return (val1 + val2); }
	if (oper == "dec(") { return (val1 - val2); }
	if (oper == "mul(") { return (val1 * val2); }
	if (oper == "div(") { return (val1 / val2); }
	if (oper == "pow(") { return pow(val1, val2); }
	return 0;
}
//Returns value to operator of function type
double Function::Func(double value, string oper)
{
	if (oper == "sin(") { return sin(value); }
	if (oper == "cos(") { return cos(value); }
	if (oper == "tan(") { return tan(value); }
	if (oper == "asin(") { return asin(value); }
	if (oper == "acos(") { return acos(value); }
	if (oper == "atan(") { return atan(value); }
	if (oper == "cosh(") { return acosh(value); }
	if (oper == "sinh(") { return sinh(value); }
	if (oper == "tanh(") { return tanh(value); }
	if (oper == "log(") { return log(value); }
	if (oper == "abs(") { return abs(value); }
	return 0;
}