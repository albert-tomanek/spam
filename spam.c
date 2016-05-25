#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define STRLEN 256
#define VERSION 1	// int 1 - 255

int main()
{
	/*for (int file_no = 0; file_no < 256; file_no++)
	{*/ int file_no = 0;
		
		char filename[STRLEN];
		sprintf(filename, "SPAM_%X.SPA", file_no);		// %X - hex as upper case.
	
		FILE *out = fopen(filename, "w");
		assert(out != NULL);
		
		fputc('S', out);
		fputc('P', out);
		fputc('A', out);
		fputc('M', out);
	
		fputc(VERSION, out);

		fclose(out);
		
	//}
	
	return 0;
}