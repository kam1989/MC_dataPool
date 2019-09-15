
#include <boost\lockfree\queue.hpp>

template<typename T, int count>
class lockfreePool {
public:
	lockfreePool() {	
	}

	bool push_try(T* data) {
		return dataQueue.push(data);
	}

	void push_try_while(T* data) {

		while (!dataQueue.push(data)) {
		
		}

		return;

	}

	bool pop_try(T** rData) {
		return dataQueue.pop(*rData);
	}

	bool pop_try_while(T** rData) {
		while (!dataQueue.pop(*rData)) {
		
		}

		return true;
	}

private:


	boost::lockfree::queue<T*, boost::lockfree::capacity<count>> dataQueue;

};
