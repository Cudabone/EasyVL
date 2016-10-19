#ifndef NET_H
#define NET_H
#include <string>
#include <list>

//#ifndef PIN_H
//#define PIN_H
//#include "pin.h"
//#endif
class pin;

class net
{
	private:
	std::string name_;
	char signal_;
	std::list<pin *> connections_;

	public: 
	net(std::string name);
	void append_pin(pin *p);
};
#endif
