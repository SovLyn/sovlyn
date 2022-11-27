#pragma once
#ifndef CLASSFIELD_HPP_AKTA327J
#define CLASSFIELD_HPP_AKTA327J
#include <string>

namespace sovlyn{
namespace reflect{

class ClassField{
	public:
		ClassField():m_name(""),m_type(""),m_offset(0){}
		ClassField(const std::string & name, const std::string & type, size_t offset):
			m_name(name), m_type(type), m_offset(offset){}

		const std::string & name(){return m_name;}
		const std::string & type(){return m_type;}
		size_t offset(){return m_offset;}

	private:
		std::string m_name, m_type;
		size_t m_offset;
};


};
};

#endif /* end of include guard: CLASSFIELD_HPP_AKTA327J */
