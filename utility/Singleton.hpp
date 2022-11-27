#pragma once
#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

namespace sovlyn{
namespace utility{
template <typename T>
class Singleton{
	private:
		Singleton();
		Singleton(const Singleton &);
		~Singleton(){};
		Singleton<T> operator = (const Singleton<T> &);

	private:
		static inline T * m_instance=nullptr;

	public:
		static T & instance(){
			static T t;
			return t;
		}

};
};
};
#endif /* end of include guard: SINGLETON_HPP_ */
