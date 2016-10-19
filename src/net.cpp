#include "net.h"

net::net(std::string name)
:name_(name);
{}

void net::append_pin(pin *p)
{
	connections_.push_back(p);
}
