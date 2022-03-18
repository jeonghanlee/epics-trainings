/* caLesson2.c
   by Dirk Zimoch, 2007
   
   In this lesson, we get some more information out of EPICS
   and store the data a bit more structured.
   
   For this purpose, we define a "PV" structure and some macros
   to work on this structure. You should be familiar with macro
   programming.
*/

#include <stdio.h>

/* include EPICS headers */
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>

/* Strings describing the connection status of a channel */
const char *channel_state_str[4] = {
    "not found",
    "connection lost",
    "connected",
    "closed"
};

/* Define a "process variable" (PV) */
typedef struct {
    chid chid;
    int status;
    struct dbr_ctrl_double ctrl;
} epicsDoublePV;

/* print (some of) the contents of our PV */
void printPV(const epicsDoublePV* pv)
{
    if (ca_state(pv->chid) == cs_conn)
    {
        /* EPICS can give you a lot more infos than only the
           naked value. The most important one is the severity
           because if it is INVALID_ALARM, the IOC says:
           "Do not trust this value, it's something wrong with it."
           See /usr/local/epics/base/include/alarm.h.
           
           Precision and units are useful for displaying or prining
           the value. Limits can be used to scale a GUI display
           and to know the accepted range of set values.
           See /usr/local/epics/base/include/db_access.h for
           all existing DBR_* types and dbr_* structures.
        */
        printf("%s = %.*f %s %s range:[%.*f,%.*f] setrange:[%.*f,%.*f]\n",
            ca_name(pv->chid),
            pv->ctrl.precision, pv->ctrl.value, pv->ctrl.units,
            epicsAlarmSeverityStrings[pv->ctrl.severity],
            pv->ctrl.precision, pv->ctrl.lower_disp_limit,
            pv->ctrl.precision, pv->ctrl.upper_disp_limit,
            pv->ctrl.precision, pv->ctrl.lower_ctrl_limit,
            pv->ctrl.precision, pv->ctrl.upper_ctrl_limit);           
    }
    else
    {
        printf("%s: <%s>\n",
            ca_name(pv->chid), channel_state_str[ca_state(pv->chid)]);
    }
}

/* Define a macro around ca_get to work with the above structure.
   It first checks that the channel is currently connected and then
   does the actual ca_get. Finally, the status is stored in the PV and
   SEVCHK is called to print an error message if the status indicates failure.
   See /usr/local/epics/base/include/cadef.h for SEVCHK.
   The DBR_* type must match the dbr_* structure where the data is stored.
*/
#define caget(pv) SEVCHK(\
    (pv).status = (ca_state((pv).chid) != cs_conn ? ECA_DISCONN : \
    ca_get(DBR_CTRL_DOUBLE, (pv).chid, &(pv).ctrl)), ca_name((pv).chid))

int main()
{
    epicsDoublePV beamcurrent, gapread, gapset;
    double search_timeout = 5.0; /* seconds */
    double get_timeout = 1.0; /* seconds */
    int status;
    
    /* Step1: initialize channel access and search for all channels. */
    ca_task_initialize();
    ca_search("ARIDI-PCT:CURRENT", &beamcurrent.chid);
    ca_search("X10SA-ID-GAP:READ", &gapread.chid);
    ca_search("X10SA-ID-GAP:SET", &gapset.chid);

    /* Send all collected searches and wait in parallel
       until they have connected (or until time runs out).
    */
    status = ca_pend_io(search_timeout);
    
    /* Use channel access error reporting facility */
    SEVCHK(status, "ca_search");
    
    /* You may also try what happens if you misspell
       one of the above channel names.
    */  
    
    /* Step 2: get value plus other important infos */
    caget(beamcurrent);
    caget(gapread);
    caget(gapset);
    /* Send all collected requests and wait until all have returned. */
    status = ca_pend_io(get_timeout);
    SEVCHK(status, "ca_get");
    
    /* Step 3: use the data */
    printPV(&beamcurrent);
    printPV(&gapread);
    printPV(&gapset);
    
    /* Last step: free all channel access resources */
    ca_task_exit();
    return 0;  
}
