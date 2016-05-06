# lineSampler

* **Version:** 0.3.1
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/lineSampler.png)](https://travis-ci.org/wrathematics/lineSampler)
* **License:** [![License](http://img.shields.io/badge/license-BSD%202--Clause-orange.svg?style=flat)](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt


This is a simple R package to reasonably quickly read a random sample of lines of a flat text file (such as a csv) into R. This allows you to get a subsample into R without having to read the (possibly large) file into memory first.

Idea inspired by Eduardo Arino de la Rubia's [fast_sample](https://github.com/earino/fast_sample).



## Installation

From CRAN:

```r
devtools::install_github("wrathematics/lineSampler")
```

The development version is maintained on GitHub, and can easily be installed by any of the packages that offer installations from GitHub:

```r
### Pick your preference
devtools::install_github("wrathematics/lineSampler")
ghit::install_github("wrathematics/lineSampler")
remotes::install_github("wrathematics/lineSampler")
```



## Package Use

```r
library(lineSampler)
ret <- sample_csv(file, p=.001)
```

There is also a `sample_lines()` function for reading in (line) subsamples of unstructured text akin to `readLines()`.  See package vignette for more details.



## Code Re-Use

The C code in the `src/` tree of this package can easily be re-purposed for use outside of R.  The utilities of interest are in:

  * file_sampler.c
  * wc.c

A uniform random number generator must be set. Set `RUNIF` to the appropriate function in `rand.h`.  You will also need to set `PRINTFUN` to something, probably `printf()`.  You will also need to set `STARTRNG` and `ENDRNG` appropriately (with the latter probably being an empty define).

Finally, to handle interrupts, you will need to create an appropriate `check_interrupt()` as in interrupt.c. Or if you're lazy and don't care, just set it to always return `false`.
