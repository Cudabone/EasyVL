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
		net_ = nets_table_.find(net_name)->second;
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
gate *pin::get_gate()
{
	return gate_;
}
net *pin::get_net()
{
	return net_;
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
