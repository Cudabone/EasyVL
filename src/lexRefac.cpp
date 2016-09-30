#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

struct evl_token {
	enum token_type {NAME, NUMBER, SINGLE};

	token_type type;
	std::string str;

	int line_no;
}; // struct evl_token
typedef std::list<evl_token> evl_tokens;

struct evl_statement {
	enum statement_type {MODULE,WIRE,COMPONENT,ENDMODULE};

	statement_type type;
	evl_tokens tokens;
}; // struct evl_statement

typedef std::list<evl_statement> evl_statements;

struct evl_wire {
	std::string name;
	int width;
}; //struct evl_wire
typedef std::vector<evl_wire> evl_wires;

bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);
void display_statements(std::ostream &out,const evl_statements &statements);
bool group_tokens_into_statements(evl_statements &statements,evl_tokens &tokens);
void display_tokens(std::ostream &out, const evl_tokens &tokens);
bool store_tokens_to_file(std::string file_name,const evl_tokens &tokens);
bool extract_tokens_from_file(std::string evl_file, evl_tokens &tokens);
bool extract_tokens_from_line(std::string line, int line_no, evl_tokens &tokens); 
bool is_space(char ch);
bool is_alpha(char ch);
bool is_single(char ch);
bool is_number(char ch);
bool token_is_semicolon(const evl_token &token);
bool store_statements_to_file(std::string file_name, const evl_statements &statements);

int main(int argc, char *argv[])
{
	// verify that argv[1] exists
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }
	std::string evl_file = argv[1];

	evl_tokens tokens;
	if (!extract_tokens_from_file(evl_file, tokens))
		return -1;
	//display_tokens(std::cout,tokens);
	if (!store_tokens_to_file(evl_file+".tokens", tokens))
		return -1;
	evl_statements statements;
	if(!group_tokens_into_statements(statements,tokens))
		return -1;
	if(!store_statements_to_file(evl_file+".syntax",statements))
		return -1;
	display_statements(std::cout,statements);

	return 0;
}
bool group_tokens_into_statements(evl_statements &statements,evl_tokens &tokens)
{
	while(!tokens.empty())
	{
		evl_token token = tokens.front();
		if(token.type != evl_token::NAME)
		{
			std::cerr << "Need a NAME token but found '" << token.str << "' on line "
				<< token.line_no << std::endl;
			return false;
		}
		if(token.str == "module") //MODULE statement
		{
			evl_statement module;
			module.type = evl_statement::MODULE;

			if(!move_tokens_to_statement(module.tokens,tokens))
				return false;
			
			statements.push_back(module);

		}
		else if(token.str == "endmodule") //ENDMODULE statement
		{
			evl_statement endmodule;
			endmodule.type = evl_statement::ENDMODULE;

			/*
			if(!move_tokens_to_statement(endmodule.tokens,tokens))
				return false;
				*/

			statements.push_back(endmodule);
			tokens.pop_front();
		}
		/*
		else if(token.str == "wire") // WIRE statement
		{
				
		}
		*/
		else // COMPONENT statement 
		{
			evl_statement ignore;
			if(!move_tokens_to_statement(ignore.tokens,tokens))
				return false;
		}
	}
	return true;
}
bool move_tokens_to_statement(evl_tokens &statement_tokens,evl_tokens &tokens)
{
	evl_tokens::iterator next_sc = std::find_if(tokens.begin(), tokens.end(), token_is_semicolon);
	if(next_sc == tokens.end())
	{
		std::cerr << "Look for ';' but reached the end of file" << std::endl;
		return false;
	}

	// move tokens within [tokens.begin(), next_sc] to statement_tokens
	evl_tokens::iterator after_sc = next_sc; 
	++after_sc;
	statement_tokens.splice(statement_tokens.begin(),tokens,tokens.begin(),after_sc);

	for(evl_tokens::iterator it = tokens.begin(); it != after_sc; ++it)
		tokens.pop_front();

	return true;
}
bool token_is_semicolon(const evl_token &token)
{
	return token.str == ";";
}
void display_tokens(std::ostream &out,const evl_tokens &tokens) 
{
	for (evl_tokens::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
		{
		if (iter->type == evl_token::SINGLE) 
			out << "SINGLE " << iter->str << std::endl;
		
		else if (iter->type == evl_token::NAME) 
			out << "NAME " << iter->str << std::endl;
		
		else  // must be NUMBER
			out << "NUMBER " << iter->str << std::endl;
	}
}
void display_statements(std::ostream &out,const evl_statements &statements)
{
	for (evl_statements::const_iterator iter = statements.begin(); iter != statements.end(); ++iter)
	{
		if (iter->type == evl_statement::MODULE) 
			out << "module top" << std::endl;
		
		else if (iter->type == evl_statement::ENDMODULE);

		else if (iter->type == evl_statement::WIRE);	

		else; // must be COMPONENT
	}

}
bool store_statements_to_file(std::string file_name, const evl_statements &statements)
{
	std::ofstream output_file(file_name.c_str());
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }
	display_statements(output_file,statements);
	return true;
}
bool store_tokens_to_file(std::string file_name,const evl_tokens &tokens)
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
bool extract_tokens_from_file(std::string evl_file, evl_tokens &tokens)
{
	std::ifstream input_file(evl_file.c_str());
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
bool extract_tokens_from_line(std::string line, int line_no, evl_tokens &tokens)
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
