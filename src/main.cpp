#include "list.h"
#include "pretty_print.h"
#include "primitive_types.h"

int main() {
	PrettyPrint pp	 = PrettyPrint();
	List<string>	list = List<string>();

	// // //
	pp.set_color(pp.GREEN);
	pp.print("START\n\n");
	pp.set_color(pp.RESET);

	for(u8 i = 0; i < 10; i++) {
		list.push_back("el " + std::to_string(i));
	}

	pp.print(list);

	pp.set_color(pp.RED);
	pp.print("\nEND\n");
	return 0;
}
