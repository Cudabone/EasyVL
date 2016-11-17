#include "net.h"

net::net(std::string name)
:name_(name)
{
	num_pins = 0;
}

void net::append_pin(pin *p)
{
	connections_.push_back(p);
	num_pins++;
}

std::string net::get_name()
{
	return name_;
}
char net::get_signal()
{
	return signal_;
}
std::list<pin *> net::get_connections()
{
	return connections_;
}
int net::get_num_pins()
{
	return num_pins;
}
void net::set_driver()
{
	for(std::list<pin *>::iterator it = connections_.begin(); it != connections_.end(); it++)
	{
		assert((*it)->get_dir() != 'n');
		if((*it)->get_dir() == 'o')
		{
			driver_ = (*it)->get_gate();
			break;
		}
	}
}
gate *net::get_driver()
{
	return driver_;
}
void net::set_signal(char signal)
{
	signal_ = signal;
}
bool net::get_logic_value()
{
	return logic_value_;
}
void net::set_logic_value(bool value)
{
	logic_value_ = value;	
}
void net::set_invalid_signal()
{
	valid_ = false;
}
bool net::valid_signal()
{
	return valid_;
}
void net::set_valid_signal()
{
	valid_ = true;
}
