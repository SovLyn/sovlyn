#pragma once
#ifndef THREADPOOL_HPP_VNXMWW91
#define THREADPOOL_HPP_VNXMWW91

#include <set>
#include <queue>
#include <thread>
#include <mutex>
#include <exception>
#include "Task.hpp"
#include <memory>
#include <condition_variable>
#include <iostream>

namespace sovlyn{
namespace threadpool{
class ThreadPool{
	public:
		ThreadPool();
		~ThreadPool();

		void init(int num);
		void assign(std::shared_ptr<Task> t);
		bool busy();
		void terminate();
		int threads();
	private:
		bool m_inited;
		bool m_terminated;
		std::mutex m_mutex;
		std::condition_variable m_cond;
		std::vector<std::thread> m_threads;
		
		std::queue<std::shared_ptr<Task>> m_tasks;

		void ThreadLoop();
};

ThreadPool::ThreadPool():m_inited(false), m_terminated(false){}
ThreadPool::~ThreadPool(){terminate();}

void ThreadPool::init(int num){
	std::unique_lock<std::mutex> lock(m_mutex);
	if(m_inited){
		throw std::logic_error("thread initialized mutiple times");
	}
	if(num<=0)num=std::thread::hardware_concurrency();
	m_threads.resize(num);
	for (int i = 0; i < num; ++i) {
		m_threads.at(i)=std::thread(&ThreadPool::ThreadLoop, this);
	}
	m_inited=true;
}

void ThreadPool::ThreadLoop(){
	while(true){
		std::shared_ptr<Task> t=nullptr;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cond.wait(lock, [this]{return !this->m_tasks.empty()||this->m_terminated;});
			if(m_terminated)return;
			t=m_tasks.front();
			m_tasks.pop();
			
		}
		if(t!=nullptr)t->run();
	}

}

void ThreadPool::assign(std::shared_ptr<Task> t){
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_tasks.push(t);
	}
	m_cond.notify_one();
}

bool ThreadPool::busy(){
	bool to_return;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		to_return=m_tasks.empty();
	}
	return !to_return;
}

void ThreadPool::terminate(){
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_terminated=true;
	}
	for(std::thread & t:m_threads){
		t.join();
	}
	m_threads.clear();
}

int ThreadPool::threads(){
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_threads.size();
}

}
}

#endif /* end of include guard: THREADPOOL_HPP_VNXMWW91 */
