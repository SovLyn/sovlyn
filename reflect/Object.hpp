#pragma once
#ifndef OBJECT_HPP_DZOQD4QB
#define OBJECT_HPP_DZOQD4QB
#include <string>
namespace sovlyn{
namespace reflect{
class Object{
	public:
		Object(){}
		virtual ~Object(){}

		virtual void set_class_name(const std::string & name){m_class_name=name;}
		virtual const std::string & get_class_name(){return m_class_name;}
	private:
		std::string m_class_name;
};
};
};
#endif /* end of include guard: OBJECT_HPP_DZOQD4QB */
