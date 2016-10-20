#ifndef EVL_TYPES_H
#define EVL_TYPES_H
#include <string>
#include <list>
#include <vector>
#include <map>

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

typedef std::map<std::string, int> evl_wires_table;
#endif
