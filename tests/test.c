#include "../src/time_accum.h"
#include "../src/time_accum.c"

int main (int argc, char const *argv[]) {
	time_accum_init(1);
	int i;
	
	time_accum_start(0, EVENT1_GLOBAL);
	for (i = 0; i < 10000; i++) {
		int j;
		for (j = 0; j < atoi(argv[1]); j++); 
	}
	time_accum_stop(0, EVENT1_GLOBAL);
	time_accum_print();
	time_accum_cleanup();
	return 0;
}
