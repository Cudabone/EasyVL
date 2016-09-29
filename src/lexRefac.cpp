#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct evl_token {
	enum token_type {NAME, NUMBER, SINGLE};

	token_type type;
	std::string str;

	int line_no;
}; // struct evl_token

void display_tokens(std::ostream &out, const std::vector<evl_token> &tokens);
bool store_tokens_to_file(std::string file_name,const std::vector<evl_token> &tokens);
bool extract_tokens_from_file(std::string evl_file, std::vector<evl_token> &tokens);
bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens); 
bool is_space(char ch);
bool is_alpha(char ch);
bool is_single(char ch);
bool is_number(char ch);

int main(int argc, char *argv[])
{
	// verify that argv[1] exists
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }
	std::string evl_file = argv[1];

	std::vector<evl_token> tokens;
	if (!extract_tokens_from_file(evl_file, tokens))
		return -1;
	display_tokens(std::cout,tokens);
	if (!store_tokens_to_file(evl_file+".tokens", tokens))
		return -1;
	return 0;
}
void display_tokens(std::ostream &out,const std::vector<evl_token> &tokens) 
{
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		if (tokens[i].type == evl_token::SINGLE) 
			out << "SINGLE " << tokens[i].str << std::endl;
		
		else if (tokens[i].type == evl_token::NAME) 
			out << "NAME " << tokens[i].str << std::endl;
		
		else  // must be NUMBER
			out << "NUMBER " << tokens[i].str << std::endl;
	}
}
bool store_tokens_to_file(std::string file_name,const std::vector<evl_token> &tokens)
{
	std::ofstream output_file(file_name.c_str());
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }
	display_tokens(output_file,tokens);
	return true;
}
bool extract_tokens_from_file(std::string evl_file, std::vector<evl_token> &tokens)
{
	std::ifstream input_file(evl_file);
    if (!input_file)
    {
        std::cerr << "I can't read " << evl_file << std::endl;
        return -1;
    }
	tokens.clear();
	std::string line;
	for(int line_no = 1; std::getline(input_file, line); ++line_no)
	{
		if(!extract_tokens_from_line(line,line_no,tokens))
			return false;
	}
	return true;
}
bool extract_tokens_from_line(std::string line, int line_no, std::vector<evl_token> &tokens)
{
	for (size_t i = 0; i < line.size();)
	{
		// comments
		if (line[i] == '/')
		{
			++i;
			if ((i == line.size()) || (line[i] != '/'))
			{
				std::cerr << "LINE " << line_no
					<< ": a single / is not allowed" << std::endl;
				return false;
			}
			break; // skip the rest of the line by exiting the loop
		}

		// spaces
		if (is_space(line[i]))
		{
			++i; // skip this space character
			continue; // skip the rest of the iteration
		}

		// SINGLE
		if (is_single(line[i]))
		{
			evl_token token;
			token.line_no = line_no;
			token.type = evl_token::SINGLE;
			token.str = std::string(1, line[i]);
			tokens.push_back(token);
			++i; // we consumed this character
			continue; // skip the rest of the iteration
		}

		// NAME
		if((is_alpha(line[i])) || (line[i] == '_'))
		{
			size_t name_begin = i;
			for (++i; i < line.size(); ++i)
			{
				if (!(is_alpha(line[i]) 
							|| is_number(line[i]) 
							|| line[i] == '_'
							|| line[i] == '$'))
				{
					break; // [name_begin, i) is the range for the token
				}
			}
			evl_token token;
			token.line_no = line_no;
			token.type = evl_token::NAME;
			token.str = line.substr(name_begin, i-name_begin);
			tokens.push_back(token);
			continue; //skip the rest of the iteration
		}
		// NUMBER
		if (is_number(line[i]))
		{
			size_t num_begin = i;
			for(++i; i < line.size(); ++i)
			{
				if(!is_number(line[i]))
				{
					break;
				}
			}
			evl_token token;
			token.line_no = line_no;
			token.type = evl_token::NUMBER;
			token.str = line.substr(num_begin, i-num_begin);
			tokens.push_back(token);
		} 
		else
		{
			std::cerr << "LINE " << line_no
				<< ": invalid character" << std::endl;
			return false;
		}
	}
	return true;
}
bool is_space(char ch)
{
	return ((ch == ' ') || (ch == '\t')
		|| (ch == '\r') || (ch == '\n'));
}
bool is_alpha(char ch)
{
	return (((ch >= 'a') && (ch <= 'z'))       // a to z
		|| ((ch >= 'A') && (ch <= 'Z')));    // A to Z
}
bool is_single(char ch)
{
	return ((ch == '(') || (ch == ')')
		|| (ch == '[') || (ch == ']')
		|| (ch == ':') || (ch == ';')
		|| (ch == ','));
}
bool is_number(char ch)
{
	return ((ch >='0') && (ch <='9'));
}
