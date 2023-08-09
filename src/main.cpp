#include "channel.h"
#include "list.h"
#include "pretty_print.h"
#include "primitive_types.h"

int main() {
	PrettyPrint pp		= PrettyPrint();
	Channel<u8> channel = Channel<u8>(5);

	// // //
	pp.set_color(pp.GREEN);
	pp.print("START\n\n");
	pp.set_color(pp.RESET);

	for(u32 i = 0; i < 5; i++) {
		channel.push(i);
		pp.print(channel);
	}

	pp.set_color(pp.RED);
	pp.print("\nEND\n");
	return 0;
}
