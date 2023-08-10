#include "channel.h"
#include "list.h"
#include "pretty_print.h"
#include "primitive_types.h"
#include <thread>

static auto pp = PrettyPrint();

void push(Channel<u8> &channel) {
	for(u8 i = 0; i < 10; i++) {
		pp.print("pushing: ", i, "\n");
		channel.push_wait(i);
		pp.print(channel, "\n");
	}
}

void pop(Channel<u8> &channel) {
	for(u8 i = 0; i < 10; i++) {
		u8 val = *(channel.pop_wait());
		pp.print("popped: ", val, "\n");
		pp.print(channel, "\n");
	}
}

int main() {
	auto channel = Channel<u8>(5);

	// // //
	std::cout.flush();
	pp.set_color(pp.GREEN);
	pp.print("START\n\n");
	pp.set_color(pp.RESET);

	auto producer_1 = std::thread(push, std::ref(channel));
	// auto producer_2 = std::thread(push, std::ref(channel), std::ref(pp));
	auto consumer_1 = std::thread(pop, std::ref(channel));
	// auto consumer_2 = std::thread(pop, std::ref(channel), std::ref(pp));

	producer_1.join();
	// producer_2.join();
	consumer_1.join();
	// consumer_2.join();

	pp.set_color(pp.RED);
	pp.print("\nEND\n");
	return 0;
}
