#ifndef PIN_H
#define PIN_H
#include <string>
//#ifndef GATE_H
//#define GATE_H
#include "gate.h"
//#endif
//#ifndef NET_H
//#define NET_H
#include "net.h"
//#endif

class pin
{
	private:
	char dir_;
	gate *gate_;
	size_t index_;
	net *net_;

	bool validate_structural_semantics();

	public:
	bool create(gate *g, size_t index, const evl_pin &p, const std::map<std::string, net*> &nets_table);
};
#endif
