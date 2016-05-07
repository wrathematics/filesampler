library(lineSampler)

file <- tools::file_path_as_absolute(system.file("rawdata/small.csv", package="lineSampler"))

### Argument checks

# nlines
badp <- "<assertError: nlines is not a count (a single positive integer)>"
badval <- tryCatch(sampled <- sample_csv(file, param=-1, method="exact"), error=capture.output)
stopifnot(all.equal(badp, badval))



### general functionality
set.seed(1234)
sampled <- sample_csv(file, param=5, method="exact")

sampled_actual <-
structure(list(A = c(13L, 28L, 83L, 88L, 87L), B = structure(c(3L, 
2L, 4L, 4L, 1L), .Label = c("g", "n", "p", "q"), class = "factor"), 
    C = structure(c(5L, 4L, 1L, 3L, 2L), .Label = c("H", "S", 
    "V", "W", "Z"), class = "factor"), D = c(0.34508125577122, 
    0.573690160643309, 0.0123868996743113, 0.366345942718908, 
    0.260312086436898), E = c(-0.455623686756772, 1.48579320914896, 
    0.680072131251977, -0.114730600414802, -0.0554640013763592
    ), F = c(40.096492273733, 72.228856375441, 84.4729636912234, 
    92.6017211284488, 92.6275063771755)), .Names = c("A", "B", 
"C", "D", "E", "F"), class = "data.frame", row.names = c(NA, 
-5L))


stopifnot(all.equal(sampled, sampled_actual))
