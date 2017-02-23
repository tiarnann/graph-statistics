#(1) change directory

library('igraph')
library('statnet')

source("C:\\Users\\Joanna\\Documents\\College\\Senior Freshman\\Software Engineering\\butt_list_cycle_nodes.R")

nodes <- read.csv("Dataset1-Media-Example-NODES.csv", header=T, as.is=T)
edges <- read.csv("Dataset1-Media-Example-EDGES.csv", header=T, as.is=T)

net <- graph_from_data_frame(d=edges, vertices=nodes, directed=T)
adjacencyMat <- get.adjacency(net, sparse=FALSE)
#plot(net, edge.arrow.size=.4)

#cc<-kcycle.census.edited(adjacencyMat, maxlen = 5, tabulate.by.vertex = FALSE, cycle.comembership = "bylength")
#cc$cycle.count