<h1>README</h1>

<h2>MIXD-Specific Documentation:</h2>

**mixd** focuses on comparing bytes in two files. 
It builds upon the contributions of the original hexd project but
currently has some trade-offs for simplicity's sake:

<ol>
<li>The option to repeat file lines has been removed.</li>
<li>The option to supply hex values via stdin has been removed. This feature may be re-added in the future.</li>
<li>The option to input an infinite number of files in a single run of the program has been removed.</li> 
</ol>

<h3>PROBLEM DEFINITION:</h3>

<ol>
<li><strong>The Problem -</strong> hexdump / pixd's colorful visualizers are helpful
but the blur of the colors/bytes makes it difficult to quickly identify
visual similarities between two different files.</li>

<li><strong>The Importance -</strong> Malware often makes use of a previous codebase, so 
this a quick means of identifying trends with building on the visualization provided 
by pixd/hexd. This project actually grew out of my own challenges in malware analysis 
and a desire to have a tool like this.</li>

<li><strong>Existing tools -</strong> As far as I have seen the only option for visual
comparison (according to Gemini) is a combination of other tools - i.e
diff and xxd. However, even then, these tools make it difficult to find 
the matching bytes. One has to perform a confusing, manual eye search.</li> 

<li><strong>Gap Fixed -</strong> This tool fills the gap of providing a colorful visualization 
AND file comparison all in one command.</li> 
</ol>

<h3>SYSTEM DESIGN:</h3>
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
<li><strong>Testing -</strong> I used the basic mixd command:<br />
<strong>./mixd /path/to/file1 /path/to/file2</strong><br />
Where file1 and file2 are copied of the same binary but some bytes in 
the beginning of file1 have been modified.</li>

<li><strong>Results -</strong> It worked! The bytes that are the same appear as normal. The
bytes that differ appear as an __.</li>
 
<li><strong>Known issues -</strong><br />Manually shifting byte ranges to find an overall
between files can be tedious. I may implement a "brute-force" match feature that selects 
the range where there is the most overall between files.</li>
</ol>
<h3>SAMPLE TESTS:</h3>
<p>Try running the following the project root directory:</p>

./mixd samples/test.txt samples/test_shifted.txt<br />
./mixd -R 24 samples/test.txt samples/test_shifted.txt

<p>The files look highly similar based on the first execution, but
shifting the range produces a noticeable byte match.</p>

<h2>HOW TO GET MIXD RUNNING ON YOUR SYSTEM</h2>
<p><strong>Note:</strong> This project has been tested in DSU IALAB on an
Ubuntu 24.04 VM.</p>
<ol>
<li>Download the project folder from this GitHub repo.</li>
<li>If needed, install gcc using <br />
<strong>sudo apt update; sudo apt upgrade; sudo apt install gcc <li>
<li>Open the Linux Terminal.</li>
<li>Enter the project folder. You can run <strong>cd ~/Downloads/mixd</strong></li>
<li>Run </strong>./mixd path/to/file1 path/to/file2</strong> to get started.</li>

<h3>Installing the man pages.</h3>
Note: mixd.1 is the man page 
<ol>
<li><strong>sudo mkdir /usr/local/share/man/man1</strong> to create a folder for man pages in the "User Commands" section. 
<li><strong>sudo cp mixd.1 /usr/local/share/man/man1/</strong> to copy the man page</li>
<li><strong>sudo mandb</strong> to update the manpage database.</li>
</ol>

<h2>AI USAGE DISCLOSURE:</h2>
<p>AI was used in the following ways for developing the files and documentation:</p>
<ol>
<li>Identify similar tools to mixd.</li>
<li>Identify resources for "safe" binary files.</li>
<li>Identify the purpose of .1 files.</li>
<li>Add a page to the manpages Linux.</li>
<li>Learn how ANSI code are used to print colors in the Terminal.</li>
<li>Learn what manpage "sections" are for.</li>
<li>Updating the man pages.</li>
</ol>

<h2>OTHER RESOURCES USED IN THE MAKING OF THIS RESOURCE:</h2>
<ol>
<li>To retrieve the original sample1 file: <a href="https://github.com/JonathanSalwan/binary-samples/blob/master/MachO-iOS-armv7s-Helloworld">Jonathan Salwan GitHub Repo</a></li>
<li>Generating SSH keys: <a href="https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent">GitHub Tutorial</a></li>
<li>Testing SSH connection: <a href="https://docs.github.com/en/authentication/connecting-to-github-with-ssh/testing-your-ssh-connection">GitHub Tutorial</a></li>
<li>Forking GitHub Repository: <a href="https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo">GitHub Tutorial</a></li>
<li>Adding Locally Hosted Code to GitHub: <a href="https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github">GitHub Tutorial</a></li>
</ol>

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
