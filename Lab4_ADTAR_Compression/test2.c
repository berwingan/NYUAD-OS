#include <stdio.h>
#include "lab4/common.h"

int main(){
	FILE * fp_write;
	FILE * fp_read;
	fp_write = fopen("hello5.ad","wb");
	fp_read = fopen("hello5.txt", "rb");
	fclose(fp_write);
	fclose(fp_read);
	return 0;
}
