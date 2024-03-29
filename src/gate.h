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
	bool output_;
	bool visited_;
	bool state_, next_state_;
	std::vector<bool> outputs;

	void set_name(std::string name);
	void set_type(std::string type);
	bool validate_structural_semantics();
	bool verify_pin_width(int width);

	public:
	gate();
	~gate();
	int get_num_pins();
	std::vector<uint32_t> get_output_cycle();
	std::vector<pin *> get_pins();
	std::string get_name();
	std::string get_type();
	void set_output(bool output);
	void set_out_nets(bool output);
	void set_state(bool state);
	void set_next_state();
	bool compute_output(const std::vector<bool> &inputs);
	bool get_state();
	void init_state();
	bool get_output();
	bool get_visited();
	void set_visited(bool visited);
	void evaluate();
	bool create(const evl_component &c, const nets_table &nets_table_, const evl_wires_table &wires_table);
	bool create_pin(const evl_pin &ep, size_t index,const nets_table &nets_table_, const evl_wires_table &wires_table);
};
#endif
