#' @title Read Sample of CSV
#' 
#' @description 
#' The function will read (as csv) approximately p*nlines lines. So 
#' if \code{p=.1}, then we will get roughly (probably not exactly)
#' 10% of the data.  This is the analogue of the base R function
#' \code{read.csv()}.
#' 
#' @param file
#' Location of the file (as a string) to be subsampled.
#' @param p
#' Proportion to retain; should be a numeric value between 0 and 1.
#' @param nskip
#' Number of lines to skip.  If \code{header=TRUE}, then this only
#' applies to lines after the header.
#' @param nmax
#' Max number of lines to read.  If nmax==0, then there is no read cap.
#' @param header,sep,quote,dec,fill,comment.char,...
#' As in \code{read.csv()}
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' 
#' @details
#' This function scans over the test of the input file and at each step, randomly
#' chooses whether or not to include the current line into a downsampled file.
#' Each selected line is placed in a temporary file, before being read into R
#' via \code{read.csv()}.  Additional arguments to this function (those other
#' than \code{file}, \code{p}, and \code{verbose}) are passed to \code{read.csv()},
#' and so if their behavior is unclear, you should examine the \code{read.csv()}
#' help file.
#' 
#' If \code{verbose=TRUE}, then something like:
#' 
#' \code{Read 12207 lines (0.001\%) of 12174948 line file.}
#' 
#' will be printed to the terminal.  This counts the header (if there is one)
#' as one of the lines read and as one of the lines possible.
#' 
#' 
#' @return
#' A dataframe, as with \code{read.csv()}.
#' 
#' @seealso \code{\link{sample_lines}, \link{wc}}
#' 
#' @examples \dontrun{
#' library(lineSampler)
#' file <- "/path/to/my/big.csv"
#' data  <- sample_csv(file=file)
#' }
#'
#' @export
sample_csv <- function(file, p=.05, header=TRUE, nskip=0, nmax=0, sep=",", quote="\"", dec=".", fill=TRUE, comment.char="", verbose=FALSE, ...)
{
  if (p == 0)
    stop("no lines available for input")
  
  outfile <- tempfile()
  sample_file_prob(verbose=verbose, header=header, nskip=nskip, nmax=nmax, p=p, infile=file, outfile=outfile)
  
  data <- read.csv(file=outfile, header=header, sep=sep, quote=quote, dec=dec, fill=fill, comment.char=comment.char, ...)
  unlink(outfile)
  
  return(data)
}
