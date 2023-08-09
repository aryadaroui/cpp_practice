#include "primitive_types.h"
#include <iostream>

template<typename T>
class Channel {
	std::unique_ptr<std::unique_ptr<T>[]> buffer; // unique_ptr to an array of unique_ptrs to T

	mutable std::mutex mutex;

	u32 buffer_length;
	u32 num_filled = 0;
	u32 head	   = 0;
	u32 tail	   = 0;

	void incr_head() {
		head = (head + 1) % buffer_length;
	}

	void incr_tail() {
		tail = (tail + 1) % buffer_length;
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
				os << "nullptr";
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

	void push(std::unique_ptr<T> &&data) {
		std::lock_guard<std::mutex> lock(mutex);

		if(num_filled == buffer_length) {
			throw std::overflow_error("Channel is full");
		}
		buffer[tail] = std::move(data);
		incr_tail();
		num_filled++;
	}

	void push(const T &data) {
		push(std::move(std::make_unique<T>(data)));
	}

	std::unique_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(mutex);
		if(num_filled == 0) {
			throw std::underflow_error("Channel is empty");
		}
		auto data = std::move(buffer[head]);
		incr_head();
		num_filled--;
		return data;
	}

	~Channel() {
	}
};