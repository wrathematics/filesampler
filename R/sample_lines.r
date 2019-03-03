#' Read Sample Lines of Text File
#' 
#' The function will read approximately p*nlines lines of a flat text
#' file. So if \code{p=.1}, then we will get roughly (probably not exactly)
#' 10% of the data.  This is the analogue of the base R function
#' \code{readLines()}.
#' 
#' @details
#' This function scans over the test of the input file and at each step, randomly
#' chooses whether or not to include the current line into a downsampled file.
#' Each selected line is placed in a temporary file, before being read into R
#' via \code{readLines()}.  Additional arguments to this function (those other
#' than \code{file}, \code{p}, and \code{verbose}) are passed to \code{readLines()},
#' and so if their behavior is unclear, you should examine the \code{readLines()}
#' help file.
#' 
#' If \code{verbose=TRUE}, then something like:
#' 
#' \code{Read 12207 lines (0.001\%) of 12174948 line file.}
#' 
#' will be printed to the terminal.  This counts the header (if there is one)
#' as one of the lines read and as one of the lines possible.
#' 
#' @param file
#' Location of the file (as a string) to be subsampled.
#' @param n
#' As in \code{readLines()}.
#' @param p
#' Proportion to retain; should be a numeric value between 0 and 1.
#' @param nskip
#' Number of lines to skip.
#' @param nmax
#' Max number of lines to read.  If nmax==0, then there is no read cap.
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' @param ...
#' Additional arguments passed to \code{readLines()}.
#' 
#' @return
#' A character vector, as with \code{readLines()}.
#' 
#' @examples
#' library(filesampler)
#' file = system.file("rawdata/small.csv", package="filesampler")
#' sample_lines(file, p=.05)
#'
#' @export
sample_lines = function(file, n=-1L, p=.1, nskip=0, nmax=0, verbose=FALSE, ...)
{
  check.is.int(n)
  
  if (p == 0)
    return(character(0))
  
  if (n > 0 && n < nskip)
    return(character(0))
  
  outfile = tempfile()
  file_sample_prob(verbose=verbose, header=FALSE, nskip=nskip, nmax=nmax, p=p, infile=file, outfile=outfile)
  
  data = readLines(outfile, n=n, ...)
  unlink(outfile)
  data
}
