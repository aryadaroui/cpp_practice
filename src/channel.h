#include "primitive_types.h"
#include <iostream>

template<typename T>
class Channel {
	std::unique_ptr<std::unique_ptr<T>[]> buffer; // unique_ptr to an array of unique_ptrs to T

	std::mutex mutex;

	u32 length;
	u32 num_filled = 0;
	u32 head	   = 0;
	u32 tail	   = 0;

	void incr_head() {
		head = (head + 1) % length;
	}

	void incr_tail() {
		tail = (tail + 1) % length;
	}

  public:
	Channel(u32 length_) {
		length = length_;
		buffer = std::make_unique<std::unique_ptr<T>[]>(length);
	}

	u32 get_length() const {
		return length;
	}

	void push(unique_ptr<T> data) {
		std::lock_guard<std::mutex> lock(mutex);

		if(num_filled == length) {
			throw std::overflow_error("Channel is full");
		}

		incr_tail();
		num_filled++;
		buffer[tail] = std::move(data);
	}

	void push(T data) {
		push(std::make_unique<T>(data));
	}

	std::unique_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(mutex);
		if(num_filled == 0) {
			throw std::underflow_error("Channel is empty");
		}
		incr_head();
		num_filled--;
		return std::move(buffer[head]);
	}

	~Channel() {
	}
};