#pragma once
#include <future>
#include <vector>
#include <iostream>
#include <functional>
#include <queue>
#include <mutex>
#include <stdio.h>
#if defined _WIN32
#include <Windows.h>
#elif defined __linux__
#include <pthread.h>
#include <errno.h>
#endif

class Threads
{
public:

	Threads();
	~Threads();
	
	int getThreadCount() {
		return count;
	}

private:
	mutable std::mutex m;
	int count;
#ifdef _WIN32
	std::vector<HANDLE> threads;
	DWORD WINAPI work(LPVOID param);
	void putFunc(std::function<void(unsigned int)>);
#else
	vector<pthread_t> threads;
	void* work(void* param);
	void* putFunc(function<void(double)> f, double p1);
#endif
	std::queue<std::function<void(unsigned int)>> operations;
	bool workThreads = false;
	//unsigned int getThreadID();
	std::condition_variable_any cond_stop;
};
