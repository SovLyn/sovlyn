#pragma once
#ifndef CLASSREGISTER_HPP_SRTBCC1V
#define CLASSREGISTER_HPP_SRTBCC1V
#include "sovlyn/reflect/ClassFactory.hpp"
#include <string>
#include <functional>

namespace sovlyn{
namespace reflect{
class ClassRegister{
	public:
		ClassRegister(const std::string & className, create_object method){
			sovlyn::utility::Singleton<ClassFactory>::instance().register_class(className, method);
		}

		ClassRegister(const std::string & className, const std::string & fieldName, const std::string & fieldType, size_t offset){
			sovlyn::utility::Singleton<ClassFactory>::instance().register_class_field(className, fieldName, fieldType, offset);
		}

		ClassRegister(const std::string & className, const std::string & methodName, uintptr_t method){
			sovlyn::utility::Singleton<ClassFactory>::instance().register_class_method(className, methodName, method);
		}
};
};
};

#define REGISTER_CLASS(className)\
	sovlyn::reflect::Object * createObject##className()\
	{\
		sovlyn::reflect::Object * obj = new className();\
		obj->set_class_name(#className);\
		return obj;\
	}\
	sovlyn::reflect::ClassRegister classRegister##className(#className, createObject##className)

#define REGISTER_CLASS_FIELD(className, fieldName, fieldType)\
	className className##fieldName;\
	sovlyn::reflect::ClassRegister classRegister##className##fieldName(#className, #fieldName, #fieldType\
			, (size_t)(&(className##fieldName.fieldName))-(size_t)(&className##fieldName))

#define REGISTER_CLASS_METHOD(className, methodName)\
	std::function<void(className *)> className##methodName##method = &className::methodName;\
	sovlyn::reflect::ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName##method))

#endif /* end of include guard: CLASSREGISTER_HPP_SRTBCC1V */
