#include <iostream>
#include <vector>

#include "Lexer.h"

// in this process, lexer will "crack" the languages into different tokens
// then, will send it to Parser or AST

int main(int argc, char** argv[])
{
  // as language example
	Lexer lexer(
		"function print(add x) {} \n"
		"add x = ~32; ~~ I love you so much \n" 
		"x = (x & 1) != 0; \n"
		"print(x);\n"
		"\n"
		"for (add i = 0; i < 10; x++) \n"
		"{	print(i);	}");
  
	std::vector<Token> tokens = lexer.identify();
	std::vector<TokenType> keywords;
  
	for (auto it = tokens.begin(); it != tokens.end(); it++)
	{
		if (it->type == TokenType::UNKNOWN)
		{
			std::cerr << "Error: " << it->lexeme << std::endl; // this will be happened if an unknown token appeared in the language
			break;
		}

		int type = static_cast<int>(it->type);
		if (type >= static_cast<int>(TokenType::KW_ADD) && type <= static_cast<int>(TokenType::KW_NOT))
			keywords.push_back(it->type);
		
		std::cout << "Token: " << type << ", Lexeme: " << it->lexeme << std::endl;
	}

	size_t keywordsCount = keywords.size(); // this will contain FUNCTION, ADD, ADD, FOR, ADD. 5 keywords.
	std::cout << "Keywords Count: " << keywordsCount << std::endl; 

	return EXIT_SUCCESS;
}
