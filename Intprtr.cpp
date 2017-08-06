
/////////////////////////////////////////////
//Author: Olurotimi Falana
//Subject: CS Concepts of Programming Languages
//Description: Interpreter Project
//Derived from Zach Livingston and Dr.Richard Watson
//Date: 3/28/17
/////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;

//Declarations
bool B(string, int&);

bool IT(string, int&);
bool IT_Tail(string, int&);

bool OT(string, int&);
bool OT_Tail(string, int&);

bool AT(string, int&);
bool AT_Tail(string, int&);


bool L(string, int&);
bool A(string, int&);

string get_Expression();
string eraseDelimiters(string);
void ErrorMessage(string,int, string);
bool ValidityCheck(string, int);
stack<char> BooleanExpression;

int main()
{
	//Only Loop, Program goes forever.
	//while(1){}
	bool ValidityCheck = false;
	
	//Get input
	while (!ValidityCheck)
	{
		//position
		int pos = 0;
		//Get Comparison String and erase Delimiters
		string X = get_Expression();
		X = eraseDelimiters(X);
		if (X != "False")
		{
			ValidityCheck = B(X, pos);
		}
		if(ValidityCheck)
		{
			cout << "\nValue of Expression: " << BooleanExpression.top() << endl;
			BooleanExpression.pop();
			cout << "\n";
		}
	}
	
}

//Get input from user
string get_Expression()
{
	string Expression = "";
	cout << "Enter Expression - Case Sensitive:" << endl;
	cout << "Legend: [T = True] [F = False] [~ = Not] [v = Or] [^ = And] [-> = Imply]\n" << endl;
	getline(cin, Expression);
	
	return Expression;
}

//Erases delimiters
string eraseDelimiters(string X)
{
	//Case for implication
	size_t found = X.find("- >");
	if (found !=string::npos)
	{
		ErrorMessage(X, ++found, " [ Invalid Operator] ");
		X = "False";
		return X;
	}
	else
	{
		X.erase(remove_if(X.begin(), X.end(), isspace), X.end());
		return X;
	}
}
//Notifies position where error has occured
void ErrorMessage(string X, int pos, string Function)
{
	X = X.insert(pos, Function);
	cout << "\nInvalid Expression occured at position " << pos << endl;
	cout << X << endl;
}
//B first, if last character is a period then check if IT is true
bool B(string X, int &pos)
{

	if (X[X.length()-1] != '.')
	{
		ErrorMessage(X, X.length(), "  [Missing Period]\n ");
		return false;
	}
	else if (IT(X,pos))
	{
		++pos;
		return true;
	}
	else {
		return false;
	}
}

//If OT is true, sends the expression to check IT Tail or Imply Terms
bool IT(string X, int &pos)
{
	if (OT(X, pos))
	{
		if (IT_Tail(X, pos))
		{
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

//Check if there is an implied term in the expression
bool IT_Tail(string X, int &pos)
{

	if (X[pos] == '-' && X[pos + 1] == '>')
	{

		pos+=2;
		if (OT(X, pos))
		{
			if (IT_Tail(X, pos))
			{
				char R_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				char L_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				if (L_Expr == 'F' || (L_Expr == 'T' && R_Expr == 'T'))
				{
					BooleanExpression.push('T');
				}
				else
				{
					BooleanExpression.push('F');
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if (X[pos] == '.' || X[pos] == ')')
	{
		return true;
	}
	else {
		return false;
	}
}


//If true, expression is sent to OT Tail for or terms
bool OT(string X, int &pos)
{
	if (AT(X, pos))
	{
		if (OT_Tail(X, pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
//Checks for or terms in the expression
bool OT_Tail(string X, int &pos)
{

	if (X[pos] == 'v')
	{
		++pos;

		if (AT(X, pos))
		{
			if (OT_Tail(X, pos))
			{
				//Evaluate or expression
				char R_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				char L_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				if (L_Expr == 'F' && R_Expr == 'F')
				{
					BooleanExpression.push('F');
				}
				else
				{
					BooleanExpression.push('T');
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if ((X[pos] == '-' && X[pos + 1] == '>') || X[pos] == '.' || X[pos] == ')')
	{
		return true;
	}
	else
	{ 
		return false; 
	}
}

//L Check, if true sends the expression to AT Tail for And Terms
bool AT(string X, int &pos)
{
	if (L(X, pos))
	{
		if (AT_Tail(X, pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		return false;
	}
}
bool AT_Tail(string X, int &pos)
{
	if (X[pos] == '^')
	{
		++pos;
		if (L(X, pos))
		{
			if (AT_Tail(X, pos))
			{
				//Evaluate and term
				char R_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				char L_Expr = BooleanExpression.top();
				BooleanExpression.pop();
				if (L_Expr == 'T' && R_Expr == 'T')
				{
					BooleanExpression.push('T');
				}
				else
				{
					BooleanExpression.push('F');
				}
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	//delimiters check
	else if ((X[pos] == '-' && X[pos + 1] == '>') || X[pos] == '.' || X[pos] == ')' || X[pos] == 'v')
	{
		return true;
	}
	else
	{
		ErrorMessage(X, ++pos, "   [Invalid Opperation] ");
		return false;
	}
}
//Check for Literals and returns to A for atom
bool L(string X, int &pos)
{
	if (A(X, pos))
	{
		return true;
	}
	else if (X[pos] == '~')
	{
		//Find inverse of the NOT Statement
		++pos;
		if (X[pos] == 'T')
		{
			X[pos] = 'F';
		}
		else
		{
			X[pos] = 'T';
		}
		L(X, pos);
		return true;
	}
	else
	{
		return false;
	}
}


//Atom Check and Conversion  
bool A(string X, int &pos)
{
	
	if (X[pos] == 'T' || X[pos] == 'F')
	{
		BooleanExpression.push(X[pos]);
		++pos;
		return true;
	}
	else if (X[pos] == '(')
	{
		++pos;
		IT(X, pos);
		return true;
	}
	else {
		ErrorMessage(X, ++pos, "    [Invalid Atom.] ");
		return false;
	}
}