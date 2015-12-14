Copyright (c) 2015 Victor Adrian Valle Rivera



Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:



The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

I've been interested in compilers for a while. Recently, I wanted to do a 
command-line interface for a simple program and found out that LEX can be
overkilling for simpĺe situations like mine. The intention of this utility
which I called GLEX is to parse a filed containing named regular expressions:

name1: expr1
name2: expr2
...

and generate the deterministic finite automaton for such expressions. This
automaton can be later included in your own project.

For simplicity this utility uses ascii alphabet and it will not support fancy
constructs like character classes, perl syntax and of course my intention is 
not to create a POSIX-compliant application. I just want something quick and
useful for small applications. I also want to lear something new in the
process and have fun.

At the moment this project is not ready. I've been building and destroying my
code for a while until I decided to post it here. I will work at my own pace 
and even though this is a personal project If you have a suggestion, want to
contribute or communicate with me I'm leaving my my email address: 

extradiable@gmail.com.

Victor
