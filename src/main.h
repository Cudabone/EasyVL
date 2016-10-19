#include <string>
#include <iostream>
#include <map>
//#ifndef PARSER_H
//#define PARSER_H
#include "parser.h"
//#endif
//#ifndef NETLIST_H
//#define NETLIST_H
#include "netlist.h"
//#endif

#ifndef MAIN_H
#define MAIN_H
typedef std::map<std::string, int> evl_wires_table;

void display_wires_table(std::ostream &out, const evl_wires_table &wires_table);
bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table);
#endif
