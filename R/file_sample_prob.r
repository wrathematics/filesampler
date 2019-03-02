#' Proportional File Sampler
#' 
#' Randomly sample lines from an input text file at proportion p.
#' 
#' @details
#' The sampling is done in one pass of the input file, dumping lines to a
#' temporary file as the input is read.
#' 
#' If the output file (the one pointed to by the return of this function) is
#' "large" and to be read into memory (which isn't really appropriate for text
#' files in the first place!), then this strategy is probably not appropriate.
#' 
#' @param verbose
#' Logical; indicates whether or not linecounts of the input file and the number
#' of lines sampled should be printed.
#' @param header
#' Logical; indicates whether or not there is a header on the csv file.
#' @param nskip
#' Number of lines to skip.  If \code{header=TRUE}, then this only applies to
#' lines after the header.
#' @param nmax
#' Max number of lines to read.  If nmax==0, then there is no read cap.
#' @param p
#' Proportion to retain; should be a numeric value between 0 and 1.
#' @param infile
#' Location of the file (as a string) to be subsampled.
#' @param outfile
#' Output file.  Default is a temporary file.
#' 
#' @return
#' \code{NULL}
#' 
#' @useDynLib filesampler R_fs_sample_prob
#' @export
LS_sample_prob = function(verbose, header, nskip, nmax, p, infile, outfile=tempfile())
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
  
  infile = abspath(infile)
  .Call(R_fs_sample_prob, verbose, header, as.integer(nskip), as.integer(nmax), as.double(p), infile, outfile)
  
  invisible()
}
