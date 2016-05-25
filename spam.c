#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define STRLEN 256
#define VERSION 1	// int 1 - 255
#define SPAMFILESIZE 65534
#define SPAMHEADERSIZE 63

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
	
		fputc(0, out);
		fputc(0, out);
	
		fwrite( &(uint16_t) (SPAMFILESIZE - SPAMHEADERSIZE), sizeof(uint16_t), 1, out); 		// fwrite( *data, sizeof data, count, filestream);

		fclose(out);
		
	//}
	
	return 0;
}