<H1>hex2raw</H1>

A simple utility to convert intel 16-bit address hex files into something a C compiler or Logisim can digest.

Usage: hex2raw hexfile rawfile [hex!raw]

By default, the output file looks like:

<pre><code>
#define CODE_SIZE	3
uint8_t rom[CODE_SIZE] = {
		0xc3, 0x00, 0x00
};
</code></pre>

If the third parameter is present, and is 'raw', then a logisim compatible raw file is produced. Otherwise, a hex include file is produced.

<pre><code>
v2.0 raw
c3 00 00
</code></pre>

Note: does not perform any error checking other than that the files exist and can be opened. Garbage in, garbage out... 

Built for linux but should compile for Windows.
