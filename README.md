# filesampler

* **Version:** 0.4-0
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/filesampler.png)](https://travis-ci.org/wrathematics/filesampler)
* **License:** [BSD 2-Clause](http://opensource.org/licenses/BSD-2-Clause)
* **Project home**: https://github.com/wrathematics/lineSampler
* **Bug reports**: https://github.com/wrathematics/lineSampler/issues
* **Author:** Drew Schmidt



This is a simple R package to quickly read a random sample of lines of a flat text file (such as a csv) into R.  This allows you to get a subsample into R without having to read the (possibly large) file into memory first.

If you would like to contribute to this project, please see the `CONTRIBUTING.md` file.  Idea inspired by Eduardo Arino de la Rubia's [fast_sample](https://github.com/earino/fast_sample).



## Installation

You can install the stable version from [the HPCRAN](https://hpcran.org) using the usual `install.packages()`:

```r
install.packages("filesampler", repos="https://hpcran.org")
```

The development version is maintained on GitHub:


```r
remotes::install_github("wrathematics/filesampler")
```



## Package Use

```r
library(filesampler)
ret <- sample_csv(file, p=.001)
```

There is also a `sample_lines()` function for reading in (line) subsamples of unstructured text akin to `readLines()`.  See package vignette for more details.

For more information, including benchmarks and implementation details, please see the package vignette:

```r
vignette("filesampler", package="filesampler")
```



## Code Re-Use

The C code in the `src/filesampler` tree of this package can easily be re-purposed for use outside of R with some minor modifications.  The components that need to be edited can be found in the file:

  * `src/filesampler/utils.h`

Detailed explanations are contained there, but you will need:

* a uniform random number generator
* a print function (probably `printf()`)
* an interrupt checker
