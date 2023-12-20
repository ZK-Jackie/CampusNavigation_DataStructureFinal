#include <stdio.h>
#include <stdbool.h>

bool PrintPageHead() {
	for (int i = 0; i < 20; ++i) {
		printf("===");
	}
	putchar('\n');
	return true;
}