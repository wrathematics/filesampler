# lineSampler

* **Version:** 0.4-0
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/lineSampler.png)](https://travis-ci.org/wrathematics/lineSampler)
* **License:** [BSD 2-Clause](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt
* **Project home**: https://github.com/wrathematics/lineSampler
* **Bug reports**: https://github.com/wrathematics/lineSampler/issues



This is a simple R package to quickly read a random sample of lines of a flat text file (such as a csv) into R.  This allows you to get a subsample into R without having to read the (possibly large) file into memory first.

If you would like to contribute to this project, please see the `CONTRIBUTING.md` file.  Idea inspired by Eduardo Arino de la Rubia's [fast_sample](https://github.com/earino/fast_sample).



## Installation

From CRAN:

```r
install.packages("lineSampler")
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

For more information, including benchmarks and implementation details, please see the package vignette:

```r
vignette("lineSampler", package="lineSampler")
```



## Code Re-Use

The C code in the `src/lineSampler` tree of this package can easily be re-purposed for use outside of R with some minor modifications.  The components that need to be edited can be found in the file:

  * `src/lineSampler/utils.h`

Detailed explanations are contained there, but you will need:

* a uniform random number generator
* a print function (probably `printf()`)
* an interrupt checker
