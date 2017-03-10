#(1) change directory
library('cycleanalysis')
library('igraph')
library('statnet')


nodes <- read.csv("sample_data/Dataset1-Media-Example-NODES.csv", header=T, as.is=T)
edges <- read.csv("sample_data/Dataset1-Media-Example-EDGES.csv", header=T, as.is=T)

net <- graph_from_data_frame(d=edges, vertices=nodes, directed=T)
adjacencyMat <- get.adjacency(net, sparse=FALSE)
edgeMat <- as.matrix(edges, sparse=FALSE)
plot(net, edge.arrow.size=.1)

# maxlen is the max cycle length to search for
cc<-kcycle.censusExtension(adjacencyMat, edgeMat, maxlen = 10, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")

