library(filesampler)

file <- system.file("rawdata/small.csv", package="filesampler")

### Argument checks

# p
badp <- "<simpleError in LS_sample_prob(verbose = verbose, header = header, nskip = nskip,     nmax = nmax, p = p, infile = file, outfile = outfile): Argument 'p' must be between 0 and 1>"
badval <- tryCatch(sampled <- sample_csv(file, param=-1), error=capture.output)
stopifnot(all.equal(badp, badval))
badval <- tryCatch(sampled <- sample_csv(file, param=1.1), error=capture.output)
stopifnot(all.equal(badp, badval))


# nmax
set.seed(1234)
sampled <- sample_csv(file, param=.05, nmax=1)

sampled_actual <-
structure(list(A = 30L, B = structure(1L, .Label = "m", class = "factor"), 
    C = structure(1L, .Label = "U", class = "factor"), D = 0.663265228504315, 
    E = -0.102609033060013, F = 46.2487461487763), .Names = c("A", 
"B", "C", "D", "E", "F"), class = "data.frame", row.names = c(NA, 
-1L))

stopifnot(all.equal(sampled, sampled_actual))


### general functionality
set.seed(1234)
sampled <- sample_csv(file, param=.05)

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
verb <- capture.output(invisible(sample_csv(file, param=.05, verbose=TRUE)))
verb_actual <- "Read 4 lines (0.03960%) of 101 line file."
stopifnot(all.equal(verb, verb_actual))
