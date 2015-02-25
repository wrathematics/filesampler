#' File Line Sampler
#' 
#' A simple package for reading subsamples of files (primarily csv's)
#' by line in a reasonably efficient manner.  We do so by sampling as the
#' input file is scanned and randomly choosing whether or not to
#' dump the current line to an external temporary file.  This is
#' done in a single pass.
#'
#' The heavy lifting is all done in C.
#'
#' This package, including the underlying C library, is licensed under
#' the permissive 2-clause BSD license.
#'
#' @useDynLib lineSampler, R_file_sampler, R_wc
#'
#' @name lineSampler
#' @docType package
#' @title File Line Sampler
#' @author Drew Schmidt \email{wrathematics@@gmail.com}
#' @keywords package
NULL
