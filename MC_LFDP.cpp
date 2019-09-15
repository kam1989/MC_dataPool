#include <Windows.h>
#include <iostream>
#include <atomic>
#include <array>
#include <chrono>


template<typename T, int N>
class lockfreePool {
public:
	lockfreePool() {
		int count = 0;
		for (count; count < size; count++) {
			dataArray[count] = NULL;
		}
	}

	bool push_try(T* data) {
		T* temp = NULL;
		int pos = in_pos.fetch_add(1);

		check_size(&in_pos, &pos);

		if (dataArray[pos].compare_exchange_strong(temp, data)) {
			count.fetch_add(1);
			return true;
		}

		return false;
	}

	void push_try_while(T* data) {
		T* temp = NULL;
		int pos = 0;

		while (true) {
			pos = in_pos.fetch_add(1);

			check_size(&in_pos, &pos);

			if (dataArray[pos].compare_exchange_strong(temp, data)) {
				count.fetch_add(1);
				return;
			}
		}
	}

	bool pop_try(T** rData) {
		int temp = 0;
		int pos = 0;

		if (count > 0) {
			temp = count.fetch_sub(1);

			if (temp > 0) {
				pos = out_pos.fetch_add(1);

				check_size(&out_pos, &pos);

				if (dataArray[pos] != NULL) {
					*rData = dataArray[pos].exchange((T*)NULL);

					if (*rData != NULL) {
						return true;
					}
				}
			}
			count.fetch_add(1);
		}
		return false;
	}

	bool pop_try_while(T** rData) {
		int temp = 0;
		int pos = 0;

		if (count > 0) {
			temp = count.fetch_sub(1);

			if (temp > 0) {
				while (true) {
					pos = out_pos.fetch_add(1);

					check_size(&out_pos, &pos);

					if (dataArray[pos] != NULL) {
						*rData = dataArray[pos].exchange((T*)NULL);

						if (*rData != NULL) {
							return true;
						}
					}
				}
			}
			count.fetch_add(1);
		}
		return false;
	}

private:

	void check_size(std::atomic<int>* ato, int* pnum) {
		if (*pnum >= size) {
			*pnum = *pnum % size;
			if (*pnum == 0) {
				ato->fetch_sub(size);
			}
		}
		return;
	}


	const int size = N;
	std::array<std::atomic<T*>, N> dataArray;

	std::atomic<int> in_pos = 0;
	std::atomic<int> out_pos = 0;
	std::atomic<int> count = 0;
};
