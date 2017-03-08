
/*
######################################################################
#
# paths.h
#
# copyright (c) 2007, Carter T. Butts <buttsc@uci.edu>
# Last Modified 3/27/09
# Licensed under the GNU General Public License version 2 (June, 1991)
#
# Part of the R/sna package
#
# This file contains headers for paths.c.
#
######################################################################
*/
#ifndef PATHS_EXTENSION_H
#define PATHS_EXTENSION_H

/*DECLARATIONS/INCLUSIONS---------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include <Rinternals.h>
#include "utils.h"
#include "Cycle.h"
#include "CycleList.h"
#include "string.h"

void cycleCensusID_R(int *g, int *pn, int *pm, double *count, double *cccount, int *pmaxlen, int *pdirected, int *pbyvertex, int *pcocycles, char **id_names);
void edgewiseCycleCensusID(snaNet *g, int src, int dest, double *count, double *cccount, int maxlen, int directed, int byvertex, int cocycles, CycleList *cyclelist, char **id_names);
void edgewisePathRecurseID(snaNet *g, int src, int dest, int curnode, int *availnodes, int availcount, int *usednodes, int curlen, double *count,
                           double *cpcount, double *dpcount, int maxlen, int directed, int byvertex, int copaths, int dyadpaths, char *ids[],
                           int id_idx, CycleList *cyclelist, char **id_names);


SEXP cycleToVector(Cycle *cycle);
SEXP cycleListToVector(Cycle *list);
#endif

