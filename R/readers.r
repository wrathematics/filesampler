file_sampler <- function(verbose, header, nskip, p, infile)
{
  must_be(verbose, "logical")
  must_be(header, "logical")
  must_be(nskip, "int")
  must_be(p, "numeric")
  must_be(infile, "character")
  
  if (p < 0 || p > 1)
    stop("Argument 'p' must be between 0 and 1")
  
  infile <- tools::file_path_as_absolute(infile)
  
  outfile <- tempfile()
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
  
  return(outfile)
}



file_sampler_exact <- function(header, nskip, nlines, infile)
{
  must_be(header, "logical")
  must_be(nskip, "int")
  must_be(nlines, "int")
  must_be(infile, "character")
  
  infile <- tools::file_path_as_absolute(infile)
  
  outfile <- tempfile()
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
  
  return(outfile)
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
#' @param header
#' Logical; indicates whether or not there is a header on the csv file.
#' @param nskip
#' Number of lines to skip.  If \code{header=TRUE}, then this only
#' applies to lines after the header.
#' @param sep
#' Separator character.
#' @param quote
#' Quote character for delimiting columns read as strings.
#' @param dec
#' "Decimal point" character.
#' @param fill
#' Logical; standardizes rows of unequal length to have implicit blanks added.
#' @param comment.char
#' Comments character.
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' @param ...
#' Additional arguments to be passed to \code{read.csv()}.
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
  
  outfile <- file_sampler(verbose=verbose, header=header, nskip=nskip, p=p, infile=file)
  
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
#' @param n
#' The max number of lines to read. Negative value means read everything.
#' @param ok
#' Logical; Ok to reach end of connection before \code{n} lines are read? Only
#' relevant when \code{n>0}.
#' @param warn
#' Logical; warn if file is missing EOL.
#' @param encoding
#' Character encoding for strings.
#' @param skipNul
#' Logical; should \code{nul}'s be skipped?
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
  
  outfile <- file_sampler(verbose=verbose, header=FALSE, nskip=nskip, p=p, infile=file)
  
  data <- readLines(outfile, n=n, ok=ok, warn=warn, encoding=encoding, skipNul=skipNul)
  unlink(outfile)
  
  return(data)
}

