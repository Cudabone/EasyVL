#ifndef NET_H
#define NET_H
#include <string>
#include <list>
#include "pin.h"

class gate;
class pin;
class net
{
	private:
	std::string name_;
	char signal_;
	bool logic_value_;
	std::list<pin *> connections_;
	int num_pins;
	gate *driver_;

	public: 
	gate *get_driver();
	net(std::string name);
	void append_pin(pin *p);
	std::string get_name();
	char get_signal();
	std::list<pin *> get_connections();
	int get_num_pins();
	void set_driver();
	void set_signal(char signal);
	bool get_logic_value();
	void set_logic_value(bool value);
};
#endif
