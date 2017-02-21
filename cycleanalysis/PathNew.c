
#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include "utils.h"
#include "paths.h"


void cycleCensusID_R(int *id[],int *g, int *pn, int *pm, double *count, double *cccount, int *pmaxlen, int *pdirected, int *pbyvertex, int *pcocycles)
/*Count the number of cycles associated with the (src,dest) edge in g, assuming that this edge exists.  The byvertex and cocycles flags indicate whether cycle counts should be broken down by participating vertex, and whether cycle co-membership counts should be returned (respectively).  In either case, count and cccount must be structured per count and pccount in edgewisePathRecurse.*/
{
  int i,r,c,n,m;
  double *dval;
  snaNet *ng;

  GetRNGstate();
  /*Allocate memory for the new graph object*/
  /*Rprintf("Initializing ng\n");*/
  n=(*pn);
  m=(*pm);
  ng=(snaNet *)R_alloc(1,sizeof(struct snaNettype));
  ng->n=(*pn);
  ng->indeg=(int *)R_alloc(n,sizeof(int));
  ng->outdeg=(int *)R_alloc(n,sizeof(int));
  ng->iel=(slelement **)R_alloc(n,sizeof(slelement *));
  ng->oel=(slelement **)R_alloc(n,sizeof(slelement *));

  /*Initialize the graph*/
  for(i=0;i<n;i++){
    ng->indeg[i]=0;
    ng->outdeg[i]=0;
    ng->iel[i]=NULL;
    ng->oel[i]=NULL;
  }

  /*Walk the graph, adding edges and accumulating cycles*/
  /*Rprintf("Building graph/accumulating cycles\n\tn=%d,%d\n",n,ng->n);*/
  for(i=0;i<m;i++)
    if((!IISNA(g[i+2*m]))&&((*pdirected)||(g[i]<g[i+m]))){
      r=g[i]-1;
      c=g[i+m]-1;
      /*First, accumulate the cycles to be formed by the (r,c) edge*/
      /*Rprintf("\tEdge at (%d,%d); counting cycles\n",r+1,c+1);*/
      edgewiseCycleCensus(ng,r,c,count,cccount,*pmaxlen,*pdirected,
        *pbyvertex,*pcocycles);
      /*for(k=0;k<*pmaxlen-1;k++)
      Rprintf("%d:%d ",k+2,(int)(count[k]));
      Rprintf("\n");*/
      /*Next, add the (r,c) edge to the graph*/
      /*Rprintf("\tGot cycles, now adding edge to ng\n");*/
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

  PutRNGstate();
}
