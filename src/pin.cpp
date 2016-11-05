#include "pin.h"

pin::pin()
{
	width_ = 0;
}
bool pin::create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_, const evl_wires_table &wires_table)
{
	gate_ = g;
	index_ = index;
	if(p.bus_msb == -1) //1-bit wire or bus
	{
		std::string net_name = p.name;
		if(nets_table_.find(net_name) == nets_table_.end()) //A bus name
		{
			//If not in wires table
			if(wires_table.find(net_name) == wires_table.end())
				return false;
			int width = wires_table.find(net_name)->second;	
			width_ = width;
			msb_ = width - 1;
			lsb_ = 0;
			int i;
			for(i = 0; i < width; i++)
			{
				std::ostringstream oss;
				oss << p.name << "[" << i << "]";
				std::string name = oss.str();
				net *n = nets_table_.find(name)->second;
				nets_.push_back(n);
				n->append_pin(this);
			}
		}
		else //Just a 1 bit wire
		{
			if(nets_table_.find(net_name) == nets_table_.end())
				return false;
			width_ = 1;
			msb_ = -1;
			lsb_ = -1;
			net *n = nets_table_.find(net_name)->second;
			nets_.push_back(n);
			n->append_pin(this);
		}
	}
	else //Guarenteed Bus
	{
		int start = 0, end = 0;
		//Multple Nets
		if(p.bus_msb != -1 && p.bus_lsb != -1)
		{
			width_ = p.bus_msb-p.bus_lsb + 1;
			start = p.bus_lsb;
			end = p.bus_msb;
		}
		//One net of bus, one bit wire
		else if(p.bus_msb != -1 && p.bus_lsb == -1)
		{
			width_ = 1;
			msb_ = p.bus_msb;
			lsb_ = -1;
			start = p.bus_msb;
			end = start;
		}
		assert(start >= 0);
		assert(end >= 0);
		int i;
		for(i = start; i <= end; i++)
		{
			std::ostringstream oss;
			oss << p.name << "[" << i << "]";
			std::string net_name = oss.str();
			if(nets_table_.find(net_name) == nets_table_.end())
				return false;
			net *n = nets_table_.find(net_name)->second;
			nets_.push_back(n);
			n->append_pin(this);
		}
	}
	return true;
}
void pin::set_dir(std::string type, int index)
{
	if(type == "evl_output")
	{
		dir_ = 'o';
	}
	else if(type == "evl_input")
	{
		dir_ = 'i';
	}
	else if(index == 0)
	{
		dir_ = 'o';
	}
	else
	{
		dir_ = 'i';
	}
}
gate *pin::get_gate()
{
	return gate_;
}
std::vector<net *> pin::get_nets()
{
	return nets_;
}
char pin::get_dir()
{
	return dir_;
}
size_t pin::get_index()
{
	return index_;
}
int pin::get_width()
{
	return width_;
}
