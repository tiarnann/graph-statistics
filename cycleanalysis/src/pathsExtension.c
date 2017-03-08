#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include "utils.h"
#include "paths.h"
#include "pathsExtension.h"
#include "Cycle.h"
#include "CycleList.h"
#include "string.h"

void addListToCycleList(char **ids, int length, CycleList *cyclelist) {
  //Create cycle
  Cycle *cycle = createCycle();
  for (int i=0; i<=length; i++)
    appendNode(ids[i], cycle);

  //Append cycle to cyclelist
  appendCycle(cycle, cyclelist);
}

void edgewisePathRecurseID(snaNet *g, int src, int dest, int curnode, int *availnodes, int availcount, int *usednodes, int curlen, double *count,
                           double *cpcount, double *dpcount, int maxlen, int directed, int byvertex, int copaths, int dyadpaths, char *ids[],
                           int id_idx, CycleList *cyclelist, char **id_names)
/*Recursively count the paths from src to dest.  (This is an adaptation of the routine I wrote for statnet.)  count should be vector of path
  counts (starting with length 1) if !byvertex, or else a matrix of dimension (maxlen-1)x(n+1) whose first column contains aggregate counts and
  i+1th column contains counts for vertex i.  If copaths==1, cpcount should contain an nxn matrix containing path co-membership counts.  If
  copaths=2, cpcount should contain a (maxlen-1)xnxn array containing path co-membership counts at each length.  This is ignored if copaths==0.
  (Odd note: if this is being used to construct a path census, rather than a cycle census, maxlen should be one greater than the true maximum
  length.  It looks wrong, but it isn't.  All of the maxlen-1 stuff in here is also correct, even though (e.g., for dyad paths) it may appear
  otherwise.)*/
{
  int *newavail,i,j,k,newavailcount,*newused,n;

  //add id to id list
  ids[id_idx++] = id_names[curnode];

  /*Rprintf("\t\t\tRecursion: src=%d, dest=%d, curnode=%d, curlen=%d, availcount=%d\n",src,dest,curnode,curlen,availcount);*/

  n=g->n;
  // Rprintf("N=%d\n",n); N = Length
  /*If we've found a path to the destination, increment the census vector*/
  if(directed||(curnode<dest)){
    if(snaIsAdjacent(curnode,dest,g,2)){
      //Rprintf("\t\t\t\t%d is adjacent to target (%d)\n",curnode+1,dest+1);
      count[curlen]++;                       /*Basic update*/
      //TODO: remove print statements
      addListToCycleList(ids, curlen+1, cyclelist);
      /*printf("1st count increment:\t");
      for (int i=0; i<=curlen+1; i++)
        printf("%d, ", ids[i]+1);
      printf("\n");*/

      if(byvertex){                          /*Update path incidence counts*/
        for(j=0;j<curlen;j++) {
          count[curlen+(1+usednodes[j])*(maxlen-1)]++;
          count[curlen+(1+curnode)*(maxlen-1)]++;
          count[curlen+(1+dest)*(maxlen-1)]++;
        }
      }
      if(copaths==1){                        /*Update copath incidence counts*/
        for(j=0;j<curlen;j++){
          for(k=j;k<curlen;k++){
            cpcount[usednodes[j]+usednodes[k]*n]++;
            if(k!=j)
              cpcount[usednodes[k]+usednodes[j]*n]++;
          }
          cpcount[usednodes[j]+dest*n]++;
          cpcount[dest+usednodes[j]*n]++;
          cpcount[usednodes[j]+curnode*n]++;
          cpcount[curnode+usednodes[j]*n]++;
        }
        cpcount[curnode+dest*n]++;
        cpcount[dest+curnode*n]++;
        cpcount[curnode+curnode*n]++;
        cpcount[dest+dest*n]++;
      }
      if(copaths==2){                        /*Update copath counts using len*/
        for(j=0;j<curlen;j++){
          for(k=j;k<curlen;k++){
            cpcount[curlen+usednodes[j]*(maxlen-1)+ usednodes[k]*(maxlen-1)*n]++;
            if(k!=j)
              cpcount[curlen+usednodes[k]*(maxlen-1)+ usednodes[j]*(maxlen-1)*n]++;
          }
          cpcount[curlen+usednodes[j]*(maxlen-1)+dest*(maxlen-1)*n]++;
          cpcount[curlen+dest*(maxlen-1)+usednodes[j]*(maxlen-1)*n]++;
          cpcount[curlen+usednodes[j]*(maxlen-1)+curnode*(maxlen-1)*n]++;
          cpcount[curlen+curnode*(maxlen-1)+usednodes[j]*(maxlen-1)*n]++;
        }
        cpcount[curlen+curnode*(maxlen-1)+dest*(maxlen-1)*n]++;
        cpcount[curlen+dest*(maxlen-1)+curnode*(maxlen-1)*n]++;
        cpcount[curlen+dest*(maxlen-1)+dest*(maxlen-1)*n]++;
        cpcount[curlen+curnode*(maxlen-1)+curnode*(maxlen-1)*n]++;
      }
      if(dyadpaths==1){                      /*Update dyadic path counts*/
        dpcount[src+dest*n]++;
        if(!directed)
          dpcount[dest+src*n]++;
      }
      if(dyadpaths==2){                  /*Update dyadic path counts using len*/
        dpcount[curlen+src*(maxlen-1)+dest*(maxlen-1)*n]++;
        if(!directed)
          dpcount[curlen+dest*(maxlen-1)+src*(maxlen-1)*n]++;
      }
    }
  }
  else{
    if(snaIsAdjacent(dest,curnode,g,2)){
      count[curlen]++;                       /*Basic update*/
      //TODO: remove print statements
      addListToCycleList(ids, curlen+1, cyclelist);
      /*printf("2nd count increment:\t");
      for (int i=0; i<=curlen+1; i++)
        printf("%d, ", ids[i]+1);
      printf("\n");*/

      if(byvertex){                          /*Update path incidence counts*/
        for(j=0;j<curlen;j++)
          count[curlen+(1+usednodes[j])*(maxlen-1)]++;
          count[curlen+(1+curnode)*(maxlen-1)]++;
          count[curlen+(1+dest)*(maxlen-1)]++;
      }
      if(copaths==1){                       /*Update copath incidence counts*/
        for(j=0;j<curlen;j++){
          for(k=j;k<curlen;k++){
            cpcount[usednodes[j]+usednodes[k]*n]++;
            if(k!=j)
              cpcount[usednodes[k]+usednodes[j]*n]++;
          }
          cpcount[usednodes[j]+dest*n]++;
          cpcount[dest+usednodes[j]*n]++;
          cpcount[usednodes[j]+curnode*n]++;
          cpcount[curnode+usednodes[j]*n]++;
        }
        cpcount[curnode+dest*n]++;
        cpcount[dest+curnode*n]++;
        cpcount[curnode+curnode*n]++;
        cpcount[dest+dest*n]++;
      }
      if(copaths==2){                      /*Update copath counts using len*/
        for(j=0;j<curlen;j++){
          for(k=j;k<curlen;k++){
            cpcount[curlen+usednodes[j]*(maxlen-1)+ usednodes[k]*(maxlen-1)*n]++;
            if(k!=j)
              cpcount[curlen+usednodes[k]*(maxlen-1)+ usednodes[j]*(maxlen-1)*n]++;
          }
          cpcount[curlen+usednodes[j]*(maxlen-1)+dest*(maxlen-1)*n]++;
          cpcount[curlen+dest*(maxlen-1)+usednodes[j]*(maxlen-1)*n]++;
          cpcount[curlen+usednodes[j]*(maxlen-1)+curnode*(maxlen-1)*n]++;
          cpcount[curlen+curnode*(maxlen-1)+usednodes[j]*(maxlen-1)*n]++;
        }
        cpcount[curlen+curnode*(maxlen-1)+dest*(maxlen-1)*n]++;
        cpcount[curlen+dest*(maxlen-1)+curnode*(maxlen-1)*n]++;
        cpcount[curlen+curnode*(maxlen-1)+curnode*(maxlen-1)*n]++;
        cpcount[curlen+dest*(maxlen-1)+dest*(maxlen-1)*n]++;
      }
      if(dyadpaths==1){                      /*Update dyadic path counts*/
        dpcount[src+dest*n]++;
        if(!directed)
          dpcount[dest+src*n]++;
      }
      if(dyadpaths==2){                  /*Update dyadic path counts using len*/
        dpcount[curlen+src*(maxlen-1)+dest*(maxlen-1)*n]++;
        if(!directed)
          dpcount[curlen+dest*(maxlen-1)+src*(maxlen-1)*n]++;
      }
    }
  }

  /*If possible, keep searching for novel paths*/
  if((availcount>0)&&(curlen<maxlen-2)){

    if(availcount>1){    /*Remove the current node from the available list*/
      /*Rprintf("\t\t\tRemoving %d from available node list (availcount=%d)\n", curnode,availcount);*/
      if((newavail=(int *)malloc(sizeof(int)*(availcount-1)))==NULL){
        Rprintf("Unable to allocate %d bytes for available node list in edgewisePathRecurseID.  Trying to terminate recursion gracefully, but your path count is probably wrong.\n",sizeof(int)*(availcount-1));
        return;
      }
      j=0;
      for(i=0;i<availcount;i++)      /*Create the reduced list, fur passin'*/
        if(availnodes[i]!=curnode)
          newavail[j++]=availnodes[i];

    /*Rprintf("\t\t\tBuilt newavail without apparent issue\n");*/
    }
    else
        newavail=NULL;                 /*Set to NULL if we're out of nodes*/
    newavailcount=availcount-1;      /*Decrement the available count*/
    if(byvertex||copaths||dyadpaths){  /*Add the current node to the used list*/
      if((newused=(int *)malloc(sizeof(int)*(curlen+1)))==NULL){
        Rprintf("Unable to allocate %d bytes for used node list in edgewisePathRecurseID.  Trying to terminate recursion gracefully, but your path count is probably wrong.\n",sizeof(int)*(curlen+1));
        return;
      }
      for(i=0;i<curlen;i++)
        newused[i]=usednodes[i];
      newused[curlen]=curnode;
    }
    else
      newused=NULL;

    /*Recurse on all available nodes*/
    /*Rprintf("\t\t\tAbout to recurse on available nodes (newavail=%d)\n", newavailcount);*/
    for(i=0;i<newavailcount;i++){
      if(directed||(curnode<newavail[i])){
        if(snaIsAdjacent(curnode,newavail[i],g,2)){
          edgewisePathRecurseID(g,src,dest,newavail[i],newavail,newavailcount,newused,curlen+1,count,cpcount,dpcount,maxlen,directed,byvertex,
                              copaths,dyadpaths,ids,id_idx,cyclelist,id_names);
        }
      }
      else{
        if(snaIsAdjacent(newavail[i],curnode,g,2)){
          edgewisePathRecurseID(g,src,dest,newavail[i],newavail,newavailcount,newused,curlen+1,count,cpcount,dpcount,maxlen,directed,byvertex,
                              copaths,dyadpaths,ids,id_idx,cyclelist,id_names);
        }
      }
    }

    /*Free the available node and used node lists*/
    /*Rprintf("\t\t\tDone with available node recursion, freeing\n");*/
    if(newavail!=NULL){
      /*Rprintf("\t\t\t\tFreeing newavail; count=%d\n",newavailcount);*/
      free((void *)newavail);
    }
    if(newused!=NULL){
      /*Rprintf("\t\t\t\tFreeing newused; count=%d\n",curlen);*/
      free((void *)newused);
    }
  }

  /*Free the used node list*/
  /*Rprintf("\t\t\tBacking out for src=%d, dest=%d, curnode=%d\n",src,dest, curnode);*/
  /*if(usednodes!=NULL)
   free((void *)usednodes);*/
  /*Check for interrupts (if recursion is taking way too long...)*/
  R_CheckUserInterrupt();
}



//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------



void edgewiseCycleCensusID(snaNet *g, int src, int dest, double *count, double *cccount, int maxlen, int directed, int byvertex, int cocycles, CycleList *cyclelist, char **id_names)
  /*Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether cycle counts should be broken down by participating vertex, and whether a cycle co-membership matrix should be returned (respectively).  In either case, count and cccount must be structured per count and pccount in edgewisePathRecurseID.*/
{
  int n,i,j,*availnodes,*usednodes;

  /*Set things up*/
  n = g->n;
  usednodes = NULL;

  /*First, check for a 2-cycle (but only if directed)*/
  //Rprintf("\t\tChecking for (%d,%d) edge\n",src+1,dest+1);
  if(directed&&snaIsAdjacent(dest,src,g,2)){
    count[0]++;

    //add new Cycle to CycleList
    Cycle *cycle = createCycle();
    appendNode(id_names[src], cycle);
    appendNode(id_names[dest], cycle);
    appendCycle(cycle, cyclelist);

    if(byvertex){
      count[ (1+src)*(maxlen-1) ]++;
      count[ (1+dest)*(maxlen-1) ]++;
    }
    if(cocycles==1){
      cccount[ src+dest*n ]++;
      cccount[ dest+src*n ]++;
      cccount[ src+src*n ]++;
      cccount[ dest+dest*n ]++;
    }
    if(cocycles==2){
      cccount[ src*(maxlen-1)+dest*(maxlen-1)*n ]++;
      cccount[ dest*(maxlen-1)+src*(maxlen-1)*n ]++;
      cccount[ src*(maxlen-1)+src*(maxlen-1)*n ]++;
      cccount[ dest*(maxlen-1)+dest*(maxlen-1)*n ]++;
    }
  }
  if(n==2)
    return;                           /*Failsafe for graphs of order 2*/

  /*Perform the recursive path count*/
  if((availnodes=(int *)malloc(sizeof(int)*(n-2)))==NULL){
    Rprintf("Unable to allocate %d bytes for available node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int)*(n-2));
    return;
  }
  j=0;                                /*Initialize the list of available nodes*/
  for(i=0;i<n;i++) {
    if((i!=src)&&(i!=dest))
      availnodes[j++]=i;
    if(byvertex||cocycles){           /*Initialize the list of already used nodes*/
      if((usednodes=(int *)malloc(sizeof(int)))==NULL){
        Rprintf("Unable to allocate %d bytes for used node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int));
        return;
      }
      usednodes[0]=dest;
    }
  }

  //Create list of ID's
  int id_idx = 0;
  char *ids[maxlen];
  ids[id_idx++] = id_names[src];
  ids[id_idx++] = id_names[dest];

  /*Rprintf("\t\tBeginning recursion\n");*/
  for(i=0;i<n-2;i++) {               /*Recurse on each available vertex*/
    if(directed||(dest<availnodes[i])){
      if(snaIsAdjacent(dest,availnodes[i],g,2)){
        edgewisePathRecurseID(g,dest,src,availnodes[i],availnodes,n-2,usednodes,1,
                            count,cccount,NULL,maxlen,directed,byvertex,cocycles,0,ids,id_idx,cyclelist,id_names);
      }
    }
    else{
      if(snaIsAdjacent(availnodes[i],dest,g,2)){

        edgewisePathRecurseID(g,dest,src,availnodes[i],availnodes,n-2,usednodes,1,
                            count,cccount,NULL,maxlen,directed,byvertex,cocycles,0,ids,id_idx,cyclelist,id_names);
      }
    }
  }

  /*Rprintf("\t\tReturned from recursion; freeing memory\n");*/
  if(availnodes!=NULL)
    free((void *)availnodes);  /*Free the available node list*/
  if(usednodes!=NULL)
    free((void *)usednodes); /*Free the used node list, if needed*/

}



//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------



void cycleCensusID_R(int *g, int *pn, int *pm, double *count, double *cccount, int *pmaxlen, int *pdirected, int *pbyvertex, int *pcocycles, char **id_names)
/*Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether
  cycle counts should be broken down by participating vertex, and whether cycle co-membership counts should be returned (respectively).  In either case, count and cccount must be structured per count and pccount in edgewisePathRecurseID.*/
/*  (*pn) is the number of nodes */
{
  int i,r,c,n,m,id;
  double *dval;
  snaNet *ng;

  GetRNGstate();
  /*Allocate memory for the new graph object*/
  /*Rprintf("Initializing ng\n");*/
  n=(*pn);
  m=(*pm);
  ng=(snaNet *)R_alloc(1,sizeof(struct snaNettype));
  ng->n=(*pn);                                            //here the 'n' of 'ng' is being assigned to (*pn)
  ng->indeg=(int *)R_alloc(n,sizeof(int));
  ng->outdeg=(int *)R_alloc(n,sizeof(int));
  ng->iel=(slelement **)R_alloc(n,sizeof(slelement *));
  ng->oel=(slelement **)R_alloc(n,sizeof(slelement *));
  id=0;
  /*Initialize the graph*/
  for(i=0;i<n;i++){
    ng->indeg[i]=0;
    ng->outdeg[i]=0;
    ng->iel[i]=NULL;
    ng->oel[i]=NULL;
  }

  /*Initialise new CycleList object */
  CycleList *cyclelist = createCycleList();

  /*Walk the graph, adding edges and accumulating cycles*/
  //Rprintf("Building graph/accumulating cycles\n\tn=%d,%d\n",n,ng->n);
  for(i=0;i<m;i++){

    if((!IISNA(g[i+2*m]))&&((*pdirected)||(g[i]<g[i+m]))){
      r=g[i]-1;
      c=g[i+m]-1;

      // *********** NB **************
      // Node ID: g[i] Node ID: g[i+m]
      // *********** NB **************
      //Rprintf("ID:%d ID:%d\n",r+1,c+1);
      // *********** NB **************
      // Node ID: g[i] Node ID: g[i+m]
      // *********** NB **************


      /*First, accumulate the cycles to be formed by the (r,c) edge*/
      //Rprintf("\tEdge at (%d,%d); counting cycles\n",r+1,c+1);
      edgewiseCycleCensusID(ng,r,c,count,cccount,*pmaxlen,*pdirected,
        *pbyvertex,*pcocycles, cyclelist, id_names);
      //for(int k=0;k<*pmaxlen-1;k++){
      //  Rprintf("%d:%d ",k+2,(int)(count[k]));
      //  Rprintf("\n");
      //  }
      /*Next, add the (r,c) edge to the graph*/
    //  Rprintf("\tGot cycles, now adding edge to ng:%d\n",ng[i].n);
      dval=(double *)R_alloc(1,sizeof(double));   /*Create iel element*/
      dval[0]=(double)g[i+2*m];
      ng->iel[c]=slistInsert(ng->iel[c],(double)r,(void *)dval);
      ng->indeg[c]++;
      dval=(double *)R_alloc(1,sizeof(double));   /*Create oel element*/
      dval[0]=(double)g[i+2*m];
      ng->oel[r]=slistInsert(ng->oel[r],(double)c,(void *)dval);
      ng->outdeg[r]++;
      if(!(*pdirected)){
        dval=(double *)R_alloc(1,sizeof(double));   /*Create iel element*/
        dval[0]=(double)g[i+2*m];
        ng->iel[r]=slistInsert(ng->iel[r],(double)c,(void *)dval);
        ng->indeg[r]++;
        dval=(double *)R_alloc(1,sizeof(double));   /*Create oel element*/
        dval[0]=(double)g[i+2*m];
        ng->oel[c]=slistInsert(ng->oel[c],(double)r,(void *)dval);
        ng->outdeg[c]++;
      }
    }
  }

  printCycleList(cyclelist);

  PutRNGstate();
}

SEXP cycleToVector(Cycle *cycle){
  int cycleLength = cycle -> size;

  SEXP result = PROTECT(allocVector(VECSXP, cycleLength));

  int i = 0;
  Node * currentNode = cycle -> head;
  while(currentNode != NULL){
    //Do something special
    SEXP id = PROTECT(mkString(currentNode->id));
    SET_VECTOR_ELT(result, i++, id);
    currentNode = currentNode -> next;
  }
  UNPROTECT(cycleLength + 1);

  return result;
}

SEXP cycleListToVector(CycleList *list){
  int listLength = list -> size;

  SEXP result = PROTECT(allocVector(VECSXP, listLength));

  int i = 0;
  CycleListNode * currentNode = list -> head;
  while(currentNode != NULL){
    //Do something special with dat list
    SEXP cycle = cycleToVector(currentNode->list);
    SET_VECTOR_ELT(result, i++, cycle);
    currentNode = currentNode -> next;
  }

  UNPROTECT(1);

  return result;
}

SEXP cycleTestR(){
  Cycle *cycle = createCycle();
  appendNode("c1",cycle);
  appendNode("c2",cycle);
  appendNode("c3",cycle);
  appendNode("c4",cycle);
  appendNode("c5",cycle);

  CycleList *list = createCycleList();
  appendCycle(cycle,list);
  appendCycle(cycle,list);

  return cycleListToVector(list);
}
