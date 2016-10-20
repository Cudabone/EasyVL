#ifndef NET_H
#define NET_H
#include <string>
#include <list>

class pin;

class net
{
	private:
	std::string name_;
	char signal_;
	std::list<pin *> connections_;
	int num_pins;

	public: 
	net(std::string name);
	void append_pin(pin *p);
	std::string get_name();
	char get_signal();
	std::list<pin *> get_connections();
	int get_num_pins();
};
#endif
