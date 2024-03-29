#include <fstream>
#include "parser.h"

bool parse_evl_file(std::string evl_file, evl_modules &modules)
{
	evl_tokens tokens;
	evl_wires wires;
	if (!extract_tokens_from_file(evl_file, tokens))
		return false;
	//display_tokens(std::cout,tokens);
	if (!store_tokens_to_file(evl_file+".tokens", tokens))
		return false;
	if(!group_tokens_into_statements(modules,tokens))
		return false;
	if(!store_statements_to_file(evl_file+".syntax",modules))
		return false;
	//display_statements(std::cout,modules);

	return true;
}
bool group_tokens_into_statements(evl_modules &modules,evl_tokens &tokens)
{
	evl_module module;
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
			module.type = evl_module::TOP;
			evl_statement modulestatement;
			modulestatement.type = evl_statement::MODULE;

			if(!move_tokens_into_statement(modulestatement.tokens,tokens))
				return false;
			if(!process_module_statement(module,modulestatement))
				return false;
			
			module.statements.push_back(modulestatement);
		}
		else if(token.str == "endmodule") //ENDMODULE statement
		{
			evl_statement endmodulestatement;
			endmodulestatement.type = evl_statement::ENDMODULE;

			module.statements.push_back(endmodulestatement);
			tokens.pop_front();
		}
		else if(token.str == "wire") // WIRE statement
		{
			evl_statement wirestatement;
			wirestatement.type = evl_statement::WIRE;
			if(!move_tokens_into_statement(wirestatement.tokens,tokens))		
				return false;
			if(!process_wire_statement(module.wires,wirestatement))
				return false;
			module.statements.push_back(wirestatement);
		}
		else // COMPONENT statement 
		{
			evl_statement componentstatement;
			componentstatement.type = evl_statement::COMPONENT;
			if(!move_tokens_into_statement(componentstatement.tokens,tokens))
				return false;
			if(!process_component_statement(module.components,componentstatement))
				return false;
			module.statements.push_back(componentstatement);
		}
	}
	modules.push_back(module);
	return true;
}
void pparse_error(std::string expected,std::string found,int line_no)
{
	std::cerr << "Need '" << expected << "' but found '" << found << "' on line " << line_no << std::endl; 
}
bool process_module_statement(evl_module &module, const evl_statement &statement)
{
	evl_tokens tokens = statement.tokens;
	for(evl_tokens::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
	{
		size_t size = tokens.size();
		if(size >= 2)
		{
			evl_tokens::iterator eit = tokens.begin();
			eit++;
			module.name = eit->str;
		}
		else if(size == 0)
			return false;
	}
	return true;
}
bool process_component_statement(evl_components &components,evl_statement &s)
{
	enum state_type {INIT,TYPE,NAME,PINS,PIN_NAME,PINS_DONE,BUS,BUS_MSB,BUS_COLON,BUS_LSB,BUS_DONE,DONE};
	state_type state = INIT;
	evl_pin pin;
	evl_component component;
	component.numpins = 0;
	for(; !s.tokens.empty() && (state != DONE); s.tokens.pop_front())
	{
		evl_token t = s.tokens.front();
		if(state == INIT)
		{
			if(t.type == evl_token::NAME)
			{
				component.type = t.str;
				component.name = "";
				state = TYPE;
			}
			else
			{
				pparse_error("NAME",t.str,t.line_no);
				return false;
			}
		}
		else if(state == TYPE)
		{
			if(t.type == evl_token::NAME)
			{
				component.name = t.str;
				component.numpins = 0;
				state = NAME;
			}
			else if(t.str == "(")
			{
				state = PINS;
			}
			else
			{
				pparse_error("NAME or (",t.str,t.line_no);
				return false;
			}
		}
		else if(state == NAME)
		{
			if(t.str == "(")
			{
				state = PINS;
			}
			else
			{
				pparse_error("(",t.str,t.line_no);
				return false;
			}
		}
		else if(state == PINS)
		{
			if(t.type == evl_token::NAME)
			{
				pin.name = t.str;
				pin.bus_msb = -1;
				pin.bus_lsb = -1;
				component.numpins++;
				state = PIN_NAME;
			}
			else
			{
				pparse_error("NAME",t.str,t.line_no);
				return false;
			}
		}
		else if(state == PIN_NAME)
		{
			if(t.str == "[")
				state = BUS;
			else if(t.str == ",")
			{
				component.pins.push_back(pin);	
				state = PINS;
			}
			else if(t.str == ")")
			{
				component.pins.push_back(pin);
				state = PINS_DONE;
			}
			else
			{
				pparse_error("[ or , or )",t.str,t.line_no);
				return false;
			}
		}
		else if(state == PINS_DONE)
		{
			if(t.str == ";")
			{
				state = DONE;
			}
			else
			{
				pparse_error(";",t.str,t.line_no);
				return false;
			}
		}
		else if(state == BUS)
		{
			if(t.type == evl_token::NUMBER)
			{
				pin.bus_msb = atoi(t.str.c_str());
				state = BUS_MSB;
			}
			else
			{
				pparse_error("NUMBER",t.str,t.line_no);
				return false;
			}
		}
		else if(state == BUS_MSB)
		{
			if(t.str == ":")
				state = BUS_COLON;
			else if(t.str == "]")
				state = BUS_DONE;
			else
			{
				pparse_error(": or ]",t.str,t.line_no);
				return false;
			}
		}
		else if(state == BUS_COLON)
		{
			if(t.type == evl_token::NUMBER)
			{
				pin.bus_lsb = atoi(t.str.c_str());
				state = BUS_LSB;
			}
			else
			{
				pparse_error("NUMBER",t.str,t.line_no);
				return false;
			}
		}
		else if(state == BUS_LSB)
		{
			if(t.str == "]")
				state = BUS_DONE;
			else
			{
				pparse_error("]",t.str,t.line_no);
				return false;
			}
		}
		else if(state == BUS_DONE)
		{
			if(t.str == ",")
			{ 
				component.pins.push_back(pin);
				state = PINS; 
			}
			else if(t.str == ")")
			{
				component.pins.push_back(pin);
				state = PINS_DONE;	
			}
			else
			{
				pparse_error(", or )",t.str,t.line_no);
				return false;
			}
		}
	}
	if (!s.tokens.empty() || (state != DONE))
	{
		std::cerr << "Something went wrong with the component statement" << std::endl;
		return false;
	}
	components.push_back(component);
	return true;
}
bool process_wire_statement(evl_wires &wires, evl_statement &s)
{
	enum state_type {INIT,WIRE,DONE,WIRES,WIRE_NAME,BUS,BUS_MSB,BUS_COLON,BUS_LSB,BUS_DONE};

	state_type state = INIT;
	int bus_width;
	for(; !s.tokens.empty() && (state != DONE); s.tokens.pop_front())
	{
		evl_token t = s.tokens.front();
		if(state == INIT)
		{
			if(t.str == "wire")
				state = WIRE;
			else
			{
				std::cerr << "Need 'wire' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if(state == WIRE)
		{
			if(t.type == evl_token::NAME) 
			{
				bus_width = 1;
				evl_wire wire;
				wire.name = t.str;
				wire.width = bus_width;
				wires.push_back(wire);
				state = WIRE_NAME;
			}
			else if(t.type == evl_token::SINGLE)
			{
				if(t.str == "[")	
					state = BUS;
				else
				{
					std::cerr << "Need [ but found '" << t.str << "' on line " << t.line_no << std::endl;
					return false;
				}
			}
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}

		}
		else if(state == WIRES)
		{
			if(t.type == evl_token::NAME)
			{
				evl_wire wire;
				wire.name = t.str;
				wire.width = bus_width;
				wires.push_back(wire);
				state = WIRE_NAME;
			}
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}

		}
		else if(state == WIRE_NAME)
		{
			if(t.str == ",")
				state = WIRES;
			else if(t.str == ";")
				state = DONE;
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}

		}
		else if(state == BUS)
		{
			if(t.type == evl_token::NUMBER)
			{
				bus_width = atoi(t.str.c_str())+1;
				state = BUS_MSB;
			}
			else
			{
				std::cerr << "Need NUMBER but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if(state == BUS_MSB)
		{
			if(t.str == ":")
				state = BUS_COLON;
			else
			{
				std::cerr << "Need : but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if(state == BUS_COLON)
		{
			if(t.str == "0")
				state = BUS_LSB;
			else
			{
				std::cerr << "Need 0 but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if(state == BUS_LSB)
		{
			if(t.str == "]")
				state = BUS_DONE;
			else
			{
				std::cerr << "Need ] but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if(state == BUS_DONE)
		{
			if(t.type == evl_token::NAME)
			{
				evl_wire wire;
				wire.name = t.str;
				wire.width = bus_width;
				wires.push_back(wire);
				state = WIRE_NAME;
			}
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
	}
	if (!s.tokens.empty() || (state != DONE))
	{
		std::cerr << "Something went wrong with the wire statement" << std::endl;
		return false;
	}
	return true;
}
bool move_tokens_into_statement(evl_tokens &statement_tokens,evl_tokens &tokens)
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
void display_statements(std::ostream &out,const evl_modules &modules)
{
	evl_module module = modules.front();
	int numcomps = module.components.size();
	int numwires = module.wires.size();
	
	if(module.name != "")
		out << "module " << module.name << std::endl;
	else
		out << "module" << std::endl;


	/* wires */
	out << "wires " << numwires << std::endl;
	for(evl_wires::const_iterator witer = module.wires.begin(); witer != module.wires.end(); ++witer)
	{
		out << "\twire " << witer->name << " " << witer->width << std::endl;
	}
		
	/*components*/
	out << "components " << numcomps << std::endl;
	for (evl_components::const_iterator iter = module.components.begin(); iter != module.components.end(); ++iter)
	{
			evl_component component = *iter;
			if(component.name != "")
				out << "\tcomponent " << component.type + " " << component.name + " "<< component.numpins << std::endl;
			else
				out << "\tcomponent " << component.type + " "<< component.numpins << std::endl;
			evl_pins pins = component.pins;
			evl_pin pin; 
			for(evl_pins::const_iterator piter = pins.begin(); piter != pins.end(); ++piter)
			{
				pin = *piter;
				if(pin.bus_msb == -1 && pin.bus_lsb == -1)
					out << "\t\tpin " << piter->name << std::endl;
				else if(pin.bus_msb != -1 && pin.bus_lsb == -1)
					out << "\t\tpin " << piter->name << " "<< piter->bus_msb <<std::endl;
				else if(pin.bus_msb != -1 && pin.bus_lsb != -1)
					out << "\t\tpin " << piter->name << " " << piter->bus_msb << " " << piter->bus_lsb <<std::endl;
		}
	}

}
bool store_statements_to_file(std::string file_name,const evl_modules &modules)
{
	std::ofstream output_file(file_name.c_str());
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }
	display_statements(output_file,modules);
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
