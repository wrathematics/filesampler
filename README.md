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
