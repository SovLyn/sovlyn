#pragma once
#ifndef TASK_HPP_OGXLDN7Y
#define TASK_HPP_OGXLDN7Y
#include <mutex>

namespace sovlyn{
namespace threadpool{
class Task{
public:
	Task();
	Task(void* data);
	virtual ~Task();

	void * get_data();
	void set_data(void * data);

	virtual void run()=0;

protected:
	void * m_data;
	std::mutex m_mutex;
};

Task::Task(){}

Task::Task(void* data):m_data(data){}

Task::~Task(){}

void *Task::get_data(){
	std::unique_lock<std::mutex> lock(m_mutex);
	return m_data;
}

void Task::set_data(void *data){
	std::unique_lock<std::mutex> lock(m_mutex);
	m_data=data;
}

}
}

#endif /* end of include guard: TASK_HPP_OGXLDN7Y */
