#pragma once
#ifndef DATASTREAM_HPP_7BLABYXO
#define DATASTREAM_HPP_7BLABYXO
#include <vector>
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <string>
#include <algorithm>
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

enum ByteOrder{
	BIGENDIAN=0,
	SMALLENDIAN,
	UNDECIDED
};

class DataStream{
	private:
		std::vector<u_char> m_buf;
		int m_loc=0;
		static ByteOrder s_endian;

	public:
		DataStream();
		explicit DataStream(const std::string & filename);
		~DataStream();

		static ByteOrder machinebyteorder();

		size_t size(){return m_buf.size();}
		size_t capacity(){return m_buf.capacity();}

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

		template <typename T>
		DataStream & operator<<(T value){write(value);return *this;}
		template <typename T>
		DataStream & operator>>(T & value){read(value);return *this;}

		int write_file(const std::string& filename);
};

ByteOrder DataStream::s_endian=UNDECIDED;

ByteOrder DataStream::machinebyteorder(){
	if(s_endian==UNDECIDED){
		int64_t n=1;
		if(((char *)&n)[0]==0)s_endian=BIGENDIAN;
		else s_endian=SMALLENDIAN;
	}
	return DataStream::s_endian;
}

DataStream::DataStream(){
	m_buf.reserve(10);
	if(s_endian==UNDECIDED){
		int64_t n=1;
		if(((char *)&n)[0]==0)s_endian=BIGENDIAN;
		else s_endian=SMALLENDIAN;
	}
	m_loc=0;
}

DataStream::DataStream(const std::string & filename){
	m_buf.reserve(10);
	if(s_endian==UNDECIDED){
		int64_t n=1;
		if(((char *)&n)[0]==0)s_endian=BIGENDIAN;
		else s_endian=SMALLENDIAN;
	}
	std::ifstream file;
	file.open(filename, std::fstream::in|std::fstream::binary);
	file.seekg(0, std::ios::end);
	size_t l=file.tellg();
	std::cout << "file size:"<<l << std::endl;
	m_buf.resize(l);
	file.seekg(0, std::ios::beg);
	file.read((char *)&(m_buf[0]), l);
	m_loc=0;
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

void DataStream::write(int32_t value){
	char type = DataType::INT32;
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(int32_t));
	}
	write(&type, sizeof(char));
	write((char *)&value, sizeof(int32_t));
}

bool DataStream::read(int32_t & value){
	if(m_buf[m_loc]!=DataType::INT32){
		return false;
	}
	++m_loc;
	value = *((int32_t *)&m_buf[m_loc]);
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(int32_t));
	}
	m_loc+=sizeof(int32_t);
	return true;
}

void DataStream::write(int64_t value){
	char type = DataType::INT64;
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(int64_t));
	}
	write(&type, sizeof(char));
	write((char *)&value, sizeof(int64_t));
}

bool DataStream::read(int64_t & value){
	if(m_buf[m_loc]!=DataType::INT64){
		return false;
	}
	++m_loc;
	value = *((int64_t *)&m_buf[m_loc]);
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(int64_t));
	}
	m_loc+=sizeof(int64_t);
	return true;
}

void DataStream::write(float value){
	char type = DataType::FLOAT;
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(float));
	}
	write(&type, sizeof(char));
	write((char *)&value, sizeof(float));
}

bool DataStream::read(float & value){
	if(m_buf[m_loc]!=DataType::FLOAT){
		return false;
	}
	++m_loc;
	value = *((float *)&m_buf[m_loc]);
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(float));
	}
	m_loc+=sizeof(float);
	return true;
}

void DataStream::write(double value){
	char type = DataType::DOUBLE;
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(double));
	}
	write(&type, sizeof(char));
	write((char *)&value, sizeof(double));
}

bool DataStream::read(double & value){
	if(m_buf[m_loc]!=DataType::DOUBLE){
		return false;
	}
	++m_loc;
	value = *((double *)&m_buf[m_loc]);
	if(s_endian==SMALLENDIAN){
		std::reverse((char *)&value, (char *)&value+sizeof(double));
	}
	m_loc+=sizeof(double);
	return true;
}

void DataStream::write(const std::string & value){
	char type = DataType::STRING;
	write(&type, sizeof(char));
	int64_t len=value.size();
	write(len);
	write(value.data(), len);
}

bool DataStream::read(std::string & value){
	if(m_buf[m_loc]!=DataType::STRING){
		return false;
	}
	m_loc+=1;
	int64_t l;
	read(l);
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
	m_loc+=1;
	int64_t l;
	read(l);
	int i=0;
	for (i = 0; i < l; ++i) {
		value[i]=m_buf[m_loc+i];
	}
	value[i]='\0';
	m_loc+=l;
	return true;
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
	m_loc +=1;
	int64_t l;
	read(l);
	vec.clear();
	vec.reserve(l);
	vec.resize(l);
	for (int i = 0; i < l; ++i) {
		(*this)>>vec[i];
	}
	return true;
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
	m_loc +=1;
	int64_t l;
	read(l);
	lis.clear();
	for (int i = 0; i < l; ++i) {
		T temp;
		read(temp);
		lis.push_back(temp);
	}
	return true;
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
	m_loc +=1;
	int64_t l;
	read(l);
	s.clear();
	for (int i = 0; i < l; ++i) {
		T temp;
		read(temp);
		s.insert(temp);
	}
	return true;
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
	m_loc +=1;
	int64_t l;
	read(l);
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
void DataStream::write_args(const T & head, const Args&... args){
    write(head);
    write_args(args...);
}

template <typename T, typename ...Args>
bool DataStream::read_args(T & head, Args&... args){
    read(head);
    return read_args(args...);
}

int DataStream::write_file(const std::string & filename){
	std::ofstream file;
	file.open(filename, std::fstream::trunc|std::fstream::out|std::fstream::binary);
	file.write((char *)(&m_buf[0]), m_buf.size());
	return 0;
}
}
}

#endif /* end of include guard: DATASTREAM_HPP_7BLABYXO */
