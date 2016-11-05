#ifndef PIN_H
#define PIN_H
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <assert.h>
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
	//net *net_;
	std::vector<net *> nets_;
	int width_;

	bool validate_structural_semantics();

	public:
	pin();
	bool create(gate *g, size_t index, const evl_pin &p, const nets_table &nets_table_);
	gate *get_gate();
	std::vector<net *> get_nets();
	char get_dir();
	size_t get_index();
	int get_width();
};
#endif