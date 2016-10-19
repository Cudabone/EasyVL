#include <string>
#include <map>
//#ifndef GATE_H
//#define GATE_H
//#include "gate.h"
//#endif
//#ifndef NET_H
//#define NET_H
#include "net.h"
//#endif
#include "parser.h"

#ifndef PIN_H
#define PIN_H
class net;
class gate;
typedef std::map<std::string,net *> nets_table;

class pin
{
	private:
	char dir_;
	gate *gate_;
	size_t index_;
	net *net_;

	bool validate_structural_semantics();

	public:
	pin();
	bool create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_);
};
#endif
