#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

struct evl_token {
	enum token_type {NAME, NUMBER, SINGLE};

	token_type type;
	std::string str;

	int line_no;
}; // struct evl_token
typedef std::list<evl_token> evl_tokens;

struct evl_wire {
	std::string name;
	int width;
}; //struct evl_wire
typedef std::vector<evl_wire> evl_wires;

struct evl_pin {
	std::string name;
	int bus_msb;
	int bus_lsb;
}; //struct evl_pin
typedef std::vector<evl_pin> evl_pins;

struct evl_component {
	std::string type;
	std::string name;
	int numpins;
	evl_pins pins;
}; //struct evl_component
typedef std::list<evl_component> evl_components;

struct evl_statement {
	enum statement_type {MODULE,WIRE,COMPONENT,ENDMODULE};

	statement_type type;
	evl_tokens tokens;
}; // struct evl_statement
typedef std::list<evl_statement> evl_statements;

struct evl_module
{
	enum module_type {TOP,OTHER};

	module_type type;
	std::string name;
	evl_statements statements;
	evl_components components;
	evl_wires wires;
};
typedef std::list<evl_module> evl_modules;

/* Function Prototypes */
bool parse_evl_file(std::string evl_file, evl_modules &modules);
bool process_module_statement(evl_module &module,const evl_statement &statement);
void pparse_error(std::string expected,std::string found,int line_no);
bool process_component_statement(evl_components &components,evl_statement &s);
bool process_wire_statement(evl_wires &wires, evl_statement &s);
bool move_tokens_into_statement(evl_tokens &statement_tokens, evl_tokens &tokens);
void display_statements(std::ostream &out,const evl_modules &modules);
bool group_tokens_into_statements(evl_modules &modules,evl_tokens &tokens);
void display_tokens(std::ostream &out, const evl_tokens &tokens);
bool store_tokens_to_file(std::string file_name,const evl_tokens &tokens);
bool extract_tokens_from_file(std::string evl_file, evl_tokens &tokens);
bool extract_tokens_from_line(std::string line, int line_no, evl_tokens &tokens); 
bool is_space(char ch);
bool is_alpha(char ch);
bool is_single(char ch);
bool is_number(char ch);
bool token_is_semicolon(const evl_token &token);
bool store_statements_to_file(std::string file_name,const evl_modules &modules);
#endif
