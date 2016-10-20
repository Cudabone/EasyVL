#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include "evl_types.h"

/* Function Prototypes */
bool parse_evl_file(std::string evl_file, evl_modules &modules);
bool process_module_statement(evl_module &module,const evl_statement &statement);
void pparse_error(std::string expected,std::string found,int line_no);
bool process_component_statement(evl_components &components,evl_statement &s);
bool process_wire_statement(evl_wires &wires, evl_statement &s);
bool move_tokens_into_statement(evl_tokens &statement_tokens, evl_tokens &tokens);
void display_statements(std::ostream &out,const evl_modules &modules);
bool group_tokens_into_statements(evl_modules &modules,evl_tokens &tokens);
void display_tokens(std::ostream &out, const evl_tokens &tokens);
bool store_tokens_to_file(std::string file_name,const evl_tokens &tokens);
bool extract_tokens_from_file(std::string evl_file, evl_tokens &tokens);
bool extract_tokens_from_line(std::string line, int line_no, evl_tokens &tokens); 
bool is_space(char ch);
bool is_alpha(char ch);
bool is_single(char ch);
bool is_number(char ch);
bool token_is_semicolon(const evl_token &token);
bool store_statements_to_file(std::string file_name,const evl_modules &modules);
#endif
