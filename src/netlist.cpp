#include "netlist.h"
#include <sstream>
#include <cassert>

netlist::netlist()
{
	num_nets = 0;
	num_gates = 0;
}

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
		{
			if(!create_net(name))
				return false;
			num_nets++;
		}
		else
		{
			for(int i = 0; i < width; i++)
			{
				if(!create_net(make_net_name(name,i)))
					return false;
				num_nets++;
			}
		}
	}
	return true;
}

std::string netlist::make_net_name(std::string wire_name, int i)
{
	assert(i >= 0);
	std::ostringstream oss;
	oss << wire_name << "[" << i << "]";
	return oss.str();
}

bool netlist::create_net(std::string net_name)
{
	if(nets_table_.find(net_name) != nets_table_.end())
		return false;
	net *n = new net(net_name);
	nets_table_[net_name] = n;
	nets_.push_back(n);
	return true;
}

bool netlist::create_gates(const evl_components &comps,const evl_wires_table &wires_table)
{
	for(evl_components::const_iterator it = comps.begin(); it != comps.end(); it++)
	{
		if(create_gate(*it,wires_table) == false)
			return false;
		num_gates++;
	}
	return true;
}

bool netlist::create_gate(const evl_component &comp,const evl_wires_table &wires_table)
{
	gate *g = new gate;
	gates_.push_back(g);
	return g->create(comp,nets_table_,wires_table);
}

void netlist::display_netlist(std::ostream &out) const
{
	//print nets and components
	out << "nets " << num_nets << std::endl;
	for(std::list<net *>::const_iterator it = nets_.begin(); it != nets_.end(); it++)
	{
		net *n = *it;	
		out << "  net " << n->get_name() << " " << n->get_num_pins() << std::endl;
		std::list<pin *> connections = n->get_connections();
		for(std::list<pin *>::const_iterator it = connections.begin(); it != connections.end(); it++)
		{
			pin *p = *it;
			gate *g = p->get_gate();
			if(g->get_name() != "")
				out << "    " << g->get_type() << " " << g->get_name() << " " << p->get_index() << std::endl; 
			else
				out << "    " << g->get_type() << " " << p->get_index() << std::endl; 
		}
	}
	//print gates/components
	out << "components " << num_gates << std::endl;
	for(std::list<gate *>::const_iterator it = gates_.begin(); it != gates_.end(); it++)
	{
		gate *g = *it;
		if(g->get_name() != "")
			out << "  component " << g->get_type() << " " << g->get_name() << " " << g->get_num_pins() << std::endl;
		else
			out << "  component " << g->get_type() << " " << g->get_num_pins() << std::endl;
		std::vector<pin *> pins = g->get_pins();
		for(std::vector<pin *>::const_iterator it = pins.begin(); it != pins.end(); it++)
		{
			pin *p = *it;
			//Implement bus printing here
			// pin.get_width(); 
			std::vector<net *> nets = p->get_nets();
			out << "    pin " << p->get_width() << " ";
			for(std::vector<net *>::const_iterator npp = nets.begin(); npp != nets.end(); npp++)
			{
				net *n = *npp;
				out << n->get_name() << " ";
			}
			out << std::endl;

		}
	}
}
