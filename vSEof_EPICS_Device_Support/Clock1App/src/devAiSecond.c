/**
   devAiSecond.c
   kazuro furukawa, dec.14.2006.

**/
/*NOTUSED*/
static char what[] =
"@(#)devAiSecond v0.1 support for Second, k.furukawa, dec.2006";

#define DEBUG_ON

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "aiRecord.h"
#include "link.h"
#include "epicsExport.h"
#include "devAiSecond.h"

/* Create the dset for devAiSecond */
static long init_record();
static long init_ai();
static long read_ai();
struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_ai;
	DEVSUPFUN	special_linconv;
} devAiSecond = {
	6,
	NULL,
	init_ai,
	init_record,
	NULL,
	read_ai,
	NULL
};
epicsExportAddress(dset,devAiSecond);


/************************************************************************/
/* Ai Record								*/
/*  INP = "hostname-or-ipaddress:data-number"				*/
/************************************************************************/

/* init_ai for debug */
static long init_ai(int after)
{
#ifdef DEBUG_ON 
  printf("devAiSecond (init) called, pass=%d\n", after);
#endif
  return(0);
}

static long init_record(struct aiRecord *pai)
{
#ifdef DEBUG_ON
  printf("devAiSecond (init_record) called\n");
#endif
  pai->udf = FALSE;
  return(0);
}


static long read_ai(struct aiRecord *pai)
{
  time_t t;
  struct tm *tm;
  
#ifdef DEBUG_ON 
  printf("devAiSecond (read_ai) accessed\n");
#endif
  time(&t);
  tm = localtime(&t);
  pai->udf = FALSE;
  pai->rval = tm->tm_sec;
#ifdef DEBUG_ON 
  printf("devAiSecond (read_ai) data %d\n", pai->rval);
#endif
  return(0);				/* convert */

#if 0					/* for non-conversion */
  pai->val = *get_val;
  return(2);				/* do not convert */
#endif
}
