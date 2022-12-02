#pragma once
#ifndef DATASTREAM_HPP_7BLABYXO
#define DATASTREAM_HPP_7BLABYXO
#include <vector>
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include "Serializable.hpp"

namespace sovlyn{
namespace serialize{
enum DataType { 
	BOOL=0,
	CHAR,
	INT32,
	INT64,
	FLOAT,
	DOUBLE,
	STRING,
	VECTOR,
	LIST,
	MAP,
	SET,
	CUSTOM
};

class DataStream{
	private:
		std::vector<u_char> m_buf;
		int m_loc=0;

	public:
		DataStream();
		~DataStream();

		void write(const char * data, int len);

		void write(bool value);
		bool read(bool & value);
		void write(char value);
		bool read(char & value);
		void write(int32_t value);
		bool read(int32_t & value);
		void write(int64_t value);
		bool read(int64_t & value);
		void write(float value);
		bool read(float & value);
		void write(double value);
		bool read(double & value);
		void write(const std::string & value);
		bool read(std::string & value);
		void write(const char * value);
		bool read(char * value);

		template <typename T>
		void write(const std::vector<T> & vector);
		template <typename T>
		bool read(std::vector<T> & vector);

		template <typename T>
		void write(const std::list<T> & lis);
		template <typename T>
		bool read(std::list<T> & lis);

		template <typename T>
		void write(const std::set<T> & s);
		template <typename T>
		bool read(std::set<T> & s);

		template <typename K, typename V>
		void write(const std::map<K, V> & m);
		template <typename K, typename V>
		bool read(std::map<K, V> & m);

		void write(const Serialzable & value);
		bool read(Serialzable & value);

		template <typename T, typename ...Args>
		void write_args(const T & head, const Args&... args);
		void write_args(){}
		template <typename T, typename ...Args>
		bool read_args(T & head, Args&... args);
		bool read_args(){return true;}

		DataStream & operator <<(bool value);
		DataStream & operator >>(bool & value);
		DataStream & operator <<(char value);
		DataStream & operator >>(char & value);
		DataStream & operator <<(int32_t value);
		DataStream & operator >>(int32_t & value);
		DataStream & operator <<(int64_t value);
		DataStream & operator >>(int64_t & value);
		DataStream & operator <<(float value);
		DataStream & operator >>(float & value);
		DataStream & operator <<(double value);
		DataStream & operator >>(double & value);
		DataStream & operator <<(const std::string & value);
		DataStream & operator >>(std::string & value);
		DataStream & operator <<(const char * value);
		DataStream & operator >>(char * value);

		template <typename T>
		DataStream & operator <<(const std::vector<T> & vector);
		template <typename T>
		DataStream & operator >>(std::vector<T> & vector);

		template <typename T>
		DataStream & operator <<(const std::list<T> & lis);
		template <typename T>
		DataStream & operator >>(std::list<T> & lis);

		template <typename T>
		DataStream & operator <<(const std::set<T> & s);
		template <typename T>
		DataStream & operator >>(std::set<T> & s);

		template <typename K, typename V>
		DataStream & operator <<(const std::map<K, V> & m);
		template <typename K, typename V>
		DataStream & operator >>(std::map<K, V> & m);

		DataStream & operator <<(const Serialzable & value);
		DataStream & operator >>(Serialzable & value);

		void show();
};

DataStream::DataStream(){
	m_buf.reserve(10);
}

DataStream::~DataStream(){}

void DataStream::write(const char * data, int len){
	if(m_buf.size()+len>m_buf.capacity()){
		m_buf.reserve(1.5*m_buf.capacity());
	}
	int bg=m_buf.size();
	m_buf.resize(bg+len);
	std::memcpy(&m_buf[bg], data, len);
}

DataStream & DataStream::operator <<(bool value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(bool & value){
	read(value);
	return *this;
}

void DataStream::write(bool value){
	char type = DataType::BOOL;
	write(&type, sizeof(char));
	write((char *)&value, sizeof(char));
}

bool DataStream::read(bool & value){
	if(m_buf[m_loc]!=DataType::BOOL){
		return false;
	}
	++m_loc;
	value = m_buf[m_loc];
	++m_loc;
	return true;
}

DataStream & DataStream::operator <<(char value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(char & value){
	read(value);
	return *this;
}

void DataStream::write(char value){
	char type = DataType::CHAR;
	write(&type, sizeof(char));
	write(&value, sizeof(char));
}

bool DataStream::read(char & value){
	if(m_buf[m_loc]!=DataType::CHAR){
		return false;
	}
	++m_loc;
	value = m_buf[m_loc];
	++m_loc;
	return true;
}

DataStream & DataStream::operator <<(int32_t value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(int32_t & value){
	read(value);
	return *this;
}

void DataStream::write(int32_t value){
	char type = DataType::INT32;
	write(&type, sizeof(char));
	write((char *)&value, sizeof(int32_t));
}

bool DataStream::read(int32_t & value){
	if(m_buf[m_loc]!=DataType::INT32){
		return false;
	}
	++m_loc;
	value = *((int32_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int32_t);
	return true;
}

DataStream & DataStream::operator <<(int64_t value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(int64_t & value){
	read(value);
	return *this;
}

void DataStream::write(int64_t value){
	char type = DataType::INT64;
	write(&type, sizeof(char));
	write((char *)&value, sizeof(int64_t));
}

bool DataStream::read(int64_t & value){
	if(m_buf[m_loc]!=DataType::INT64){
		return false;
	}
	++m_loc;
	value = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	return true;
}

DataStream & DataStream::operator <<(float value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(float & value){
	read(value);
	return *this;
}

void DataStream::write(float value){
	char type = DataType::FLOAT;
	write(&type, sizeof(char));
	write((char *)&value, sizeof(float));
}

bool DataStream::read(float & value){
	if(m_buf[m_loc]!=DataType::FLOAT){
		return false;
	}
	++m_loc;
	value = *((float *)&m_buf[m_loc]);
	m_loc+=sizeof(float);
	return true;
}

DataStream & DataStream::operator <<(double value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(double & value){
	read(value);
	return *this;
}

void DataStream::write(double value){
	char type = DataType::DOUBLE;
	write(&type, sizeof(char));
	write((char *)&value, sizeof(double));
}

bool DataStream::read(double & value){
	if(m_buf[m_loc]!=DataType::DOUBLE){
		return false;
	}
	++m_loc;
	value = *((double *)&m_buf[m_loc]);
	m_loc+=sizeof(double);
	return true;
}

DataStream & DataStream::operator <<(const std::string & value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(std::string & value){
	read(value);
	return *this;
}

void DataStream::write(const std::string & value){
	char type = DataType::STRING;
	write(&type, sizeof(char));
	int64_t len=value.size();
	std::cout << len << std::endl;
	write(len);
	write(value.data(), len);
}

bool DataStream::read(std::string & value){
	if(m_buf[m_loc]!=DataType::STRING){
		return false;
	}
	m_loc+=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	char* buf=new char[l+1];
	int i=0;
	for (i = 0; i < l; ++i) {
		buf[i]=m_buf[m_loc+i];
	}
	buf[i]='\0';
	value=std::string(buf);
	m_loc+=l;
	delete[] buf;
	return true;
}

DataStream & DataStream::operator <<(const char * value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(char * value){
	read(value);
	return *this;
}

void DataStream::write(const char * value){
	char type = DataType::STRING;
	write(&type, sizeof(char));
	int64_t len=std::strlen(value);
	write(len);
	write(value, len);
}

bool DataStream::read(char * value){
	if(m_buf[m_loc]!=DataType::STRING){
		return false;
	}
	m_loc+=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	int i=0;
	for (i = 0; i < l; ++i) {
		value[i]=m_buf[m_loc+i];
	}
	value[i]='\0';
	m_loc+=l;
	return true;
}

template <typename T>
DataStream & DataStream::operator <<(const std::vector<T> & value){
	write(value);
	return *this;
}

template <typename T>
DataStream & DataStream::operator >>(std::vector<T> & value){
	read(value);
	return *this;
}

template <typename T>
void DataStream::write(const std::vector<T> & vec){
	char type=DataType::VECTOR;
	write(&type, sizeof(char));
	int64_t len=vec.size();
	write(len);
	for(const T & ele:vec){
		write(ele);
	}
}

template <typename T>
bool DataStream::read(std::vector<T> & vec){
	if(m_buf[m_loc]!=DataType::VECTOR){
		return false;
	}
	m_loc +=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	vec.clear();
	vec.reserve(l);
	vec.resize(l);
	for (int i = 0; i < l; ++i) {
		(*this)>>vec[i];
	}
	return true;
}

template <typename T>
DataStream & DataStream::operator <<(const std::list<T> & lis){
	write(lis);
	return *this;
}

template <typename T>
DataStream & DataStream::operator >>(std::list<T> & lis){
	read(lis);
	return *this;
}

template <typename T>
void DataStream::write(const std::list<T> & lis){
	char type=DataType::LIST;
	write(&type, sizeof(char));
	int64_t len=lis.size();
	write(len);
	for(const T & ele:lis){
		write(ele);
	}
}

template <typename T>
bool DataStream::read(std::list<T> & lis){
	if(m_buf[m_loc]!=DataType::LIST){
		return false;
	}
	m_loc +=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	lis.clear();
	for (int i = 0; i < l; ++i) {
		T temp;
		read(temp);
		lis.push_back(temp);
	}
	return true;
}

template <typename T>
DataStream & DataStream::operator <<(const std::set<T> & s){
	write(s);
	return *this;
}

template <typename T>
DataStream & DataStream::operator >>(std::set<T> & s){
	read(s);
	return *this;
}

template <typename T>
void DataStream::write(const std::set<T> & s){
	char type=DataType::SET;
	write(&type, sizeof(char));
	int64_t len=s.size();
	write(len);
	for(const T & ele:s){
		write(ele);
	}
}

template <typename T>
bool DataStream::read(std::set<T> & s){
	if(m_buf[m_loc]!=DataType::SET){
		return false;
	}
	m_loc +=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	s.clear();
	for (int i = 0; i < l; ++i) {
		T temp;
		read(temp);
		s.insert(temp);
	}
	return true;
}

template <typename K, typename V>
DataStream & DataStream::operator <<(const std::map<K, V> & m){
	write(m);
	return *this;
}

template <typename K, typename V>
DataStream & DataStream::operator >>(std::map<K, V> & m){
	read(m);
	return *this;
}

template <typename K, typename V>
void DataStream::write(const std::map<K, V> & m){
	char type=DataType::MAP;
	write(&type, sizeof(char));
	int64_t len=m.size();
	write(len);
	for(auto & ele:m){
		write(ele.first);
		write(ele.second);
	}
}

template <typename K, typename V>
bool DataStream::read(std::map<K, V> & m){
	if(m_buf[m_loc]!=DataType::MAP){
		return false;
	}
	m_loc +=2;
	int64_t l = *((int64_t *)&m_buf[m_loc]);
	m_loc+=sizeof(int64_t);
	m.clear();
	for (int i = 0; i < l; ++i) {
		K key;
		V value;
		read(key);
		read(value);
		m[key]=value;
	}
	return true;
}


DataStream & DataStream::operator <<(const Serialzable & value){
	write(value);
	return *this;
}

DataStream & DataStream::operator >>(Serialzable & value){
	read(value);
	return *this;
}

void DataStream::write(const Serialzable & value){
	value.serialize(*this);
}

bool DataStream::read(Serialzable & value){
	if(m_buf[m_loc]!=DataType::CUSTOM){
		return false;
	}
	m_loc++;
	value.unserialize(*this);
	return true;
}

template <typename T, typename ...Args>
void DataStream::write_args(const T & head, const Args&... args)
{
    write(head);
    write_args(args...);
}

template <typename T, typename ...Args>
bool DataStream::read_args(T & head, Args&... args)
{
    read(head);
    return read_args(args...);
}

void DataStream::show(){
	std::cout<<"length: " << m_buf.size() << std::endl;
	int i=0;
	while(i<m_buf.size()){
		switch ((DataType)m_buf[i]) {
			case DataType::BOOL:{
				std::cout << "BOOL: ";
				i++;
				bool * b=(bool *)&m_buf[i];
				std::cout << *b << std::endl;
				i++;
				break;
			}
				
			case DataType::CHAR:{
				std::cout << "CHAR: ";
				i++;
				char * c=(char *)&m_buf[i];
				std::cout << *c << std::endl;
				i++;
				break;
			}

			case DataType::INT32:{
				std::cout << "INT32: ";
				i++;
				int32_t * i32=(int32_t *)&m_buf[i];
				std::cout << *i32 << std::endl;
				i+=sizeof(int32_t);
				break;
			}

			case DataType::INT64:{
				std::cout << "INT64: ";
				i++;
				int64_t * i64=(int64_t *)&m_buf[i];
				std::cout << *i64 << std::endl;
				i+=sizeof(int64_t);
				break;
			}

			case DataType::FLOAT:{
				std::cout << "FLOAT: ";
				i++;
				float * f=(float *)&m_buf[i];
				std::cout << *f << std::endl;
				i+=sizeof(float);
				break;
			}

			case DataType::DOUBLE:{
				std::cout << "DOUBLE: ";
				i++;
				double * d=(double *)&m_buf[i];
				std::cout << *d << std::endl;
				i+=sizeof(double);
				break;
			}

			case DataType::STRING:{
				std::cout << "STRING: ";
				i+=2;
				int64_t * l=(int64_t *)&m_buf[i];
				std::cout<<"length: " << *l<<" ";
				i+=sizeof(int64_t);
				for (int j = 0; j < *l; ++j) {
					std::cout<<m_buf[i+j];
				}
				i+=*l;
				std::cout<<std::endl;
				break;
			}

			case DataType::VECTOR:{
				std::cout<<"VECTOR:";
				i+=2;
				int64_t * l=(int64_t *)&m_buf[i];
				std::cout <<"length: "<<*l<<":"<<std::endl;
				i+=sizeof(int64_t);
				break;
			}

			case DataType::LIST:{
				std::cout<<"LIST:";
				i+=2;
				int64_t * l=(int64_t *)&m_buf[i];
				std::cout <<"length: "<<*l<<":"<<std::endl;
				i+=sizeof(int64_t);
				break;
			}

			case DataType::SET:{
				std::cout<<"SET:";
				i+=2;
				int64_t * l=(int64_t *)&m_buf[i];
				std::cout <<"length: "<<*l<<":"<<std::endl;
				i+=sizeof(int64_t);
				break;
			}

			case DataType::MAP:{
				std::cout<<"MAP:";
				i+=2;
				int64_t * l=(int64_t *)&m_buf[i];
				std::cout <<"length: "<<*l<<":"<<std::endl;
				i+=sizeof(int64_t);
				break;
			}

			default:
				std::cout << "CUSTOM:" << std::endl;
				i++;
			
		}
	}
}
}
}

#endif /* end of include guard: DATASTREAM_HPP_7BLABYXO */
