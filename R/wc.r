#' @title 
#' Count Letters, Words, and Lines of a File
#' 
#' @description 
#' See title.
#' 
#' @param file
#' Location of the file (as a string) from which the counts will be generated.
#' 
#' @details
#' The summary statistics of the input file are determined by
#' scanning through the file as quickly as possible (i.e., it
#' should be completely I/O bound).
#' 
#' @return
#' A list containing the requested counts.
#' 
#' @seealso \code{\link{sample_csv}, \link{sample_lines}}
#' 
#' @examples \dontrun{
#' library(lineSampler)
#' file <- "/path/to/my/big.txt"
#' data  <- wc(file=file)
#' }
#'
#' @export
wc <- function(file)
{
  assert_that(is.string(file))
  
  file <- tools::file_path_as_absolute(file)
  
  ret <- .Call(R_wc, file)
  
  if (as.integer(ret[1]) == -2)
    stop("Invalid argument 'infile'; perhaps it doesn't exist?")
  
  counts <- list(nletters=ret[2], nwords=ret[3], nlines=ret[4])
  class(counts) <- "wc"
  attr(counts, "file") <- file
  
  return(counts)
}



#' @title Print \code{wc} objects
#' @description Printing for \code{wc()}
#' @param x \code{wc} object
#' @param ... unused
#' @name print-wc
#' @rdname print-wc
#' @method print wc
#' @export
print.wc <- function(x, ...)
{
  cat("file:  ", attr(x, "file"), "\n")
  
  maxlen <- max(sapply(names(x), nchar))
  names <- gsub(names(x), pattern="_", replacement=" ")
  names <- title_case(x=names)
  spacenames <- simplify2array(lapply(names, function(str) paste0(str, ":", paste0(rep(" ", maxlen-nchar(str)), collapse=""))))
  
  cat(paste(spacenames, x, sep=" ", collapse="\n"), "\n")
  invisible()
}
