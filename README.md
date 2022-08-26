# FunctionCreator
FunctionCreator is a small c++ class used to generate mathematical function during runtime and then compiling them. It is using its own pseudocode for 
better and faster compiling speed, it also has functionality for it to be integrated (simpsons method) and to get partial derivative. Main functionality
is to serve mainly as a boilerplate code for further editing by user to serve its specific needs.

## Using the class
Below is the basic use of the class:
```cpp
#include "function.h"
int main(){
Function func = Function("log(;sum(;var(;x;);con(;7;););)", vector<string> {"x"}); //declaring the class
map<string, double> variable_values{ {"x", 7} }; //map to assigning values to variables
double result = func.get(variable_values); //use the map to get the function value
}
```
There is also some basic, additional functionality than getting the value. this includes integrating and 
derivating by one variable:
```cpp
double integrated = func.Integrate(variable_values, "x", 2, 3, 1e4); //Integrates the function using Simpson's method by one variable
                                                    //Works only function with single variable
double derivated = func.PartialDerivative(variable_values, "x"); //Returns partial derivative of function
```
Function can also accept function with multiple variables:
```cpp
Function func = Function("mul(;log(;sum(;var(;x;);con(;7;);););sum(;con(;3;);var(;y;););)", vector<string> {"x", "y"});
map<string, double> variable_values{ {"x", 7}, {"y", 3} };
double result = func.get(variable_values);
```

## How to write functions
The language used to write function is pseudo proggraming language, this is for speeding up the process of function reading and processing.
There are 4 types of operators present.
### Separators and Brackets
Separators are essential part of the functio definition, they tell the class what is present in the string defining the mathematical
function. They must be included after every function definition, variable, constant or bracket. Separator is not put at the begging of 
function declarator **(  not ;sin(; but sin(;  )**.

Regarding brackets, '(' brackets are present as part of the functions (  such as ;sin(;  ) and are not separated. On the
other hand the ')' brackets need to be separated (  **sin(;con(;4.0123;););**  )
### Variables and Constants
- Variables represent gamer defined values, such as x, y or z, defined with function initialization. Are named as var(;x;)
- Constants are double values, given as **con(;1.2135;);**.
```cpp
Function("log(;sum(;var(;x;);con(;7.0;););)", vector<string> {"x"}); //In normal notation log(x + 7)
                                                                   //x's value given at get() function
```
### Basic operators
Basic operators are summation, decrement, multiplication, division  and power including square root. 
They are represented by **;sum(;, ;dec(;, ;mul(;, ;div(; and ;pow(;**. These operators must have two parts to them, the left and the right
```cpp
Function("sum(;con(;1.25;);var(;x;););", vector<string> {"x"}); //Here two parts for summation, same for other basic operators
```

### Function operators
These include all functions defined in *<math.h>*, examples are **sin(;, asin(;, log(;** and so on. Thses only need one value inside their definition.
``` cpp
Function("log(var(;x;);)", vector<string> {"x"}); //Only one value inside the function definition
```

## Additional classes
In addition to the basic class capable of creating functions during runtime there are also multiple supporting ones which use the main class as framework upon which they build.

### VectorFunction
Currently, there is only one such class, but others are being worked on: Vector Function class. It accepts three functions when being initiatized, one variable each. Excluding the basic functions, such as getting value or derivating, there also function like getting value from plane or size.
``` cpp
#include "function.h"
int main() {
Function func1 = Function("var(;t;)", vector<string> {"t"}); //declaring the class
Function func2 = Function("sin(;var(;t;);)", vector<string> {"t"}); //declaring the class
Function func3 = Function("con(;1;)", vector<string> {"t"}); //declaring the class
    
vector<Function> functions{ func1, func2, func3 }; //Vector of functions to be included in vector function

VectorFunction funcv = VectorFunction(functions, "t"); //Initializing the vector function
}
```
Below are examples of vector functions.
```
funcv.get(3); //Returns vector of vector function values
funcv.SizeFunctionValue(1, 2) //Length of function between t=1 and t=2
funcv.Derivate(3) //Retrurns derivate at t=3
```

## Dependencies
All dependencies are parts of the c++ standart library, so no additional installations are not necessary.
