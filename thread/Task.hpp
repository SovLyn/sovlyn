#pragma once
#ifndef TASK_HPP_OGXLDN7Y
#define TASK_HPP_OGXLDN7Y
#include <mutex>

namespace sovlyn{
namespace utility{
class Task{
public:
	Task();
	Task(void* data);
	virtual ~Task();

	void * get_data();
	void set_data(void * data);

	virtual void run()=0;
	virtual void destory()=0;

protected:
	void * m_data;
	std::mutex m_mutex;
};

Task::Task(){}

}
}

#endif /* end of include guard: TASK_HPP_OGXLDN7Y */
