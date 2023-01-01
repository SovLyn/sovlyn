#pragma once
#ifndef PARSER_HPP_GWCYIXDD
#define PARSER_HPP_GWCYIXDD
#include <string>
#include "Xml.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <exception>
#include <cctype>
#include <cstdlib>

namespace sovlyn{
namespace xml{
class wrong_format_error:public std::exception{
	private:
		std::string m_msg;
	
	public:
		wrong_format_error(const std::string & msg):m_msg(msg){}
		virtual const char * what() const noexcept override{return m_msg.c_str();}

};

class Parser{
	private:
		std::string m_str;
		int m_idx;
	
	public:
		Parser();
		~Parser();
		bool load_file(const std::string & file);
		bool load_string(const std::string & str);
		Xml parse();

	private:
		void skip_white();
		bool parse_declaration();
		bool parse_comment();
		Xml parse_element();
		std::string parse_element_name();
		std::string parse_element_text();
		std::string parse_element_attr_key();
		Value parse_element_attr_value();
};

Parser::Parser(){
}

Parser::~Parser(){
}

bool Parser::load_file(const std::string & file){
	std::ifstream f(file);
	if(!f.is_open())return false;
	std::stringstream buf;
	buf<<f.rdbuf();
	m_str=buf.str();
	m_idx=0;
	return true;
}

bool Parser::load_string(const std::string & str){
	m_str=str;
	m_idx=0;
	return true;
}

Xml Parser::parse(){
	skip_white();
	if(m_idx>=m_str.size())return Xml();
	if(!parse_declaration()){
		throw wrong_format_error("invalid declaration");
	}
	skip_white();
	while(parse_comment())skip_white();
	Xml to_return=parse_element();
	while(parse_comment())skip_white();
	return to_return;
}


void Parser::skip_white(){
	while(m_idx<m_str.size()&&(m_str[m_idx]==' '||m_str[m_idx]=='\r'||m_str[m_idx]=='\n'||m_str[m_idx]=='\t')){
		m_idx++;
	}
}

bool Parser::parse_declaration(){
	if(m_str.compare(m_idx, 5, "<?xml")!=0){
		return false;
	}
	m_idx+=5;
	size_t pos=m_str.find("?>", m_idx);
	if(pos>=m_str.size()){
		return false;
	}
	m_idx=pos+2;
	return true;
}

bool Parser::parse_comment(){
	if(m_str.compare(m_idx, 4, "<!--")!=0){
		return false;
	}
	m_idx+=4;
	size_t pos=m_str.find("-->", m_idx);
	if(pos>=m_str.size()){
		throw wrong_format_error("invalid comment");
	}
	m_idx=pos+3;
	return true;
}

Xml Parser::parse_element(){
	Xml to_return=Xml();
	if(m_str[m_idx]!='<'){
		return to_return;
	}
	m_idx++;
	std::string name=parse_element_name();
	to_return.set_name(name);
	//short form
	int end=m_idx;
	while(end<m_str.size()&&m_str[end]!='>'&&m_str[end]!='/')end++;
	if(end>=m_str.size())throw wrong_format_error("staring tag never ends");
	if(m_str[end]=='/'){
		skip_white();
		while(m_idx<end){
			std::string key=parse_element_attr_key();
			skip_white();
			if(m_str[m_idx]!='=')throw wrong_format_error("no = found");
			m_idx++;
			skip_white();
			Value val=parse_element_attr_value();
			to_return.set_attr(key, val);
			skip_white();
		}
		m_idx=end+2;
		return to_return;
	}
	//long form
	skip_white();
	while(m_idx<end){
		std::string key=parse_element_attr_key();
		skip_white();
		if(m_str[m_idx]!='=')throw wrong_format_error("no = found");
		m_idx++;
		skip_white();
		Value val=parse_element_attr_value();
		to_return.set_attr(key, val);
		skip_white();
	}
	m_idx++;
	std::string end_tag="</"+to_return.name()+'>';
	end=m_str.find(end_tag, m_idx);
	if(end>=m_str.size())throw wrong_format_error("no end tag found");
	while(m_idx<end){
		while(parse_comment())skip_white();
		if(m_idx>=end)continue;
		if(m_str[m_idx]!='<'){
			std::string new_text=parse_element_text();
			if(to_return.text().empty())to_return.set_text(new_text);
			else to_return.set_text(to_return.text()+' '+new_text);
		}else{
			Xml c=parse_element();
			if(c.name().empty())throw wrong_format_error("parse child element fail");
			to_return.append(c);
		}
		skip_white();
	}
	m_idx=end+end_tag.size();
	return to_return;
}

std::string Parser::parse_element_name(){
	if(!std::isalpha(m_str[m_idx])&&m_str[m_idx!='_']){
		throw wrong_format_error("invalid element name");}
	int pos=m_idx;
	while(pos<m_str.size()&&(std::isalnum(m_str[pos])||m_str[pos]=='_'))pos++;
	std::string to_return=m_str.substr(m_idx, pos-m_idx);
	m_idx=pos;
	return to_return;
}

std::string Parser::parse_element_text(){
	int pos=m_idx;
	while(pos<m_str.size()&&m_str[pos]!='\n'&&m_str[pos]!='\r')pos++;
	std::string to_return=m_str.substr(m_idx, pos-m_idx);
	m_idx=pos;
	while(to_return[to_return.size()-1]==' ')to_return.resize(to_return.size()-1);
	return to_return;
}

std::string Parser::parse_element_attr_key(){
	if(!std::isalpha(m_str[m_idx])&&m_str[m_idx!='_']){
		throw wrong_format_error("invalid attribute name");}

	int pos=m_idx;
	while(pos<m_str.size()&&(std::isalnum(m_str[pos])||m_str[pos]=='_'))pos++;
	std::string to_return=m_str.substr(m_idx, pos-m_idx);
	m_idx=pos;
	return to_return;
}

Value Parser::parse_element_attr_value(){
	std::string raw_value;
	if(m_str[m_idx]=='"'){
		m_idx++;
		int end=m_idx;
		while(end<m_str.size()&&(m_str[end]!='"'||m_str[end-1]=='\"'))end++;
		if(end>=m_str.size()||m_str[end]!='"')throw wrong_format_error("found \" in start but not in end");
		raw_value=m_str.substr(m_idx, end-m_idx);
		m_idx=end+1;
	}else if(m_str[m_idx]=='\''){
		m_idx++;
		int end=m_idx;
		while(end<m_str.size()&&std::isalnum(m_str[end]))end++;
		if(end>=m_str.size()||(m_str[end]!='\''||m_str[end-1]!='\''))throw wrong_format_error("found \' in start but not in end");
		raw_value=m_str.substr(m_idx, end-m_idx);
		m_idx=end+1;
	}else{
		throw wrong_format_error("not \" or ' found");
	}
	if(raw_value=="true"){
		return Value(true);
	}
	if(raw_value=="false"){
		return Value(true);
	}
	char* num_end;
	long num=std::strtol(raw_value.c_str(), &num_end, 10);
	if((num_end-raw_value.c_str())>=raw_value.size())return Value(num);
	double f=std::strtod(raw_value.c_str(), &num_end);
	if((num_end-raw_value.c_str())>=raw_value.size())return Value(f);
	return Value(raw_value);
}

}
}

#endif /* end of include guard: PARSER_HPP_GWCYIXDD */
