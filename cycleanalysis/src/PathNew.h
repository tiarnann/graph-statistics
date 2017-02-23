
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
#ifndef PATHS_H
#define PATHS_H

/*DECLARATIONS/INCLUSIONS---------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <R.h>
#include "utils.h"




void cycleCensusID_R(int *id[],int *g, int *pn, int *pm, double *count, double *cccount, int *pmaxlen, int *pdirected, int *pbyvertex, int *pcocycles);


#endif

