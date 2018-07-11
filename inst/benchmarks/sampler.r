library(filesampler)


system.time({
    x <- sample_csv("big.csv", method="exact", param=15000, verbose=TRUE)
})


system.time({
    x <- sample_csv("big.csv", param=.001, verbose=TRUE)
})
