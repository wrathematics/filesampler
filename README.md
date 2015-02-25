# lineSampler

This is a simple R package to reasonably quickly read a random sample
of a csv file into R.

Idea inspired by Eduardo Arino de la Rubia's [fast_sample](https://github.com/earino/fast_sample).


## Package Use

The idea is to quickly randomly select a subset of a large-ish flat text
file.

```r
library(lineSampler)

file <- "/tmp/big.csv"
wc(file)
# file:   /home/mschmid3/tmp/csv/big.csv 
# Nletters: 766639674
# Nwords:   12174948
# Nlines:   12174948 

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

There is also a `readLines_sampled()` function for reading in 
(line) subsamples of unstructured text.


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


