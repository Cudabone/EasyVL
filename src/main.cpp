#include <fstream>
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
	netlist *nl = new netlist(evl_file);
	if(!nl->create(wires,comps,wires_table))
		return -1;
	if(!store_netlist_to_file(evl_file+".netlist",module,*nl))
		return -1;
	//display_netlist(std::cout,module,*nl);
	nl->simulate(1000);
	return 0;	
	//Delete Netlist
	delete nl;
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
void display_netlist(std::ostream &out,const evl_module &module,const netlist &nl)
{
	//Print module
	out << "module " << module.name << std::endl;
	nl.display_netlist(out);
}
bool store_netlist_to_file(std::string file_name,const evl_module &module,const netlist &nl)
{
	std::ofstream output_file(file_name.c_str());
    if (!output_file)
    {
        std::cerr << "I can't write " << file_name << std::endl;
        return -1;
    }
	display_netlist(output_file,module,nl);
	return true;
}
