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
LS_sample_prob <- function(verbose, header, nskip, nmax, p, infile, outfile=tempfile())
{
  check.is.flag(verbose)
  check.is.flag(header)
  check.is.natnum(nskip)
  check.is.natnum(nmax)
  check.is.scalar(p)
  check.is.string(infile)
  
  if (p == 0)
    stop("no lines available for input")
  if (p < 0 || p > 1)
    stop("Argument 'p' must be between 0 and 1")
  
  infile <- tools::file_path_as_absolute(infile)
  
  ret <- .Call(R_LS_sample_prob, verbose, header, as.integer(nskip), as.integer(nmax), as.double(p), infile, outfile)
  
  invisible()
}



#' @title 
#' Exact File Sampler
#' 
#' @description 
#' Randomly sample exactly \code{nlines} lines from an input text 
#' file.
#' 
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
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
LS_sample_exact <- function(verbose, header, nskip, nlines, infile, outfile=tempfile())
{
  check.is.flag(verbose)
  check.is.flag(header)
  check.is.natnum(nskip)
  check.is.posint(nlines)
  check.is.natnum(nskip)
  check.is.posint(nlines)
  check.is.string(infile)
  
  infile <- tools::file_path_as_absolute(infile)
  
  ret <- .Call(R_LS_sample_exact, as.integer(verbose), as.integer(header), as.integer(nskip), as.integer(nlines)-1L, infile, outfile)
  
  invisible()
}
