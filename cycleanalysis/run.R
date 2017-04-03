library('cycleanalysis')
library('igraph')
library('statnet')

# Read data from .csv files
nodes <- read.csv("sample_data/Dataset_nodes_200_500.csv", header=T, as.is=T)
edges <- read.csv("sample_data/Dataset_edges_200_500.csv", header=T, as.is=T)

# Manipulate data into correct format
net <- graph_from_data_frame(d=edges, vertices=nodes, directed=T)
adjacencyMat <- get.adjacency(net, sparse=FALSE)
edgeMat <- as.matrix(edges, sparse=FALSE)

# Plot the graph
plot(net, edge.arrow.size=0.01, edge.arrow.width=0.001, edge.color="lightpink", vertex.size=2, vertex.color="white", vertex.label.color="black", vertex.label.cex=0.6)

# Run kcycle.censusExtension using the adjacency matrix for the graph
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=5, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
message(as.character(length(cc$cycles)), " Cycles Found.")

a<-cc$minimumEdgeWeight
b<-cc$edgeWeightTotal
c<-cc$mostImportantNodes
d<-cc$cyclecounts

