#include "gate.h"

gate::gate()
{
	num_pins = 0;
}
gate::~gate()
{
	for(std::vector<pin *>::iterator it = pins_.begin(); it != pins_.end(); it++)
		delete *it;
}

void gate::set_name(std::string name)
{
	name_ = name;
}
void gate::set_type(std::string type)
{
	type_= type;
}
void gate::set_output(bool output)
{
	output_ = output;
}
bool gate::get_output()
{
	return output_;
}
bool gate::get_visited()
{
	return visited_;
}
void gate::set_visited(bool visited)
{
	visited_ = visited;
}
bool gate::get_state()
{
	return state_;
}
void gate::set_state(bool state)
{
	state_ = state;
}
void gate::set_next_state()
{
	state_ = next_state_;
}
void gate::evaluate()
{
	std::vector<bool> inputs;
	if(visited_ == false)
	{
		for(std::vector<pin *>::iterator pp = pins_.begin(); pp != pins_.end(); pp++)
		{
			std::vector<net *> pin_nets = (*pp)->get_nets();
			if((*pp)->get_dir() == 'i')
			{
				for(std::vector<net *>::iterator np = pin_nets.begin(); np != pin_nets.end(); np++)
				{
					if((*np)->get_signal() == '?')
						(*np)->get_driver()->evaluate();
					inputs.push_back((*np)->get_signal());
				}
			}
		}
		bool output = compute_output(inputs);
		std::vector<net *> out_nets = pins_[0]->get_nets();
		for(std::vector<net *>::iterator it = out_nets.begin(); it != out_nets.end(); it++)
		{
			(*it)->set_logic_value(output);
		}
		visited_ = true;
	}
}
bool gate::compute_output(std::vector<bool> inputs)
{
	bool output;
	if(type_ == "not")
	{	
		if(inputs[0] == false)
			output = true;
		else
			output = false;
	}
	else if(type_ == "evl_dff")
	{
		state_ = next_state_;
		assert(inputs.size() == 1);
		next_state_ = inputs[0];
		return state_;
	}
	else if(type_ == "evl_output")
	{

	}
	assert(false);
	return false;
}
void gate::set_output(bool output)
{
	for(std::vector<net *>::iterator it = pins)
}
void gate::init_state()
{
	if(type_ == "evl_output")
	{
		state_ = false;
		next_state_ = false;
	}
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
	p->set_dir(name_, index);
	return p->create(this,index,ep,nets_table_,wires_table);
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
		else if(!verify_pin_width(1))
				return false;
	}
	else if(type_ == "not" || type_ == "buf")
	{
		if(num_pins != 2)
		{
			std::cout << "Expected at exactly 2 pins for component '"<< type_ << "' but found '"<< num_pins <<"'" <<std::endl;
				return false;
		}
		else if(!verify_pin_width(1))
				return false;
	}
	else if(type_ == "evl_dff" || type_ == "tris")
	{
		if(num_pins != 3)
		{
			std::cout << "Expected at exactly 3 pins for component '"<< type_ << "' but found '"<< num_pins <<"'" <<std::endl;
				return false;
		}
		else if(!verify_pin_width(1))
				return false;
	}
	else if(type_ == "evl_one" || type_ == "evl_zero" || type_ == "evl_input")
	{
		if(num_pins < 1)
		{
			std::cout << "Expected at least 1 pins for component '"<< type_ << "' but found '"<< num_pins <<"'" <<std::endl;
				return false;
		}
		//Verify all pins are outputs
	}
	else if(type_ == "evl_output")
	{
		if(num_pins < 1)
		{
			std::cout << "Expected at least 1 pins for component '"<< type_ << "' but found '"<< num_pins <<"'" <<std::endl;
				return false;
		}
		//Verify all pins are inputs
	}
	return true;
}
bool gate::verify_pin_width(int width)
{
	pin *p;
	for(std::vector<pin *>::const_iterator it = pins_.begin(); it != pins_.end(); it++)
	{
		p = *it;
		if(p->get_width() != width)
		{
			std::cout << "Expected a pin width of " << width << " for component '"<< type_ << "'" <<std::endl;
				return false;
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
