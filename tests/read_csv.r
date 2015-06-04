library(lineSampler)

set.seed(1234)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="lineSampler"))


### read_csv_sampled
sampled <- read_csv_sampled(file, p=.05)

sampled_actual <-
structure(list(A = c(30L, 92L, 6L, 49L, 77L, 54L, 67L), B = structure(c(2L, 
1L, 3L, 2L, 4L, 1L, 5L), .Label = c("b", "m", "p", "r", "w"), class = "factor"), 
    C = structure(c(4L, 1L, 1L, 3L, 2L, 2L, 3L), .Label = c("B", 
    "G", "P", "U"), class = "factor"), D = c(0.663265228504315, 
    0.647247697459534, 0.145372492726892, 0.647244467865676, 
    0.248602577485144, 0.155096606584266, 0.231593899428844), 
    E = c(-0.102609033060013, 1.20942736228284, 1.52611855611189, 
    -0.59059429990917, 0.356037725132404, -0.933558833688986, 
    0.712171107833738), F = c(46.2487461487763, 34.8187624011189, 
    20.8286189078353, 91.2623915518634, 42.5667992956005, 17.3642539186403, 
    47.2880723653361)), .Names = c("A", "B", "C", "D", "E", "F"
), class = "data.frame", row.names = c(NA, -7L))

stopifnot(all.equal(sampled, sampled_actual))


### verbose
verb <- capture.output(invisible(read_csv_sampled(file, p=.05, verbose=TRUE)))
verb_actual <- "Read 4 lines (0.040%) of 101 line file."
stopifnot(all.equal(verb, verb_actual))
