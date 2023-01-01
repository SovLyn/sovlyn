#pragma once
#ifndef PARSER_HPP_MAB1ROVK
#define PARSER_HPP_MAB1ROVK
#include <cctype>
#include <exception>
#include <ios>
#include <string>
#include <memory>
#include <sovlyn/utility/Value.hpp>
#include <fstream>
#include <sstream>
#include <map>

namespace sovlyn{
namespace ini{
using sovlyn::utility::Value;
class wrong_format_error:public std::exception{
public:
	wrong_format_error(const std::string & msg):m_msg(msg){}
	virtual const char * what()const noexcept override{return m_msg.c_str();}

private:
	std::string m_msg;

};

typedef std::shared_ptr<std::map<std::string, Value>> section;
typedef std::shared_ptr<std::map<std::string, section>> config;

class Parser{
public:
	Parser(){}
	~Parser(){}

	bool load_file(const std::string & filename);
	bool load_string(const std::string & str);

	config parse();

private:
	std::shared_ptr<std::istringstream> m_str;

	void parse_blank(const std::string & line, int & i);
	Value parse_val(const std::string & line, int & i);
};

bool Parser::load_file(const std::string & filename){
	std::ifstream f;
	f.open(filename);
	if(!f.is_open())return false;
	std::stringstream buf;
	buf<<f.rdbuf();
	m_str.reset(new std::istringstream(buf.str()));
	return true;
}

bool Parser::load_string(const std::string & str){
	m_str.reset(new std::istringstream(str));
	return true;
}

config Parser::parse(){
	m_str->seekg(std::ios_base::beg);
	config to_return(new std::map<std::string, section>());
	std::string cur_sec="";

	std::string line;
	int line_num=0;
	while(getline(*m_str, line)){
		std::cout << line_num<<' '<<line << std::endl;
		int i=0;
		parse_blank(line, i);
		if((i>=line.size())||(line[i]==';'||line[i]=='#')){
			line_num++;
			continue;
		}
		if(line[i]=='['){
			int end=line.find(']', i);
			if(end>=line.size())
				throw wrong_format_error("line "+std::to_string(line_num)+" : cannot found ']' to match '[' at "+std::to_string(i));
			cur_sec=line.substr(i+1, end-i-1);
			i=end+1;
			parse_blank(line, i);
			if(i<line.size()&&(line[i]!=';'&&line[i]!='#'))
				throw wrong_format_error("line "+std::to_string(line_num)+" : extra charactor found at "+std::to_string(i));
			line_num++;
			continue;
		}
		int end=i;
		while(end<line.size()&&(line[end]!=' '&&line[end]!='\t'&&line[end]!='\r'&&line[end]!='='))end++;
		std::string key=line.substr(i, end-i);
		i=end;
		parse_blank(line, i);
		if(line[i]!='=')
			throw wrong_format_error("line "+std::to_string(line_num)+" : cannot found '=' at "+std::to_string(i));
		i++;
		parse_blank(line, i);
		Value val=parse_val(line, i);
		auto sec=to_return->find(cur_sec);
		if(sec==to_return->end())to_return->emplace(cur_sec, new std::map<std::string, Value>());
		(*(*to_return)[cur_sec])[key]=val;
		parse_blank(line, i);
		if(i<line.size()&&(line[i]!=';'&&line[i]!='#'))
			throw wrong_format_error("line "+std::to_string(line_num)+" : extra charactor found at "+std::to_string(i));

		line_num++;
	}

	return to_return;
}

void Parser::parse_blank(const std::string & line, int & i){
	while(i<line.size()&&(line[i]==' '||line[i]=='\t'||line[i]=='\r'))i++;
}

Value Parser::parse_val(const std::string & line, int & i){
	if(line[i]=='\"'){
		i++;
		int end=i;
		while(end<line.size()&&(line[end]!='\"'||line[end-1]=='\\'))end++;
		int bg=i;
		i=end+1;
		return line.substr(bg, end-bg);
	}else{
		int end=i;
		while(end<line.size()&&(line[end]!=';'&&line[end]!='#'))end++;
		std::string raw=line.substr(i, end-i);
		i=end;
		end=raw.size()-1;
		while(raw[end]==' '||raw[end]=='\t'||raw[end]=='\r')end--;
		raw=raw.substr(0, end+1);
		std::cout << "raw: "<<raw<<"|" << std::endl;
		if(raw=="null")return Value();
		if(raw=="true")return true;
		if(raw=="false")return false;
		char* num_end;
		long num=std::strtol(raw.c_str(), &num_end, 10);
		if((num_end-raw.c_str())>=raw.size())return num;
		double f=std::strtod(raw.c_str(), &num_end);
		if((num_end-raw.c_str())>=raw.size())return f;
		return Value(raw, true);
	}
}
}
}

#endif /* end of include guard: PARSER_HPP_MAB1ROVK */
