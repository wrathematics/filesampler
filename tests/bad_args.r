library(lineSampler)

set.seed(1234)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="lineSampler"))


### p
badp <- "<simpleError in sample_file_prob(verbose = verbose, header = header, nskip = nskip,     nmax = nmax, p = p, infile = file, outfile = outfile): Argument 'p' must be between 0 and 1>"
badval <- tryCatch(sampled <- sample_csv(file, p=-1), error=capture.output)
stopifnot(all.equal(badp, badval))
badval <- tryCatch(sampled <- sample_csv(file, p=1.1), error=capture.output)
stopifnot(all.equal(badp, badval))
