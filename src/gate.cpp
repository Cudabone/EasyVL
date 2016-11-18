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
	if(type_ == "evl_dff")
	{
		net *out_net = pins_[0]->get_nets()[0];
		out_net->set_valid_signal();
		out_net->set_logic_value(state_);
		//Ensure we dont find driver of clock
		net *clk_net = pins_[2]->get_nets()[0];
		clk_net->set_valid_signal();
		clk_net->set_logic_value(true);
		/*
		if(state_ == true)
			std::cout << 1;
		else
			std::cout << 0;
		std::cout << std::endl;
		*/
	}
	for(std::vector<pin *>::iterator pp = pins_.begin(); pp != pins_.end(); pp++)
	{
		std::vector<net *> pin_nets = (*pp)->get_nets();
		if((*pp)->get_dir() == 'i')
		{
			for(std::vector<net *>::iterator np = pin_nets.begin(); np != pin_nets.end(); np++)
			{
				if(!((*np)->valid_signal()))
				{
					(*np)->get_driver()->evaluate();
				}
				//if((*np)->get_signal() == '?')
				/*
				   if((*np)->get_driver()->get_visited() == false)
				   {
				   (*np)->get_driver()->evaluate();
				   }
				   */
				inputs.push_back((*np)->get_logic_value());
			}
		}
	}
	if(type_ == "evl_dff")
	{
		next_state_ = inputs[0];
		state_ = next_state_;
	}
	else if(type_ != "evl_output")
	{
		bool output = compute_output(inputs);
		set_out_nets(output);
	}
}
bool gate::compute_output(const std::vector<bool> &inputs)
{
	if(type_ == "not")
	{	
		if(inputs[0] == false)
			return true;
		else
			return false;
	}
	assert(false);
	return false;
}
std::vector<uint32_t> gate::get_output_cycle()
{
	std::vector<uint32_t> outputs;
	for(std::vector<pin *>::iterator pp = pins_.begin(); pp != pins_.end(); pp++)
	{
		std::vector<net *> pin_nets = (*pp)->get_nets();
		int i = 0;
		uint32_t num = 0;
		for(std::vector<net *>::iterator np = pin_nets.begin(); np != pin_nets.end(); np++)
		{
			if((*np)->get_logic_value() == true)
			{
				num += ((unsigned int)1 << i);
			}
			i++;
		}
		outputs.push_back(num);
	}
	return outputs;
}
/*
std::vector<std::string> gate::get_output_cycle()
{
	std::vector<std::string> outputs;
	for(std::vector<pin *>::iterator pp = pins_.begin(); pp != pins_.end(); pp++)
	{
		std::vector<net *> pin_nets = (*pp)->get_nets();
		std::ostringstream oss;
		size_t len = pin_nets.size();
		size_t rem = len % 4;
		if(rem != 0)
		{
			for(int i = 0; i < rem; i++)
				oss << "0";
		}
		int count = 0;
		for(std::vector<net *>::iterator np = pin_nets.begin(); np != pin_nets.end(); np++)
		{
			count++;
			if((*np)->get_logic_value() == true)
			{
				//write char 1
				oss << "1";
			}
			else
			{
				//write char 0
				oss << "0";
			}
		}
		outputs.push_back(oss.str());
	}
	return outputs;
}
*/
void gate::set_out_nets(bool output)
{
	std::vector<net *> out_nets = pins_[0]->get_nets();
	for(std::vector<net *>::iterator it = out_nets.begin(); it != out_nets.end(); it++)
	{
		if(!((*it)->valid_signal()))
		{ 
			(*it)->set_logic_value(output); 
			(*it)->set_valid_signal();
		}
	}
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
