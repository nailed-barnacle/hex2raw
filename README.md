<H1>hex2raw</H1>

A simple utility to convert intel 16-bit address hex files into something a C compiler can digest.

Produces a file looking something like:

#define CODE_SIZE	3
uint8_t rom[CODE_SIZE] = {
		195, 000, 000
};

Decimal is used to allow easy manual conversion to Logisim's raw data format.

Note: does not perform any error checking other than that the files exist and can be opened. Garbage in, garbage out... 

Built for linux but should compile for Windows.
