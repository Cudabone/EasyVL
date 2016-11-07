#ifndef NETLIST_H
#define NETLIST_H
#include <list>
#include <iostream>
#include "net.h"
#include "gate.h"
#include "evl_types.h"

typedef std::map<std::string,net *> nets_table;

class netlist 
{
	private:
	std::list<gate *> gates_;
	std::list<net *> nets_;
	nets_table nets_table_;
	evl_wires_table wires_table_;
	int num_nets;
	int num_gates;

	//Private member funcitons
	bool create_nets(const evl_wires &wires);
	std::string make_net_name(std::string wire_name, int i);
	bool create_net(std::string net_name);
	bool create_gates(const evl_components &comps,const evl_wires_table &wires_table);
	bool create_gate(const evl_component &comp, const evl_wires_table &wires_table);

	public:
	netlist();
	~netlist();
	bool create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table);
	void display_netlist(std::ostream &out) const;
};
#endif
