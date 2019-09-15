#include <Windows.h>
#include <iostream>
#include <mutex>
#include <list>

template<typename T , int N>
class lockfreePool {
public:
	lockfreePool() {
		int count = 0;
	}

	bool push_try(T* data) {
		return false;
	}

	void push_try_while(T* data) {
		mutLock.lock();

		dataList.push_back(data);
		count = count + 1;

		mutLock.unlock();
	}

	bool pop_try(T** rData) {
		return false;
	}

	bool pop_try_while(T** rData) {
		if (count > 0) {
			mutLock.lock();
			if (count > 0) {
				*rData = *dataList.begin();
				dataList.pop_front();
				count = count - 1;
				mutLock.unlock();
				return true;
			}
			mutLock.unlock();
			return false;
		}
		return false;
	}

private:

	int check_size(int num) {
		if (num >= size) {
			num = num % size;
		}
		return num;
	}

	std::mutex mutLock;

	const int size = 30;
	std::list<T*> dataList;

	int count = 0;
};
