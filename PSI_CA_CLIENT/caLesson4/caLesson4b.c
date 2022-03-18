/* caLesson4.c
   by Dirk Zimoch, 2007
   
   In this lesson we will learn to use monitors to read channels
   whenever they change instead of polling them.
   
   Whenever you need to know about changes quickly, use monitors
   instead of high rate polling. It unnecessarily wastes network
   bandwidth to ask for a value 10 times per second when it only
   changes about once per minute. With any poll rate, you will
   always have a delay and you might still miss short peaks. With
   monitors you won't. And it only produces network traffic when
   something "interesting" happens.
   
   For analog (i.e. DOUBLE) values, it is defined in the record
   how much change is required to be "interesting". For other types,
   e.g. ENUM, every change is "interesting".
   
   To reduce the level of confusion, we leave away PV and macros for
   now and use the CA fundtions directly.

   This file uses EPICS 3.14 functions.
*/

#include <stdio.h>

/* include EPICS headers */
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>

/* This is a user-defined callback function.
   Whenever a channel has a new value, this function is called.
   See /usr/local/epics/base/include/cadef.h for the definition of
   struct event_handler_args. 
   We don't use the fields 'count' and 'usr' here. The field 'count'
   if for arrays (that comes later) and 'usr' is an arbitrary pointer
   which you can pass to the monitor installation function (see below).
*/   
static void monitor(struct event_handler_args args)
{
    if (args.status != ECA_NORMAL)
    {
        /* Something went wrong. */
        SEVCHK(args.status, "monitor");
        return;
    }
    /* Let's have a look at the type of the data.
       It should be one of the types that we have requested.
    */
    switch (args.type)
    {
        case DBR_STS_DOUBLE:
            {
                const struct dbr_sts_double* data = args.dbr;
                printf ("%s = %#g  %s\n",
                    ca_name(args.chid), data->value,
                        epicsAlarmSeverityStrings[data->severity]);
                break;
            }
        case DBR_STS_ENUM:
            {
                const struct dbr_sts_enum* data = args.dbr;
                printf ("%s = %i  %s\n",
                    ca_name(args.chid),
                        data->value,
                        epicsAlarmSeverityStrings[data->severity]);
                break;
            }
        default:
            printf ("%s unsupported data type\n", ca_name(args.chid));
    }
}

int main()
{
    char* gapName="X10SA-ID-GAP:READ";
    char* doneName="X10SA-ID-GAP:DONE";
    chid gapChannel, doneChannel;
    double search_timeout = 5.0; /* seconds */
    
    /* Step1: initialize channel access and search for all channels. */
    ca_context_create(ca_disable_preemptive_callback);
    
    /* ca_create_channel has more parameters than the old ca_search
       but we don't need them here.
    */
    ca_create_channel(gapName, NULL, NULL, CA_PRIORITY_DEFAULT, &gapChannel);
    ca_create_channel(doneName, NULL, NULL, CA_PRIORITY_DEFAULT, &doneChannel);
    
    SEVCHK(ca_pend_io(search_timeout), "ca_search");

    /* Step 2: setup the monitors */
    
    /* Create two monitors with different data types.
       Connect them to the same callback function.
       The 6th argument will be passed to the 'usr' element
       in the handler arguments. We don't need it here.
    */    
    ca_create_subscription(DBR_STS_DOUBLE, 1, gapChannel,
        DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    ca_create_subscription(DBR_STS_ENUM, 1, doneChannel,
        DBE_VALUE|DBE_ALARM, monitor, NULL, NULL);
    /* In 3.13 we have actually used a macro with default
       values for some arguments. Here, we have to specify:
          * we want scalars, not arrays (count=1)
          * we are interested in value and alarm changes
    */
    
    SEVCHK(ca_flush_io(), "ca_add_event");    
    /* We have used ca_flush_io() here because there is nothing
       to wait for. We just send out the request.
       Note: ca_pend_io(timeout) works like ca_flush_io() plus
       additional waiting for outstanding replies.
    */
    
    /* Step 3: wait forever and do Channel Access in the background */
    ca_pend_event(0.0);

    /* We should never reach this point! */
    printf("Done\n");
    ca_context_destroy();
    return 0;  
}
