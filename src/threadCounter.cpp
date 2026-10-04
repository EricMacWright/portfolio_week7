/*
 * threadCounter.cpp
 *
 *  Created on: Oct 2, 2026
 *      Author: ew300170
 */
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;
mutex mLock;
bool countUpComplete = false;
condition_variable cv;

//method for counting up or down
void countDown(int& currCount) {
	unique_lock lk(mLock);
	while (countUpComplete == false) {
		cv.wait(lk);
	}

	for (int i=0; i<=19; i++) {
		cout << "Count down " << currCount << endl;
		currCount--;
	}
	//lock automatically releases here as it leaves the scope
}

//method for counting up or down
void countUp(int& currCount) {
	{
		//added a second set of braces to put our lock into
		//a smaller scope.  This way it can be destroyed prior
		//to us notifying the waiting threads
		scoped_lock lock(mLock);
		for (int i=0; i<=19; i++) {
			currCount++;
			cout << "count up " << currCount << endl;
		}
		//lock automatically releases here as it leaves the scope
		countUpComplete = true;
	}
	//lock is destroyed, tell the other threads they're
	//ready to proceed
	cv.notify_all();
}

int main() {
	int currCount = 0;

	//two threads which each call the counting routing
	thread t1(countUp, ref(currCount));
	thread t2(countDown, ref(currCount));

	//join the t1 thread
	if (t1.joinable()) {
		t1.join();
	}
	//join the t2 thread
	if (t2.joinable()) {
		t2.join();
	}
}



