cycle.mostImportantNodes <- function(cycles, count) {
  mostImportantNodes <- as.matrix(count)
  theList <- list()   #indexes are the nodes, values are the amount of times they reoccur

  for(z in 1: length(adjacencyMat[1, ])){  #fill this list with 0 values
    theList[z] = 0;
  }
  for( i in 1:count){
    for(j in 1: length( cycles[[i]])){
      #print(cycles[[i]][[j]])
      formatting <- unlist(strsplit(cycles[[i]][[j]], 's', fixed=TRUE))     #convert character values into ints
      theNumber <- as.numeric(formatting[[2]])
      #print(theNumber)
      #print(theList)
      theList[[theNumber]] = theList[[theNumber]] + 1                              #find the index of the character and increment
    }

  }
  #print(theList)
  big <- which.max(theList)                                                #taking the highest three indexes and deleting them from the list
  theList[which.max(theList)] <- 0

  secondBig <- which.max(theList)
  theList[which.max(theList)] <- 0

  thirdBig <- which.max(theList)
  theList[which.max(theList)] <- 0

  mostImportantNodes[1] <- big
  mostImportantNodes[2] <-secondBig
  mostImportantNodes[3] <- thirdBig
  #output
  mostImportantNodes
}





cycle.edgeWeightTotal <- function(cycles, edgeMat, count) {
  # Retrieve index associated with column names
  columnNames <- as.matrix(colnames(edgeMat))
  for (k in 1:length(columnNames)){
    if (columnNames[k] == "from"){
      fromIdx <- k
    }
    else if (columnNames[k] == "to"){
      toIdx <- k
    }
    else if (columnNames[k] == "weight"){
      weightIdx <- k
    }
  }

  edgeWeightTotals <- as.matrix(count)
  for (i in 1:count) {
    totalWeight <- 0
    for (j in 1:( length( cycles[[i]] ))  ) {
      #safe checking for the last pair of nodes
      if (j == length( cycles[[i]] )) {
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][1]
      }
      else{
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][j+1]
      }

      #find node pair in edgelist and get weight
      foundBoth  <- FALSE
      idx <- 1
      while (!foundBoth && idx<=nrow(edgeMat)) {
        if (node1 == edgeMat[idx, fromIdx] && node2 == edgeMat[idx, toIdx]) {
          totalWeight <- totalWeight +  as.double(edgeMat[idx, weightIdx])
          foundBoth <- TRUE
        }
        else {
          idx <- idx + 1
        }
      }
    }
    edgeWeightTotals[i] <- totalWeight
  }
  #Output
  edgeWeightTotals
}


cycle.minimumEdgeWeight <- function(cycles, edgeMat, count) {
  # Retrieve index associated with column names
  columnNames <- as.matrix(colnames(edgeMat))
  for (k in 1:length(columnNames)){
    if (columnNames[k] == "from"){
      fromIdx <- k
    }
    else if (columnNames[k] == "to"){
      toIdx <- k
    }
    else if (columnNames[k] == "weight"){
      weightIdx <- k
    }
  }

  minimumEdgeWeights <- as.matrix(count)
  for (i in 1:count) {
    minWeight <- 0
    for (j in 1:( length( cycles[[i]] ))  ) {
      #safe checking for the last pair of nodes
      if (j == length( cycles[[i]] )) {
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][1]
      }
      else{
        node1 <- cycles[[i]][j]
        node2 <- cycles[[i]][j+1]
      }

      #find node pair in edgelist & get weight
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
  #Output
  minimumEdgeWeights
}

#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------

#kcycle.list 	- Compute the cycle census of a graph, possibly along with
#additional information on the inidence of cycles.
#				- Output list of each node in every cycle

kcycle.censusExtension<-function(dat, edges, maxlen=3,mode="digraph",tabulate.by.vertex=TRUE,cycle.comembership=c("none","sum","bylength")){
  #Pre-process the raw input
  dat<-as.edgelist.sna(dat)
  if(is.list(dat))
    return(lapply(dat,kcycle.list,maxlen=maxlen,mode=mode, tabulate.by.vertex=tabulate.by.vertex,cycle.comembership=cycle.comembership))
  #End pre-processing

  n<-attr(dat,"n")						# n =  the number of nodes
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

  #Generate the data structures for the counts
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


  #Calculate the cycle information
  ccen <- .Call("cycleCensusID_R", dat, n, NROW(dat), count, cccount, maxlen, directed, tabulate.by.vertex,cocycles, c(vnam),PACKAGE = "cycleanalysis")

  #Calculate edge weight information
  edgeMat <- as.matrix(edges, sparse=FALSE)
  edgeWeightTotal <- cycle.edgeWeightTotal(ccen, edgeMat, length(ccen))
  minimumEdgeWeight <- cycle.minimumEdgeWeight(ccen, edgeMat, length(ccen))
  mostImportantNodes <- cycle.mostImportantNodes(ccen, length(ccen))           #AAAAAAAAAA
  #Return the result
  out<-list(cyclecounts=count)
  out$cycles <- ccen
  out$edgeWeightTotal <- edgeWeightTotal
  out$minimumEdgeWeight <- minimumEdgeWeight
  out$mostImportantNodes <-mostImportantNodes                                 #AAAAAAAAAA
  return (out)

  # # Coerce the cycle counts into the right form
  # if(!tabulate.by.vertex){
  #   count<-ccen$count
  #   names(count)<-2:maxlen
  # }else{
  #   count<-matrix(ccen$count,maxlen-1,n+1)
  #   rownames(count)<-2:maxlen
  #   colnames(count)<-c("Agg",vnam)
  # }
  # if(cocycles==1){
  #   cccount<-matrix(ccen$cccount,n,n)
  #   rownames(cccount)<-vnam
  #   colnames(cccount)<-vnam
  # }else if(cocycles==2){
  #   cccount<-array(ccen$cccount,dim=c(maxlen-1,n,n))
  #   dimnames(cccount)<-list(2:maxlen,vnam,vnam)
  # }
  #
  #
  # #Return the result
  # out<-list(cycle.count=count)
  # out$vnam <- c("Agg",vnam)
  # out$cycle.ccenCount<-ccen$count
  # out$cycle.matrix = outList
  # #out$cycle.ccenCccount<-ccen$cccount
  # if(cocycles>0)
  #   out$cycle.comemb<-cccount
  # out
}
