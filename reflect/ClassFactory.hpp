#pragma once
#ifndef CLASSFACTORY_HPP_JSTNDKEP
#define CLASSFACTORY_HPP_JSTNDKEP
#include <map>
#include <vector>
#include <functional>
#include "sovlyn/utility/Singleton.hpp"
#include "ClassField.hpp"
#include "ClassMethod.hpp"

namespace sovlyn{
namespace reflect{
class Object{
	public:
		Object(){}
		virtual ~Object(){}

		void set_class_name(const std::string & name){m_class_name=name;}
		const std::string & get_class_name(){return m_class_name;}

		int get_field_count();
		ClassField * get_field(int pos);
		ClassField * get_field(const std::string & fieldName);

		int get_method_count();
		ClassMethod * get_method(int pos);
		ClassMethod * get_method(const std::string & MethodName);

		template<typename T>
		void get(const std::string & fieldName, T & value);
		
		template<typename T>
		void set(const std::string & fieldName, const T & value);

		void call(const std::string & methodName);

	private:
		std::string m_class_name;
};

typedef Object *(*create_object)(void);

class ClassFactory{
	friend class sovlyn::utility::Singleton<ClassFactory>;
	private:
		ClassFactory(){}
		~ClassFactory(){}

	private:
		std::map<std::string, create_object> m_classMap;
		std::map<std::string, std::vector<ClassField *>> m_classFields;
		std::map<std::string, std::vector<ClassMethod *>> m_classMethods;

	public:
		void register_class(const std::string & className, create_object method);
		Object * create_class(const std::string & className);

		void register_class_field(const std::string & className, const std::string & fieldName, const std::string & fieldType, size_t offset);
		int get_field_count(const std::string & className);
		ClassField * get_field(const std::string & className, int pos);
		ClassField * get_field(const std::string & className, const std::string & fieldName);

		void register_class_method(const std::string & className, const std::string & methodName, uintptr_t method);
		int get_method_count(const std::string & className);
		ClassMethod * get_method(const std::string & className, int pos);
		ClassMethod * get_method(const std::string & className, const std::string & methodName);
};

void ClassFactory::register_class(const std::string & className, create_object method){
	m_classMap[className] = method;
}

Object * ClassFactory::create_class(const std::string & className){
	auto it = m_classMap.find(className);
	if (it == m_classMap.end()) {
		return nullptr;
	}
	return it->second();
}

void ClassFactory::register_class_field(const std::string & className, const std::string & fieldName, const std::string & fieldType, size_t offset){
	m_classFields[className].push_back(new ClassField(fieldName, fieldType, offset));
}

int ClassFactory::get_field_count(const std::string & className){
	return m_classFields[className].size();
}

ClassField * ClassFactory::get_field(const std::string & className, int pos){
	if(pos<0 || pos>m_classFields[className].size()){
		return nullptr;
	}
	return m_classFields[className][pos];
}

ClassField * ClassFactory::get_field(const std::string & className, const std::string & fieldName){
	for( ClassField * it:m_classFields[className] ){
		if(it->name()==fieldName){
			return it;
		}
	}
	return nullptr;
}

int Object::get_field_count(){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_field_count(this->m_class_name);
}

ClassField * Object::get_field(int pos){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_field(this->m_class_name, pos);
}

ClassField * Object::get_field(const std::string & fieldName){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_field(this->m_class_name, fieldName);
}

int Object::get_method_count(){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_method_count(this->m_class_name);
}

ClassMethod * Object::get_method(int pos){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_method(this->m_class_name, pos);
}

ClassMethod * Object::get_method(const std::string & methodName){
	return sovlyn::utility::Singleton<ClassFactory>::instance().get_method(this->m_class_name, methodName);
}

void ClassFactory::register_class_method(const std::string & className, const std::string & methodName, uintptr_t method){
	m_classMethods[className].push_back(new ClassMethod(methodName, method));
}

int ClassFactory::get_method_count(const std::string & className){
	return m_classMethods[className].size();
}

ClassMethod * ClassFactory::get_method(const std::string & className, int pos){
	if(pos<0||pos>=m_classMethods[className].size()){
		return nullptr;
	}
	return m_classMethods[className][pos];
}

ClassMethod * ClassFactory::get_method(const std::string & className, const std::string & methodName){
	for(ClassMethod * it:m_classMethods[className]){
		if(it->name()==methodName){
			return it;
		}
	}
	return nullptr;
}

template<typename T>
void Object::get(const std::string & fieldName, T & value){
	ClassField * field = sovlyn::utility::Singleton<ClassFactory>::instance().get_field(m_class_name, fieldName);
	value = *(T *)((unsigned char *)(this)+field->offset());
}

template<typename T>
void Object::set(const std::string & fieldName, const T & value){
	ClassField * field = sovlyn::utility::Singleton<ClassFactory>::instance().get_field(m_class_name, fieldName);
	*(T *)((unsigned char *)(this)+field->offset()) = value;
}

void Object::call(const std::string & methodName){
	ClassMethod * cm = sovlyn::utility::Singleton<ClassFactory>::instance().get_method(m_class_name, methodName);
	uintptr_t func=cm->method();
	typedef std::function<void(decltype(this))> class_method;
	(*((class_method *)func))(this);
}

}
}

#endif /* end of include guard: CLASSFACTORY_HPP_JSTNDKEP */
