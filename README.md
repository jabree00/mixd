MIXD-Specific Documentation: 

**mixd** focused on comparing bytes in two files. 
It builds upon the contributions of the original hexd project but
currently has some trade-offs:
- Omitting repeat file lines (this would hide some critical details for 
comparative analysis) 
- Analysis of an infinite number of files in a single run of the program. 

PROBLEM DEFINITION:
- Problem Addressed: hexdump / pixd's colorful visualizers are helpful
but the blur of the colors/bytes makes it difficult to quickly identify
visual similarities between different filed. 
- Why Important: Malware often makes use of a previous codebase, so 
this is a quick means of identifying trends. This actually grew out 
of my own challenges in malware analyss and a desire to have a tool 
like this. 
- Existing tools: As far as I have seen the only option for visual
comparison (according to Gemini) is a combination of other tools - i.e
diff and xxd. However, even then, these tools make it difficult to find 
the matching bytes. One has to perform a confusing, manual eye search. 
- Gap Fixed: This tool fills the gap of providing a colorful visualization 
AND file comparison all in one command. 

SYSTEM DESIGN:
- Architecture: This is a one-file C program that should portable for any
C-supporting terminal application. 
- Tech choices and justification: Pixd is a tool that has come up in our 
coursework, so I believed the tool could be especially useful to this 
audience. hexd is the foundation of pixd, so I decided to start with a 
hexd implementation in preparation for the Tool 1 Refinements. 

EVALUATION:
- TESTING: I used the basic mixd command:
./mixd /path/to/file1 /path/to/file2

where file1 and file2 are copied of the same binary but some bytes in 
the beginning of file1 have been modified

- RESULTS: It worked! The bytes that are the same appear as normal. The
bytes that differ appear as an __. 
- KNOWN ISSUES: 
--I need to allow the offsets for both files to be changed
individually. Two files might be nearly identical depending on the offsets
that we start comparing from. Bytes can be shifted in one file. 
--This is not a standalone analysis tool - just a simple modification 
to an existing one.  

ORIGINAL DOCUMENTATION BELOW: 

**hexd** prints a human-readable hexdump of the specified files, or standard
input if omitted.  Its main distinguishing feature is the use of colours to
visually indicate which range of values an octet belongs to, aiding in
spotting patterns in binary data.

By default, *hexd* relies on 256-color SGR escape sequences.  Most terminal
emulators should support these today, but technically they're only defacto
standard.  However, you can override the formatting used with the
`HEXD_COLORS` environment variable (see manpage), or use the `-p` option for
plaintext output.

## Screenshot
![Screenshot](meta/screenshot.png "Output after being run on an ELF binary")

## License
MIT license.

## See also
* [pixd](http://github.com/FireyFly/pixd):
  like *hexd*, but visualizes the data fed to it using a 256-colour palette
  and half-block ("▀") characters.
