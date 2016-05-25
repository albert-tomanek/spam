#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define STRLEN 256
#define VERSION 1	// int 1 - 255
#define SPAMFILESIZE 65534
#define SPAMHEADERSIZE 63

// For the .SPA spamfile format, see "spam format.asc"


int main()
{
	uint16_t filesize;
	uint32_t file_id;
	
	//for (int file_no = 0; file_no < 256; file_no++)
	//{   
		int file_no = 216;
		
		char filename[STRLEN];
		sprintf(filename, "SPAM_%X.SPA", file_no);		// %X - hex as upper case.
	
		FILE *out = fopen(filename, "w");
		assert(out != NULL);
		
		fputc('S', out); 	// magic bytes
		fputc('P', out);
		fputc('A', out);
		fputc('M', out);
	
		fputc(VERSION, out);	 // format version
	
		fputc(0, out);		// padding
		fputc(0, out);
		
		filesize = (SPAMFILESIZE - SPAMHEADERSIZE);
		fwrite( &filesize , sizeof(uint16_t), 1, out); 		// fwrite( *data, sizeof data, count, filestream);
		
		fputc(0, out);		// padding
		fputc(0, out);
		
		file_id = (uint32_t) file_no;		// In later versions a number more unique to the file will be generated
		fwrite( &file_id , sizeof(uint32_t), 1, out);
		
		fclose(out);
		
	//}
	
	return 0;
}
