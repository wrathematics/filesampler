#' Count Letters, Words, and Lines of a File
#' 
#' @description 
#' See title.
#' 
#' \code{wc_l()} is a shorthand for counting only lines, similar to \code{wc -l}
#' in the terminal.
#' 
#' @details
#' The summary statistics of the input file are determined by
#' scanning through the file as quickly as possible (i.e., it
#' should be completely I/O bound).
#' 
#' @param file
#' Location of the file (as a string) from which the counts will be generated.
#' @param chars,words,lines
#' Logical; show char/word/line counts be shown? At least one of
#' the three must be \code{TRUE}.
#' 
#' @return
#' A list containing the requested counts.
#' 
#' @seealso \code{\link{sample_csv}, \link{sample_lines}}
#' 
#' @examples \dontrun{
#' library(filesampler)
#' file = "/path/to/my/big.txt"
#' data  = wc(file=file)
#' }
#'
#' @name wc
#' @rdname wc
NULL


#' @useDynLib filesampler R_fs_wc
#' @rdname wc
#' @export
wc = function(file, chars=TRUE, words=TRUE, lines=TRUE)
{
  check.is.string(file)
  check.is.flag(chars)
  check.is.flag(words)
  check.is.flag(lines)
  
  if (!chars && !words && !lines)
    stop("at least one of the arguments 'chars', 'words', or 'lines' must be TRUE")
  
  file = tools::file_path_as_absolute(file)
  ret = .Call(R_fs_wc, file, chars, words, lines)
  
  counts = list(chars=ret[1L], words=ret[2L], lines=ret[3L])
  class(counts) = "wc"
  attr(counts, "file") = file
  
  counts
}


#' @rdname wc
#' @export
wc_l = function(file)
{
  wc(file=file, chars=FALSE, words=FALSE, lines=TRUE)
}



#' @title Print \code{wc} objects
#' @description Printing for \code{wc()}
#' @param x \code{wc} object
#' @param ... unused
#' @name print-wc
#' @rdname print-wc
#' @method print wc
#' @export
print.wc = function(x, ...)
{
  cat("file:  ", attr(x, "file"), "\n")
  
  x = x[which(x != -1)]
  
  maxlen = max(sapply(names(x), nchar))
  names = gsub(names(x), pattern="_", replacement=" ")
  names = title_case(x=names)
  spacenames = simplify2array(lapply(names, function(str) paste0(str, ":", paste0(rep(" ", maxlen-nchar(str)), collapse=""))))
  
  cat(paste(spacenames, x, sep=" ", collapse="\n"), "\n")
  invisible()
}
