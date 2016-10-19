#include <string>

#ifndef PIN_H
#define PIN_H
#include "pin.h"
#endif

#ifndef NET_H
#define NET_H
class net
{
	private:
	std::string name_;
	char signal_;
	std::list<pin *> connnections_;

	public: 
	net(std::string name);
	append_pin(pin *p);
};
#endif
