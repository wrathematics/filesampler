#' @title 
#' File Sampler
#' 
#' @description 
#' Randomly sample lines from an input text file at proportion p.
#' 
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' @param header
#' Logical; indicates whether or not there is a header on the csv file.
#' @param nskip
#' Number of lines to skip.  If \code{header=TRUE}, then this only
#' applies to lines after the header.
#' @param nmax
#' Max number of lines to read.  If nmax==0, then there is no read cap.
#' @param p
#' Proportion to retain; should be a numeric value between 0 and 1.
#' @param infile
#' Location of the file (as a string) to be subsampled.
#' @param outfile
#' Output file.  Default is a temporary file.
#' 
#' @details
#' The sampling is done in one pass of the input file, dumping
#' lines to a temporary file as the input is read.
#' 
#' If the output file (the one pointed to by the return of this 
#' function) is "large" and to be read into memory (which isn't 
#' really appropriate for text files in the first place!), then 
#' this strategy is probably not appropriate.
#' 
#' @return
#' \code{NULL}
#' 
#' @seealso \code{\link{sample_file_exact}}
#' 
#' @export
sample_file_prob <- function(verbose, header, nskip, nmax, p, infile, outfile=tempfile())
{
  assert_that(is.flag(verbose))
  assert_that(is.flag(header))
  assert_that(nskip == 0 || is.count(nskip))
  assert_that(nmax == 0 || is.count(nmax))
  assert_that(is.scalar(p))
  assert_that(is.string(infile))
  
  if (p < 0 || p > 1)
    stop("Argument 'p' must be between 0 and 1")
  
  infile <- tools::file_path_as_absolute(infile)
  
  ret <- .Call(R_file_sampler, verbose, header, as.integer(nskip), as.integer(nmax), as.double(p), infile, outfile)
  
  if (ret < 0)
  {
    unlink(outfile)
    
    if (ret == -2)
      stop("Invalid argument 'infile'; perhaps it doesn't exist?")
    else if (ret == -3)
      stop("Could not generate tempfile for writing for some reason?")
    else
      stop("what?")
  }
  
  invisible()
}



#' @title 
#' Exact File Sampler
#' 
#' @description 
#' Randomly sample exactly \code{nlines} lines from an input text 
#' file.
#' 
#' @param header
#' Logical; indicates whether or not there is a header on the csv file.
#' @param nskip
#' Number of lines to skip.  If \code{header=TRUE}, then this only
#' applies to lines after the header.
#' @param nlines
#' The (exact) number of lines to sample from the input file.
#' @param infile
#' Location of the file (as a string) to be subsampled.
#' @param outfile
#' Output file.  Default is a temporary file.
#' 
#' @details
#' The sampling is done in two passes of the input file.  First, 
#' the number of lines of the input file are determined by
#' scanning through the file as quickly as possible (i.e., it
#' should be completely I/O bound).  Next, an index of lines
#' to keep is produced by reservoir sampling.  Then finally, 
#' the input file is scanned again line by line with the chosen
#' lines dumped into a temporary file.
#' 
#' If the output file (the one pointed to by the return of this 
#' function) is "large" and to be read into memory (which isn't 
#' really appropriate for text files in the first place!), then 
#' this strategy is probably not appropriate.
#' 
#' @return
#' \code{NULL}
#' 
#' @seealso \code{\link{sample_file_prob}}
#' 
#' @export
sample_file_exact <- function(header, nskip, nlines, infile, outfile=tempfile())
{
  assert_that(is.flag(header))
  assert_that(is.count(nskip))
  assert_that(is.count(nlines))
  assert_that(is.string(infile))
  
  infile <- tools::file_path_as_absolute(infile)
  
  ret <- .Call(R_file_sampler_exact, header, as.integer(nskip), as.integer(nlines), infile, outfile)
  
  if (ret < 0)
  {
    unlink(outfile)
    
    if (ret == -2)
      stop("Invalid argument 'infile'; perhaps it doesn't exist?")
    else if (ret == -3)
      stop("Could not generate tempfile for writing for some reason?")
    else
      stop("what?")
  }
  
  invisible()
}



#' @title Read Sample of CSV
#' 
#' @description 
#' The function will read (as csv) approximately p*nlines lines. So 
#' if \code{p=.1}, then we will get roughly (probably not exactly)
#' 10% of the data.
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



#' @title Read Sample Lines of Text File
#' 
#' @description 
#' The function will read approximately p*nlines lines of a flat text
#' file. So if \code{p=.1}, then we will get roughly (probably not exactly)
#' 10% of the data.
#' 
#' @param file
#' Location of the file (as a string) to be subsampled.
#' @param p
#' Proportion to retain; should be a numeric value between 0 and 1.
#' @param nskip
#' Number of lines to skip.
#' @param nmax
#' Max number of lines to read.  If nmax==0, then there is no read cap.
#' @param n,ok,warn,encoding,skipNul
#' As in \code{readLines()}.
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' 
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
#' 
#' @return
#' A character vector, as with \code{readLines()}.
#' 
#' @seealso \code{\link{sample_csv}, \link{wc}}
#' 
#' @examples \dontrun{
#' library(lineSampler)
#' file <- "/path/to/my/big.txt"
#' data  <- sample_lines(file=file)
#' }
#'
#' @export
sample_lines <- function(file, p=.1, nskip=0, nmax=0, n=-1L, ok=TRUE, warn=TRUE, encoding="unknown", skipNul=FALSE, verbose=FALSE)
{
  if (p == 0)
    return(character(0))
  
  if (n > 0 && n < nskip)
    return(character(0))
  
  outfile <- tempfile()
  sample_file_prob(verbose=verbose, header=FALSE, nskip=nskip, nmax=nmax, p=p, infile=file, outfile=outfile)
  
  data <- readLines(outfile, n=n, ok=ok, warn=warn, encoding=encoding, skipNul=skipNul)
  unlink(outfile)
  
  return(data)
}
