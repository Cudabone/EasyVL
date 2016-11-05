#include "pin.h"

pin::pin()
{
	width_ = 0;
}
bool pin::create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_)
{
	gate_ = g;
	index_ = index;
	if(p.bus_msb == -1) //1-bit wire
	{
		width_ = 1;
		std::string net_name = p.name;
		net *n = nets_table_.find(net_name)->second;
		nets_.push_back(n);
		n->append_pin(this);
	}
	else //Bus
	{
		int start = 0, end = 0;
		//Multple Nets
		if(p.bus_msb != -1 && p.bus_lsb != -1)
		{
			width_ = p.bus_msb-p.bus_lsb + 1;
			start = p.bus_lsb;
			end = p.bus_msb;
		}
		//One net of bus
		else if(p.bus_msb != -1 && p.bus_lsb == -1)
		{
			width_ = 1;
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
			net *n = nets_table_.find(net_name)->second;
			nets_.push_back(n);
			n->append_pin(this);
		}
	}
	return validate_structural_semantics();
}
bool pin::validate_structural_semantics()
{
	return true;
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
