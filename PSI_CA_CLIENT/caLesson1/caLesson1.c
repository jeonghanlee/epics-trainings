/* caLesson1.c
   by Dirk Zimoch, 2007
   
   This is a very simple channel access client program.
   It uses the EPICS R3.13 channel access functions
   but can as well run with EPICS 3.14.
*/

#include <stdio.h>

/* include EPICS headers */
#include <cadef.h>

/*
#define WITH_NOT_EXISTING_CHANNEL
*/

/* Strings describing the connection status of a channel */
const char *channel_state_str[4] = {
    "not found",
    "connection lost",
    "connected",
    "closed"
};

int main ()
{
    chid beamcurrentID;
    double beamcurrent;

    chid gapID;
    double gap;

#ifdef WITH_NOT_EXISTING_CHANNEL
    chid doesnotexistID;
    double doesnotexist;
#endif

    int status;
    
    
    /* Step1: initialize channel access and search for all channels. */
    ca_task_initialize();
    
    /* Assign channel names to channel IDs. */
    ca_search ("ARIDI-PCT:CURRENT", &beamcurrentID);
#ifdef WITH_NOT_EXISTING_CHANNEL
    ca_search ("doesnotexist", &doesnotexistID);
#endif
    ca_search ("X10SA-ID-GAP:READ", &gapID);
    /* Nothing has been sent to the network so far! */
    
    /* Send all requests in parallel, wait for maximal 5.0 seconds. */
    printf ("searching ...\n");
    status = ca_pend_io(5.0);
    
    /* This ca_pend_io() is a very expensive action in terms of network
       bandwidth because UDP broadcasts are sent to all IOCs in the subnet.

       For every broadcast, each IOC has to check if it owns one of the
       requested channels. If no IOC replies, the boradcast request
       is repeated up to 100 times.
       Do not search for obsolete channels! If channels have been
       removed, also remove them from your clients to reduce unnecessary
       broadcast traffic. Check the spelling of channel names if
       channels don't connect.
       
       One broadcast package can contain many channel requests. This
       is much more efficient than sending only one request at a time.
       Thus, always try to connect all channels at once, using only
       one ca_pend_io() after all ca_search() calls. This also speeds up
       your program: waiting 10 seconds for 1000 channels in parallel
       is much shorter than even waiting only 1 second for 1000 sequential
       channel searches. ca_pend_io() returns early when all channels are
       found.
    */
    
    /* Check for errors */
    switch (status)
    {
        case ECA_NORMAL:
            printf ("all channels found\n");
            break;
        case ECA_TIMEOUT:
            printf ("some channels not found yet\n");
            break;
        default:
            printf ("unexpected error while searching: %s\n",
                ca_message(status));
    }
    /* If not all channels can be found now, the IOC is probably down.
       Searching continues in the background and channels connect
       automatically when the IOC comes up.
       
       Try to uncomment the #define WITH_NOT_EXISTING_CHANNEL above to
       see what happens if a channel cannot be found.       
       
       Normally, ca_search() should not be called any more after startup.
       There may be exceptions, when channels are added dynamically to a
       running program. But this is not the normal case.

       Connected channels may disconnect and reconnect later automatically
       when an IOC reboots. Always keep this in mind when doing any
       network traffic. Any long-lived program, such as GUIs or servers,
       MUST be written in a way to survive disconnected channels and
       they MUST react in a reasonable manner.
       
       It depends on the application and is generally is your problem what
       "reasonable" means. 
    */
    
    /* Step 2: do channel access data transfer. */
    ca_get(DBR_DOUBLE, beamcurrentID, &beamcurrent);
#ifdef WITH_NOT_EXISTING_CHANNEL
    ca_get(DBR_DOUBLE, doesnotexistID, &doesnotexist);
#endif
    ca_get(DBR_DOUBLE, gapID, &gap);
    /* Nothing has been sent to the network so far! */

    /* Send all request in parallel, wait for maximal 1.0 second. */
    printf ("reading ...\n");
    status = ca_pend_io(1.0);
    
    /* As before, it increases network performance to do as many ca_get()
       calls as possible with one ca_pend_io(). In opposite to searching,
       data transfer is done via TCP. Thus, it affects only the client
       and the IOC and all network components in between. It does not
       affect all IOCs on the same network as searching does! But still,
       many requests can be sent in the same message if they go to the
       same IOC -- which is often the case. Luckily, you don't have to
       care about this. Just always try to read as many channels as
       possible in parallel.    
    */
    
    switch (status)
    {
        case ECA_NORMAL:
            printf ("all values received\n");
            break;
        case ECA_TIMEOUT:
            printf ("some values not received\n");
            break;
        default:
            printf ("unexpected error while reading: %s\n",
                ca_message(status));
    }
    
    /* Print values of all channels but inform the user
       if a channel is connected or not. The value of a
       not connected channel is not valid, of course.
       Never take such a value for serious!
       Always when the result of ca_pend_io() after ca_get()
       is not ECA_NORMAL, you MUST check ca_state() of all
       involved channels before trusting any value.
    */
    printf ("Beam current (%s): %g\n",
        channel_state_str[ca_state(beamcurrentID)],
        beamcurrent);
#ifdef WITH_NOT_EXISTING_CHANNEL
    printf ("Does not exist (%s): %g\n",
        channel_state_str[ca_state(doesnotexistID)],
        doesnotexist);
#endif
    printf ("Gap          (%s): %g\n",
        channel_state_str[ca_state(gapID)],
        gap);
    
    /* Last step: free all channel access resources */
    ca_task_exit();
    return 0;  
}
