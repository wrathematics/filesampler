#' Exact File Sampler
#' 
#' Randomly sample exactly \code{nlines} lines from an input text file.
#' 
#' @details
#' The sampling is done in two passes of the input file.  First, the number of
#' lines of the input file are determined by scanning through the file as
#' quickly as possible (i.e., it should be completely I/O bound).  Next, an
#' index of lines to keep is produced by reservoir sampling.  Then finally,  the
#' input file is scanned again line by line with the chosen lines dumped into a
#' temporary file.
#' 
#' If the output file (the one pointed to by the return of this function) is
#' "large" and to be read into memory (which isn't really appropriate for text
#' files in the first place!), then this strategy is probably not appropriate.
#' 
#' @param nlines
#' The (exact) number of lines to sample from the input file.
#' @param infile
#' Location of the file (as a string) to be subsampled.
#' @param outfile
#' Output file location (as a string).
#' @param header
#' Is a header (line of column names) on the first line of the csv file?
#' @param nskip
#' Number of lines to skip. If \code{header=TRUE}, then this only applies to
#' lines after the header.
#' @param verbose
#' Should linecounts of the input file and the number of lines sampled be
#' printed?
#' 
#' @return
#' \code{NULL}
#' 
#' @useDynLib filesampler R_fs_sample_exact
#' @export
file_sample_exact = function(nlines, infile, outfile=tempfile(), header=TRUE, nskip=0, verbose=FALSE)
{
  check.is.posint(nlines)
  check.is.string(infile)
  infile = abspath(infile)
  check.is.string(outfile)
  check.is.flag(header)
  check.is.natnum(nskip)
  check.is.flag(verbose)
  
  .Call(R_fs_sample_exact, as.integer(verbose), as.integer(header), as.integer(nskip), as.integer(nlines)-1L, infile, outfile)
  
  invisible()
}
