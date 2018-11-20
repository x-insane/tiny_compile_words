#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
using namespace std;

enum class TOKEN_TYPE {
	NONE, ANNOTATION,
	KEY_TRUE, KEY_FALSE, // true false
	KEY_OR, KEY_AND, KEY_NOT, // or and not
	KEY_INT, KEY_BOOL, KEY_STRING, // int bool string
	KEY_DO, KEY_WHILE, // do while
	KEY_REPEAT, KEY_UNTIL, // repeat until
	KEY_IF, KEY_THEN, KEY_ELSE, KEY_END, // if then else end
	KEY_READ, KEY_WRITE, // read write

	OP_GTR, OP_LSS, // > <
	OP_LEQ, OP_GEQ, OP_EQU, // <= >= =
	OP_COMMA, OP_SEMICOLON, OP_ASSIGN, // , ; :=
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, // + - * /
	OP_LP, OP_RP, // ( )

	ID, NUMBER, STRING
};

string toString(TOKEN_TYPE type) {
	switch (type) {
		case TOKEN_TYPE::KEY_TRUE:
			return "KEY_TRUE";
		case TOKEN_TYPE::KEY_FALSE:
			return "KEY_FALSE";
		case TOKEN_TYPE::KEY_OR:
			return "KEY_OR";
		case TOKEN_TYPE::KEY_AND:
			return "KEY_AND";
		case TOKEN_TYPE::KEY_NOT:
			return "KEY_NOT";
		case TOKEN_TYPE::KEY_INT:
			return "KEY_INT";
		case TOKEN_TYPE::KEY_BOOL:
			return "KEY_BOOL";
		case TOKEN_TYPE::KEY_STRING:
			return "KEY_STRING";
		case TOKEN_TYPE::KEY_DO:
			return "KEY_DO";
		case TOKEN_TYPE::KEY_WHILE:
			return "KEY_WHILE";
		case TOKEN_TYPE::KEY_REPEAT:
			return "KEY_REPEAT";
		case TOKEN_TYPE::KEY_UNTIL:
			return "KEY_UNTIL";
		case TOKEN_TYPE::KEY_IF:
			return "KEY_IF";
		case TOKEN_TYPE::KEY_THEN:
			return "KEY_THEN";
		case TOKEN_TYPE::KEY_ELSE:
			return "KEY_ELSE";
		case TOKEN_TYPE::KEY_END:
			return "KEY_END";
		case TOKEN_TYPE::KEY_READ:
			return "KEY_READ";
		case TOKEN_TYPE::KEY_WRITE:
			return "KEY_WRITE";

		case TOKEN_TYPE::OP_GTR:
			return "OP_GTR";
		case TOKEN_TYPE::OP_LSS:
			return "OP_LSS";
		case TOKEN_TYPE::OP_LEQ:
			return "OP_LEQ";
		case TOKEN_TYPE::OP_GEQ:
			return "OP_GEQ";
		case TOKEN_TYPE::OP_EQU:
			return "OP_EQU";
		case TOKEN_TYPE::OP_COMMA:
			return "OP_COMMA";
		case TOKEN_TYPE::OP_SEMICOLON:
			return "OP_SEMICOLON";
		case TOKEN_TYPE::OP_ASSIGN:
			return "OP_ASSIGN";
		case TOKEN_TYPE::OP_ADD:
			return "OP_ADD";
		case TOKEN_TYPE::OP_SUB:
			return "OP_SUB";
		case TOKEN_TYPE::OP_MUL:
			return "OP_MUL";
		case TOKEN_TYPE::OP_DIV:
			return "OP_DIV";
		case TOKEN_TYPE::OP_LP:
			return "OP_LP";
		case TOKEN_TYPE::OP_RP:
			return "OP_RP";

		case TOKEN_TYPE::ID:
			return "ID";
		case TOKEN_TYPE::NUMBER:
			return "NUMBER";
		case TOKEN_TYPE::STRING:
			return "STRING";
	}
	return "NONE";
}

TOKEN_TYPE getType(string token) {
	if (token == "true")
		return TOKEN_TYPE::KEY_TRUE;
	else if (token == "false")
		return TOKEN_TYPE::KEY_FALSE;
	else if (token == "or")
		return TOKEN_TYPE::KEY_OR;
	else if (token == "and")
		return TOKEN_TYPE::KEY_AND;
	else if (token == "not")
		return TOKEN_TYPE::KEY_NOT;
	else if (token == "int")
		return TOKEN_TYPE::KEY_INT;
	else if (token == "bool")
		return TOKEN_TYPE::KEY_BOOL;
	else if (token == "string")
		return TOKEN_TYPE::KEY_STRING;
	else if (token == "while")
		return TOKEN_TYPE::KEY_WHILE;
	else if (token == "do")
		return TOKEN_TYPE::KEY_DO;
	else if (token == "if")
		return TOKEN_TYPE::KEY_IF;
	else if (token == "then")
		return TOKEN_TYPE::KEY_THEN;
	else if (token == "else")
		return TOKEN_TYPE::KEY_ELSE;
	else if (token == "end")
		return TOKEN_TYPE::KEY_END;
	else if (token == "repeat")
		return TOKEN_TYPE::KEY_REPEAT;
	else if (token == "until")
		return TOKEN_TYPE::KEY_UNTIL;
	else if (token == "read")
		return TOKEN_TYPE::KEY_READ;
	else if (token == "write")
		return TOKEN_TYPE::KEY_WRITE;

	else if (token == ">")
		return TOKEN_TYPE::OP_GTR;
	else if (token == "<")
		return TOKEN_TYPE::OP_LSS;
	else if (token == "<=")
		return TOKEN_TYPE::OP_LEQ;
	else if (token == ">=")
		return TOKEN_TYPE::OP_GEQ;
	else if (token == "=")
		return TOKEN_TYPE::OP_EQU;
	else if (token == ",")
		return TOKEN_TYPE::OP_COMMA;
	else if (token == ";")
		return TOKEN_TYPE::OP_SEMICOLON;
	else if (token == ":=")
		return TOKEN_TYPE::OP_ASSIGN;
	else if (token == "+")
		return TOKEN_TYPE::OP_ADD;
	else if (token == "-")
		return TOKEN_TYPE::OP_SUB;
	else if (token == "*")
		return TOKEN_TYPE::OP_MUL;
	else if (token == "/")
		return TOKEN_TYPE::OP_DIV;
	else if (token == "(")
		return TOKEN_TYPE::OP_LP;
	else if (token == ")")
		return TOKEN_TYPE::OP_RP;

	return TOKEN_TYPE::NONE;
}

bool isLetter(char ch) {
	if (ch >= 'A' && ch <= 'Z')
		return true;
	if (ch >= 'a' && ch <= 'z')
		return true;
	return false;
}

bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}

bool isValidSign(char ch) {
	static const string signs = "><=,;:+-*/()";
	return string::npos != signs.find(ch);
}

bool isSeparator(char ch) {
	static const string signs = " \t\r\n";
	return string::npos != signs.find(ch);
}

bool compile_error = false;

void error(string err, int line, int offset) {
	compile_error = true;
	cerr << "ERROR IN LINE " << line << ":" << offset << " " << err << endl;
}

struct Token {
	TOKEN_TYPE type;
	string token;
};

class WordsAnalysis {
	ifstream& in;
	vector<Token> list;
private:
	// temporary variables
	string word;
	TOKEN_TYPE type;
	int line_number;
	int char_number;
	bool isSign = false;
public:
	WordsAnalysis(ifstream& fin) : in(fin) {
		analyse();
	}
	vector<Token> getTokens() {
		return list;
	}
private:
	void new_line() {
		line_number ++;
		char_number = 0;
		type = TOKEN_TYPE::NONE;
		isSign = false;
	}
	void close_word() {
		Token token;
		token.token = word;
		token.type = type;
		TOKEN_TYPE t = getType(word);
		if (token.type == TOKEN_TYPE::NONE)
			token.type = t;
		else if (token.type == TOKEN_TYPE::ID && t != TOKEN_TYPE::NONE)
			token.type = t;
		if (token.type != TOKEN_TYPE::ANNOTATION)
			list.push_back(token);
		word.clear();
		type = TOKEN_TYPE::NONE;
		isSign = false;
	}
	void analyse() {
		char ch;
		line_number = 1;
		char_number = 0;
		isSign = false;
		while (true)
		{
			in.get(ch);
			char_number ++;
			if (in.fail() || in.eof()) {
				if (type == TOKEN_TYPE::STRING)
					error("unclosed string", line_number, char_number);
				if (type == TOKEN_TYPE::ANNOTATION)
					error("unclosed annotation", line_number, char_number);
				if (!word.empty())
					close_word();
				break;
			}

			if (isSign) {
	            if (getType(word) != TOKEN_TYPE::NONE &&
	                    getType(word + ch) == TOKEN_TYPE::NONE)
	                close_word();
	            else if (!isValidSign(ch))
	                close_word();
	        }

			if (!word.empty() && type != TOKEN_TYPE::ANNOTATION && type != TOKEN_TYPE::STRING
				&& !isSign && isValidSign(ch))
				close_word();

			if (word.empty()) {
				if (ch == '{')
					type = TOKEN_TYPE::ANNOTATION;
				else if (ch == '\'')
					type = TOKEN_TYPE::STRING;
				else if (isDigit(ch))
					type = TOKEN_TYPE::NUMBER;
				else if (isLetter(ch))
					type = TOKEN_TYPE::ID;
				else if (isValidSign(ch))
					isSign = true;
				else if (!isSeparator(ch))
					error(string("unknown symbol: ") + ch, line_number, char_number);
			} else {
				if (type == TOKEN_TYPE::STRING && ch == '\'') {
					word += ch;
					close_word();
					continue;
				}
				if (type == TOKEN_TYPE::ANNOTATION && ch == '}') {
					word += ch;
					close_word();
					continue;
				}
			}

			if (ch == '\n') {
				if (type == TOKEN_TYPE::STRING) {
					error("unclosed string", line_number, char_number);
					close_word();
					new_line();
					continue;
				}
				if (type == TOKEN_TYPE::ANNOTATION) {
					error("unclosed annotation", line_number, char_number);
					close_word();
					new_line();
					continue;
				}
			}

			if (type != TOKEN_TYPE::ANNOTATION && type != TOKEN_TYPE::STRING) {
				if (isSeparator(ch)) {
					if (!word.empty())
						close_word();
					if (ch == '\n')
						new_line();
					continue;
				}
			}

			if (type == TOKEN_TYPE::NUMBER && !isDigit(ch))
				error(string("here must be a digit: ") + ch, line_number, char_number);
			if (type == TOKEN_TYPE::ID && !isLetter(ch) && !isDigit(ch))
				error(string("here must be a letter: ") + ch, line_number, char_number);

			word += ch;
			if (ch == '\n')
				new_line();
		}
		if (!word.empty())
			close_word();
	}
};

int main(int argc, char *argv[])
{
	string filename;
	if (argc > 1)
		filename = argv[1];
	if (filename.empty()) {
		cout << "please input a file name to open: ";
		getline(cin, filename);
	}
	ifstream in(filename);
	if (!in) {
		cout << "can not open file!";
		return -1;
	}
	WordsAnalysis worker(in);
	if (!compile_error) {
		cout << "All words list:\n\n";
		vector<Token> list = worker.getTokens();
		for (int i = 0; i < list.size(); ++i)
			cout << "(" << toString(list[i].type) << ", " << list[i].token << ")" << endl;
	} else
		cout << "\ncomplie fail.";
    return 0;
}