<h1>README</h1>

<h2>MIXD-Specific Documentation:<h2>

**mixd** focused on comparing bytes in two files. 
It builds upon the contributions of the original hexd project but
currently has some trade-offs:

The option to repeat file lines has been removed. This might interfere
with the ease of analyzing similarities between files. For this reason, revising 
the feature for mix has been omitted.

Analysis of an infinite number of files in a single run of the program.</li> 


<h3>PROBLEM DEFINITION:</h3>

<ol>
<li><strong>The Problem -</strong> hexdump / pixd's colorful visualizers are helpful
but the blur of the colors/bytes makes it difficult to quickly identify
visual similarities between different filed.</li>

<li><strong>The Importance -</strong> Malware often makes use of a previous codebase, so 
this is a quick means of identifying trends. This actually grew out 
of my own challenges in malware analyss and a desire to have a tool 
like this.</li>

<li><strong>Existing tools -</strong> As far as I have seen the only option for visual
comparison (according to Gemini) is a combination of other tools - i.e
diff and xxd. However, even then, these tools make it difficult to find 
the matching bytes. One has to perform a confusing, manual eye search.</li> 

<li><strong>Gap Fixed -</strong> This tool fills the gap of providing a colorful visualization 
AND file comparison all in one command.</li> 
</ol>

<h3>SYSTEM DESIGN:<h3>
<ol>
<li><strong>Architecture -</strong> This is a one-file C program that should portable for any
C-supporting terminal application.</li>

<li><strong>Tech Choices and Justification -</strong> Pixd is a tool that has come up in our 
coursework, so I believed the tool could be especially useful to this 
audience. hexd is the foundation of pixd, so I decided to start with a 
hexd implementation in preparation for the Tool 1 Refinements.</li> 
</ol>

<h3>EVALUATION:</h3>
<ol>
<li><strong>TESTING -</strong> I used the basic mixd command:<br />
<strong>./mixd /path/to/file1 /path/to/file2</strong><br />
Where file1 and file2 are copied of the same binary but some bytes in 
the beginning of file1 have been modified.</li>

<li><strong>RESULTS -</strong> It worked! The bytes that are the same appear as normal. The
bytes that differ appear as an __.</li>
 
<li><strong>KNOWN ISSUES: -</strong><br />Manually shifting byte ranges to find an overall
between files can be tedious. I may implement a "brute-force" match feature that selects 
the range where there is the most overall between files.</li>
</ol>
<h3>SAMPLE TESTS:</h3>
<p>Try running the following the project root directory:</p>

./mixd samples/test.txt samples/test_shifted.txt
./mixd -R 24 samples/test.txt samples/test_shifted.txt

<p>The files look highly similar based on the first execution, but
shifting the range produces a noticeable byte match.</p>
 




<h2>ORIGINAL DOCUMENTATION BELOW:</h2> 

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
