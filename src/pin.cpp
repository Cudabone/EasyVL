#include "pin.h"

pin::pin()
{

}
bool pin::create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_)
{
	gate_ = g;
	index_ = index;
	if(p.bus_msb == -1) //1-bit wire
	{
		std::string net_name = p.name;
		net *net_ = nets_table_.find(net_name)->second;
		net_->append_pin(this);
	}
	else
	{
		//a bus
	}
	return validate_structural_semantics();
}

bool pin::validate_structural_semantics()
{
	return true;
}
