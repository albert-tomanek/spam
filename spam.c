#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 256
#define VERSION 1       // int 1 - 255
#define SPAMFILESIZE 65534
#define SPAMHEADERSIZE 63
#define COMMAND "./spam"        // This is the string put into the 'command' field, which can be executed. Note that it is limited to 30 chars long.

// define the shell-specific commands...
//#define COMMAND_CLEAR "del *.SPA"
#define COMMAND_CLEAR "rm *.SPA"

// For the .SPA spamfile format, see "spam format.asc"

void explain();
void generaterandomdata( uint8_t *data, int count);
uint32_t sdbm_hash(uint8_t *data, int length);          // Hashes an array of bits
void fputstrn(char *chars, FILE *file, int count);      // like fputs, but specifies limit
void spam();

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		spam();
	}
	else if (! strcmp(argv[1], "unspam"))
	{
		system(COMMAND_CLEAR);
	}
	else if (! strcmp(argv[1], "explain"))
	{
		explain();
	}
	else if (! strcmp(argv[1], "help")   ||
			 ! strcmp(argv[1], "-h")     ||
			 ! strcmp(argv[1], "--help") ||
			 ! strcmp(argv[1], "/?")     )
	{
		printf("%s [unspam, explain, help]\n", __FILE__);
	}
	
	return 0;
}

void spam()
{
	char filename[STRLEN];
	
	uint16_t datasize = (SPAMFILESIZE - SPAMHEADERSIZE - 2);
	uint32_t file_id;
	uint32_t datahash;
	
	char command[34];
	memset(command, '\0', 34);              // clears the command field.
	strcpy(command, COMMAND);               // copies the default command string into 'command'.
	
	uint8_t randomdata[datasize];
	
	for (int file_no = 0; file_no < 256; file_no++)
	{   
		//int file_no = 216;
	
		generaterandomdata(randomdata, datasize);
		//strcpy("Testing.", randomdata);
		
		sprintf(filename, "SPAM_%X.SPA", file_no);              // %X - hex as upper case.
	
		FILE *out = fopen(filename, "w");
		assert(out != NULL);
		
		fputc('S', out);        // magic bytes
		fputc('P', out);
		fputc('A', out);
		fputc('M', out);
	
		fputc((uint8_t) VERSION, out);   // format version
	
		fputc(0x1a, out);               // DOS EOF character
		fputc(0x00, out);				// padding
		
		fwrite( &datasize , sizeof(uint16_t), 1, out);          // fwrite( *data, sizeof data, count, filestream);
		
		fputc(0x00, out);               // padding
		fputc(0x00, out);
		
		file_id = (uint32_t) file_no;           // In later versions a number more unique to the file will be generated
		fwrite( &file_id , sizeof(uint32_t), 1, out);
		
		fputc(0x00, out);               // padding
		fputc(0x00, out);
		
		datahash = sdbm_hash(randomdata, datasize);
		fwrite( &datahash , sizeof(uint32_t), 1, out);          // 32-bit sdbm hash of the random data
		
		fputc(0x00, out);               // padding
		fputc(0x00, out);
		
		fputc(0xFF, out);               // private use
		fputc(0xFF, out);
		
		fputc(0x00, out);               // more padding
		
		fputstrn(command, out, 34);             // see method at bottom of file.s
		
		fputc(0x00, out);               // even more padding
		fputc(0x00, out);
		fputc(0x00, out);
		fputc(0x00, out);
		
		for (int i = 0; i < (int) datasize; i++)
		{
			fputc(randomdata[i], out);
		}
		
		fputc(0x00, out);               // padding
		fputc(0x00, out);
		
		fclose(out);
		
	}
}

void explain()
{
	FILE *out;
	out = fopen("SPAM.TXT", "w");
	
	const char *string = "\
 FORMAT OF SPAM FILES                                                     \n\
 ====================                                                     \n\
                                                                          \n\
 Byte              Use                         Value                      \n\
                                                                          \n\
 0,1,2,3           Header                      53, 50, 41, 4D (\"SPAM\")  \n\
 4                 Format version              8-bit int                  \n\
 5,6               padding                     1A, 00                     \n\
 7,8               length of data              16-bit int                 \n\
 9,10              padding                     00, 00                     \n\
 11,12,13,14       File ID                     32-bit int                 \n\
 15,16             padding                     00, 00                     \n\
 17 - 25           sdbm hash of random data    32-bit int                 \n\
 26,27             padding                     00, 00                     \n\
 28,29             private use                 FF, FF                     \n\
 30                padding                     00                         \n\
 31 - 61           command to execute          null term. string          \n\
 63                padding                     00                         \n";
	
	fputs(string, out);
	fclose(out);
	
	printf("See SPAM.TXT\n");
}


/****************************/

void generaterandomdata( uint8_t *data, int count)
{
	for (int i = 0; i < count; i++)
	{
		data[i] = (uint8_t) rand() % 255;
	}
}

uint32_t sdbm_hash(uint8_t *data, int length)           // Hashes an array of bits
{
	uint32_t hash = 0;
	
	for (int i = 0; i < length; i++)
	{
		hash = data[i] + (hash << 6) + (hash << 16) - hash;
	}
	
	return hash;
}

void fputstrn(char *chars, FILE *file, int count)
{
	for (int i = 0; i < count; i++)
	{
		fputc(chars[i], file);
	}
}
