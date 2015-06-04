# lineSampler

* **Version:** 0.2
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/lineSampler.png)](https://travis-ci.org/wrathematics/lineSampler)
* **License:** [![License](http://img.shields.io/badge/license-BSD%202--Clause-orange.svg?style=flat)](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt


This is a simple R package to reasonably quickly read a random sample of a flat
text file (such as a csv) into R. This allows you to get a subsample int R 
without having to read the (possibly large) file into memory.

Idea inspired by Eduardo Arino de la Rubia's [fast_sample](https://github.com/earino/fast_sample).



## Package Use

The idea is to quickly randomly select a subset of a large-ish flat text file.

```r
library(lineSampler)

file <- "/tmp/big.csv"
wc(file)
# file:   /tmp/big.csv 
# Nletters: 766639674
# Nwords:   12174948
# Nlines:   12174948 

### Read in approximately 0.1% of the input file
system.time({
  ret <- read_csv_sampled(file, .001)
})
#    user  system elapsed 
#   0.805   0.104   0.909 

dim(ret)
# [1] 12062     6

str(ret) ### Just some random nonsense
# 'data.frame':	12062 obs. of  6 variables:
#  $ A: int  57 59 17 23 44 4 78 34 74 15 ...
#  $ B: Factor w/ 26 levels "a","b","c","d",..: 10 3 13 20 15 13 16 19 8 3 ...
#  $ C: Factor w/ 26 levels "A","B","C","D",..: 21 14 10 18 26 11 16 13 5 3 ...
#  $ D: num  0.723 0.608 0.751 0.197 0.605 ...
#  $ E: num  -0.0946 0.0317 1.2372 -0.0678 0.3253 ...
#  $ F: num  31.9 52.2 69.4 59.9 19.1 ...
```

There is also a `readLines_sampled()` function for reading in (line) subsamples
of unstructured text.



## How It Works

The input file is scanned, line-by-line, and lines are randomly placed into a
temporary file at the given proportion (by the user). This requires one pass
through the file. In the "exact" version, a reservoir sampler is used to
determine which lines will be read, and then pass through the input file and
dumping lines to the temporary file as necessary (i.e., if that line number was
chosen by the sampler). In each case, R's `read.csv()` or `readLines()` is used
to read the temp file into R.  

The package attempts to be reasonably efficient, with the underlying I/O handled
by very ad hoc C code via `fgets()`. The exact readers necessarily require 2
passes through the file (one to get the linecounts, then one to sample), while
the basic version only requires one pass. On Linux (and possibly other OS's),
your file may get cached on the first read, so the second read might be
comparatively cheap. But for very large files (which shouldn't be csv anyway!),
downsampling with the inexact version and a p of .001 or smaller should be more
than sufficient.

As a side note, you can see how large the file is without scanning it, just
based on reports from the file system. In R, you can do

```r
file.info("/tmp/big.csv")$size
778814622
```

or about 743 MiB on disk.

From C, you can use `stat()` on *NIX systems, and `GetFileSizeEx()`
on Windows (using the Win32 API).  A real example is provided in
the [meminfo library](https://github.com/wrathematics/memuse/blob/master/src/meminfo/src/fileinfo.c).



## Installation

```r
library(devtools)
install_github("wrathematics/lineSampler")
```



## License

    Copyright (c) 2015, Drew Schmidt
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    
    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    
    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.


