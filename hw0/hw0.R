library(rworldmap)
library(reshape2)
library(xtable)

top500 = read.csv("/Users/Evan/Dropbox/A-Grad-School/CSE380/TOP500_201606.csv")
countryData = 100 * colSums(xtabs(~ top500$First.Rank + top500$Country)[1:9,]) /
  colSums(xtabs(~ top500$First.Rank + top500$Country))

countryData = melt(countryData)

df = data.frame(Country = labels(countryData)[[1]],
           Value = countryData[[1]])

#join to a coarse resolution map
spdf <- joinCountryData2Map(df,
                            joinCode = "NAME",
                            nameJoinColumn = "Country")

mapCountryData(spdf,
               nameColumnToPlot = "Value",
               catMethod = "fixedWidth",
               oceanCol = 'lightblue',
               colourPalette = brewer.pal(9, "PuBu")[2:8],
               missingCountryCol = '#ffffff',
               borderCol = 'black',
               mapTitle = 'Percent of Country\'s Supercomputers\nDebuting in Top 10')

print(xtable(subset(df[order(df$Value, decreasing = TRUE),], Value > 0)), include.rownames = FALSE)




segmentData = 100 * colSums(xtabs(~ top500$First.Rank + top500$Segment)[1:19,]) /
  colSums(xtabs(~ top500$First.Rank + top500$Segment))

segmentData = melt(segmentData)

segmentdf = data.frame(Segment = labels(segmentData)[[1]],
                Value = segmentData[[1]])
barplot(segmentdf$Value,
        names.arg = segmentdf$Segment,
        col = '#7777ee',
        main = "Percent of Segment\'s Supercomputers\nDebuting in Top 20", las=2)

print(xtable(segmentdf[order(segmentdf$Value, decreasing = TRUE), ]), include.rownames = FALSE)



