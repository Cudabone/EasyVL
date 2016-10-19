#include "gate.h"

gate::gate();

void gate::set_name(std::string name)
{
	name_ = name;
}
void gate::set_type(std::string type)
{
	type_= type;
}
bool gate::create(const evl_component &c, const std::map<std::string, net *> &nets_table, const evl_wires_table &wires_table)
{
	set_name(c.name);
	set_type(c.type);
	size_t index=0;
	evl_pins pins = c->pins;
	for(evl_pins::const_iterator it = pins.begin(); it != pins.end; it++)
	{
		create_pin(ep, index, nets_table, wires_table);
		++index;
	}
	return validate_structural_semantics();
}

bool gate::create_pin(const evl_pin &ep, size_t index,const std::map<std::string,net *> &nets_table, const evl_wires_table &wires_table)
{
	//resolve semantics of ep using wires_table_
	pin *p = new pin;
	pins_.push_back(p);
	return p->create(this,index,ep,nets_table_);
}

bool validate_structural_semantics()
{
	return true;
}
