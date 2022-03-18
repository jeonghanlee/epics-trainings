/* caLesson5a.c
   by Dirk Zimoch, 2007
   
   This lesson introduces writing to channels and waiting for channels.
   We will write a value to a (simulated) device which takes a while and
   wait until the device is done. The device provides a DONE record.
   We will use a monitor to get informed when the device is done without
   polling it over the network.
   
   A configuration file for a soft IOC is provided in this directory.
   Before trying this program, start the soft IOC with:
   
       xterm -e ioch caLesson5.db P=prefix &
       
   where prefix should be something unique to you (e.g. your initials).
   
   Use this program with the same prefix:
   
      caLesson5a prefix

   
   This is a single-threaded EPICS 3.13. compatible version of the example.
   For the EPICS 3.14. multi-threaded version see caLesson5b.c.
   
*/

#include <stdio.h>
#include <string.h>

/* include EPICS headers */
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>

/* Strings describing the connection status of a channel.
   See also enum channel_state in /usr/local/epics/base/include/cadef.h
*/
const char *channel_state_str[4] = {
    "not found",
    "connection lost",
    "connected",
    "closed"
};

/* Define process variable (PV) structures.
   Each PV contains static information in info and
   current value in data.
   See /usr/local/epics/base/include/db_access.h for fields
   of dbr_* structures.
*/

typedef struct {
    chid channel;
    struct dbr_ctrl_double info;
    struct dbr_sts_double data;
} epicsDoublePV;

typedef struct {
    chid channel;
    struct dbr_ctrl_enum info;
    struct dbr_sts_enum data;
} epicsEnumPV;


/* Print NAME = VALUE for double.
   Precision and units are taken from info.
*/
void printDoublePV(const epicsDoublePV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s <%s>\n",
            ca_name(pv->channel),
            channel_state_str[ca_state(pv->channel)]);
        return;
    }

    printf("%s = %.*f %s",
        ca_name(pv->channel),
        pv->info.precision, pv->data.value, pv->info.units);

    if (pv->data.severity != NO_ALARM)
    {
        printf(" <%s %s>\n",
            epicsAlarmSeverityStrings[pv->data.severity],
            epicsAlarmConditionStrings[pv->data.status]);
    }
    else
    {
        printf("\n");
    }
}

/* Print NAME = VALUE for enum.
   VALUE is printed as string if possible, otherwise as number.
*/
void printEnumPV(const epicsEnumPV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s <%s>\n",
            ca_name(pv->channel),
            channel_state_str[ca_state(pv->channel)]);
        return;
    }
    if (pv->data.value < pv->info.no_str)
        printf("%s = %s",
            ca_name(pv->channel), pv->info.strs[pv->data.value]);
    else
    {
        printf("%s = %d",
            ca_name(pv->channel), pv->data.value);
    }
    
    if (pv->data.severity != NO_ALARM)
    {
        printf(" <%s %s>\n",
            epicsAlarmSeverityStrings[pv->data.severity],
            epicsAlarmConditionStrings[pv->data.status]);
    }
    else
    {
        printf("\n");
    }
}

/* Generic monitor event handler.
   See /usr/local/epics/base/include/cadef.h for the definition of
   struct event_handler_args. 
   This handler copies the new value into the PV and writes a message.
   We get the address of the PV in the 'usr' field of 'args'
   because we give that as the 4th argument to ca_add_event (see below).
*/

static void monitor(struct event_handler_args args)
{
    printf("Monitor: ");
    if (args.status != ECA_NORMAL)
    {
        /* Something went wrong. */
        SEVCHK(args.status, "monitor");
        return;
    }
    /* Copy the value to the 'data' field of the PV.      
       The current data, its type and the number of elements (for arrays) is
       stored in several fields of 'args'.
    */
    switch (args.type)
    {
        case DBR_STS_DOUBLE:
        {
            epicsDoublePV* pv = args.usr;
            memcpy(&pv->data, args.dbr, dbr_size_n(args.type, args.count));
            printDoublePV(pv);
            break;
        }
        case DBR_STS_ENUM:
        {
            epicsEnumPV* pv = args.usr;
            memcpy(&pv->data, args.dbr, dbr_size_n(args.type, args.count));
            printEnumPV(pv);
            break;
        }
        default:
        {
            printf ("%s unsupported data type\n", ca_name(args.chid));
        }
    }
}

int main(int argc, char** args)
{
    char recordname[28];
    double search_timeout = 5.0; /* seconds */
    double put_timeout = 1.0; /* seconds */
    double get_timeout = 1.0; /* seconds */
    
    int status;
    CA_SYNC_GID gid; /* required for blocking put (see below) */
    
    epicsDoublePV setvalue;
    epicsDoublePV readvalue;
    epicsEnumPV doneflag;
    
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <prefix>\n"
            "Where <prefix> is a prefix to :SET, :READ and :DONE.\n",
            args[0]);
        return 1;
    }
    
    /* Step1: Initialize channel access and search for all channels. */
    ca_task_initialize();
    
    sprintf(recordname, "%.19s:SET", args[1]);
    ca_search(recordname, &setvalue.channel);
    
    sprintf(recordname, "%.19s:READ", args[1]);
    ca_search(recordname, &readvalue.channel);
    
    sprintf(recordname, "%.19s:DONE", args[1]);
    ca_search(recordname, &doneflag.channel);
    
    SEVCHK(status = ca_pend_io(search_timeout), "searching channels");
    if (status != ECA_NORMAL) goto end;

    /* Step 2: Get available infos and setup monitor for DONE flag*/

    ca_get(DBR_CTRL_DOUBLE, setvalue.channel, &setvalue.info);
    ca_get(DBR_CTRL_DOUBLE, readvalue.channel, &readvalue.info);
    ca_get(DBR_CTRL_ENUM, doneflag.channel, &doneflag.info);
    ca_add_event(DBR_STS_DOUBLE, readvalue.channel, monitor, &readvalue, NULL);
    ca_add_event(DBR_STS_ENUM, doneflag.channel, monitor, &doneflag, NULL);

    SEVCHK(status = ca_pend_io(search_timeout), "initializing channels");
    if (status != ECA_NORMAL) goto end;


    /* Create the "synchronous group id" (gid) used later for put. */
    SEVCHK(status = ca_sg_create(&gid), "creating synchronous group");
    if (status != ECA_NORMAL) goto end;
    
    /* Step 3: Enter main loop */
    while (1)
    {
        char userinput[40];
        double newvalue;
        
        /* Get current setting */
        ca_get(DBR_STS_DOUBLE, setvalue.channel, &setvalue.data);
        SEVCHK(ca_pend_io(search_timeout), ca_name(setvalue.channel));
        printDoublePV(&setvalue);

        /* Ask for new setting */
        if (setvalue.info.lower_ctrl_limit < setvalue.info.upper_ctrl_limit)
        {
            printf("Enter new value (range %.*f~%.*f): ",
                setvalue.info.precision, setvalue.info.lower_ctrl_limit, 
                setvalue.info.precision, setvalue.info.upper_ctrl_limit);
        }
        else
        {
            /* No limits known */
            printf("Enter new value: ");
        }
        fflush(stdout);
        fgets(userinput, sizeof(userinput), stdin);
        if (sscanf(userinput, "%lf", &newvalue) != 1)
        {
            printf("Invalid input \"%s\". Need a number.\n", userinput);
            continue;
        }

        /* Set new value and wait to complete.
        
           This is a very important timing issue!
           The records are build in a way that the DONE record
           is set to "ACTIVE" before a put to the SET record completes.
           Insider info: They are linked via FLNK and PP output links.

           Timing:
           
           ca_put        (0)-(1)---(2)----(3)
                          :
           ca_sg_put     (0)-------(2)----(3)
                          :
           "DONE"    =====*=====*       *========
                                |       |
           "ACTIVE"             *=======*
           
           Some time after the put (0), the device becomes active.
           
           A normal put may return before (1), while (2), or after (3)
           activity. A following reading of the DONE record cannot
           distinguish between (1) and (3).
           
           However, a synchonous put will not return before (2).
           Thus, we can wait until the DONE record becomes "DONE".
           If it returns late after activity finished (3), the DONE record
           is already "DONE" and we don't need to wait. But we can never be
           in the situation that the DONE record is not yet "ACTIVE" (1).
           
           To implement a synchronous put, we use the "synchronous group"
           mechanism. ca_sg_block does not return until all outstanding
           ca_sg_* requests with the same gid have completed (or timeout).
           
           Note that we put a bare double here, no DBR_CTRL_*.
        */
        ca_sg_put(gid, DBR_DOUBLE, setvalue.channel, &newvalue);
        SEVCHK(ca_sg_block(gid, put_timeout), ca_name(setvalue.channel));
        
        /* Wait until activity is done.
           This uses the monitor on the DONE record.
           Remember that monitors are handled in the background, but only
           while certain ca functions are active, e.g. ca_pend_event(),
           ca_pend_io(), or ca_sg_block().
        */
        
        /* First actively read current value of DONE record. */
        ca_get(DBR_STS_ENUM, doneflag.channel, &doneflag.data);
        SEVCHK(status = ca_pend_io(get_timeout), ca_name(doneflag.channel));
        printEnumPV(&doneflag);
        
        /* When not already done, wait for monitor events */
        while (!doneflag.data.value)
        {
            /* wait for the next monitor events */
            ca_pend_event(0.01);
            /* Unfortunately, there is no function "wait until next event".
               Thus, we do "wait for 0.01 seconds and process events" in
               a loop.
               Note that we don't poll the IOC for "DONE"!.
               This only generates network traffic when something happens
               with a maximum latency of 0.01 seconds.
            */
        }
        printEnumPV(&doneflag);
    }
        
    /* Step 4: clean up */
    ca_sg_delete(gid);
end:
    ca_task_exit();
    return 0;  
}
