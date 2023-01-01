#pragma once
#include <cctype>
#ifndef PARSER_HPP_SXBQ9ATJ
#define PARSER_HPP_SXBQ9ATJ
#include <sstream>
#include <fstream>
#include <exception>
#include <string>
#include "json.hpp"
#include <memory>
namespace sovlyn{
namespace json{
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
		bool load_file(const std::string & filename);
		bool load_string(const std::string & str);
		json parse();

	private:
		void parse_blank();
		std::string parse_key();
		std::shared_ptr<Value> parse_simple_value();
		void parse_list(jsonList *);
		void parse_json(json *);
};

Parser::Parser(){}

Parser::~Parser(){}

bool Parser::load_file(const std::string & filename){
	std::ifstream file(filename);
	if(!file.is_open())return false;
	std::stringstream buf;
	buf<<file.rdbuf();
	m_str=buf.str();
	return true;
}

bool Parser::load_string(const std::string & str){
	m_str=str;
	return true;
}

json Parser::parse(){
	m_idx=0;
	parse_blank();
	if(m_idx>=m_str.size()||m_str[m_idx]!='{')
		throw wrong_format_error("cannot found '{' at "+std::to_string(m_idx));

	json to_return;
	parse_json(&to_return);
	parse_blank();
	if(m_idx<m_str.size())
		throw wrong_format_error("extra charactor found at "+std::to_string(m_idx));
	return to_return;
}

void Parser::parse_blank(){
	while(m_idx<m_str.size()&&
			(m_str[m_idx]==' '||m_str[m_idx]=='\n'||m_str[m_idx]=='\t'||m_str[m_idx]=='\r'))m_idx++;
}

void Parser::parse_json(json * j){
	m_idx++;
	parse_blank();
	while(m_str[m_idx]!='}'){
		parse_blank();
		std::string key=parse_key();
		parse_blank();
		if(m_str[m_idx]!=':')
			throw wrong_format_error("cannot found ':' at "+std::to_string(m_idx));
		m_idx++;
		parse_blank();
		std::shared_ptr<jsonValue> val;
		if(m_str[m_idx]=='['){
			int pos=m_str.find(']', m_idx);
			if(pos>=m_str.size())
				throw wrong_format_error("cannot found ']' to match '[' at "+std::to_string(m_idx));
			val.reset(new jsonList());
			parse_list(dynamic_cast<jsonList*>(val.get()));
		}else if(m_str[m_idx]=='{'){
			int pos=m_str.find('}', m_idx);
			if(pos>=m_str.size())
				throw wrong_format_error("cannot found '}' to match '{' at "+std::to_string(m_idx));
			val.reset(new json());
			parse_json(dynamic_cast<json*>(val.get()));
		}else{
			val.reset(new jsonSimpleValue(parse_simple_value()));
		}
		j->insert(key, val);
		parse_blank();
		if(m_str[m_idx]!=','&&m_str[m_idx]!='}')
			throw wrong_format_error("illigal charactor at "+std::to_string(m_idx));
		if(m_str[m_idx]==',')m_idx++;
	}
	m_idx++;
}

void Parser::parse_list(jsonList *l){
	m_idx++;
	parse_blank();
	while(m_str[m_idx]!=']'){
		parse_blank();
		if(m_str[m_idx]=='{'){
			int pos=m_str.find('}', m_idx);
			if(pos>=m_str.size())
				throw wrong_format_error("cannot found '}' to match '{' at "+std::to_string(m_idx));
			std::shared_ptr<jsonValue> val(new json());
			parse_json(dynamic_cast<json*>(val.get()));
			l->add(val);
		}else if(m_idx=='['){
			int pos=m_str.find(']', m_idx);
			if(pos>=m_str.size())
				throw wrong_format_error("cannot found ']' to match '[' at "+std::to_string(m_idx));
			std::shared_ptr<jsonValue> val(new jsonList());
			parse_list(dynamic_cast<jsonList*>(val.get()));
		}else{
			auto val=std::shared_ptr<jsonValue>(new jsonSimpleValue(parse_simple_value()));
			l->add(val);	
		}
		parse_blank();
		if(m_str[m_idx]!=','&&m_str[m_idx]!=']')
			throw wrong_format_error("illigal charactor at "+std::to_string(m_idx));
		if(m_str[m_idx]==',')m_idx++;
	}
	m_idx++;
}

std::string Parser::parse_key(){
	if(m_str[m_idx]!='"')
		throw wrong_format_error("cannot find '\"' at "+std::to_string(m_idx));
	m_idx++;
	int pos=m_str.find('"', m_idx);
	if(pos>=m_str.size())
		throw wrong_format_error("cannot find '\"' to match '\"' at "+std::to_string(m_idx-1));
	while(m_str[pos-1]=='\\'){
		pos=m_str.find('"', pos+1);
		if(pos>=m_str.size())
			throw wrong_format_error("cannot find '\"' to match '\"' at "+std::to_string(m_idx-1));
	}
	int i=m_idx;
	m_idx=pos+1;
	return m_str.substr(i, pos-i);
}

std::shared_ptr<Value> Parser::parse_simple_value(){
	if(m_str[m_idx]=='"'){
		std::shared_ptr<Value> to_return(new Value(parse_key()));
		return to_return;
	}else if(m_str.compare(m_idx, 4, "true")==0){
		m_idx+=4;
		std::shared_ptr<Value> to_return(new Value(true));
		return to_return;
	}else if(m_str.compare(m_idx, 5, "false")==0){
		m_idx+=5;
		std::shared_ptr<Value> to_return(new Value(false));
		return to_return;
	}else if(m_str.compare(m_idx, 4, "null")==0){
		m_idx+=4;
		std::shared_ptr<Value> to_return(new Value());
		return to_return;
	}else{
		int end=m_idx;
		while(end<m_str.size()&&(std::isalnum(m_str[end])||m_str[end]=='.'))
			end++;
		if(end==m_idx)
			throw wrong_format_error("cannot found value at "+std::to_string(m_idx));
		std::string raw=m_str.substr(m_idx, end-m_idx);
		m_idx=end;
		char *num_end;
		long num=std::strtol(raw.c_str(), &num_end, 10);
		if((num_end-raw.c_str())>=raw.size())return std::shared_ptr<Value>(new Value(num));
		double f=std::strtod(raw.c_str(), &num_end);
		if((num_end-raw.c_str())>=raw.size())return std::shared_ptr<Value>(new Value(f));
		return std::shared_ptr<Value>(new Value(raw, true));
	}
}

}
}


#endif /* end of include guard: PARSER_HPP_SXBQ9ATJ */
