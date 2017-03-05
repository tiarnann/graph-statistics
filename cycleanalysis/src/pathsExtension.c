#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include "utils.h"
#include "paths.h"
#include "pathsExtension.h"
#include "Cycle.h"
#include "CycleList.h"
#include "string.h"

void edgewiseCycleCensusID(snaNet *g, int src, int dest, double *count, double *cccount, int maxlen, int directed, int byvertex, int cocycles, CycleList *cyclelist)
  /*Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether cycle counts should be broken down by participating vertex, and whether a cycle co-membership matrix should be returned (respectively).  In either case, count and cccount must be structured per count and pccount in edgewisePathRecurse.*/
{
  int n,i,j,*availnodes,*usednodes;

  /*Set things up*/
  n = g->n;
  usednodes = NULL;

  /*First, check for a 2-cycle (but only if directed)*/
  Rprintf("\t\tChecking for (%d,%d) edge\n",src+1,dest+1);
  if(directed&&snaIsAdjacent(dest,src,g,2)){
    count[0]++;

    //add new Cycle to CycleList
    Cycle *cycle = createCycle();
    char *tmp = src;
    appendNode(tmp, cycle);
    tmp = dest;
    appendNode(tmp, cycle);
    appendCycle(cycle, cyclelist);  //TODO check this is correct

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
    return;                 /*Failsafe for graphs of order 2*/

  /*Perform the recursive path count*/
  if((availnodes=(int *)malloc(sizeof(int)*(n-2)))==NULL){
    Rprintf("Unable to allocate %d bytes for available node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int)*(n-2));
    return;
  }
  j=0;                              /*Initialize the list of available nodes*/
  for(i=0;i<n;i++)
    if((i!=src)&&(i!=dest))
      availnodes[j++]=i;
    if(byvertex||cocycles){          /*Initialize the list of already used nodes*/
  if((usednodes=(int *)malloc(sizeof(int)))==NULL){
    Rprintf("Unable to allocate %d bytes for used node list in edgewiseCycleCensus.  Exiting.\n",sizeof(int));
    return;
  }
  usednodes[0]=dest;
    }
    /*Rprintf("\t\tBeginning recursion\n");*/
    for(i=0;i<n-2;i++)               /*Recurse on each available vertex*/
    if(directed||(dest<availnodes[i])){
      if(snaIsAdjacent(dest,availnodes[i],g,2))
        edgewisePathRecurse(g,dest,src,availnodes[i],availnodes,n-2,usednodes,1,
                            count,cccount,NULL,maxlen,directed,byvertex,cocycles,0);
    }else{
      if(snaIsAdjacent(availnodes[i],dest,g,2))
        edgewisePathRecurse(g,dest,src,availnodes[i],availnodes,n-2,usednodes,1,
                            count,cccount,NULL,maxlen,directed,byvertex,cocycles,0);
    }
    /*Rprintf("\t\tReturned from recursion; freeing memory\n");*/
    if(availnodes!=NULL)
      free((void *)availnodes);  /*Free the available node list*/
    if(usednodes!=NULL)
      free((void *)usednodes); /*Free the used node list, if needed*/
}





void cycleCensusID_R(int *g, int *pn, int *pm, double *count, double *cccount, int *pmaxlen, int *pdirected, int *pbyvertex, int *pcocycles)
/*Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether cycle counts should be broken down by participating vertex, and whether cycle co-membership counts should be returned (respectively).  In either case, count and cccount must be structured per count and pccount in edgewisePathRecurse.*/
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
  Rprintf("Building graph/accumulating cycles\n\tn=%d,%d\n",n,ng->n);
  for(i=0;i<m;i++){

    if((!IISNA(g[i+2*m]))&&((*pdirected)||(g[i]<g[i+m]))){
      r=g[i]-1;
      c=g[i+m]-1;

      // *********** NB **************
      // Node ID: g[i] Node ID: g[i+m]
      // *********** NB **************
      Rprintf("ID:%d ID:%d\n",r+1,c+1);
      // *********** NB **************
      // Node ID: g[i] Node ID: g[i+m]
      // *********** NB **************


      /*First, accumulate the cycles to be formed by the (r,c) edge*/
      Rprintf("\tEdge at (%d,%d); counting cycles\n",r+1,c+1);
      edgewiseCycleCensusID(ng,r,c,count,cccount,*pmaxlen,*pdirected,
        *pbyvertex,*pcocycles, cyclelist);
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

