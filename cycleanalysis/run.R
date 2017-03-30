#(1) change directory
library('cycleanalysis')
library('igraph')
library('statnet')

nodes <- read.csv("sample_data/Dataset_nodes_500:1000.csv", header=T, as.is=T)
edges <- read.csv("sample_data/Dataset_edges_500:1000.csv", header=T, as.is=T)

net <- graph_from_data_frame(d=edges, vertices=nodes, directed=T)
adjacencyMat <- get.adjacency(net, sparse=FALSE)
edgeMat <- as.matrix(edges, sparse=FALSE)
plot(net, edge.arrow.size=0.01, edge.arrow.width=0.01, vertex.size=6, vertex.color=rainbow(5, alpha=.5), vertex.label=NA, layout=layout_randomly(net))

# maxlen is the max cycle length to search for
start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=2, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=3, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=4, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=5, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=6, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

start.time <- Sys.time()
cc <- kcycle.censusExtension(adjacencyMat, edges, maxlen=7, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
end.time <- Sys.time()
time.taken <- end.time - start.time
time.taken

#s <- paste(c("List length ", as.character(length(cc)),"\nHow bou dat"), sep=" ")
#message(s)
