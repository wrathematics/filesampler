library(filesampler)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="filesampler"))

nchars = 6426
nwords = 101
nlines = 101

truth = c(nchars, nwords, nlines)
test = as.integer(wc(file))
stopifnot(all.equal(truth, test))

truth = c(-1, -1, nlines)
test = as.integer(wc(file, FALSE, FALSE))
stopifnot(all.equal(truth, test))

truth = c(nchars, -1, nlines)
test = as.integer(wc(file, TRUE, FALSE, TRUE))
stopifnot(all.equal(truth, test))

truth = c(nchars, nwords, -1)
test = as.integer(wc(file, lines=FALSE))
stopifnot(all.equal(truth, test))

truth = c(nchars, -1, -1)
test = as.integer(wc(file, words=FALSE, lines=FALSE))
stopifnot(all.equal(truth, test))
