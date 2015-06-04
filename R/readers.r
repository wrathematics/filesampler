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
#' NULL
#' 
#' @seealso \code{\link{file_sampler_exact}}
#' 
#' @export
file_sampler <- function(verbose, header, nskip, p, infile, outfile=tempfile())
{
  must_be(verbose, "logical")
  must_be(header, "logical")
  must_be(nskip, "int")
  must_be(p, "numeric")
  must_be(infile, "character")
  
  if (p < 0 || p > 1)
    stop("Argument 'p' must be between 0 and 1")
  
  infile <- tools::file_path_as_absolute(infile)
  outfile <- tools::file_path_as_absolute(outfile)
  
  ret <- .Call(R_file_sampler, verbose, header, as.integer(nskip), as.double(p), infile, outfile)
  
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
#' NULL
#' 
#' @seealso \code{\link{file_sampler}}
#' 
#' @export
file_sampler_exact <- function(header, nskip, nlines, infile, outfile=tempfile())
{
  must_be(header, "logical")
  must_be(nskip, "int")
  must_be(nlines, "int")
  must_be(infile, "character")
  
  infile <- tools::file_path_as_absolute(infile)
  outfile <- tools::file_path_as_absolute(outfile)
  
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
#' @param header,nskip,sep,quote,dec,fill,comment.char,...
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
#' @seealso \code{\link{readLines_sampled}, \link{wc}}
#' 
#' @examples \dontrun{
#' library(lineSampler)
#' file <- "/path/to/my/big.csv"
#' data  <- read_csv_sampled(file=file)
#' }
#'
#' @export
read_csv_sampled <- function(file, p=.1, header=TRUE, nskip=0, sep=",", quote="\"", dec=".", fill=TRUE, comment.char="", verbose=FALSE, ...)
{
  if (p == 0)
    stop("no lines available for input")
  
  outfile <- tempfile()
  file_sampler(verbose=verbose, header=header, nskip=nskip, p=p, infile=file, outfile=outfile)
  
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
#' @param n,ok,warn,encoding,skipNul
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
#' @seealso \code{\link{read_csv_sampled}, \link{wc}}
#' 
#' @examples \dontrun{
#' library(lineSampler)
#' file <- "/path/to/my/big.txt"
#' data  <- readLines_sampled(file=file)
#' }
#'
#' @export
readLines_sampled <- function(file, p=.1, nskip=0, n=-1L, ok=TRUE, warn=TRUE, encoding="unknown", skipNul=FALSE, verbose=FALSE)
{
  if (p == 0)
    return(character(0))
  
  if (n > 0 && n < nskip)
    return(character(0))
  
  outfile <- tempfile()
  file_sampler(verbose=verbose, header=FALSE, nskip=nskip, p=p, infile=file, outfile=outfile)
  
  data <- readLines(outfile, n=n, ok=ok, warn=warn, encoding=encoding, skipNul=skipNul)
  unlink(outfile)
  
  return(data)
}

