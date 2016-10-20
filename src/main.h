#ifndef MAIN_H
#define MAIN_H
#include <string>
#include <iostream>
#include "parser.h"
#include "netlist.h"

void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);
bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
bool store_netlist_to_file(std::string file_name,const evl_module &module,const netlist &nl);
void display_netlist(std::ostream &out,const evl_module &module,const netlist &nl);
#endif
