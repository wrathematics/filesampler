file_sampler <- function(header, p, infile)
{
  must_be(header, "logical")
  must_be(p, "numeric")
  must_be(infile, "character")
  
  if (p < 0 || p > 1)
    stop("Argument 'p' must be between 0 and 1")
  
  infile <- tools::file_path_as_absolute(infile)
  
  outfile <- tempfile()
  ret <- .Call(R_file_sampler, as.logical(header), as.double(p), infile, outfile)
  
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
#' 
#' @details
#' TODO
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
read_csv_sampled <- function(file, p=.1, header=TRUE)
{
  outfile <- file_sampler(header=header, p=p, infile=file)
  
  data <- read.csv(file=outfile, header=header)
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
#' 
#' @details
#' TODO
#' 
#' @return
#' A character vector \code{readLines()}.
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
readLines_sampled <- function(file, p=.1)
{
  outfile <- file_sampler(header=FALSE, p=p, infile=file)
  
  data <- readLines(outfile)
  unlink(outfile)
  
  return(data)
}

