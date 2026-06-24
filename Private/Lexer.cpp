#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>

enum class TokenType
{
	// Blocks
	LEFT_PAREN, RIGHT_PAREN,
	LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, SEMICOLON,
	
	// Operators
	MINUS, PLUS, SLASH, STAR, MODULO,
	QUESTION, TERNARY, NULL_COALESING,
	LEFT_SHIFT, RIGHT_SHIFT, XOR, COMMENT, BITWISE_NOT, BITWISE_OR, BITWISE_AND,
	INCREMENT, DECREMENT, INCREMENT_ONE, DECREMENT_ONE,

	// Assignment
	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL,
	AND, OR,

	// Literals
	IDENTIFIER, STRING, NUMBER, CHAR,

	// Specials
	END_OF_FILE, UNKNOWN,

	// Keywords
	KW_ADD, KW_IF, KW_ELSE_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_FUNCTION, KW_RETURN,
	KW_TRUE, KW_FALSE, KW_NULL, KW_BREAK, KW_CONTINUE, KW_CONST, KW_STRUCT, KW_THIS, KW_AND, KW_OR, KW_NOT
};

struct Token
{
	TokenType type;
	std::string_view lexeme;
};

class Lexer
{

private:
	size_t current = 0;
	std::string_view source;

	void Advance()
	{
		current++;
	}

	bool IsAtEnd()
	{
		return current >= source.size();
	}

	char Peek()
	{
		return IsAtEnd() ? '\0' : source.at(current);
	}

	char NextPeek()
	{
		return (current + 1) >= source.size() ? '\0' : source.at(current + 1);
	}

	TokenType IdentifyKeyword(std::string_view lexeme)
	{
		switch (lexeme.length())
		{
		case 2:
			if (lexeme == "if") return TokenType::KW_IF;
			if (lexeme == "or") return TokenType::KW_OR;
			break;
		case 3:
			if (lexeme == "add") return TokenType::KW_ADD;
			if (lexeme == "for") return TokenType::KW_FOR;
			if (lexeme == "not") return TokenType::KW_NOT;
			if (lexeme == "and") return TokenType::KW_AND;
			break;
		case 4:
			if (lexeme == "else") return TokenType::KW_ELSE;
			if (lexeme == "true") return TokenType::KW_TRUE;
			if (lexeme == "null") return TokenType::KW_NULL;
			if (lexeme == "this") return TokenType::KW_THIS;
			break;
		case 5:
			if (lexeme == "while") return TokenType::KW_WHILE;
			if (lexeme == "const") return TokenType::KW_CONST;
			if (lexeme == "break") return TokenType::KW_BREAK;
			if (lexeme == "false") return TokenType::KW_FALSE;
			break;
		case 6:
			if (lexeme == "return") return TokenType::KW_RETURN;
			if (lexeme == "struct") return TokenType::KW_STRUCT;
			break;
		case 7:
			if (lexeme == "continue") return TokenType::KW_CONTINUE;
			break;
		case 8:
			if (lexeme == "function") return TokenType::KW_FUNCTION;
			break;
		}

		return TokenType::IDENTIFIER;
	}

public:
	explicit Lexer(std::string_view source) : source(source), current(0) {}

	std::vector<Token> identify()
	{
		std::vector<Token> tokens;

		while (!IsAtEnd() && Peek() != '\0')
		{
			char x = Peek();

			if (std::isspace(x))
			{
				Advance();

				continue;
			}

			if (std::isalpha(x) || x == '_')
			{
				size_t start = current;
				while (std::isalnum(Peek()) || Peek() == '_')
					Advance();

				std::string_view lexeme = source.substr(start, current - start);
				TokenType type = IdentifyKeyword(lexeme);
				tokens.push_back({ type, lexeme });

				continue;
			}

			if (std::isdigit(x))
			{
				size_t start = current;
				while (std::isdigit(Peek()))
					Advance();
				tokens.push_back({ TokenType::NUMBER, source.substr(start, current - start) });

				continue;
			}

			if (x == '"')
			{
				size_t start = current + 1;
				
				while (Peek() != '"' && !IsAtEnd())
					Advance();

				if (!IsAtEnd())
					Advance();

				tokens.push_back({ TokenType::STRING, source.substr(start, current - start - 1) });

				continue;
			}

			if (x == '\'')
			{
				size_t start = current + 1;
				
				while (Peek() != '\'' && !IsAtEnd())
					Advance();

				if (!IsAtEnd())
					Advance();

				tokens.push_back({ TokenType::CHAR, source.substr(start, current - start - 1) });
				continue;
			}

			switch (x)
			{
			case '(': tokens.push_back({ TokenType::LEFT_PAREN, "(" }); break;
			case ')': tokens.push_back({ TokenType::RIGHT_PAREN, ")" }); break;
			case '{': tokens.push_back({ TokenType::LEFT_BRACE, "{" }); break;
			case '}': tokens.push_back({ TokenType::RIGHT_BRACE, "}" }); break;
			case ',': tokens.push_back({ TokenType::COMMA, "," }); break;
			case '.': tokens.push_back({ TokenType::DOT, "." }); break;
			case '/': tokens.push_back({ TokenType::SLASH, "/" }); break;
			case '*': tokens.push_back({ TokenType::STAR, "*" }); break;
			case '%': tokens.push_back({ TokenType::MODULO, "%" }); break;
			case ';': tokens.push_back({ TokenType::SEMICOLON, ";" }); break;
			case '^': tokens.push_back({ TokenType::XOR, "^" }); break;
			case '-':
			{
				if (NextPeek() == '-')
					tokens.push_back({ TokenType::DECREMENT, "--" });
				else if (NextPeek() == '=')
					tokens.push_back({ TokenType::DECREMENT_ONE, "-=" });
				else
					tokens.push_back({ TokenType::MINUS, "-" });

				Advance();
				break;
			}
			case '+':
			{
				if (NextPeek() == '+')
					tokens.push_back({ TokenType::INCREMENT, "++" });
				else if (NextPeek() == '=')
					tokens.push_back({ TokenType::INCREMENT_ONE, "+=" });
				else
					tokens.push_back({ TokenType::PLUS, "+" });

				Advance();
				break;
			}
			case '!':
			{
				if (NextPeek() == '=')
					tokens.push_back({ TokenType::BANG_EQUAL, "!=" });
				else
					tokens.push_back({ TokenType::BANG, "!" });

				Advance();
				break;
			}
			case '=':
			{
				if (NextPeek() == '=')
					tokens.push_back({ TokenType::EQUAL_EQUAL, "==" });
				else
					tokens.push_back({ TokenType::EQUAL, "=" });

				Advance();
				break;
			}
			case '>':
			{
				if (NextPeek() == '=')
					tokens.push_back({ TokenType::GREATER_EQUAL, ">=" });
				else if (NextPeek() == '>')
					tokens.push_back({ TokenType::RIGHT_SHIFT, ">>" });
				else
					tokens.push_back({ TokenType::GREATER, ">" });

				Advance();
				break;
			}
			case '<':
			{
				if (NextPeek() == '=')
					tokens.push_back({ TokenType::LESS_EQUAL, "<=" });
				else if (NextPeek() == '<')
					tokens.push_back({ TokenType::LEFT_SHIFT, "<<" });
				else
					tokens.push_back({ TokenType::LESS, "<" });

				Advance();
				break;
			}
			case '&':
			{
				if (NextPeek() == '&')
					tokens.push_back({ TokenType::AND, "&&" });
				else
					tokens.push_back({ TokenType::BITWISE_AND, "&" });

				Advance();
				break;
			}
			case '~':
			{
				if (NextPeek() == '~')
				{
					size_t now = current + 2;
					while (Peek() != '\n' && !IsAtEnd())
						Advance();
					tokens.push_back({ TokenType::COMMENT, source.substr(now, current - now) });
				}
				else
				{
					tokens.push_back({ TokenType::BITWISE_NOT, "~" });
				}

				break;
			}
			case '|':
			{
				if (NextPeek() == '|')
					tokens.push_back({ TokenType::OR, "||" });
				else
					tokens.push_back({ TokenType::BITWISE_OR, "|" });

				Advance();
				break;
			}
			default:
			{
				std::cerr << "Unknown token: " << x << " at " << current << ", resulting error." << std::endl;

				tokens.clear();
				tokens.push_back({ TokenType::UNKNOWN, "unknown token." });

				return tokens;
				break;
			}

			}

			Advance();
		}

		tokens.push_back({ TokenType::END_OF_FILE, "" });
		return tokens;
	}
};
