
# cycle.mostImportantNodes - Returns the 3 most important nodes in the graph
# Parameters: cycles = list of cycles
#             count = number of cycles
# Returns:    mostImportantNodes = list of the 3 most important nodes
cycle.mostImportantNodes <- function(cycles, count) {
  # create a list of counts with a value for each node
  theList <- list()   # indexes are the nodes, values are the amount of times they reoccur in the given cycles

  for(z in 1: length(adjacencyMat[1, ])){  # initialise list to 0
    theList[z] = 0;
  }
  for (i in 1:count){
    for (j in 1:length(cycles[[i]]) ){
      formatting <- unlist(strsplit(cycles[[i]][[j]], 's', fixed=TRUE)) # convert character values into ints
      theNumber <- as.numeric(formatting[[2]])
      theList[[theNumber]] = theList[[theNumber]] + 1                   # find the index of the character and increment
    }
  }

  # find the highest three values and delete them from the list
  big <- which.max(theList)
  theList[which.max(theList)] <- 0

  secondBig <- which.max(theList)
  theList[which.max(theList)] <- 0

  thirdBig <- which.max(theList)
  theList[which.max(theList)] <- 0

  mostImportantNodes <- as.matrix(3)
  mostImportantNodes[1] <- big
  mostImportantNodes[2] <- secondBig
  mostImportantNodes[3] <- thirdBig

  # output
  mostImportantNodes
}

# cycle.edgeWeightTotal - Calculates the total edge weight of each cycle
# Parameters: cycles = list of cycles
#             edgeMat = matrix of edges and associated weights with column names "from", "to" and "weight"
#             count = number of cycles
# Returns:    edgeWeightTotal = list of total edge weight for each cycle
cycle.edgeWeightTotal <- function(cycles, edgeMat, count) {
  # retrieve indices associated with column names
  columnNames <- as.matrix(colnames(edgeMat))
  for (k in 1:length(columnNames)){
    if (columnNames[k] == "from"){        fromIdx   <- k }
    else if (columnNames[k] == "to"){     toIdx     <- k }
    else if (columnNames[k] == "weight"){ weightIdx <- k }
  }

  # initialize and fill list of total edge weights
  edgeWeightTotals <- as.matrix(count)
  for (i in 1:count) { # for every cycle
    totalWeight <- 0

    for (j in 1:( length( cycles[[i]] ))  ) { # for every node
      # read node names
      if (j == length( cycles[[i]] )) { # safe check for the last pair of nodes
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][1]
      }
      else{
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][j+1]
      }

      # find node pair in edgelist matrix and get weight
      foundBoth  <- FALSE
      idx <- 1
      while (!foundBoth && idx<=nrow(edgeMat)) {
        if (node1 == edgeMat[idx, fromIdx] && node2 == edgeMat[idx, toIdx]) {
          totalWeight <- totalWeight +  as.double(edgeMat[idx, weightIdx])
          foundBoth <- TRUE
        }
        else { idx <- idx + 1 }
      }
    }
    edgeWeightTotals[i] <- totalWeight
  }
  # output
  edgeWeightTotals
}

# cycle.minimumEdgeWeight - Calculates the minimum edge weight in each cycle
# Parameters: cycles = list of cycles
#             edgeMat = matrix of edges and associated weights with column names "from", "to" and "weight"
#             count = number of cycles
# Returns:    minimumEdgeWeight = list of the minimum edge weight for every cycle
cycle.minimumEdgeWeight <- function(cycles, edgeMat, count) {
  # retrieve index associated with column names
  columnNames <- as.matrix(colnames(edgeMat))
  for (k in 1:length(columnNames)){
    if (columnNames[k] == "from"){        fromIdx   <- k }
    else if (columnNames[k] == "to"){     toIdx     <- k }
    else if (columnNames[k] == "weight"){ weightIdx <- k }
  }

  # initialize and fill list of total edge weights
  minimumEdgeWeights <- as.matrix(count)
  for (i in 1:count) { # for every cycle
    minWeight <- 0

    for (j in 1:( length( cycles[[i]] ))  ) { # for every node
      # read node names
      if (j == length( cycles[[i]] )) { # safe check for the last pair of nodes
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][1]
      }
      else{
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][j+1]
      }

      # find node pair in edgelist & get weight
      foundBoth  <- FALSE
      idx <- 1
      while (!foundBoth && idx<=nrow(edgeMat)) {
        if (node1 == edgeMat[idx, fromIdx] && node2 == edgeMat[idx, toIdx]) {
          thisWeight <- as.double(edgeMat[idx, weightIdx])
          if (j == 1){
            minWeight <- thisWeight
          }
          else if (minWeight !=0 && thisWeight < minWeight) {
            minWeight <- thisWeight
          }
          foundBoth <- TRUE
        }
        else {
          idx <- idx + 1
        }
      }
    }
    minimumEdgeWeights[i] <- minWeight
  }
  # output
  minimumEdgeWeights
}

#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------

# kcycle.censusExtension - Compute the cycle census of a graph, along with additional cycle information
#  on the inidence of cycles.
# Parameters:   dat = adjacency matrix for the graph
#               edges = matrix of edges and associated weights with column names "from", "to" and "weight"
#               maxlen = the maximum path/cycle length to evaluate
#               mode = "digraph" for directed graphs, or "graph" for undirected graphs
#               tabulate.by.vertex =  logical; should path or cycle incidence counts be tabulated by vertex?
#               cycle.comembership =  the type of cycle co-membership information to be tabulated, if any. "sum"
#                                     returns a vertex by vertex matrix of cycle co-membership counts; these are
#                                     disaggregated by cycle length if "bylength" is used. If "none" is given, no
#                                     co-membership information is computed
#	Returns:      cycles = list of nodes in each cycle
#               edgeWeightTotal = list of total edge weight for each cycle
#               minimumEdgeWeight = list of the minimum edge weight for every cycle
#               mostImportantNodes = XXXX
kcycle.censusExtension<-function(dat, edges, maxlen=3,mode="digraph",tabulate.by.vertex=TRUE,cycle.comembership=c("none","sum","bylength")){
  # pre-process the raw input
  dat<-as.edgelist.sna(dat)
  if(is.list(dat))
    return(lapply(dat,kcycle.list,maxlen=maxlen,mode=mode, tabulate.by.vertex=tabulate.by.vertex,cycle.comembership=cycle.comembership))

  # process function parameters
  n<-attr(dat,"n")	# n is the number of nodes
  if(is.null(maxlen))
    maxlen<-n
  if(maxlen<2)
    stop("maxlen must be >=2")
  if(is.null(attr(dat,"vnames")))
    vnam<-paste("v",1:n,sep="")
  else
    vnam<-attr(dat,"vnames")
  if(mode=="digraph")
    directed<-TRUE
  else
    directed<-FALSE
  cocycles<-switch(match.arg(cycle.comembership), "none"=0, "sum"=1, "bylength"=2)

  print(vnam)

  # generate the data structures for the counts
  if(!tabulate.by.vertex)
    count<-rep(0,maxlen-1)
  else
    count<-matrix(0,maxlen-1,n+1)		# matrix(matrixValues, nrows, ncolumns)
  if(!cocycles)
    cccount<-NULL
  else if(cocycles==1)
    cccount<-matrix(0,n,n)
  else
    cccount<-array(0,dim=c(maxlen-1,n,n))
  if(is.null(maxlen))
    maxlen<-n

  # calculate the cycle information
  ccen <- .Call("cycleCensusID_R", dat, n, NROW(dat), count, cccount, maxlen, directed, tabulate.by.vertex,cocycles, c(vnam),PACKAGE = "cycleanalysis")

  # calculate additional information from the cycles found
  edgeMat <- as.matrix(edges, sparse=FALSE)
  edgeWeightTotal <- cycle.edgeWeightTotal(ccen, edgeMat, length(ccen))
  minimumEdgeWeight <- cycle.minimumEdgeWeight(ccen, edgeMat, length(ccen))
  mostImportantNodes <- cycle.mostImportantNodes(ccen, length(ccen))

  # output
  out<-list(cyclecounts=count)
  out$cycles              <- ccen
  out$edgeWeightTotal     <- edgeWeightTotal
  out$minimumEdgeWeight   <- minimumEdgeWeight
  out$mostImportantNodes  <- mostImportantNodes
  return (out)
}
