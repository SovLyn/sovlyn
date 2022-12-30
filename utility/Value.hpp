#pragma once
#include <stdexcept>
#ifndef VALUE_HPP_SCSIH4CD
#define VALUE_HPP_SCSIH4CD
#include <string>
#include <iostream>

namespace sovlyn{
namespace utility{

class Value{
	private:
		std::string m_val;

	public:
		enum Type {
			NULLTYPE=0,
			BOOL,
			INT,
			DOUBLE,
			STRING,
			OBJECT
		};

	private:
		Value::Type m_type=NULLTYPE;
	
	public:
		Value();
		virtual ~Value();
		Value(const Value & other);
		Value & operator=(const Value & other);

		Value(bool val);
		Value(long val);
		Value(int val);
		Value(double val);
		Value(float val);
		Value(const char * val, bool isObject=false);
		Value(const std::string & val, bool isObject=false);

		Value & operator=(bool val);
		Value & operator=(long val);
		Value & operator=(int val);
		Value & operator=(double val);
		Value & operator=(float val);
		Value & operator=(const char * val);
		Value & operator=(const std::string & val);

		bool operator==(const Value & other);
		bool operator!=(const Value & other);

		operator bool();
		operator long();
		operator double();
		operator std::string();

		Value::Type type(){return m_type;}

		std::string str()const;

		void asObject(bool isObject=true);
		
		friend std::ostream & operator<<(std::ostream &os, const Value& p);
};

Value::Value(){

}

Value::~Value(){

}

Value::Value(const Value & other){
	if(this!=&other){
		m_val=other.m_val;
		m_type=other.m_type;
	}
}

Value & Value::operator=(const Value & other){
	if(this!=&other){
		m_val=other.m_val;
		m_type=other.m_type;
	}
	return *this;
}

Value::Value(bool val){
	m_type=BOOL;
	m_val.resize(sizeof(bool)+1, '\0');
	*(bool *)m_val.c_str()=val;
}

Value::Value(long val){
	m_type=INT;
	m_val.resize(sizeof(long)+1, '\0');
	*(long *)m_val.c_str()=val;
}

Value::Value(int val){
	m_type=INT;
	m_val.resize(sizeof(long)+1, '\0');
	*(long *)m_val.c_str()=val;
}

Value::Value(double val){
	m_type=DOUBLE;
	m_val.resize(sizeof(double)+1, '\0');
	*(double *)m_val.c_str()=val;
}

Value::Value(float val){
	m_type=DOUBLE;
	m_val.resize(sizeof(double)+1, '\0');
	*(double *)m_val.c_str()=val;
}

Value::Value(const char * val, bool isObject){
	m_type=isObject?Value::OBJECT:Value::STRING;
	m_val=val;
}

Value::Value(const std::string & val, bool isObject){
	m_type=isObject?Value::OBJECT:Value::STRING;
	m_val=val;
}

Value & Value::operator=(bool val){
	m_val.clear();
	m_type=BOOL;
	m_val.resize(sizeof(bool)+1, '\0');
	*(bool *)m_val.c_str()=val;
	return *this;
}

Value & Value::operator=(long val){
	m_val.clear();
	m_type=INT;
	m_val.resize(sizeof(long)+1, '\0');
	*(long *)m_val.c_str()=val;
	return *this;
}

Value & Value::operator=(int val){
	return (*this)=long(val);
}

Value & Value::operator=(double val){
	m_val.clear();
	m_type=DOUBLE;
	m_val.resize(sizeof(double)+1, '\0');
	*(double *)m_val.c_str()=val;
	return *this;
}

Value & Value::operator=(float val){
	return (*this)=double(val);
}

Value & Value::operator=(const char * val){
	m_type=STRING;
	m_val=val;
	return *this;
}

Value & Value::operator=(const std::string & val){
	m_type=STRING;
	m_val=val;
	return *this;
}

bool Value::operator==(const Value & other){
	return m_type==other.m_type&&m_val==other.m_val;
}

bool Value::operator!=(const Value & other){
	return !(*this==other);
}


Value::operator bool(){
	if(m_type!=BOOL)return false;
	return *m_val.c_str();
}

Value::operator long(){
	if(m_type!=INT)return 0;
	return *m_val.c_str();
}

Value::operator double(){
	if(m_type!=DOUBLE)return 0;
	return *m_val.c_str();
}

Value::operator std::string(){
	if(m_type!=STRING)return "";
	return m_val;
}

std::string Value::str()const{
	switch (m_type) {
		case Value::NULLTYPE:{
			return "null";
		}
		case Value::BOOL:{
			return (*((bool *)m_val.c_str())?"true":"false");
		}
		case Value::INT:{
			return std::to_string(*((long *)m_val.c_str()));
		}
		case Value::DOUBLE:{
			return std::to_string(*((double *)m_val.c_str()));
		}
		case Value::STRING:{
			return '\"'+m_val+'\"';
		}
		case Value::OBJECT:{
			return m_val;
		}
	}
}

void Value::asObject(bool isObject){
	if(isObject){
		if(m_type==Value::STRING){
			m_type=Value::OBJECT;
		}else{
			throw std::logic_error("cannot change a non string Value to object");
		}
	}else{
		if(m_type==Value::OBJECT){
			m_type=Value::STRING;
		}
	}
}

std::ostream & operator<<(std::ostream &os, const Value& v){
	switch (v.m_type) {
		case Value::NULLTYPE:{
			os<<"null";
			break;
		}
		case Value::BOOL:{
			os<<(*((bool *)v.m_val.c_str())?"true":"false");
			break;
		}
		case Value::INT:{
			os<<*((long *)v.m_val.c_str());
			break;
		}
		case Value::DOUBLE:{
			os<<*((double *)v.m_val.c_str());
			break;
		}
		case Value::STRING:{
			os<<'\"'<<v.m_val<<'\"';
			break;
		}
		case Value::OBJECT:{
			os<<v.m_val;
			break;
		}
	}
	return os;
}
}
}
#endif /* end of include guard: VALUE_HPP_SCSIH4CD */
