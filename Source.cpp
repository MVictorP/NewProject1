//Pearson, Matthew

#include <sstream>
#include <cctype>
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

bool precedence(const char& left, const char& right) //function to determing precidence of operators
{
	if (left == '^') //give precidence to the ^ operator
	{
		switch (right)
		{

		case '*':
		case '/':
		case '-':
		case '+':
			return true;
		}
	}
	if (left == '*' || left == '/') //give precidence to the * and / operators before * / = +
	{
		switch (right)
		{
		case '*':
		case '/':
		case '-':
		case '+':
			return true;
		}
	}
	if (left == '+' || left == '-')
	{
		switch (right)
		{
		case '+':
		case '-':
			return true;
		}
	}
	return false;
}

string::size_type find(const string& object, const string& delimiters) //function to find positions of defined delimiters
{
	string::size_type position = INT_MAX; //initialize position value to large integer
	for (char c : delimiters) //iterate through delimiters
	{
		string::size_type tmp = object.find(c); //assign tmp to value of delimiter location in object if found otherwise tmp is unsigned long
		if (c != string::npos && tmp < position) //when delimiter is found and c is not npos
			position = tmp; //assign position to delimiter location
	}
	if (position == INT_MAX) //if position did not get an updated value because there are no delimiters found in object
	{
		return string::npos;
	}
	return position; //return position (object) of first delimiter found each time
}

vector<string> split(string object, string delimiters) //take in input string and delimiters
{
	vector<string> result; //create vector for the result
	string::size_type n;

	while ((n = find(object, delimiters)) != string::npos) //when n and position from find function are equal and not equal to 
	{
		string term = object.substr(0, n); //assign term/number determined by value of n (delimiter location) in object
		if (!term.empty())
		{
			result.push_back(term); //push term/number to result if term has a value
		}
		result.push_back(object.substr(n, 1)); //push delimeter into result
		object = object.substr(n + 1); //change beginning of object
	}
	if (!object.empty())
	{
		result.push_back(object);
	}
	return result;
}

vector<string> postfix(const string& input)
{
	vector<string> expression; //create vector for the expression
	stack<string> oper;

	vector<string> symbol = split(input, "()^*/+-!%"); //send input string and delimiters to split function

	for_each(symbol.begin(), symbol.end(), [&](string symbol)
	{
		if (symbol.length() > 1 && isdigit(symbol[0])) //if the number is greater than 9
		{
			expression.push_back(symbol);
		}
		else if (symbol.length() == 1)
		{
			char c = symbol[0]; //assign c to symbol or number less than or equal to 9
			switch (c)
			{
			case '(': oper.push(symbol); break; //if c is ( push into oper
			case ')':
				if (oper.empty()) //if c is ) and oper has no values, break
				{
					break;
				}
				while (oper.top() != "(") //while top of stack oper is not (
				{
					expression.push_back(oper.top()); //push_back top of oper stack to expression
					oper.pop(); //remove what was added above from oper stack
				}
				oper.pop(); //if top of stack is ( remove it from oper stack
				break;
			case '^':
			case '*':
			case '/':
			case '+':
			case '-':
			case '!':

				while (!oper.empty() && precedence(oper.top()[0], c)) //when oper has a value, go to precedence function
				{
					expression.push_back(oper.top()); //if top of oper stack has precidence over current symbol add to expression stack
					oper.pop(); //remove above from oper
				}
				oper.push(symbol); //push the symbol into oper
				break;

			default:
				if (isdigit(c)) //if the number is less than or equal to 9
				{
					expression.push_back(symbol);
				}
				break;
			}
		}
	});
	while (!oper.empty())
	{
		if (oper.top() != "(") //determine if top of stack is (
		{
			expression.push_back(oper.top()); //add top of oper stack to expression
		}
		oper.pop(); //remove what was added to expression above from oper
	}
	return expression;
}

double evaluate(vector<string> postfix)
{
	if (postfix.empty()) //if input is blank quit
		return 0;

	stack<double> numbers; //create empty stack

	for_each(postfix.begin(), postfix.end(), [&](const string& symbol) //iterate through postfix
	{
		double a;
		static stringstream str;
		str.clear(); //remove contents of stringstream
		if (symbol.length() > 1) //if symbol is digit greater than 9
		{
			str << symbol; //pass value of symbol into stringstream for conversion
			str >> a; //assign converted value to a
			numbers.push(a); //push number on numbers stack
		}
		else if (isdigit(symbol[0])) //if symbol is digit less than or equal to 9
		{
			str << symbol; //pass value of symbol into stringstream for conversion
			str >> a; //assign converted value to a
			numbers.push(a); //push number on numbers stack
		}
		else if (symbol.length() == 1) //if symbol is an operator
		{
			char op = symbol[0]; //create char op and assign to operator in symbol[0]
			double b;

			b = numbers.top(); //assign b value to value at top of stack numbers
			numbers.pop(); //remove value above from top of stack numbers
			a = numbers.top(); //assign a value to value at top of stack numbers
			numbers.pop(); //remove value above rom top of stack numbers

			switch (op) //take values of a and b, perform math and push result to top of stack numbers
			{
			case '^': numbers.push(pow(a, b)); break;
			case '*': numbers.push(a * b); break;
			case '/': numbers.push(a / b); break;
			case '+': numbers.push(a + b); break;
			case '-': numbers.push(a - b); break;
			default: break;
			}
		}
	});
	return numbers.top();
}

void EndProg()
{
	system("pause");
	exit(0);
}

int main()
{
	string input;
	vector<string> postfixIn;

	cout << "Enter An Expression: "; //input interface for user
	getline(cin, input);
	int j = 0;
	for (int i = 0; i < input.length(); i++)
	{
		j++;
		if (isdigit(input[i]) && input[i + 1] == ' ' && isdigit(input[i + 2])) //iterate through string, determine if 2 digits separated by whitespace follow each other
		{
			cout << "Two operands in a row @ char " << ++j << endl; //output error if above is true
			EndProg(); //end program
		}
	}

	input.erase(remove(input.begin(), input.end(), ' '), input.end()); //remove whitespace from input string

	if ((input[0] == '!') && (input[1] == '=')) //if input string starts with != show error
	{
		cout << "Expression can't start with a binary operator @ char: 0\n";
		EndProg(); //end program
	}
	switch (input[0]) //if input string starts with provided characters show error
	{
	case')': cout << "Expression can't start with closing parenthesis @ char: 0\n"; EndProg();
	case'<':
	case'>':
	case'=':
	case'/':
	case'*':
	case'^':
	case'|':
	case'&': cout << "Expression can't start with a binary operator @ char: 0\n"; EndProg();
	}

	int k = 0;
	for (int i = 0; i < input.length(); i++) //iterate through input string to find division by zero
	{
		k++;
		if (input[i] == '/' && input[i + 1] == '0') //if division by zero is found show error
		{
			cout << "Division by zero @ char " << k << endl;
			EndProg(); //end program
		}

	}
	postfixIn = postfix(input); //send input string to postfix function
	cout << "Evaluated Expression: " << evaluate(postfixIn) << endl; //output evauluaton of postfix
	EndProg();
}