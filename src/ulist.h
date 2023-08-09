#include "primitive_types.h"
#include <iostream>
#include <memory>

template<typename T>
struct Node {
	T						 data;
	std::shared_ptr<Node<T>> next;
	std::weak_ptr<Node<T>>	 prev;

	friend std::ostream &operator<<(std::ostream &os, const Node<T> &node) {
		if constexpr(std::is_same<T, string>::value) {
			os << node.data;
			return os;
		} else {
			os << std::to_string(node.data);
			return os;
		}
	}

	Node(T data_):
		data(data_) {}
};

template<typename T>
class UList {
  private:
	std::shared_ptr<Node<T>> start;
	std::weak_ptr<Node<T>>	 end;
	size_t					 length = 0;

  public:
	size_t get_length() const {
		return length;
	}

	T at(u64 index) const {
		if(index >= length) {
			throw std::out_of_range("Index out of range.");
		}

		auto current = start;

		for(size_t i = 0; i < index; i++) {
			current = current->next;
		}

		return current->data;
	}

	friend std::ostream &operator<<(std::ostream &os, const UList<T> &list) {
		auto current = list.start;

		for(size_t i = 0; i < list.length; i++) {
			os << *current << " ";
			current = current->next;
		}

		return os;
	}

	void push_back(T data) {
		auto new_node = std::make_shared<Node<T>>(data);
		if(length == 0) {
			start = new_node;
			end	  = new_node;
		} else {
			new_node->prev	 = end;
			end.lock()->next = new_node;
			end				 = new_node;
		}
		length++;
	}
};