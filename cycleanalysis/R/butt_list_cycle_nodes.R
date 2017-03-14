edges.iterationTest <- function(edgeMat, count) {
  weightsBackwards <- as.matrix(count,count)
  for (i in 1:52) {
    weightsBackwards[[i]] <- as.matrix(edgeMat[52+1-i, 3], weightsBackwards)
  }

  #Output
  weightsBackwards
}

cycle.edgeWeightTotal <- function(cyclelists, edgeMat, count) {
  edgeWeightTotals <- as.matrix(count,count)
  for (i in 1:count) {
    totalWeight <- 0
    for (j in 1:( length( cyclelists[[i]] ))  ) {
      if (j == length( cyclelists[[i]] )) {
        node1 <- cyclelists[[i]][j]
        node2 <- cyclelists[[i]][1]
      }
      else{
        node1 <- cyclelists[[i]][j]
        node2 <- cyclelists[[i]][j+1]
      }

      #find node pair in edgelist
      foundBoth  <- FALSE
      idx <- 1
      while (!foundBoth) {
        if (node1 == edgeMat[idx, 1] && node2 == edgeMat[idx, 2]) {
          totalWeight <- totalWeight + edgeMat[idx, 3]
          foundBoth == TRUE
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

cycle.minimumEdgeWeight <- function(cyclelists, edgeMat, count) {
  minimumEdgeWeights <- as.matrix(count,count)
  for (i in 1:count) {
    minWeight <- 0
    for (j in 1:( length( cyclelists[[i]] ))  ) {
      if (j == length( cyclelists[[i]] )) {
        node1 <- cyclelists[[i]][j]
        node2 <- cyclelists[[i]][1]
      }
      else{
        node1 <- cyclelists[[i]][j]
        node2 <- cyclelists[[i]][j+1]
      }

      #find node pair in edgelist
      foundBoth  <- FALSE
      idx <- 1
      while (!foundBoth) {
        if (node1 == edgeMat[idx, 1] && node2 == edgeMat[idx, 2]) {
          if (minWeight == 0){
            minWeight <- edgeMat[idx, 3]
          }
          else if (minWeight !=0 && edgeMat[idx, 3] < minWeight) {
            minWeight <- edgeMat[idx, 3]
          }
          foundBoth == TRUE
        }
        else  {
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

kcycle.censusExtension<-function(dat,maxlen=3,mode="digraph",tabulate.by.vertex=TRUE,cycle.comembership=c("none","sum","bylength")){
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



  #Return the result
  out<-list(cyclecounts=count)
  out$cycles <- ccen
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
