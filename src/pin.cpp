#include "pin.h"

bool pin::create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net*> &nets_table)
{
	gate_ = g;
	index_ = index;
	if(p.msb == -1) //1-bit wire
	{
		net_name = p.name;
		net_ = nets_table_.find(net_name);
		net_->append_pin(this);
	}
	else
	{
		//a bus
	}
	return validate_structural_semantics()
}

bool pin::validate_structural_semantics()
{
	return true;
}
