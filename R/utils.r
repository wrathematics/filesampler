must_be <- function(x, type)
{
  Rstuff <- c("character", "numeric", "integer", "double", "logical", "matrix", "data.frame", "vector")
  type <- match.arg(type, Rstuff)
  
  nm <- deparse(substitute(x))
  
  fun <- eval(parse(text=paste("is.", type, sep="")))
  
  if (!fun(x))
      stop(paste("argument '", nm, "' must be of type ", type, sep=""), call.=FALSE)
  
  return(invisible(TRUE))
}



title_case <- function(x) gsub(x, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)

