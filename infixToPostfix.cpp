/*
Author:		Jesus Rodriguez
Date;		10/5/2017
Program Name:	program1.cpp
Description:	This program serves to get an infix expression from the user and change the original 
		expression to a postfix expression. The program determines if the expression has the
		appropriate parenthetical statements and, if the expression is valid, determines the
		result using a stack.
*/
#include <iostream>
#include <stack>
#include <queue>
#include <sstream>
#include <cmath>
using namespace std;

bool postfix(queue<string> &tokens, string equation);
bool precedence(string optr, string optrtop);
void evaluation(queue<string> tokens);

int main()
{
	queue<string> exptokens; // the queue that will hold the postfix expression tokens
	string expression; // a variable used to hold user input

	// retrive the user expression
	cout << "Enter a valid infix expression: ";
	getline(cin, expression);

	// parse the string, check if the expression was valid, and evaluate the result
	if(postfix(exptokens, expression)) evaluation(exptokens);
	else cout << "Invalid expression. Found unmatched parentheses.\n"; // the expression is not valid

	return 0;
}
/*
Name:		postfix
Parameters:	a queue used to hold string tokens and a string that will be parsed
Return:		a boolean used to represent if the expression was valid
Description:	This function is used to parse a string representing a mathematical expression into tokens
		and, at the same time, place the tokens in a queue of strings representing a postfix
		expression. The function returns a boolean representing whether the expression has
		the appropriate amount of parentheses.
*/
bool postfix(queue<string> &tokens, string equation)
{
	stack<string> operatorstk; // a stack to hold operators before adding them to the expression
	stack<string> parenthesesStk; // a stack used to hold parenthesis operators
	int pos = 0; // an int used to represent the current position in a string
	int prev = 0; // an int used to represent a position in a string
	string substr; // a string used to hold a substring from equation
	string::iterator iter = equation.begin(); // an string iterator

	// loop while there are characters in the string to read
	while(pos != equation.length())
	{
		// if the iterator finds a space
		if(*iter == ' ')
		{
			// determine the substring
			substr = equation.substr(prev, pos - prev);

			// determine the action according to the string value
			if(substr == "+" || substr == "-" || substr == "*" || substr == "/" || substr == "%" || substr == "^") // if the string is an oprator
			{		
				if(operatorstk.empty()) operatorstk.push(substr); // push the operator when the stack is empty
				else if(operatorstk.top() == "(") operatorstk.push(substr); // push the operator when the top string is "("
				else if(precedence(substr, operatorstk.top())) operatorstk.push(substr); // push the operator if the operator has a higher precedence
				else // the operators have the same precedence
				{	
					// add the top operator to the postfix expression and remove from stack
					tokens.push(operatorstk.top());
					operatorstk.pop();
		
					// if there are other operators in the stack
					if(!operatorstk.empty())
					{		
						// push the operator if it has a higher precedence
						if(precedence(substr, operatorstk.top())) operatorstk.push(substr);
						else if(operatorstk.top() != "(") // else add operators to postfix
						{ 
							while(operatorstk.top() != "(") // add all operators between parentheses
							{
								tokens.push(operatorstk.top());
								operatorstk.pop();
							}
							operatorstk.push(substr); // push the new operator
						}
						else operatorstk.push(substr); // a ( is at the top of the stack so push the new operator
					}
					else operatorstk.push(substr); // push the new operator
				}
			}
			else if(substr == "(") // if the token is a left parenthesis
			{
				// push the parentheses to the stacks
				operatorstk.push(substr);
				parenthesesStk.push(substr);
			}
			else if(substr == ")") // if the token is a right parenthesis
			{
				if(parenthesesStk.empty()) return false; // return false if there are no right parenthesis
				else
				{
					// append all operators between parentheses to the postfix expression
					while(operatorstk.top() != "(")
					{
						tokens.push(operatorstk.top());
						operatorstk.pop();
					}
					// pop the left parenthesis
					operatorstk.pop();
					parenthesesStk.pop();
				}
			}
			else // the token is number
			{
				// append the number to the postfix expression
				tokens.push(substr);
			}
			prev = pos + 1; // move the location of previous
		}
		pos++; // move the location of pos
		iter++; // move the iterator
	}

	// gather the last the token
	substr = equation.substr(prev, equation.length());
	if(substr == ")") // the last token is a right parenthesis 
	{
		if(parenthesesStk.empty()) return false; // if there are no right parenthesis the expression is invalid
		else
		{
			// append all operators between parentheses to the postfix expression
			while(operatorstk.top() != "(") 
			{
				tokens.push(operatorstk.top());
				operatorstk.pop();
			}
			// remove the left parenthesis
			operatorstk.pop();
			parenthesesStk.pop();
		}
	}
	else tokens.push(substr);// the last token is a number so append to the postfix expression

	if(!parenthesesStk.empty()) return false; // if there are unmatched left parentheses, the expression is invalid

	// pop any remaining operators and append to postfix expression
	while(!operatorstk.empty())
	{
		tokens.push(operatorstk.top());
		operatorstk.pop();
	}

	return true;
}
/*
Name:		precedence
Parameters:	a string that represents an operator and a string that represents the top
		operator on a stack
Return:		a bool representing the precedence of parameters
Description:	This function determines the precedence between two operators represented by
		strings.
*/
bool precedence(string optr, string optrtop)
{
	if((optr == "+" || optr == "-") && (optrtop == "*" || optrtop == "/" || optrtop == "%")) return false;
	else if((optr == "+" || optr == "-") && (optrtop == "+" || optrtop == "-")) return false;
	else if((optr == "*" || optr == "/" || optrtop == "%") && (optrtop == "*" || optrtop == "/" || optrtop == "%")) return false;
	else if((optr == "*" || optr == "/" || optrtop == "%") && (optrtop == "+" || optrtop == "-")) return true;
	else if(optr == "^") return true;
	else if(optrtop == "^") return false;
}
/*
Name:		evaluation
Parameters:	a queue of string tokens
Return:		
Description:	This function is used to evaluate a postfix expression using a stack. The
		function removes tokens from a queue and places them on a stack if the
		token is an operand value otherwise, if an operator is read, two operands
		are removed and the operation is carried out. The tokens are printed as 
		they are accessed and the result is displayed at the end.
*/
void evaluation(queue<string> tokens)
{
	stack<double> result; // a stack that will hold numerical values from a postfix expression
	double left = 0; // a double representing the left operand
	double right = 0; // a double representing the right operand
	
	cout << "The expression in postfix is: ";

	// while the queue of tokens is not empty
	while(!tokens.empty())
	{
		cout << tokens.front() << ' ';	

		// use the tokens to determine the appropriate operation
		if(tokens.front() == "+") // evaluate addition
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(left + right);
			tokens.pop();
		}
		else if(tokens.front() == "/") // evaluate division
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(left / right);
			tokens.pop();
		}
		else if(tokens.front() == "*") // evaluate multiplication
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(left * right);
			tokens.pop();
		}
		else if(tokens.front() == "-") // evaluate subtraction
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(left - right);
			tokens.pop();
		}
		else if(tokens.front() == "%") // evaluate modulo
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(static_cast<int>(left) % static_cast<int>(right));
			tokens.pop();
		}
		else if(tokens.front() == "^") // evaluate exponential expression
		{
			right = result.top();
			result.pop();
			left = result.top();
			result.pop();
			result.push(pow(left, right));
			tokens.pop();
		}
		else // the token is a numerical value
		{
			// use string stream to convert the string token to a double value
			stringstream s;
			s << tokens.front();
			s >> left;
			
			result.push(left); // push the double value to the top of the stack
			tokens.pop(); // remove the token
		}
	}

	cout << endl << "The result of the expression is: " << result.top() << endl; // display the result
	return;
}
