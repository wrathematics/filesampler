title_case <- function(x) gsub(x, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)


is.badval <- function(x)
{
   is.na(x) || is.nan(x) || is.infinite(x)
}

is.inty <- function(x)
{
  abs(x - round(x)) < 1e-10
}

is.zero <- function(x)
{
  abs(x) < 1e-10
}

is.negative <- function(x)
{
  x < 0
}

is.annoying <- function(x)
{
  length(x) != 1 || is.badval(x)
}



check.is.flag <- function(x)
{
  if (!is.logical(x) || is.annoying(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be TRUE or FALSE"))
  }
  
  invisible(TRUE)
}



check.is.scalar <- function(x)
{
  if (!is.numeric(x) || is.annoying(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be a single number (not NA, Inf, NaN)"))
  }
  
  invisible(TRUE)
}



check.is.string <- function(x)
{
  if (!is.character(x) || is.annoying(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be a single string"))
  }
  
  invisible(TRUE)
}



check.is.int <- function(x)
{
  if (!is.numeric(x) || is.annoying(x) || !is.inty(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be an integer"))
  }
  
  invisible(TRUE)
}



check.is.natnum <- function(x)
{
  if (!is.numeric(x) || is.annoying(x) || !is.inty(x) || is.negative(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be a natural number (0 or positive integer)"))
  }
  
  invisible(TRUE)
}



check.is.posint <- function(x)
{
  if (!is.numeric(x) || is.annoying(x) || !is.inty(x) || is.negative(x) || is.zero(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be a positive integer"))
  }
  
  invisible(TRUE)
}



check.is.function <- function(x)
{
  if (!is.function(x))
  {
    nm <- deparse(substitute(x))
    stop(paste0("argument '", nm, "' must be a function"))
  }
  
  invisible(TRUE)
}
