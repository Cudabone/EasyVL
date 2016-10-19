#ifndef GATE_H
#define GATE_H
#include <string>
#include <list>
#include <vector>
#include <map>
//#ifndef PIN_H
//#define PIN_H
#include "pin.h"
//#endif
//#ifndef PARSER_H
//#define PARSER_H
#include "parser.h"
//#endif
//#ifndef NET_H
//#define NET_H
#include "net.h"
//#endif
//#ifndef MAIN_H
//#define MAIN_H
#include "main.h"
//#endif

class gate
{
	private:
	std::string name_;
	std::string type_;
	std::vector<pin *> pins_;

	void set_name(std::string name);
	void set_type(std::string type);
	bool validate_structural_semantics();

	public:
	bool create(const evl_component &c, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table);
	bool create_pin(const evl_pin &ep, size_t index,const std::map<std::string,net *> &nets_table, const evl_wires_table &wires_table);
};
#endif
