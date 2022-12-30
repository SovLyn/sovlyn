#pragma once
#ifndef XML_HPP_LHZ2CFID
#define XML_HPP_LHZ2CFID
#include <string>
#include <map>
#include <list>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <memory>
#include "../utility/Value.hpp"

namespace sovlyn{
namespace xml{
using sovlyn::utility::Value;
class Xml{
	private:
		const static std::string s_null_name;
		const static Value s_null_Value;
		std::shared_ptr<std::string> m_name;
		std::shared_ptr<std::string> m_text;
		std::shared_ptr<std::map<std::string, Value>> m_attrs;
		std::shared_ptr<std::list<Xml>> m_child;

	public:
		Xml();
		Xml(const Xml & other);
		Xml & operator=(const Xml &other);
		Xml(const char* name);
		Xml(const std::string & name);
		~Xml();
	
		const std::string & name()const;
		void set_name(const std::string & name);

		const Value & attr(const std::string & key)const;
		void set_attr(const std::string & key, const Value & value);

		std::string str()const;

		const std::string & text()const;
		void set_text(const std::string & text);

		void append(const Xml & child);

		size_t size()const;

		Xml & operator[](int index);
		Xml & operator[](const std::string & name);

		void remove(int index);
		void remove(const std::string & name);

		typedef std::list<Xml>::iterator iterator;
		iterator begin(){if(m_child==nullptr)m_child=std::make_shared<std::list<Xml>>();return m_child->begin();}
		iterator end(){if(m_child==nullptr)m_child=std::make_shared<std::list<Xml>>();return m_child->end();}
		iterator erase(iterator it){return m_child->erase(it);}
};

const std::string Xml::s_null_name="";
const Value Xml::s_null_Value;

Xml::Xml(){
	m_name=nullptr;
	m_text=nullptr;
	m_attrs=nullptr;
	m_child=nullptr;
}

Xml::Xml(const char * name){
	m_name = std::make_shared<std::string>(name);
	m_text=nullptr;
	m_attrs=nullptr;
	m_child=nullptr;
}

Xml::Xml(const std::string & name){
	m_name = std::make_shared<std::string>(name);
	m_text=nullptr;
	m_attrs=nullptr;
	m_child=nullptr;
}

Xml::Xml(const Xml & other){
	m_name=other.m_name;
	m_attrs=other.m_attrs;
	m_text=other.m_text;
	m_child=other.m_child;
}

Xml & Xml::operator=(const Xml & other){
	m_name=other.m_name;
	m_attrs=other.m_attrs;
	m_text=other.m_text;
	m_child=other.m_child;
	return *this;
}

Xml::~Xml(){
}

const std::string & Xml::name()const{
	if(m_name==nullptr){
		return s_null_name;
	}else{
		return *m_name;
	}
}

void Xml::set_name(const std::string & name){
	m_name=std::make_shared<std::string>(name);
}

const Value & Xml::attr(const std::string & key)const{
	if(m_attrs==nullptr){
		return s_null_Value;
	}
	auto it=m_attrs->find(key);
	if(it==m_attrs->end()){
			return s_null_Value;
	}else{
		return it->second;
	}
}

void Xml::set_attr(const std::string & key, const Value & value){
	if(m_attrs==nullptr){
		m_attrs=std::make_shared<std::map<std::string, Value>>();
	}
	(*m_attrs)[key]=value;
}

std::string Xml::str()const{
	if(m_name==nullptr){
		throw std::logic_error("empty name for a Xml node");
	}
	std::stringstream ss;
	ss<<'<'<<*m_name;
	if(m_attrs!=nullptr){
		for(auto & x:*m_attrs){
			ss<<' '<<x.first<<'='<<x.second;
		}
	}
	ss<<'>';
	if(m_child!=nullptr){
		for(auto & c:*m_child){
			ss<<c.str();
		}
	}
	if(m_text!=nullptr){
		ss<<*m_text;
	}
	ss<<"</"<<*m_name<<'>';
	return ss.str();
}

const std::string & Xml::text()const{
	if(m_text==nullptr){
		return s_null_name;
	}else{
		return *m_text;
	}
}

void Xml::set_text(const std::string & text){
	m_text =std::make_shared<std::string>(text);
}

void Xml::append(const Xml & child){
	if(m_child==nullptr){
		m_child=std::make_shared<std::list<Xml>>();
	}
	m_child->push_back(child);
}
		
Xml & Xml::operator[](int index){
	if(index<0){
		throw std::invalid_argument("index less than zero");
	}
	if(m_child==nullptr){
		m_child=std::make_shared<std::list<Xml>>();
	}
	int size=m_child->size();
	if(index<size){
		auto it=m_child->begin();
		while(index-->0){
			it++;
		}
		return *it;
	}
	//index>=m_child.size()
	for(int i=size; i<=index; i++){
		m_child->push_back(Xml());
	}
	return m_child->back();
}

Xml & Xml::operator[](const std::string & name){
	if(m_child==nullptr){
		m_child=std::make_shared<std::list<Xml>>();
	}
	for(auto it=m_child->begin(); it!=m_child->end(); it++){
		if(it->name()==name)return *it;
	}
	m_child->push_back(Xml(name));
	return m_child->back();
}

size_t Xml::size() const{
	if(m_child==nullptr){
		return 0;
	}
	return m_child->size();
}

void Xml::remove(int index){
	if(m_child==nullptr||index<0||m_child->size()<=index){
		return;
	}
	auto it=m_child->begin();
	for(int i=0; i<index; i++)it++;
	m_child->erase(it);
}

void Xml::remove(const std::string & name){
	if(m_child==nullptr){
		return;
	}
	for(auto it=m_child->begin(); it!=m_child->end();){
		if(it->m_name==nullptr&&name==""){
			it=m_child->erase(it);
		}else if(it->m_name!=nullptr&&*it->m_name==name){
			it=m_child->erase(it);
		}else{
			it++;
		}
	}
}
}
}

#endif /* end of include guard: XML_HPP_LHZ2CFID */
