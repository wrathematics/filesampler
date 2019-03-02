title_case = function(x) gsub(x, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)

abspath = function(file)
{
  p = path.expand(file)
  if (!file.exists(p))
    stop("file does not exist")
  
  p
}
