#pragma once
#ifndef THREADPOOL_HPP_VNXMWW91
#define THREADPOOL_HPP_VNXMWW91

#include <set>
#include <thread>

namespace sovlyn{
namespace utility{
class ThreadPool{
	public:
		ThreadPool();
		~ThreadPool();

	private:
		std::set<std::thread *> m_idle;
		std::set<std::thread *> m_busy;

};
}
}

#endif /* end of include guard: THREADPOOL_HPP_VNXMWW91 */
