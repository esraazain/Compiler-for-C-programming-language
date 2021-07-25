#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <iomanip>

using namespace std;

enum Token
{
    MAIN, IF, ELSE, INT, CHAR, FLOAT, OTHER, RETURN, CONSTANT, LEFT_PRANTH, RIGHT_PRANTH, SPECIAL_CHARACTER,
    LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET, LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET, LETTERS,INT_NUMBER,NEGATIVE_INT_NUMBER,
    UNDERSCORE, DOLLAR_SIGN, EXCLAMATION_MARK, DOT, QUESTION_MARK, PERCENTAGE_MARK, AMPERSAND,FLOAT_NUMBER,NEGATIVE_FLOAT_NUMBER,
    AT, HASH, EQUIV, CARET, CRAVE, COMA, MINUS, PLUS, LOE, SMALLER, EQUAL, GREATER, GOE, NE, MULT,
    DIVI, COLON, SEMICOLON, END_SOURCE, DIGITS, CHARACTER, IDENTIFIER, SINGLE_QUOTE, ERROR
};

class compiler
{
public:
    compiler(string file_name)
    {
        scanner(file_name);
        program();
    }

private:
    vector<pair<pair<string, Token>, int>> code;
    ifstream file;
    int line_number = 1, itr = 0;

    Token check_reserved(string str)
    {
        if (str == "main")
            return MAIN;
        else if (str == "const")
            return CONSTANT;
        else if (str == "int")
            return INT;
        else if (str == "char")
            return CHAR;
        else if (str == "float")
            return FLOAT;
        else if (str == "if")
            return IF;
        else if (str == "else")
            return ELSE;
        else if (str == "return")
            return RETURN;
        else if (regex_match(str, regex("[a-zA-Z_$][a-zA-Z0-9_$]*")))
            return IDENTIFIER;
        else
            return ERROR;
    }

    Token get_token()
    {
        char chr;
        string str;

        file.get(chr);

        while (isspace(chr) && !file.eof())
        {
            if (chr == '\n')
                line_number += 1;
            file.get(chr);
        }

        if (file.eof())
        {
            code.push_back(make_pair(make_pair("EOF", END_SOURCE), line_number));
            return END_SOURCE;
        }
        else if (chr == ',')
        {
            code.push_back(make_pair(make_pair(string(1, chr), COMA), line_number));
            return COMA;
        }
        else if (chr == ';')
        {
            code.push_back(make_pair(make_pair(string(1, chr), SEMICOLON), line_number));
            return SEMICOLON;
        }
        else if (chr == '.')
        {
            str = chr;
            file.get(chr);
            if(isdigit(chr))
            {
                while (isdigit(chr) || chr == '.')
                {
                    str += chr;
                    file.get(chr);
                }
                file.putback(chr);
                if(regex_match(str, regex("[+-]?([0-9]*[.])?[0-9]+")))
                {
                    code.push_back(make_pair(make_pair(str, FLOAT_NUMBER), line_number));
                    return FLOAT_NUMBER;
                }
                else
                {
                    code.push_back(make_pair(make_pair(str, ERROR), line_number));
                    return ERROR;
                }
            }
            else
            {
                str += chr;
                code.push_back(make_pair(make_pair(str, ERROR), line_number));
                return ERROR;
            }
        }
        else if (chr == '(')
        {
            code.push_back(make_pair(make_pair(string(1, chr), LEFT_PRANTH), line_number));
            return LEFT_PRANTH;
        }
        else if (chr == ')')
        {
            code.push_back(make_pair(make_pair(string(1, chr), RIGHT_PRANTH), line_number));
            return RIGHT_PRANTH;
        }
        else if (chr == '{')
        {
            code.push_back(make_pair(make_pair(string(1, chr), LEFT_CURLY_BRACKET), line_number));
            return LEFT_CURLY_BRACKET;
        }
        else if (chr == '}')
        {
            code.push_back(make_pair(make_pair(string(1, chr), RIGHT_CURLY_BRACKET), line_number));
            return RIGHT_CURLY_BRACKET;
        }
        else if (chr == '[')
        {
            code.push_back(make_pair(make_pair(string(1, chr), LEFT_SQUARE_BRACKET), line_number));
            return LEFT_SQUARE_BRACKET;
        }
        else if (chr == ']')
        {
            code.push_back(make_pair(make_pair(string(1, chr), RIGHT_SQUARE_BRACKET), line_number));
            return RIGHT_SQUARE_BRACKET;
        }
        else if (chr == '<')
        {
            code.push_back(make_pair(make_pair(string(1, chr), SMALLER), line_number));
            return SMALLER;
        }
        else if (chr == '>')
        {
            code.push_back(make_pair(make_pair(string(1, chr), GREATER), line_number));
            return GREATER;
        }
        else if (chr == '!')
        {
            code.push_back(make_pair(make_pair(string(1, chr), EXCLAMATION_MARK), line_number));
            return EXCLAMATION_MARK;
        }
        else if (chr == '=')
        {
            code.push_back(make_pair(make_pair(string(1, chr), EQUAL), line_number));
            return EQUAL;
        }
        else if (chr == '*')
        {
            code.push_back(make_pair(make_pair(string(1, chr), MULT), line_number));
            return MULT;
        }
        else if (chr == '/')
        {
            code.push_back(make_pair(make_pair(string(1, chr), DIVI), line_number));
            return DIVI;
        }
        else if (chr == '+')
        {
            code.push_back(make_pair(make_pair(string(1, chr), PLUS), line_number));
            return PLUS;
        }
        else if (chr == '-')
        {
            str = chr;
            file.get(chr);
            if(isdigit(chr))
            {
                if(code.back().first.second == FLOAT_NUMBER || code.back().first.second == INT_NUMBER || code.back().first.second == CHARACTER)
                {
                    file.putback(chr);
                    code.push_back(make_pair(make_pair("-", MINUS), line_number));
                    return MINUS;
                }
                else
                {
                    str += chr;
                    file.get(chr);
                    while(isdigit(chr) || chr == '.')
                    {
                        str += chr;
                        file.get(chr);
                    }
                    file.putback(chr);

                    if(regex_match(str, regex("[+-]?[0-9][0-9]*")))
                    {
                        code.push_back(make_pair(make_pair(str, NEGATIVE_INT_NUMBER), line_number));
                        return INT_NUMBER;
                    }
                    else if (regex_match(str, regex("[+-]?([0-9]*[.])?[0-9]+")))
                    {
                        code.push_back(make_pair(make_pair(str, NEGATIVE_FLOAT_NUMBER), line_number));
                        return FLOAT_NUMBER;
                    }
                    else
                    {
                        code.push_back(make_pair(make_pair(str, ERROR), line_number));
                        return ERROR;
                    }
                }
            }
            else
            {
                code.push_back(make_pair(make_pair(str, MINUS), line_number));
                return MINUS;
            }
        }
        else if (isalpha(chr) || chr == '$' || chr == '_')
        {
            str = chr;
            file.get(chr);
            while (isalnum(chr) || chr == '$' || chr == '_')
            {
                str += chr;
                file.get(chr);
            }
            file.putback(chr);
            code.push_back(make_pair(make_pair(str, check_reserved(str)), line_number));
            return check_reserved(str);
        }
        else if (isdigit(chr))
        {
            str = chr;
            file.get(chr);
            while (isdigit(chr) || chr == '.')
            {
                str += chr;
                file.get(chr);
            }
            file.putback(chr);

            if(regex_match(str, regex("[+-]?[0-9][0-9]*")))
            {
                code.push_back(make_pair(make_pair(str, INT_NUMBER), line_number));
                return INT_NUMBER;
            }
            else if (regex_match(str, regex("[+-]?([0-9]*[.])?[0-9]+")))
            {
                code.push_back(make_pair(make_pair(str, FLOAT_NUMBER), line_number));
                return FLOAT_NUMBER;
            }
            else
            {
                code.push_back(make_pair(make_pair(str, ERROR), line_number));
                return ERROR;
            }
        }
        else if (chr == '\'')
        {
            str = chr;
            file.get(chr);
            if (isalpha(chr))
            {
                str += chr;
                file.get(chr);
                if (chr == '\'')
                {
                    str += chr;
                    code.push_back(make_pair(make_pair(str, CHARACTER), line_number));
                    return CHARACTER;
                }
                else
                {
                    code.push_back(make_pair(make_pair(str, ERROR), line_number));
                    return ERROR;
                }
            }
            code.push_back(make_pair(make_pair(str, ERROR), line_number));
            return ERROR;
        }
        else if ((chr >= 33 && chr <= 47) || (chr >= 58 && chr <= 64) || (chr >= 91 && chr <= 96) || (chr >= 123 && chr <= 126))
        {
            code.push_back(make_pair(make_pair(string(1, chr), SPECIAL_CHARACTER), line_number));
            return SPECIAL_CHARACTER;
        }
        else
        {
            code.push_back(make_pair(make_pair(string(1, chr), ERROR), line_number));
            return ERROR;
        }
    }

    string token_name(Token t)
    {
        if(t == END_SOURCE)
        {
            return "END OF FILE";
        }
        else if (t == IDENTIFIER)
        {
            return "IDENTIFIER TOKEN";
        }
        else if (t == COMA)
        {
            return "COMA TOKEN";
        }
        else if (t == SEMICOLON)
        {
            return "SEMICOLON TOKEN";
        }
        else if (t == FLOAT_NUMBER)
        {
            return "FLOAT_NUMBER TOKEN";
        }
        else if (t == NEGATIVE_FLOAT_NUMBER)
        {
            return "NEGATIVE_FLOAT_NUMBER TOKEN";
        }
        else if (t == INT_NUMBER)
        {
            return "INT_NUMBER TOKEN";
        }
        else if (t == NEGATIVE_INT_NUMBER)
        {
            return "NEGATIVE_INT_NUMBER TOKEN";
        }
        else if (t == LEFT_PRANTH)
        {
            return "LEFT_PRANTH TOKEN";
        }
        else if (t == RIGHT_PRANTH)
        {
            return "RIGHT_PRANTH TOKEN";
        }
        else if (t == LEFT_CURLY_BRACKET)
        {
            return "LEFT_CURLY_BRACKET TOKEN";
        }
        else if (t == RIGHT_CURLY_BRACKET)
        {
            return "RIGHT_CURLY_BRACKET TOKEN";
        }
        else if (t == LEFT_SQUARE_BRACKET)
        {
            return "LEFT_SQUARE_BRACKET TOKEN";
        }
        else if (t == RIGHT_SQUARE_BRACKET)
        {
            return "RIGHT_SQUARE_BRACKET TOKEN";
        }
        else if (t == SMALLER)
        {
            return "SMALLER TOKEN";
        }
        else if (t == GREATER)
        {
            return "GREATER TOKEN";
        }
        else if (t == EXCLAMATION_MARK)
        {
            return "EXCLAMATION_MARK TOKEN";
        }
        else if (t == EQUAL)
        {
            return "EQUAL TOKEN";
        }
        else if (t == MULT)
        {
            return "MULT TOKEN";
        }
        else if (t == DIVI)
        {
            return "DIVI TOKEN";
        }
        else if (t == PLUS)
        {
            return "PLUS TOKEN";
        }
        else if (t == MINUS)
        {
            return "MINUS TOKEN";
        }
        else if (t == DOLLAR_SIGN)
        {
            return "DOLLAR_SIGN TOKEN";
        }
        else if (t == CHARACTER)
        {
            return "CHARACTER TOKEN";
        }
        else if (t == LETTERS)
        {
            return "LETTERS TOKEN";
        }
        else if (t == DIGITS)
        {
            return "DIGITS TOKEN";
        }
        else if (t == UNDERSCORE)
        {
            return "UNDERSCORE TOKEN";
        }
        else if (t == SPECIAL_CHARACTER)
        {
            return "SPECIAL_CHARACTER TOKEN";
        }
        else if (t == MAIN)
        {
            return "MAIN TOKEN";
        }
        else if (t == IF)
        {
            return "IF TOKEN";
        }
        else if (t == ELSE)
        {
            return "ELSE TOKEN";
        }
        else if (t == INT)
        {
            return "INT TOKEN";
        }
        else if (t == CHAR)
        {
            return "CHAR TOKEN";
        }
        else if (t == FLOAT)
        {
            return "FLOAT TOKEN";
        }
        else if (t == RETURN)
        {
            return "RETURN TOKEN";
        }
        else if (t == CONSTANT)
        {
            return "CONSTANT TOKEN";
        }
        else
        {
            return "ERROR";
        }
    }

    void scanner(string file_name)
    {
        file.open(file_name);
        for (int i = 0; get_token() != END_SOURCE; i++)
        {
            cout << code[i].first.first << "\t\t";
            cout << setw(25);
            cout << token_name(code[i].first.second)  << "\t";
            cout << setw(10);
            cout << code[i].second;// << endl;

            cout << setw(10);
            cout << i << endl;
        }
        cout << "\n";
        file.close();
    }

    bool match_and_step(Token T)
    {
        if(code[itr].first.second == T)
        {
            itr++;
            return true;
        }
        return false;
    }

    bool match(Token T)
    {
        if(code[itr].first.second == T)
        {
            return true;
        }
        return false;
    }

    // <program> ::= int main() {<statements> <return statement>} | int main() {<return statement>;}
    void program()
    {
        if (match_and_step(INT))
        {
            if(match_and_step(MAIN))
            {
                if(match_and_step(LEFT_PRANTH))
                {
                    if(match_and_step(RIGHT_PRANTH))
                    {
                        if(match_and_step(LEFT_CURLY_BRACKET))
                        {
                            if(!statements())
                            {
                                return;
                            }
                            else if(match_and_step(RIGHT_CURLY_BRACKET))
                            {
                                cout << "Successed ^_^" << endl;
                            }
                            else
                            {
                                cout << "error: expected \'}\' after \';\' token \t line : " << code[itr].second << endl;
                            }
                        }
                        else
                        {
                            cout << "error: expected \'{\' after \')\' token \t line : " << code[itr].second << endl;
                        }
                    }
                    else
                    {
                        cout << "error: expected \')\' after \'(\' token \t line : " << code[itr].second << endl;
                    }
                }
                else
                {
                    cout << "error: expected \'(\' after main token \t line : " << code[itr].second << endl;
                }
            }
            else
            {
                cout << "error: expected main after int token \t line : " << code[itr].second << endl;
            }
        }
        else
        {
            cout << "error: expected int token \t line : " << code[itr].second << endl;
        }
    }

    // <statements> ::= <statement>; | <statement>; <statements>
    bool statements()
    {
        while (true)
        {
            if(statement())
            {
                if(!match(SEMICOLON) && !match(RIGHT_CURLY_BRACKET))
                {
                    itr++;
                    cout << "error: \';\' or \'}\' token missed \t line : " << code[itr].second-1 << endl;
                    return false;
                }
                else if(match(RIGHT_CURLY_BRACKET))
                {
                    if(code[itr-1].first.second == LEFT_CURLY_BRACKET)
                    {
                        itr++;
                        if(match(SEMICOLON))
                        {
                            itr++;
                        }
                    }
                    else if(code[itr-1].first.second == SEMICOLON)
                    {
                        if(code[itr].first.second != code[code.size()-1].first.second)
                        {
                            return true;
                        }
                        itr++;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if(match(LEFT_CURLY_BRACKET))
                {
                    if(code[itr+1].first.second == RIGHT_CURLY_BRACKET)
                    {
                        if(code[itr+2].first.second == SEMICOLON)
                        {
                            itr+=3;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    itr++;
                }
            }
            else if(match(RIGHT_CURLY_BRACKET))
            {
                if(code[itr-1].first.second == LEFT_CURLY_BRACKET)
                {
                    itr++;
                    if(match(SEMICOLON))
                    {
                        itr++;
                    }
                }
                else if(code[itr-1].first.second == SEMICOLON)
                {
                    if(code[itr].first.second != code[code.size()-1].first.second)
                    {
                        return true;
                    }
                    itr++;
                }
                else
                {
                    return false;
                }
            }
            else if(match(LEFT_CURLY_BRACKET))
            {
                if(code[itr+1].first.second == RIGHT_CURLY_BRACKET)
                {
                    if(code[itr+2].first.second == SEMICOLON)
                    {
                        itr+=3;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
        }
    }

    // <statement> :: = <declaration statement> | <if statement> | <return statement> | <arithmetic expression>
    bool statement()
    {
        if (assignment_expression() | declaration_statement() | return_statement() | if_statement())
            return true;
        return false;
    }

    //<if statement> ::= <if part> |<if part> <else part>
    //<if part> ::= if(<expression>) <statements> | if(<expression>) <block> | if(<expression>) <empty statement>
    //<else part> ::= else <statement> | else <block>
    bool if_statement()
    {
        int old_position = itr;
        if(match_and_step(IF))
        {
            if(match_and_step(LEFT_PRANTH))
            {
                if(conditional_expression())
                {
                    if(match_and_step(RIGHT_PRANTH))
                    {
                        if(match_and_step(LEFT_CURLY_BRACKET))
                        {
                            if(statements())
                            {
                                if(match_and_step(RIGHT_CURLY_BRACKET))
                                {
                                    if(match_and_step(ELSE))
                                    {
                                        if(match(LEFT_CURLY_BRACKET))
                                        {
                                            itr++;
                                            if(statements())
                                            {
                                                if(match_and_step(RIGHT_CURLY_BRACKET))
                                                {
                                                    return true;
                                                }
                                                else
                                                {
                                                    cout << "expect \'}\' after \';\' token \t line : " << code[itr].second << endl;
                                                    return false;
                                                }
                                            }
                                            else
                                            {
                                                cout << "expect stateents after \'{\' token \t line : " << code[itr].second << endl;
                                                return false;
                                            }
                                        }
                                        else if(statement())
                                        {
                                            return true;
                                        }
                                        else
                                        {
                                            cout << "expect statement after \'else\' token \t line : " << code[itr].second << endl;
                                            return false;
                                        }
                                    }
                                    itr--;
                                    return true;
                                }
                                else
                                {
                                    cout << "expect \'}\' after \';\' token \t line : " << code[itr].second << endl;
                                    return false;
                                }
                            }
                            else if (match(RIGHT_CURLY_BRACKET))
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else if(statement())
                        {
                            return true;
                        }
                        else if(match(SEMICOLON))
                        {
                            return true;
                        }
                        else
                        {
                            cout << "expect \';\' token or statement or block of statements \t line : " << code[itr].second << endl;
                            return false;
                        }
                    }
                    else
                    {
                        cout << "expect \')\' token after conditional expression \t line : " << code[itr].second << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "expect conditional expression after \'(\' token \t line : " << code[itr].second << endl;
                    return false;
                }
            }
            else
            {
                cout << "expect \'(\' token after \'if\' token \t line : " << code[itr].second << endl;
                return false;
            }
        }
        else
        {
            itr = old_position;
            return false;
        }
    }

    // <declaration statement> ::=  <type> <identifier>
    //                            | <type> <assignment expression>
    //                            | const <type> <assignment expression>
    //
    bool declaration_statement()
    {
        int old_position = itr;
        if (match(INT) || match(FLOAT) || match(CHAR))
        {
            itr++;
            if(assignment_expression())
            {
                return true;
            }
            else if (match(IDENTIFIER))
            {
                itr++;
                return true;
            }
            else
            {
                cout << "expected assignment expression \t line : " << code[itr].second << endl;
                return false;
            }
        }
        else if (match(CONSTANT) && (match(INT) || match(FLOAT) || match(CHAR)))
        {
            itr++;
            if (assignment_expression())
            {
                return true;
            }
            else
            {
                cout << "constant should be intialized \t line : " << code[itr].second << endl;
                return false;
            }
        }
        else
        {
            itr = old_position;
            return false;
        }
    }

    //<assignment expression> ::= <identifier> <assignment operator> <arithmetic expression>
    //					         |<identifier> <assignment operator> <conditional expression>
    //					         |<identifier> <assignment operator> <constant>
    bool assignment_expression()
    {
        int old_position = itr;
        if(match_and_step(IDENTIFIER))
        {
            if(match_and_step(EQUAL) && !match(GREATER) && !match(SMALLER))
            {
                if(arithmetic_expression())
                {
                    return true;
                }
                else if(conditional_expression())
                {
                    return true;
                }
                else if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) )
                {
                    itr++;
                    if(match(SEMICOLON))
                    {
                        return true;/////////////////////////
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    cout << "expected arithmetic expression or conditional expression or integer or float or charater \t line : " << code[itr].second << endl;
                    return false;
                }
            }
            else
            {
                itr = old_position;
                return false;
            }
        }
        itr = old_position;
        return false;
    }

    //<arithmetic expression> ::= <identifier> <arithmetic operator> <identifier>
	//						     |<identifier> <arithmetic operator> <constant>
	//						     |<identifier> <arithmetic operator> <arithmetic expression>
	//						     |<constant> <arithmetic operator> <constant>
	//						     |<constant> <arithmetic operator> <identifier>
	//						     |<constant> <arithmetic operator> <arithmetic expression>
	//						     |<arithmetic expression> <arithmetic operator> <identifier>
	//						     |<arithmetic expression> <arithmetic operator> <constant>
	//						     |(<arithmetic expression>)
	//						     |<arithmetic expression> <arithmetic operator> <arithmetic expression>
    bool arithmetic_expression()
    {
        int old_position = itr;
        if(match(IDENTIFIER) || match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER))
        {
            itr++;
            if(match(MULT) | match(DIVI) | match(PLUS) | match(MINUS))
            {
                itr++;
                if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) || match(IDENTIFIER))
                {
                    itr++;
                    if(match(MULT) | match(DIVI) | match(PLUS) | match(MINUS))
                    {
                        itr++;
                        if(arithmetic_expression())
                        {
                            return true;
                        }
                        else if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) || match(IDENTIFIER))
                        {
                            itr++;
                            if(match(SEMICOLON) || match(RIGHT_PRANTH))
                            {
                                return true;
                            }
                            else
                            {
                                cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                                return false;
                            }
                        }
                        else
                        {
                            cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                            return false;
                        }
                    }
                    else if(!match(SEMICOLON) && !match(RIGHT_PRANTH))
                    {
                        cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else if(match(LEFT_PRANTH))
                {
                    itr++;
                    if(arithmetic_expression())
                    {
                        if(match_and_step(RIGHT_PRANTH))
                        {
                            if(match(MULT) | match(DIVI) | match(PLUS) | match(MINUS))
                            {
                                itr++;
                                if(arithmetic_expression())
                                {
                                    return true;
                                }
                                else if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) || match(IDENTIFIER))
                                {
                                    itr++;
                                    if(match(SEMICOLON) || match(RIGHT_PRANTH))
                                    {
                                        return true;
                                    }
                                    else
                                    {
                                        cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                                        return false;
                                    }
                                }
                                else
                                {
                                    cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                                    return false;
                                }
                            }
                            else if(!match(SEMICOLON) && !match(RIGHT_PRANTH))
                            {
                                cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                                return false;
                            }
                            else
                            {
                                return true;
                            }
                        }
                        else
                        {
                            cout << "expected \')\' token \t line : " << code[itr].second << endl;
                            return false;
                        }
                    }
                }
                else
                {
                    cout << "expect integer or float or charater or identifier after \'"<< code[itr].first.second<<"\'"<<" \t line : " << code[itr].second << endl;
                    return false;
                }
            }
            else if(match(RIGHT_PRANTH))
            {
                itr++;
                if(!match(SEMICOLON) && !match(RIGHT_PRANTH))
                {
                    cout << "\')\' token missed after \'" << code[itr].first.first << "\' \t line :" << code[itr].second << endl;
                    return false;
                }
                itr--;
                return true;
            }
            else
            {
                itr = old_position;
                return false;
            }
        }
        else if(match_and_step(LEFT_PRANTH))
        {
            if(arithmetic_expression())
            {
                if(match_and_step(RIGHT_PRANTH))
                {
                    if(match(MULT) | match(DIVI) | match(PLUS) | match(MINUS))
                    {
                        itr++;
                        if(arithmetic_expression())
                        {
                            return true;
                        }
                        else if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) || match(IDENTIFIER))
                        {
                            itr++;
                            if(match(SEMICOLON) || match(RIGHT_PRANTH))
                            {
                                return true;
                            }
                            else
                            {
                                cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                                return false;
                            }
                        }
                        else
                        {
                            cout << "not valid arithmetic expression \t line : " << code[itr].second << endl;
                            return false;
                        }
                    }
                    else if(!match(SEMICOLON) && !match(RIGHT_PRANTH))
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                else
                {
                    cout << "expected \')\' token \t line : " << code[itr].second << endl;
                    return false;
                }
            }
        }
        else
        {
            itr = old_position;
            return false;
        }
    }

    //<conditional expression> ::= <identifier> <conditional operator> <identifier>
	//							  |<constant> <conditional operator> <constant>
	//							  |<identifier> <conditional operator> <constant>
	//                            |<constant> <conditional operator> <identifier>
	bool conditional_expression()
	{
	    int old_position = itr;
	    if(match(IDENTIFIER))
        {
            itr++;
            if(conditional_operator())
            {
                if(match(IDENTIFIER) || match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER))
                {
                    itr++;
                    return true;
                }
                else
                {
                    cout << "undefined expression \t line : " << code[itr].second << endl;
                    return false;
                }
            }
            else
            {
                itr = old_position;
                return false;
            }
        }
        else if(match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER))
        {
            itr++;
            if(conditional_operator())
            {
                if(match(IDENTIFIER) || match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER))
                {
                    itr++;
                    return true;
                }
                else
                {
                    cout << "expected conditional expression \t line : " << code[itr].second << endl;
                    return false;
                }
            }
            else
            {
                itr = old_position;
                return false;
            }
        }
        else
        {
            itr = old_position;
            return false;
        }
	}

	//<conditional operator> ::= < | > | >= | <= | == | !=
	bool conditional_operator()
	{
	    if(match(GREATER) || match(SMALLER))
        {
            itr++;
            if(match(EQUAL))
            {
                itr++;
                return true;
            }
            return true;
        }
        else if(match(EXCLAMATION_MARK))
        {
            itr++;
            if(match_and_step(EQUAL))
            {
                return true;
            }
            else
            {
                cout << "expected \'=\' token after \'!\' token \t line : " << code[itr].second << endl;
                return false;
            }
        }
        else if(match(EQUAL))
        {
            itr++;
            if(match_and_step(EQUAL))
            {
                return true;
            }
            else
            {
                cout << "not valid expression \t line : " << code[itr].second << endl;
                if(match(SEMICOLON))
                {
                    return false;
                }
                else
                {
                    while(!match(SEMICOLON))
                    {
                        itr++;
                    }
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
	}

    // <return statement> ::= return <integer number> | return <float number> | return <charater>
    bool return_statement()
    {
        int old_position = itr;
        if(match_and_step(RETURN))
        {
            if( match(INT_NUMBER) || match(NEGATIVE_INT_NUMBER) || match(FLOAT_NUMBER) || match(NEGATIVE_FLOAT_NUMBER) || match(CHARACTER) )
            {
                itr++;
                return true;
            }
            cout << "expected integer or float or charater \t line : " << code[itr].second << endl;
            return false;
        }
        else
        {
            itr = old_position;
            return false;
        }
    }


};

int main()
{
    compiler c = compiler("file.txt");
    return 0;
}
