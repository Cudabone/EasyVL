#include "netlist.h"

netlist::netlist();

bool netlist::create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table)
{
	return create_nets(wires) && create_gates(comps,wires_table);
}

bool netlist::create_nets(const evl_wires &wires)
{
	for(evl_wires::const_iterator it = wires.begin(); it != wires.end(); it++)
	{
		int width = it->width;
		std::string name = it->name;
		if(width == 1)
			create_net(name);
		else
		{
			for(int i = 0; i < width; i++)
				create_net(make_net_name(name,i));
		}
	}
	return true;
}

std::string make_net_name(std::string wire_name, int i)
{
	assert(i >= 0);
	std::ostringstream oss;
	oss << wire_name << "[" << i << "]";
	return oss.str();
}

void netlist::create_net(std::string net_name)
{
	assert(nets_table_.find(net_name) == nets_table_.end());
	net *n = new net(net_name);
	net_table_[net_name] = n;
	nets_.push_back(n);
}

bool netlist::create_gates(const evl_components &comps)
{
	for(iterator::const_iterator it = comps.begin(); it != comps.end(); it++)
		create_gate(*it,wires_table);
	return true;
}

void netlist::create_gate(const evl_component &comp,nets_table nets_table_)
{
	gate *g = new gate;
	gates_.push_back(g);
	return g->create(comp,nets_table_,wires_table_);
}
