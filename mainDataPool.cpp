#pragma once

#include "MC_LFDP.cpp"

#include <iostream>
#include <thread>
#include <array>
#include <chrono>

#define testThreadCount 8
#define poolSize 6
#define threadLoopCount 12500000

//start = _Query_perf_counter();

long long temp = _Query_perf_frequency();

struct st_recode {
	long long min = LLONG_MAX;
	long long max = 0;
	long long total = 0;
	long long count = 0;
};

void recode_update(st_recode* container, long long data) {
	container->total = container->total + data;
	container->count = container->count + 1;
	if (container->max < data) {
		container->max = data;
	}
	if (container->min > data) {
		container->min = data;
	}
}

void recode_update(st_recode* container, st_recode data) {
	container->total = container->total + data.total;
	container->count = container->count + data.count;
	if (container->max < data.max) {
		container->max = data.max;
	}
	if (container->min > data.min) {
		container->min = data.min;
	}
}

void testThread(lockfreePool<long long , poolSize>* p , st_recode* rec) {
	long long popTime = 0;

	long long dataTime = 0;

	long long* data = NULL;

	int count = 0;

	for (count = 0; count < threadLoopCount; count) {
		while (!p->pop_try_while(&data)) {
			Yield();
		}

		popTime = _Query_perf_counter();

		if (*data != 0) {
			dataTime = popTime - *data;
			recode_update(rec, dataTime);
			count = count+1;
		}


		*data = _Query_perf_counter();
		p->push_try_while(data);
		data = NULL;
	}

	return;
}

int main() {
	lockfreePool<long long , poolSize>* p = new lockfreePool<long long, 6>();
	st_recode recodeArray[testThreadCount];

	int count = 0;
	long long* data = NULL;

	for (count = 0; count < poolSize; count++) {
		data = new long long;
		*data = 0;
		p->push_try_while(data);
	}

	std::thread* tArray[testThreadCount];

	for (count = 0; count < testThreadCount; count++) {
		tArray[count] = new std::thread(testThread , p , &recodeArray[count]);
	}

	for (count = 0; count < testThreadCount; count++) {
		tArray[count]->join();
	}

	for (count = 1; count < testThreadCount; count++) {
		recode_update(&recodeArray[0], recodeArray[count]);
	}

	return 0;
}
