#ifndef PIN_H
#define PIN_H
#include <string>
#include <map>
#include "net.h"
#include "parser.h"

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
	int width_;

	bool validate_structural_semantics();

	public:
	pin();
	bool create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_);
	gate *get_gate();
	net *get_net();
	char get_dir();
	size_t get_index();
	int get_width();
};
#endif
