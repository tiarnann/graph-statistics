#(1) change directory
library('cycleanalysis')
library('igraph')
library('statnet')

nodes <- read.csv("sample_data/Dataset_nodes.csv", header=T, as.is=T)
edges <- read.csv("sample_data/Dataset_edges.csv", header=T, as.is=T)

#nodes <- read.csv("sample_data/Dataset1-Media-Example-NODES.csv", header=T, as.is=T)
#edges <- read.csv("sample_data/Dataset1-Media-Example-EDGES.csv", header=T, as.is=T)
net <- graph_from_data_frame(d=edges, vertices=nodes, directed=T)
adjacencyMat <- get.adjacency(net, sparse=FALSE)
edgeMat <- as.matrix(edges, sparse=FALSE)
plot(net, edge.arrow.size=0.01, edge.arrow.width=0.01, vertex.size=6, vertex.color=rainbow(5, alpha=.5), vertex.label=NA, layout=layout_randomly(net))

# maxlen is the max cycle length to search for
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen = 10, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")

#s <- paste(c("List length ", as.character(length(cc)),"\nHow bou dat"), sep=" ")
#message(s)
