#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include "utils.h"
#include "paths.h"
#include "pathsExtension.h"
#include "Cycle.h"
#include "CycleList.h"
#include "string.h"

/** Given t pointer to a CycleList object and list of IDs in a cycle, addListToCycleList appends the cycle to the CycleList object.
 *  @param  ids {char **}             [list of IDs in a cycle]
 *  @param  length {int}              [length of ids]
 *  @param  cyclelist {CycleList *}   [list of previously found cycles]
 */
void addListToCycleList(char **ids, int length, CycleList *cyclelist) {
  /*Create Cycle object*/
  Cycle *cycle = createCycle();
  for (int i=0; i<=length; i++)
    appendNode(ids[i], cycle);

  /*Append cycle to cyclelist*/
  appendCycle(cycle, cyclelist);
}

/** Recursively count the paths from src to dest. (This is an adaptation of the routine written for statnet.) count should be vector of path counts
 *  (starting with length 1) if !byvertex, or else a matrix of dimension (maxlen-1)x(n+1) whose first column contains aggregate counts and i+1th
 *  column contains counts for vertex i.  If copaths==1, cpcount should contain an nxn matrix containing path co-membership counts. If copaths=2,
 *  cpcount should contain a (maxlen-1)xnxn array containing path co-membership counts at each length.  This is ignored if copaths==0. (Odd note:
 *  if this is being used to construct a path census, rather than a cycle census, maxlen should be one greater than the true maximum length. It
 *  looks wrong, but it isn't.  All of the maxlen-1 stuff in here is also correct, even though (e.g., for dyad paths) it may appear otherwise.)
 */
void edgewisePathRecurseID(snaNet *g, int src, int dest, int curnode, int *availnodes, int availcount, int *usednodes, int curlen, double *count,
                           double *cpcount, double *dpcount, int maxlen, int directed, int byvertex, int copaths, int dyadpaths, char *ids[],
                           int id_idx, CycleList *cyclelist, SEXP id_names)
{
  int *newavail,i,j,k,newavailcount,*newused,n;

  //add id to id list
  ids[id_idx++] = (char *) CHAR(STRING_ELT(id_names, curnode));

  n=g->n;
  /*If we've found a path to the destination, increment the census vector*/
  if(directed||(curnode<dest)){
    if(snaIsAdjacent(curnode,dest,g,2)){
      count[curlen]++;                                /*Basic update*/
      addListToCycleList(ids, curlen+1, cyclelist);   /*Add the cycle found to cyclelist*/

      if(byvertex){                                   /*Update path incidence counts*/
        for(j=0;j<curlen;j++) {
          count[curlen+(1+usednodes[j])*(maxlen-1)]++;
          count[curlen+(1+curnode)*(maxlen-1)]++;
          count[curlen+(1+dest)*(maxlen-1)]++;
        }
      }
      if(copaths==1){                                 /*Update copath incidence counts*/
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
      if(copaths==2){                                 /*Update copath counts using len*/
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
      if(dyadpaths==1){                               /*Update dyadic path counts*/
        dpcount[src+dest*n]++;
        if(!directed)
          dpcount[dest+src*n]++;
      }
      if(dyadpaths==2){                               /*Update dyadic path counts using len*/
        dpcount[curlen+src*(maxlen-1)+dest*(maxlen-1)*n]++;
        if(!directed)
          dpcount[curlen+dest*(maxlen-1)+src*(maxlen-1)*n]++;
      }
    }
  }
  else{
    if(snaIsAdjacent(dest,curnode,g,2)){
      count[curlen]++;                                /*Basic update*/

      addListToCycleList(ids, curlen+1, cyclelist);   /*Add the cycle found to cyclelist*/

      if(byvertex){                                   /*Update path incidence counts*/
        for(j=0;j<curlen;j++)
          count[curlen+(1+usednodes[j])*(maxlen-1)]++;
          count[curlen+(1+curnode)*(maxlen-1)]++;
          count[curlen+(1+dest)*(maxlen-1)]++;
      }
      if(copaths==1){                                 /*Update copath incidence counts*/
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
      if(copaths==2){                                 /*Update copath counts using len*/
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
      if(dyadpaths==1){                               /*Update dyadic path counts*/
        dpcount[src+dest*n]++;
        if(!directed)
          dpcount[dest+src*n]++;
      }
      if(dyadpaths==2){                               /*Update dyadic path counts using len*/
        dpcount[curlen+src*(maxlen-1)+dest*(maxlen-1)*n]++;
        if(!directed)
          dpcount[curlen+dest*(maxlen-1)+src*(maxlen-1)*n]++;
      }
    }
  }

  /*If possible, keep searching for novel paths*/
  if((availcount>0)&&(curlen<maxlen-2)){

    if(availcount>1){                                 /*Remove the current node from the available list*/
      if((newavail=(int *) malloc(sizeof(int)*(availcount-1)))==NULL){
        Rprintf("Unable to allocate %d bytes for available node list in edgewisePathRecurseID.  Trying to terminate recursion gracefully, but your path count is probably wrong.\n",sizeof(int)*(availcount-1));
        return;
      }
      j=0;
      for(i=0;i<availcount;i++)                       /*Create the reduced list, for passing*/
        if(availnodes[i]!=curnode)
          newavail[j++]=availnodes[i];
    }
    else
        newavail=NULL;                                /*Set to NULL if we're out of nodes*/
    newavailcount=availcount-1;                       /*Decrement the available count*/
    if(byvertex||copaths||dyadpaths){                 /*Add the current node to the used list*/
      if((newused=(int *) malloc(sizeof(int)*(curlen+1)))==NULL){
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
    if(newavail!=NULL){
      free((void *)newavail);
    }
    if(newused!=NULL){
      free((void *)newused);
    }
  }

  /*Free the used node list*/
  /*if(usednodes!=NULL)
   free((void *)usednodes);*/

  /*Check for interrupts (if recursion is taking far too long)*/
  R_CheckUserInterrupt();
}

/** Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether cycle counts should
 *  be broken down by participating vertex, and whether a cycle co-membership matrix should be returned (respectively).  In either case, count and cccount must be structured
 *  per count and pccount in edgewisePathRecurseID.
 */
void edgewiseCycleCensusID(snaNet *g, int src, int dest, double *count, double *cccount, int maxlen, int directed, int byvertex, int cocycles, CycleList *cyclelist, SEXP id_names)
{
  int n,i,j,*availnodes,*usednodes;

  /*Set things up*/
  n = g->n;
  usednodes = NULL;

  /*First, check for a 2-cycle (but only if directed)*/
  if(directed&&snaIsAdjacent(dest,src,g,2)){
    count[0]++;

    /*Add new Cycle to cyclelist*/
    Cycle *cycle = createCycle();
    appendNode((char *) CHAR(STRING_ELT(id_names, src)), cycle);
    appendNode((char *) CHAR(STRING_ELT(id_names, dest)), cycle);
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
    return;                                           /*Failsafe for graphs of order 2*/

  /*Perform the recursive path count*/
  if((availnodes=(int *) malloc(sizeof(int)*(n-2)))==NULL){
    Rprintf("Unable to allocate %d bytes for available node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int)*(n-2));
    return;
  }
  j=0;                                                /*Initialize the list of available nodes*/
  for(i=0;i<n;i++) {
    if((i!=src)&&(i!=dest))
      availnodes[j++]=i;
    if(byvertex||cocycles){                           /*Initialize the list of nodes already used */
      if((usednodes=(int *) malloc(sizeof(int)))==NULL){
        Rprintf("Unable to allocate %d bytes for used node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int));
        return;
      }
      usednodes[0]=dest;
    }
  }

  /*Create list of IDs*/
  int id_idx = 0;
  char *ids[maxlen];

  ids[id_idx++] =  (char *) CHAR(STRING_ELT(id_names, src));
  ids[id_idx++] =  (char *) CHAR(STRING_ELT(id_names, dest));

  /*Begin recursion*/
  for(i=0;i<n-2;i++) {                                /*Recurses on each available vertex*/
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

  if(availnodes!=NULL)
    free((void *)availnodes);                         /*Free the available node list*/
  if(usednodes!=NULL)
    free((void *)usednodes);                          /*Free the used node list, if needed*/

}

/** Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether
 *  cycle counts should be broken down by participating vertex, and whether cycle co-membership counts should be returned (respectively).  In either case,
 *  count and cccount must be structured per count and pccount in edgewisePathRecurseID.
 */

SEXP cycleCensusID_R(SEXP g_SEXP, SEXP pn_SEXP, SEXP pm_SEXP, SEXP count_SEXP,  SEXP cccount_SEXP,  SEXP pmaxlen_SEXP, SEXP pdirected_SEXP, SEXP pbyvertex_SEXP, SEXP pcocycles_SEXP, SEXP id_names_SEXP)
{
  /*Type conversions after using .Call()*/
  PROTECT(g_SEXP = AS_INTEGER(g_SEXP));
  int *g = INTEGER_POINTER(AS_INTEGER(g_SEXP));

  PROTECT(pn_SEXP = AS_INTEGER(pn_SEXP));
  int *pn = INTEGER_POINTER(pn_SEXP);

  PROTECT(pm_SEXP = AS_INTEGER(pm_SEXP));
  int *pm = INTEGER_POINTER(pm_SEXP);

  PROTECT(count_SEXP = AS_NUMERIC(count_SEXP));
  double *count = REAL(count_SEXP);
  PROTECT(cccount_SEXP = AS_NUMERIC(cccount_SEXP));
  double *cccount = REAL(cccount_SEXP);

  PROTECT(pmaxlen_SEXP = AS_INTEGER(pmaxlen_SEXP));
  int *pmaxlen = INTEGER_POINTER(pmaxlen_SEXP);
  PROTECT(pdirected_SEXP = AS_INTEGER(pdirected_SEXP));
  int *pdirected = INTEGER_POINTER(pdirected_SEXP);
  PROTECT(pbyvertex_SEXP = AS_INTEGER(pbyvertex_SEXP));
  int *pbyvertex = INTEGER_POINTER(pbyvertex_SEXP);
  PROTECT(pcocycles_SEXP = AS_INTEGER(pcocycles_SEXP));
  int *pcocycles = INTEGER_POINTER(pcocycles_SEXP);
  PROTECT(id_names_SEXP = coerceVector(id_names_SEXP, STRSXP));

  int i,r,c,n,m,id;
  double *dval;
  snaNet *ng;

  /*Allocate memory for the new graph object*/
  n=(*pn);
  m=(*pm);
  ng=(snaNet *)R_alloc(1,sizeof(struct snaNettype));
  ng->n=(*pn);
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
  for(i=0;i<m;i++){

    if((!IISNA(g[i+2*m]))&&((*pdirected)||(g[i]<g[i+m]))){
      r=g[i]-1;
      c=g[i+m]-1;

      /*First, accumulate the cycles to be formed by the (r,c) edge*/
      edgewiseCycleCensusID(ng,r,c,count,cccount,*pmaxlen,*pdirected,
        *pbyvertex,*pcocycles, cyclelist, id_names_SEXP);

      /*Next, add the (r,c) edge to the graph*/
      dval=(double *)R_alloc(1,sizeof(double));           /*Create iel element*/
      dval[0]=(double)g[i+2*m];
      ng->iel[c]=slistInsert(ng->iel[c],(double)r,(void *)dval);
      ng->indeg[c]++;
      dval=(double *)R_alloc(1,sizeof(double));           /*Create oel element*/
      dval[0]=(double)g[i+2*m];
      ng->oel[r]=slistInsert(ng->oel[r],(double)c,(void *)dval);
      ng->outdeg[r]++;

      if(!(*pdirected)){
        dval=(double *)R_alloc(1,sizeof(double));         /*Create iel element*/
        dval[0]=(double)g[i+2*m];
        ng->iel[r]=slistInsert(ng->iel[r],(double)c,(void *)dval);
        ng->indeg[r]++;
        dval=(double *)R_alloc(1,sizeof(double));         /*Create oel element*/
        dval[0]=(double)g[i+2*m];
        ng->oel[c]=slistInsert(ng->oel[c],(double)r,(void *)dval);
        ng->outdeg[c]++;
      }
    }
  }

  UNPROTECT(10);
  return cycleListToVector(cyclelist);
}

/** cycleToVector returns a vector containing the cycle ids.
 *  @param  cycle {Cycle}  [cycle of nodes]
 *  @return {SEXP}         [vector of node ids]
 */
SEXP cycleToVector(Cycle *cycle)
{
  int cycleLength = cycle -> size;
  SEXP result = PROTECT(allocVector(VECSXP, cycleLength));

  int i = 0;
  Node * currentNode = cycle -> head;
  while(currentNode != NULL){
    /*Make a string (STRSXP type) from the currentNode id*/
    SEXP id = PROTECT(mkString(currentNode->id));

    /*Add the node id to the vector at index i*/
    SET_VECTOR_ELT(result, i, id);
    currentNode = currentNode -> next;
    i++;
  }
  UNPROTECT(cycleLength);

  return result;
}

/** cycleListToVector returns a vector of vectors of node ids.
 *  @param  list {CycleList}   [list of cycles]
 *  @return {SEXP}             [vector of vectors of node ids]
 */
SEXP cycleListToVector(CycleList *list)
{
  int listLength = list -> size;

  /*Create vector with the length of the passed cycle list*/
  SEXP resultantVector = PROTECT(allocVector(VECSXP, listLength));

  int i = 0;
  CycleListNode * currentNode = list -> head;

  while(currentNode != NULL){

    /*Convert current list to cycle*/
    SEXP cycle = cycleToVector(currentNode->list);

    /*Add the new cycle to the vector at index i*/
    SET_VECTOR_ELT(resultantVector, i, cycle);
    currentNode = currentNode -> next;
    i++;
  }
  UNPROTECT(listLength + 1);

  return resultantVector;
}

SEXP cycleTestR(SEXP a){
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
