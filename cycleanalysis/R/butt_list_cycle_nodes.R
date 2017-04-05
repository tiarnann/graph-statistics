
# cycle.mostImportantNodes - Returns the 3 most important nodes in the graph
# Parameters: cycles = list of cycles
#             count = number of cycles
# Returns:    mostImportantNodes = list of the 3 most important nodes
cycle.mostImportantNodes <- function(cycles, count, vnam) {
  # create a list of counts with a value for each node
  # matrix of lists, first term is the name of the index, second is the frequency
  mat<-matrix(list(), nrow=length(vnam), ncol=2)
  for(z in 1: length(vnam)){  #fill in names, and initialise counts to 0
    mat[z,1] <- vnam[z]
    mat[z,2] <- 0
  }
  for (i in 1:count){
    for (j in 1:length(cycles[[i]]) ){
      for(k in 1:length(vnam)){
        if(cycles[[i]][[j]]==mat[k,1]){
          mat[[k,2]] <- mat[[k,2]] + 1
        }
      }
    }
  }

  #list to return
  mostImportantNodes <- as.matrix(3)

  # find the highest three values, add them to the mostImportantNodes
  for(i in 1:3){
    big <- 0        #frequency
    index1 <- 0     #index of value
    for(j in 1:length(vnam)){   #find value
      if(mat[[j,2]] >= big){
        big<-mat[[j,2]]
        index1 <- j
      }
      mostImportantNodes[i] <- mat[[index1, 1]] #add to the list
      mat[index1,2] <- 0          #remove it so as not to find the same value twice
    }
  }

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
          if (j == 1) {
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

###
# overlaps [Returns whether the comparate elements are contained within cycle]
# @param  {list} cycle
# @param  {list} comparate
# @return {boolean}
# NB: Only suitable for undirected graphs
##
cycle.overlaps <- function(cycle, comparate){
  cycleLength <- length(cycle)
  comparateLength <- length(comparate)

  if(cycleLength == 0 || comparateLength == 0){
    return(FALSE)
  }else if(comparateLength > cycleLength){
    return(FALSE)
  }

  for(i in 1:comparateLength){
    contains <- comparate[[i]] %in% cycle
    if(!contains){
      return(FALSE)
    }
  }
  return(TRUE)
}

###
# cycle.findAllOverlappingCycles [Returns all overlapping cycles within passed list]
# @param  {list} cycles 	[list of cycles]
# @return {[list]}        	[list of lists containing indexes of the cycles which overlap at a given index
# 							             e.g returnedList[[i]][[j]] = whether cycles[[i]] and cycles[[j]] overlap
##
cycle.findAllOverlappingCycles <- function(cycles){
  result <- list()
  len <- length(cycles)
  for(i in 1:len){
    result[[i]] <- list()
    for(j in 1:len){
      if(i != j){
        overlapping <- overlaps(cycles[[j]], cycles[[i]])
        result[[i]][j] = overlapping
      }
      else{
        # When
        result[[i]][j] = TRUE
      }
    }
  }

  return(result)
}


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
  mostImportantNodes <- cycle.mostImportantNodes(ccen, length(ccen), vnam)

  # output
  out<-list(cyclecounts=count)
  out$cycles              <- ccen
  out$edgeWeightTotal     <- edgeWeightTotal
  out$minimumEdgeWeight   <- minimumEdgeWeight
  out$mostImportantNodes  <- mostImportantNodes
  return (out)
}

