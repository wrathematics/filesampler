library(lineSampler)

set.seed(1234)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="lineSampler"))


### read_csv_sampled
sampled <- sample_csv(file, p=.05, nmax=1)

sampled_actual <-
structure(list(A = 30L, B = structure(1L, .Label = "m", class = "factor"), 
    C = structure(1L, .Label = "U", class = "factor"), D = 0.663265228504315, 
    E = -0.102609033060013, F = 46.2487461487763), .Names = c("A", 
"B", "C", "D", "E", "F"), class = "data.frame", row.names = c(NA, 
-1L))

stopifnot(all.equal(sampled, sampled_actual))
