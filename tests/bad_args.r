library(lineSampler)

set.seed(1234)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="lineSampler"))


### p
badp <- "<assertError: nskip is not a count (a single positive integer)>"
badval <- tryCatch(sampled <- read_csv_sampled(file, p=-1), error=capture.output)
stopifnot(all.equal(badp, badval))
badval <- tryCatch(sampled <- read_csv_sampled(file, p=1.1), error=capture.output)
stopifnot(all.equal(badp, badval))
