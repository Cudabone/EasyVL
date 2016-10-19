#include "main.h"

int main(int argc, char *argv[])
{
	// verify that argv[1] exists
    if (argc < 2)
    {
        std::cerr << "You should provide a file name." << std::endl;
        return -1;
    }
	std::string evl_file = argv[1];
	evl_modules modules;
	if(!parse_evl_file(evl_file,modules))
		return -1;

	evl_module module = modules.front();
	evl_wires wires = module.wires;
	evl_components comps = module.components;
	evl_wires_table wires_table;

	make_wires_table(wires, wires_table);
	netlist nl;
	if(!nl.create(wires,comps,wires_table))
		return -1;
	return 0;	
}
bool make_wires_table(const evl_wires &wires, evl_wires_table &wires_table)
{
	for(evl_wires::const_iterator it = wires.begin(); it != wires.end(); it++)
	{
		evl_wires_table::iterator same_name = wires_table.find(it->name);
		if(same_name != wires_table.end())
		{
			std::cerr << "Wire '" << it->name << "' is already defined" << std::endl;
			return false;
		}
		wires_table[it->name] = it->width;
	}
	return true;
}
void display_wires_table(std::ostream &out, const evl_wires_table &wires_table)
{
	for(evl_wires_table::const_iterator it = wires_table.begin(); it != wires_table.end(); it++)
	{
		out << "wire " << it->first << " " << it->second << std::endl;
	}
}
