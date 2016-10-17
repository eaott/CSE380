data = read.table("1.2.out.txt")
# Upon investigation, a reasonable model is that
# log(1 / y - 1) = c + log(n ^ -b) + noise
# which is basically just looking at the inverse of the
# sigmoid of the percent error (LHS) as a linear
# function of log(N) (RHS).
lm1 = lm(log(1:100) ~ log(1/data$V4-1))
write(paste("Rsquared", summary(lm1)$r.squared), "1.2.predict.txt")
err = 1 - 0.001
npredict = ceiling(exp(sum(coef(lm1) * c(1, log(1/err - 1)))))
write(paste("N", npredict), "1.2.predict.txt", append=TRUE)