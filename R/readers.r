#' @title 
#' Proportional File Sampler
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
