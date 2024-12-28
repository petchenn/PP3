#include "Threads.h"

Threads::Threads() {
	count = std::thread::hardware_concurrency();
	//workThreads = true;
	for (int i = 0; i < count; i++) {
#ifdef _WIN32

		HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, [](void* param) -> unsigned {
			return static_cast<Threads*>(param)->work(param); }, this, 0, NULL);		
		if (thread == nullptr) {
			DWORD err = GetLastError();
			printf("Cannot create thread, GetLastError=%u", err);
		}
		else {
			threads.emplace_back(thread);
		}

#else

		pthread_t thread;
		int result = pthread_create(&thread, nullptr, [](void* param) -> unsigned {
			return static_cast<Threads*>(param)->work(param); }, this);
		if (result == 0) {
			threads.emplace_back(thread);
		}
		else {
			printf("Cannot create thread, errno=%d", errno);
		}

#endif 
	}
}

Threads::~Threads() {
	workThreads = true;
	cond_stop.notify_all();

#ifdef _WIN32
	WaitForMultipleObjects(count, threads.data(), TRUE, INFINITE); //ждём сигнал
#endif

	for (int i = 0; i < count; i++) {
#ifdef _WIN32
		CloseHandle(threads[i]);
#else
		pthread_join(threads[i], nullptr);
#endif
	}
}


#ifdef _WIN32
DWORD WINAPI Threads::work(LPVOID param){
#else 
void* threadsPool::work(void* param){
#endif

while (true) {
		std::unique_lock<std::mutex> lk(m);
		printf("block \n");
		cond_stop.wait(lk, [this]() {return !operations.empty() || workThreads; });
		std::function<void()> curop;

		printf("work ");
		if (!operations.empty()) {

			curop = std::bind(operations.front(), 100000);
			operations.pop();
			lk.unlock();
		}
		if (workThreads) break;

		curop();
	}
#ifdef _WIN32
	_endthreadex(0);
	return 0;
#else //__linux__
	return nullptr;
#endif
}

#ifdef _WIN32

void Threads::putFunc(std::function<void(unsigned int)> op) {

#else
void* threadsPool::putFunc(function<void(double)> f, double p1) {
#endif
	{
		std::lock_guard<std::mutex> guard(m);
		operations.push(op);
	}
	cond_stop.notify_one();
#if defined __linux__
	return nullptr;
#endif
}

