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
