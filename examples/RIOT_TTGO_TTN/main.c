/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example demonstrating the use of LoRaWAN with RIOT
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"

#include "periph/rtc.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD              (20U)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

uint8_t nodeactivation = NODEACTIVATION;
semtech_loramac_t loramac;

static const char *message = "This is RIOT!";

/* Information for OTAA activation
*/
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

/* Information for ABP activation
*/
uint8_t devaddr[LORAMAC_DEVADDR_LEN];
uint8_t appskey[LORAMAC_APPSKEY_LEN];
uint8_t nwkskey[LORAMAC_NWKSKEY_LEN];

static void rtc_cb(void *arg)
{
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void)
{
    struct tm time;
    rtc_get_time(&time);
    /* set initial alarm */
    time.tm_sec += PERIOD;
    mktime(&time);
    rtc_set_alarm(&time, rtc_cb, NULL);
}

static void _send_message(void)
{
    uint8_t res;

    printf("Sending: %s\n", message);
    /* The send call blocks until done */
    res = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));

    switch (res) {
        case SEMTECH_LORAMAC_TX_OK:
            puts("Lorawan TX OK.");
            break;
        case SEMTECH_LORAMAC_TX_ERROR:
            puts("Lorawan TX failed.");
            break;
        case SEMTECH_LORAMAC_BUSY:
            puts("Lorawan stack BUSY.");
            break;
        case SEMTECH_LORAMAC_NOT_JOINED:
            puts("Lorawan stack not joined.");
            break;
        case SEMTECH_LORAMAC_TX_SCHEDULE:
            puts("Lorawan stack TX scheduled.");
            break;
        default:
            printf("Lorawan send result: %d\n", res);
            break;
    }

    /* Wait until the send cycle has completed */
    puts("Waiting for semtech_loramac stack completion...");
    semtech_loramac_recv(&loramac);
    puts("Sending done!");
}

static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    puts("Startup Sender thread.");

    while (1) {

        /* Trigger the message send */
        _send_message();

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();
        puts("Waiting for trigger message");
        msg_receive(&msg);
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
    uint8_t joined = 0;

    puts("LoRaWAN Class A low-power application");
    puts("=====================================");
    printf(" -> Node activation by: ");
    if ( nodeactivation ) {
        puts("OTAA");
    } else {
        puts("ABP");
    }

    /* Create the Loramac LORAWAN stack. */
    semtech_loramac_init(&loramac);

    semtech_loramac_set_tx_mode(&loramac ,  LORAMAC_TX_UNCNF );


    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, DEVEUI);
    fmt_hex_bytes(appeui, APPEUI);
    fmt_hex_bytes(appkey, APPKEY);

    fmt_hex_bytes(devaddr, DEVADDR);
    fmt_hex_bytes(appskey, APPSKEY);
    fmt_hex_bytes(nwkskey, NWKSKEY);

    /* Initialize the loramac stack */
    if ( nodeactivation ) {
        puts("Set OTAA information.");
        semtech_loramac_set_deveui(&loramac, deveui);
        semtech_loramac_set_appeui(&loramac, appeui);
        semtech_loramac_set_appkey(&loramac, appkey);
    } else {
        puts("Set OBP information.");
        semtech_loramac_set_devaddr(&loramac, devaddr);
        semtech_loramac_set_appskey(&loramac, appskey);
        semtech_loramac_set_nwkskey(&loramac, nwkskey);
    }

    printf("devaddr: %02X%02X%02X%02X\n", devaddr[0], devaddr[1], devaddr[2], devaddr[3] );
    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_3);

    while ( !joined ) {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        joined = 1;   /* Let's assume success for now, and it fails we reset the variable. */
        if ( nodeactivation ) {
            puts("[Sender Thread] Starting join procedure...");
            if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
                puts("Join procedure failed");
                joined = 0;
            }
        } else {
            puts("[Sender Thread] Starting ABP node activation...");
            if (semtech_loramac_join(&loramac, LORAMAC_JOIN_ABP) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
                puts("Join procedure failed");
                joined = 0;
            }
        }


        if ( !joined ) {
            /* Should only reach this if the join failed.
                Let's sleep for a while and try again. */
            puts("[Sender Thread] Joined failed... Sleeping 60s...");
            xtimer_sleep(60);
            puts("[Sender Thread] Let's try again to join...");
        }
    }

    puts("[Sender Thread] Join/ABP procedure succeeded");
    xtimer_sleep(4);

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0, sender, NULL, "sender");

    xtimer_sleep(2);
    /* trigger the first send */
    puts("Send trigger message");
    msg_t msg;
    msg_send(&msg, sender_pid);
    return 0;
}
