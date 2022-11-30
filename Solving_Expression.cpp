#include <iostream>
#include <sstream>
using namespace std;

//constants for the exception handling
const int INVALID_NUMBER = -100;
const int INVALID_EXP = -101;
const int INVALID_OP = -102;
const int EMPTY_STACK = -103;
const int FULL_STACK = -104;

//Template stack class
template<typename T>
class stack
{
private:
    T* stackArray; //pointer to stack array
    int stackSize; //The stack size
    int top; //variable to keep track of top of the stack

public:
    //Constructor
    stack(int size)
    {
        stackArray = new T[size];
        stackSize = size;
        top = -1;
    }

    //to check if the stack is full
    bool isFull() const
    {
        bool status;

        if (top == stackSize - 1) status = true;
        else status = false;
        return status;
    }

    //to check if the stack is empty
    bool isEmpty() const
    {
    	bool status;

        if (top == -1) status = true;
        else status = false;

        return status;
    }


    //insert a value into the stack
    void push(T num)
    {
	    if (isFull())
	    {
            throw FULL_STACK;
	    }
        else
        {
            top++;
            stackArray[top] = num;
        }
    }

    //gets the top value and remove it from the stack
    void pop(T &num)
    {
	    if(isEmpty())
	    {
            //cout << "The stack is empty.\n";
            throw EMPTY_STACK;
	    }
        else
        {
            num = stackArray[top];
            top--;
        }
    }
    //look at the top of the stack
    T get_top()
    {
        if (isEmpty())
        {
            throw EMPTY_STACK;
        }
        return stackArray[top];
    }



   
};
//this function calculates the precedence of an operator
int calculate_precedence (string op)
{
    if (op == "+" || op == "-") return 0;
    if (op == "/") return 1;
    if (op == "*") return 2;
    //if it's not an operator in cases of ( or )
    return -1;
}

//function to evaluate two numbers and their respective operator
int evaluate(stack<int>& num_stk,stack<string>& op_stk)
{
    int a;
    int b;
    string op;
    try
    {
        num_stk.pop(a);
        num_stk.pop(b);
        op_stk.pop(op);
    }
    //if the stack is empty which means one of the stacks is empty throw exception
    catch (int e)
    {
        if (e == EMPTY_STACK) { throw INVALID_EXP; }
    }
    if (op == "+") return a + b;
    else if (op == "-") return b - a;
    else if (op == "*") return a * b;
    else 
    {
        //if dividing by zero throw an exception
        if (a == 0) throw INVALID_EXP;
        return b / a;
    }
}



//check the digits of a string to determine whether it's a number or a wrong number
void digit_check(string input)
{
    bool has_digit = false;
    bool all_digit = true;
    

    for (char i : input)
    {
        if (!isdigit(i)) { all_digit = false; }
        else has_digit = true;
    }

    if (has_digit && !all_digit) throw INVALID_NUMBER;
}


//function to parse input and evaluate the expression
int solve(string input)
{
    //string stream to work with the string
    stringstream ss;
    //Reading the whole input
    ss << input;
    //parsing input word by word
    string temp;
    stack<int> number_stack(10);
    stack<string> oper_stack(10);
    int exp_num;


    while (!ss.eof())
    {

        //reading a word from the string
        ss >> temp;
        //checking if it's number and whether it's valid
        digit_check(temp);
        if (temp == "(")
        {
            oper_stack.push(temp);
        }

        else if (temp == "*" || temp == "+" || temp == "-" || temp == "/")
        {
            while (!oper_stack.isEmpty() && calculate_precedence(oper_stack.get_top()) > calculate_precedence(temp))
            {
                number_stack.push(evaluate(number_stack, oper_stack));
            }
            oper_stack.push(temp);
        }

        else if (temp == ")")
        {
            try
            {
                while (oper_stack.get_top() != "(")
                {
                    number_stack.push(evaluate(number_stack, oper_stack));
                }
                //this variable is only here to pass something to the pop function
                string flush;
                oper_stack.pop(flush);

            }
            catch(int e)
            {
                throw INVALID_EXP;
            }
        }
        //If it's a number
        else if (isdigit(temp.at(0)) || (temp.at(0) == '-' && isdigit(temp.at(1))))
        {
            stringstream(temp) >> exp_num;
            number_stack.push(exp_num);
        }
        //invalid operator
        else
        {
            throw INVALID_OP;
        }
        //resetting temp for each run
        temp = "";
    }
    //evaluating the remaining of the stack
    while (!oper_stack.isEmpty())
    {
        number_stack.push(evaluate(number_stack, oper_stack));
    }

    //returning and printing the result
    cout << number_stack.get_top() << endl;

    return number_stack.get_top();


}

void print_error(int e)
{
    if (e == INVALID_OP)
    {
        cout << "ERROR: invalid operator(s)\n";
    }
    else if (e == INVALID_EXP)
    {
        cout << "ERROR: invalid expression\n";
    }
    else if (e == INVALID_NUMBER)
    {
        cout << "ERROR: invalid number\n";
    }
}

int main()
{
    try
    {
	    string input1 = "3 * ( ( ( 4 + 5 ) / 3 ) * ( 20 + 1 ) )";
        cout << "Evaluating: " << input1 << endl;
        int x = solve(input1);
    }
    catch (int e)
    {
        print_error(e);
    }
    try
    {
        string input2 = "3 * ( ( ( 4b + 5 ) / 3 ) * ( 20 + 1 ) )";
        cout << "Evaluating: " << input2 << endl;
        int y = solve(input2);
    }
    catch (int e)
    {
        print_error(e);
    }
    try
    {
        string input3 = "3 * ( ( ( 4 + 5 ) / 3 ) * ( 20 + 1 )";
        cout << "Evaluating: " << input3 << endl;
        int z = solve(input3);
    }
    catch (int e)
    {
        print_error(e);
    }
    try
    {
        string input4 = "3 * ( ( ( 4 < 5 ) / 3 ) * ( 20 + 1) )";
        cout << "Evaluating: " << input4 << endl;
        int g = solve(input4);
    }
    catch (int e)
    {
        print_error(e);
    }
}

