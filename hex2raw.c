// hex2raw.c
// (c) nailed barnacle/neil barnes 2019
//
// take an intel hex file (8 bit) and convert it into a file which can
// be imported into a C file, in the style:
//
// #define CODE_SIZE	3
// uint8_t rom[CODE_SIZE] = {
//		0xc3, 0x00, 0x00
// };


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
	uint8_t		byte_count;
	uint16_t	address;
	uint8_t		record_type;
	uint8_t		data[256];
} INTEL;

uint8_t get_hex_pair (char * ch);
uint16_t get_hex_quad (char * ch);


FILE * 			fi;				// the input hex file
FILE *			fo;				// the output raw file
char *			line = NULL;	// the line we're reading
size_t 			len = 0;		// how long the line we read was
ssize_t 		read;			// characters read
INTEL			intel;			// a partial intel hex line structure - ignores checksum
uint32_t		ptr;			// the next character on the line
uint8_t			memory[0x10000];	// we build the image here
uint16_t		highest = 0;	// what's the highest address we've written to?

int main (int argc, char * argv[])
{

	// basic check of input parameters
	if (argc < 3)
	{
		printf ("Usage: hex2raw infile outfile\n");
		exit (1);
	}
	
	// get the file to assemble
	if (NULL == (fi = fopen (argv[1], "r")))
	{
		printf ("Can't open hex file %s\n", argv[1]);
		exit (1);
	}
	
	if (NULL == (fo = fopen (argv[2], "w")))
	{
		printf ("Can't open output file %s\n", argv[2]);
		exit (1);
	}

	// output in decimal rather than hex; C doesn't care and it simplifies
	// a manual conversion to logisim raw format
	fprintf(fo, "// from %s\n", argv[1]);
	fprintf (fo, "#define CODE_SIZE %d\n", highest);
	fprintf (fo, "uint8_t rom[CODE_SIZE] = {");
	
	// read the hex file a line at a time
    while ((read = getline (&line, &len, fi)) != -1) 
    {
		//printf("%s", line);
		if (':' == line[0])
		{
			// get the parameters
			ptr = 1;
			intel.byte_count = get_hex_pair (&line[ptr]);
			ptr += 2;
			intel.address = get_hex_quad (&line[ptr]);
			ptr += 4;
			intel.record_type = get_hex_pair (&line[ptr]);
			
			// and then the data, stored into the memory
			for (uint32_t q = 0; q < intel.byte_count; q++)
			{
				ptr += 2;
				intel.data[q] = get_hex_pair (&line[ptr]);
				memory[intel.address + q] = intel.data[q];
			}
			// handle the current top of memory; the data may not be contiguous
			if ((intel.address + intel.byte_count) > highest)
			{
				highest = (intel.address + intel.byte_count);
			}
		}
    }
	for (uint16_t q = 0; q < highest; q++)
	{
		if (0 == (q % 32))
		{
			fprintf (fo, "\n");
		}
		fprintf (fo, "%03d, ", memory[q]);
	}
	fprintf (fo, "};\n");

    free (line);

    fclose (fi);
    fclose (fo);
	return 0;
}

uint8_t get_hex_pair (char * ch)
{
	// convert two ascii characters as hex values
	// we enter pointing at the first character
	
unsigned retval = 0;
	sscanf (ch, "%2X", &retval);
	return (uint8_t) retval;
}
	
uint16_t get_hex_quad (char * ch)
{
	return (256 * get_hex_pair (ch)) + get_hex_pair (&ch[2]);
}



