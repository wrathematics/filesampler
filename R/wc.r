#' @title 
#' Count Letters, Words, and Lines of a File
#' 
#' @description 
#' See title.
#' 
#' @details
#' The summary statistics of the input file are determined by
#' scanning through the file as quickly as possible (i.e., it
#' should be completely I/O bound).
#' 
#' @param file
#' Location of the file (as a string) from which the counts will be generated.
#' @param chars,words,Lines
#' Logical; show char/word/line counts be shown? At least one of
#' the three must be \code{TRUE}.
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
wc <- function(file, chars=TRUE, words=TRUE, lines=TRUE)
{
  assert_that(is.string(file))
  assert_that(is.flag(chars))
  assert_that(is.flag(words))
  assert_that(is.flag(lines))
  
  if (!chars && !words && !lines)
    stop("at least one of the arguments 'chars', 'words', or 'lines' must be TRUE")
  
  file <- tools::file_path_as_absolute(file)
  
  ret <- .Call(R_wc, file, chars, words, lines)
  
  if (as.integer(ret[1]) == -2)
    stop("Invalid argument 'infile'; perhaps it doesn't exist?")
  
  counts <- list(chars=ret[2], words=ret[3], lines=ret[4])
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
  
  x <- x[which(x != -1)]
  
  maxlen <- max(sapply(names(x), nchar))
  names <- gsub(names(x), pattern="_", replacement=" ")
  names <- title_case(x=names)
  spacenames <- simplify2array(lapply(names, function(str) paste0(str, ":", paste0(rep(" ", maxlen-nchar(str)), collapse=""))))
  
  cat(paste(spacenames, x, sep=" ", collapse="\n"), "\n")
  invisible()
}
