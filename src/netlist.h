#ifndef NETLIST_H
#define NETLIST_H
#include <list>
#include <sstream>
#include <assert.h>
//#ifndef NET_H
//#define NET_H
#include "net.h"
//#endif
//#ifndef GATE_H
//#define GATE_H
#include "gate.h"
//#endif
//#ifndef PARSER_H
//#define PARSER_H
#include "parser.h"
//#endif
//#ifndef MAIN_H
//#define MAIN_H
#include "main.h"
//#endif


typedef std::map<std::string,net *> nets_table;

class netlist 
{
	private:
	std::list<gate *> gates_;
	std::list<net *> nets_;
	nets_table nets_table_;
	evl_wires_table wires_table_;

	//Private member funcitons
	bool create_nets(const evl_wires &wires);
	std::string make_net_name(std::string wire_name, int i)
	void create_net(std::string net_name)
	void create_gate(const evl_component &comp,nets_table nets_table_)
	bool create_gates(const evl_components &comps);

	public:
	netlist();
	bool create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table);
};
#endif
