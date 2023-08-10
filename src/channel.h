#include "primitive_types.h"
#include <chrono>
#include <iomanip>
#include <iostream>

template<typename T>
class Channel {
	std::unique_ptr<std::unique_ptr<T>[]> buffer; // unique_ptr to an array of unique_ptrs to T

	mutable std::mutex mutex;

	u32 buffer_length;
	u32 num_filled = 0;
	u32 head	   = 0;
	u32 tail	   = 0;

	std::condition_variable not_full;
	std::condition_variable not_empty;

	void incr_head() {
		head = (head + 1) % buffer_length;
	}

	void incr_tail() {
		tail = (tail + 1) % buffer_length;
	}

	void push(const T &data) {
		buffer[tail] = std::make_unique<T>(data); // this makes a copy of data!
		incr_tail();
		num_filled++;
		not_empty.notify_one();
	}

	std::unique_ptr<T> pop() {
		auto data = std::move(buffer[head]);
		incr_head();
		num_filled--;
		not_full.notify_one();
		return data;
	}

  public:
	Channel(u32 buffer_length_) {
		buffer_length = buffer_length_;
		buffer		  = std::make_unique<std::unique_ptr<T>[]>(buffer_length);
	}

	friend std::ostream &operator<<(std::ostream &os, const Channel &channel) {
		std::lock_guard<std::mutex> lock(channel.mutex);
		os << "Channel "
		   << "[";
		for(u32 i = 0; i < channel.buffer_length; i++) {
			os << "\n  " << i << ": ";
			if(channel.buffer[i] == nullptr) {
				os << "⍉";
			} else {
				if constexpr(std::is_same<T, string>::value) {
					os << *channel.buffer[i];
				} else {
					os << std::to_string(*channel.buffer[i]);
				}
			}
		}
		os << "\n], " << channel.buffer_length << " elements, " << channel.num_filled << " filled" << std::endl;

		return os;
	}

	u32 get_length() const {
		return buffer_length;
	}

	void push_throw(const T &data) {
		std::lock_guard<std::mutex> lock(mutex);

		if(num_filled == buffer_length) {
			throw std::overflow_error("Channel is full");
		}
		push(data);
	}

	void push_wait(const T &data) {
		std::unique_lock<std::mutex> lock(mutex);
		not_full.wait(lock, [this] { return num_filled < buffer_length; });
		push(data);
	}

	void push_spin(const T &data, const u32 &spin_timeout = 1) {
		auto start_time	  = std::chrono::high_resolution_clock::now();
		auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;

		while(true) {
			if(num_filled < buffer_length) {
				std::lock_guard<std::mutex> lock(mutex);
				push(data);
				return;
			}

			elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
			if(elapsed_time > std::chrono::milliseconds(spin_timeout)) {
				push_wait(data);
				return;
			}
		}
	}

	std::unique_ptr<T> pop_throw() {
		std::lock_guard<std::mutex> lock(mutex);
		if(num_filled == 0) {
			throw std::underflow_error("Channel is empty");
		}
		auto data = std::move(buffer[head]);
		incr_head();
		num_filled--;
		return data;
	}

	std::unique_ptr<T> pop_wait() {
		std::unique_lock<std::mutex> lock(mutex);
		not_empty.wait(lock, [this] { return num_filled > 0; });
		return pop();
	}

	std::unique_ptr<T> pop_spin(const u32 &spin_timeout = 1) {
		auto start_time	  = std::chrono::high_resolution_clock::now();
		auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;

		while(true) {
			if(num_filled > 0) {
				std::lock_guard<std::mutex> lock(mutex);
				return pop();
			}

			elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
			if(elapsed_time > std::chrono::milliseconds(spin_timeout)) {
				return pop_wait();
			}
		}
	}

	~Channel() {
	}
};