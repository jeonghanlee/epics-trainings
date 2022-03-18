/* caLesson3.c
   by Dirk Zimoch, 2007
   
   Meanwhile, we know how to read double values from EPICS.
   Now we will learn about other data types. Some include
   more information than others.
   And we will read in a loop.
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

/* Define a "process variable" (PV)
   Note that it now contains two different buffers, info and data.
   See /usr/local/epics/base/include/db_access.h for
   different dbr_* structures.
   We use dbr_ctrl_* to store full information about a PV. This
   information normally does not change, it is "static". Thus,
   we need to read it only once.
   We use dbr_sts_* structures to store the "dynamic" data. This
   normally changes frequently. Thus, we read it repeatedly.
   We define different types of PVs for different data types.
   When reading info and data (via cainfo and caget) we must specify
   what data type we have. We use the DBF_* macros for that.
   At the moment, only support DBF_DOUBLE, DBF_LONG, DBF_STRING, DBF_ENUM.
   
   Note: The info structure is nice for displaying and formatting a value
   but it is not necessary. You can perfectly live without it and use
   only dbr_sts_* structures in your program.
   But always read at least dbr_sts_* (or longer structures), because it
   contains the very important severity information.
   If severity is INVALID_ALARM (=3) (see /usr/local/epics/base/include/alarm.h)
   the value is really not valid! One reason can be that the driver on the
   IOC cannot read the value from the hardware.
   Don't ignore that or your application may read rubbish and fail unexpectedly.
*/

typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_double info;
    struct dbr_sts_double data;
} epicsDoublePV;

/* There is more than just double values, e.g. long integers */
typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_long info;
    struct dbr_sts_long data;
} epicsLongPV;

/* Enums are something special: they have a table of strings attached */
typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_enum info;
    struct dbr_sts_enum data;
} epicsEnumPV;

/* Strings don't have a dbr_ctrl_ structure. They are too primitive */
typedef struct {
    chid channel;
    int status;
    struct dbr_sts_string data;
} epicsStringPV;

/* Print the contents of a PV
   We have have to print different information for different
   types of PVs. Note how static information (e.g. 'units') is taken from info
   and dynamic information ('value' and 'severity') is taken from from data.
   Also note that the data type of a PV is not necessarily the same as the
   native type of the channel. EPICS converts the types automatically.
   
   In a later lesson, we will learn how to select the correct dbr_* structure
   automatically from the native channel type. This is useful for interfaces
   to scripting languages.
*/
void printDoublePV(const epicsDoublePV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value, units, severity, range and setrange */
    printf("%s (%s as DOUBLE) = %#.*f %s  %s  range:[%#.*f ... %#.*f]  setrange:[%#.*f ... %#.*f]\n",

        /* Get name and native type from channel ID */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        
        /* Get static info 'precision' and 'units', and dynamic data 'value' */
        pv->info.precision, pv->data.value, pv->info.units,
        
        /* Get dynamic data 'severity' */
        epicsAlarmSeverityStrings[pv->data.severity],
        
        /* Get more static infos */
        pv->info.precision, pv->info.lower_disp_limit,
        pv->info.precision, pv->info.upper_disp_limit,
        pv->info.precision, pv->info.lower_ctrl_limit,
        pv->info.precision, pv->info.upper_ctrl_limit);           
}

void printLongPV(const epicsLongPV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value, units, severity, range and setrange
       Note: In EPICS, LONG means 32 bit, INT and SHORT both mean 16 bit.
    */
    printf("%s (%s as LONG) = %i %s  %s  range:[%i ... %i]  setrange:[%i ... %i]\n",

        /* This is similar to the above case but uses long everywhere */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        pv->data.value, pv->info.units,
        epicsAlarmSeverityStrings[pv->data.severity],
        pv->info.lower_disp_limit,
        pv->info.upper_disp_limit,
        pv->info.lower_ctrl_limit,
        pv->info.upper_ctrl_limit);
}

void printEnumPV(const epicsEnumPV* pv)
{
    int i;
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value as number and as string, severity, and all defined strings.
       
       Note that enums don't have units and instead of a range,
       they have a list of strings - one for each state (max 16).
    */
    printf("%s (%s as ENUM) = %i = \"%s\"  %s  %i strings:",
    
        /* Get name and native type from channel ID */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        
        /* Get dynamic data 'value' and convert it to a string using the
           static information 'strs'. Never forget to check if value
           is within bounds or unexpected "segmentation fault" crashes
           may happen.
        */
        pv->data.value,
        pv->data.value < pv->info.no_str ? pv->info.strs[pv->data.value] : "",
            
        /* Get dynamic data 'severity' */
        epicsAlarmSeverityStrings[pv->data.severity],
        
        /* Get all defined stings for this channel (from static information) */
        pv->info.no_str);
    for (i = 0; i < pv->info.no_str; i++)
    {
        printf("%s\"%s\"", i>0 ? "," : "", pv->info.strs[i]);
    }
    printf ("\n");
}

void printStringPV(const epicsStringPV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value and severity */
    printf("%s (%s as STRING) = \"%s\"  %s\n",
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        pv->data.value, 
        epicsAlarmSeverityStrings[pv->data.severity]);           
}

/* Connect a PV to a channel */
#define casearch(name, pv) SEVCHK(\
    (pv).status = (ca_search((name), &(pv).channel)), name)


/* Wrapper macros to read 'info' and 'data' fields of a PV
   Call cainfo once during initialization to get all available infos.
   Later, call caget to get only the dynamic information (value and severity).
   Both macros need an additiional DBF_* argument to know the data type.
   The dbf_type_to_DBR_* macros from db_access.h are used to translate
   DBF_* to the full data structure (either DBR_CTRL_* or DBR_STS_*).
   Both macros do nothing when the channel is disconnected.
*/

/* get full information about a PV */
#define cainfo(pv, type) SEVCHK(\
    (pv).status = (ca_state((pv).channel) != cs_conn ? ECA_DISCONN : \
    ca_get(dbf_type_to_DBR_CTRL(type), (pv).channel, &(pv).info)), ca_name((pv).channel))

/* get only usually changing information about a PV */
#define caget(pv, type) SEVCHK(\
    (pv).status = (ca_state((pv).channel) != cs_conn ? ECA_DISCONN : \
    ca_get(dbf_type_to_DBR_STS(type), (pv).channel, &(pv).data)), ca_name((pv).channel))

int main()
{
    epicsDoublePV gapreadD, gapdoneD;
    epicsLongPV gapreadL, gapdoneL;
    epicsStringPV gapreadS, gapdoneS;
    epicsEnumPV gapreadE, gapdoneE;
    double search_timeout = 5.0; /* seconds */
    double get_timeout = 1.0; /* seconds */
    double loop_period = 5.0; /* seconds */
    int num_turns = 6;
    int i;
    
    /* Step1: initialize channel access and search for all channels. */
    ca_task_initialize();
    
    /* Let's have a look how EPICS conencts to different types of PVs.
       We try here to connect an analogue value and a discrete value
       to double, long, string and enum PVs. We'll see what happens.
    */
    casearch("X10SA-ID-GAP:READ", gapreadD);
    casearch("X10SA-ID-GAP:DONE", gapdoneD);
    casearch("X10SA-ID-GAP:READ", gapreadL);
    casearch("X10SA-ID-GAP:DONE", gapdoneL);
    casearch("X10SA-ID-GAP:READ", gapreadS);
    casearch("X10SA-ID-GAP:DONE", gapdoneS);
    casearch("X10SA-ID-GAP:READ", gapreadE);
    casearch("X10SA-ID-GAP:DONE", gapdoneE);

    SEVCHK(ca_pend_io(search_timeout), "casearch");
    
    /* Step 2: get available infos */
    
    /* We get all the static information now and the dynamic
       information in a loop. That gives us all the infos we
       need for nice formatting etc., but saves network bandwidth
       in the loop.
       
       Take care that the DBF_* matches the PV type.
       
       Skip the string PVs here because they don't have info.
    */   
    cainfo(gapreadD, DBF_DOUBLE);
    cainfo(gapdoneD, DBF_DOUBLE);
    cainfo(gapreadL, DBF_LONG);
    cainfo(gapdoneL, DBF_LONG);
    cainfo(gapreadE, DBF_ENUM);
    cainfo(gapdoneE, DBF_ENUM);

    /* The above cainfo calls only fill the 'info' structure of the PV.
       Let's as well fill the 'data' structure. Of course we could
       as well do this in the loop, but I want to print the values
       once before entering the loop.
    */
    caget(gapreadD, DBF_DOUBLE);
    caget(gapdoneD, DBF_DOUBLE);
    caget(gapreadL, DBF_LONG);
    caget(gapdoneL, DBF_LONG);
    caget(gapreadS, DBF_STRING);
    caget(gapdoneS, DBF_STRING);
    caget(gapreadE, DBF_ENUM);
    caget(gapdoneE, DBF_ENUM);
    
    /* Send all above requests in parallel and wait for reply */
    SEVCHK(ca_pend_io(get_timeout), "cainfo");
    
    printf("Init\n");
    printDoublePV(&gapreadD);
    printDoublePV(&gapdoneD);
    printLongPV(&gapreadL);
    printLongPV(&gapdoneL);
    printStringPV(&gapreadS);
    printStringPV(&gapdoneS);
    printEnumPV(&gapreadE);
    printEnumPV(&gapdoneE);
        
    /* Step 3: enter the main loop */
    
    for (i = 1; i <= num_turns; i++)
    {
        /* Wait some time while doing channel access in the background.
           This allows to handle disconnection/connection and other
           "background activity" while we wait. We will learn more
           about this in future lessons when we use callbacks and
           monitors.
        */
        printf("Waiting for %g seconds\n", loop_period);
        ca_pend_event(loop_period);
        
        /* Get all current values and print them */
        printf("Turn %d/%d\n", i, num_turns);

        caget(gapreadD, DBR_DOUBLE);
        caget(gapdoneD, DBR_DOUBLE);
        caget(gapreadL, DBR_LONG);
        caget(gapdoneL, DBR_LONG);
        caget(gapreadS, DBR_STRING);
        caget(gapdoneS, DBR_STRING);
        caget(gapreadE, DBR_ENUM);
        caget(gapdoneE, DBR_ENUM);
        SEVCHK(ca_pend_io(get_timeout), "caget");

        printDoublePV(&gapreadD);
        printDoublePV(&gapdoneD);
        printLongPV(&gapreadL);
        printLongPV(&gapdoneL);
        printStringPV(&gapreadS);
        printStringPV(&gapdoneS);
        printEnumPV(&gapreadE);
        printEnumPV(&gapdoneE);
    }
    
    /* Last step: free all channel access resources */
    printf("Done\n");
    ca_task_exit();
    return 0;  
}
