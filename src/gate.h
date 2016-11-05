#ifndef GATE_H
#define GATE_H
#include <string>
#include <vector>
#include <map>
#include "pin.h"
#include "evl_types.h"
#include "net.h"

typedef std::map<std::string,net *> nets_table;

class pin;
class gate
{
	private:
	std::string name_;
	std::string type_;
	std::vector<pin *> pins_;
	int num_pins;

	void set_name(std::string name);
	void set_type(std::string type);
	bool validate_structural_semantics();
	bool verify_pin_width(int width);

	public:
	gate();
	int get_num_pins();
	std::vector<pin *> get_pins();
	std::string get_name();
	std::string get_type();
	bool create(const evl_component &c, const nets_table &nets_table_, const evl_wires_table &wires_table);
	bool create_pin(const evl_pin &ep, size_t index,const nets_table &nets_table_, const evl_wires_table &wires_table);
};
#endif
