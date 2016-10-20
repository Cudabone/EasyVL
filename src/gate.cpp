#include "gate.h"

gate::gate()
{
	num_pins = 0;
}

void gate::set_name(std::string name)
{
	name_ = name;
}
void gate::set_type(std::string type)
{
	type_= type;
}
bool gate::create(const evl_component &c, const nets_table &nets_table_, const evl_wires_table &wires_table)
{
	set_name(c.name);
	set_type(c.type);
	size_t index=0;
	evl_pins pins = c.pins;
	for(evl_pins::const_iterator it = pins.begin(); it != pins.end(); it++)
	{
		create_pin(*it, index, nets_table_, wires_table);
		++index;
		num_pins++;
	}
	return validate_structural_semantics();
}

bool gate::create_pin(const evl_pin &ep, size_t index,const nets_table &nets_table_, const evl_wires_table &wires_table)
{
	//resolve semantics of ep using wires_table_
	pin *p = new pin();
	pins_.push_back(p);
	return p->create(this,index,ep,nets_table_);
}

bool gate::validate_structural_semantics()
{
	if(type_ == "and" || type_ == "or" || type_ == "xor")
	{
		if(num_pins < 3)
		{
			std::cout << "Expected at least 3 pins for component '"<< type_ << "' but found '"<< num_pins <<"'" <<std::endl;
				return false;
		}
		else
		{
			pin *p;
			for(std::vector<pin *>::const_iterator it = pins_.begin(); it != pins_.end(); it++)
			{
				p = *it;
				if(p->get_width() != 1)
					return false;
			}
		}

	}
	return true;
}
std::string gate::get_name()
{
	return name_;
}
std::string gate::get_type()
{
	return type_;
}
int gate::get_num_pins()
{
	return num_pins;
}
std::vector<pin *> gate::get_pins()
{
	return pins_;
}
