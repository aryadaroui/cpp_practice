#include "primitive_types.h"
#include <iostream>

template<typename T>
struct Node {
	T		 data;
	Node<T> *next;
	Node<T> *prev;

	friend std::ostream &operator<<(std::ostream &os, const Node<T> &node) {
		if constexpr(std::is_same<T, string>::value) {
			os << node.data;
			return os;
		} else {
			os << std::to_string(node.data);
			return os;
		}
	}

	Node(T data_) {
		data = data_;
		next = nullptr;
		prev = nullptr;
	}

	Node() {
		next = nullptr;
		prev = nullptr;
	}

	~Node() {
		next = nullptr;
		prev = nullptr;
	}
};

template<typename T>
class List {
  private:
	size_t length;

  public:
	Node<T> *front;
	Node<T> *back;

	size_t get_length() {
		return length;
	}

	T at(u64 index) {
		if(index >= length) {
			throw std::out_of_range("Index out of range.");
		}

		Node<T> *current = front;

		for(u64 i = 0; i < index; i++) {
			current = current->next;
		}

		return current->data;
	}

	void push_back(T data) {
		if(length == SIZE_T_MAX) {
			throw std::overflow_error("List length overflow. You've already crashed before you can read this.");
		}

		Node<T> *node = new Node<T>(data);

		if(length == 0) {
			front = node;
			back = node;
		} else {
			back->next = node;
			node->prev = back;
			back	   = node;
		}

		length++;
	}

	friend std::ostream &operator<<(std::ostream &os, const List<T> &list) {
		Node<T> *current = list.front;

		os << "List [" << std::endl;
		while(current != nullptr) {
			os << "  " << *current << std::endl;
			current = current->next;
		}
		os << "]" << std::endl;

		return os;
	}

	List() {
		front   = nullptr;
		back   = nullptr;
		length = 0;
	}
	~List() {
		Node<T> *current = front;
		Node<T> *next	 = current->next;

		while(current->next != nullptr) {
			delete current;

			current = next;
			next	= current->next;
		}

		length = 0;
		front   = nullptr;
		back   = nullptr;
	};
};