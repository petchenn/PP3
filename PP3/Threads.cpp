#include "Threads.h"

Threads::Threads() {
	count = std::thread::hardware_concurrency();
	//workThreads = true;
	for (int i = 0; i < count; i++) {
		HANDLE thread = (HANDLE)_beginthreadex(NULL, 0, [](void* param) -> unsigned {
			return static_cast<Threads*>(param)->work(param); }, this, 0, NULL);		if (thread == nullptr) {
			DWORD err = GetLastError();
			printf("Cannot create thread, GetLastError=%u", err);
		}
		else {
			threads.emplace_back(thread);
		}
	}
}

Threads::~Threads() {
	workThreads = true;
	cond_stop.notify_all();
	WaitForMultipleObjects(count, threads.data(), TRUE, INFINITE);

	for (int i = 0; i < count; i++) {
		CloseHandle(threads[i]);
	}
}

DWORD WINAPI Threads::work(LPVOID param){
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
	_endthreadex(0);
	return 0;
}

void Threads::putFunc(std::function<void(unsigned int)> op) {
	{
		std::lock_guard<std::mutex> guard(m);
		operations.push(op);
	}
	cond_stop.notify_one();
}

