#pragma once
#ifndef JSON_HPP_GLFB9KJ8
#define JSON_HPP_GLFB9KJ8

#include <sstream>
#include <ostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <memory>
#include "../utility/Value.hpp"
#include <map>
#include <vector>

namespace sovlyn{
namespace json{

using sovlyn::utility::Value;

class jsonValue{
	public:
		virtual ~jsonValue(){}
		virtual std::string str() const =0;

		enum Type{VALUE, LIST, JSON};

		jsonValue::Type type()const{return m_type;}

	protected:
		jsonValue::Type m_type;

	friend std::ostream & operator<<(std::ostream & os, const jsonValue & j);
};

std::ostream & operator<<(std::ostream & os, const jsonValue & j){
	os<<j.str();
	return os;
}

class jsonSimpleValue:public jsonValue{
	public:
		jsonSimpleValue(std::shared_ptr<Value>);
		virtual ~jsonSimpleValue(){}
		std::string str() const;

	private:
		jsonSimpleValue();
		std::shared_ptr<Value> m_value;
};

jsonSimpleValue::jsonSimpleValue(std::shared_ptr<Value> v):
m_value(v){
	m_type=jsonValue::VALUE;
}

std::string jsonSimpleValue::str()const{
	return m_value->str();
}

class json:public jsonValue{
	public:
		json();
		virtual ~json(){}
		std::string str() const;
		void insert(const std::string & key, std::shared_ptr<jsonValue> & value);
		std::shared_ptr<jsonValue> & operator[](const std::string & key);
		bool hasKey(const std::string & key)const;

		typedef std::map<std::string, std::shared_ptr<jsonValue>>::iterator iterator;
		iterator begin(){return m_map.begin();}
		iterator end(){return m_map.end();}
		int size()const{return m_map.size();}

	private:
		std::map<std::string, std::shared_ptr<jsonValue>> m_map;
};

json::json(){
	m_type=jsonValue::JSON;
}

std::string json::str()const{
	if(m_map.empty())return "{}";
	std::stringstream ss;
	ss<<'{';
	auto p=m_map.begin();
	ss<<'"'<<p->first<<'"'<<"->"<<*p->second;
	p++;
	for(;p!=m_map.end(); p++){
		ss<<','<<'"'<<p->first<<'"'<<"->"<<*p->second;
	}
	ss<<'}';
	return ss.str();
}

void json::insert(const std::string & key, std::shared_ptr<jsonValue> & value){
	m_map[key]=value;
}
	
bool json::hasKey(const std::string & key)const{
	auto it=m_map.find(key);
	return it!=m_map.end();
}

std::shared_ptr<jsonValue> & json::operator[](const std::string & key){
	auto it=m_map.find(key);
	if(it!=m_map.end())return it->second;
	throw std::logic_error("not such key:\""+key+'"');
}

class jsonList:public jsonValue{
	public:
		jsonList();
		virtual ~jsonList(){}
		std::string str() const;

		std::shared_ptr<jsonValue> & operator[](int i);
		typedef std::vector<std::shared_ptr<jsonValue>>::iterator iterator;
		iterator begin(){return m_vec.begin();}
		iterator end(){return m_vec.end();}
		int size()const{return m_vec.size();}
		void add(std::shared_ptr<jsonValue>&);
	
	private:
		std::vector<std::shared_ptr<jsonValue>> m_vec;
};

jsonList::jsonList(){
	m_type=jsonValue::LIST;
}

std::string jsonList::str()const{
	if(m_vec.empty())return "[]";
	std::stringstream ss;
	auto p=m_vec.begin();
	ss<<'[';
	ss<<p->get()->str();
	p++;
	for(;p!=m_vec.end();p++){
		ss<<','<<p->get()->str();
	}
	ss<<']';
	return ss.str();
}

std::shared_ptr<jsonValue> & jsonList::operator[](int i){
	if(i<m_vec.size())return m_vec[i];
	throw std::logic_error(std::to_string(i)+" out of range");
}

void jsonList::add(std::shared_ptr<jsonValue> &val){
	m_vec.push_back(val);
}

}
}

#endif /* end of include guard: JSON_HPP_GLFB9KJ8 */
