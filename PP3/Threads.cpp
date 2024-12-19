#include "Threads.h"

Threads::Threads() {
	count = std::thread::hardware_concurrency();
	//workThreads = true;
	for (int i = 0; i < count; i++) {
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) work, this, 0, NULL);
		if (thread == nullptr) {
			DWORD err = GetLastError();
			printf("Cannot create thread, GetLastError=%u", err);
		}
		else {
			threads.emplace_back(thread);
		}
	}
}

//Threads::~Threads() {
//	workThreads = true;
//	cond_stop.notify_all();
//
//	for (int i = 0; i < count; i++) {
//		if (threads[i].joinable())
//			threads[i].join();
//	}
//}
//
////void Threads::work(int j) {
////	while (true) {
////		std::unique_lock<std::mutex> lk(m);
////		cond_stop.wait(lk, [this]() {return !operations.empty() || workThreads; });
////		std::function<void()> curop;
////
////		//printf("%d  ", getThreadID());
////		if (!operations.empty()) {
////
////			curop = std::bind(operations.front(), getThreadID());
////			operations.pop();
////			lk.unlock();
////		}
////		if (workThreads) break;
////		curop();
////	}
////}
//
//void Threads::putFunc(std::function<void(unsigned int)> op) {
//	{
//		std::lock_guard<std::mutex> guard(m);
//		operations.push(op);
//	}
//	cond_stop.notify_one();
//}
//
//unsigned int Threads::getThreadID() {
//	std::thread::id threadID = std::this_thread::get_id();
//	unsigned int v = *static_cast<unsigned int*>(static_cast<void*>(&threadID));
//	return v;
//}
