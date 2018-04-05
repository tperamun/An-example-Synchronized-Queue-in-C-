#ifndef SYNCH_QUEUE_H
#define SYNCH_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>


template <class T>
class SynchronizedQueue{

	
	std::deque<T> task_queue;
	std::mutex mutex;
	std::condition_variable cond;

public:
	SynchronizedQueue();
	~SynchronizedQueue();
	SynchronizedQueue(const SynchronizedQueue<T> &) = delete;
	void pop_front();
	void push_back(const T& element);
	T &front();
	int size();
	bool empty();
	bool getFront_and_pop(T & element);
	T& operator[](const int location) throw (const char *);

};

template <class T>
SynchronizedQueue<T>::SynchronizedQueue(){}

//template<class T>
//SynchronizedQueue<T>::SynchronizedQueue(const SynchronizedQueue<T>& other){}
template <class T>
SynchronizedQueue<T>::~SynchronizedQueue(){}

template <class T>
T &SynchronizedQueue<T>::front(){
	
	std::unique_lock<std::mutex> mlock(mutex);
	while(task_queue.empty()){
		cond.wait(mlock);	
	}
	
	return task_queue.front();

}


template <class T>
void SynchronizedQueue<T>::pop_front(){

	std::unique_lock<std::mutex> mlock(mutex);
	
	while(task_queue.empty()){
		cond.wait(mlock);	
	}

	task_queue.pop_front();

}

template <class T>
void SynchronizedQueue<T>::push_back(const T &element){

	std::unique_lock<std::mutex> mlock(mutex);
	task_queue.push_back(element);
	mlock.unlock();
	cond.notify_one();
}
template <class T>
bool SynchronizedQueue<T>::getFront_and_pop(T & element){
	std::unique_lock<std::mutex> mlock(mutex);
	
	

	while(task_queue.empty()) {
		cond.wait(mlock);
	}

	element = std::move(task_queue.front());
	int size = task_queue.size();
	task_queue.pop_front();
	int size_after = task_queue.size();

	return size > size_after ? true : false;
}

template <class T>
int SynchronizedQueue<T>::size(){
	
	std::unique_lock<std::mutex> mlock(mutex);
	return task_queue.size();

}


template <class T>
bool SynchronizedQueue<T>::empty(){

	std::unique_lock<std::mutex> mlock(mutex);
	return task_queue.empty();

}


template <class T> 
T& SynchronizedQueue<T>::operator[](const int location)
  throw (const char *) {
	std::unique_lock<std::mutex> mlock(mutex); // i dont think ill need this

    	if (location < 0 || location >= task_queue.size()) throw "Out of bounds access";
    	else return task_queue[location];
}



#endif
