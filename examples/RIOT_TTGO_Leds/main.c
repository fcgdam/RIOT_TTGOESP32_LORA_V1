
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "shell.h"
#include "shell_commands.h"
#include "xtimer.h"
#include "led.h"
#include "periph/gpio.h"

#ifdef MODULE_NETIF
#include "net/gnrc/pktdump.h"
#include "net/gnrc.h"
#endif

// The TTGO ESP32 Lora OLED pins

static int led_cmd(int argc, char **argv) {
    if ( argc != 2 ) {
        (void) puts("Usage: led [on|off]");
        return 1;
    }
    if ( strcmp( argv[1] , "on" ) == 0 ) {
        LED_ON(0);
        return 0;
    }

    if ( strcmp( argv[1] , "off" ) == 0 ) {
        LED_OFF(0);
        return 0;
    }

    puts("led: Invalid command");
    return 1;
}

const shell_command_t shell_commands[] = {
    {"led", "Turns on the onboard led.", led_cmd },
    {NULL, NULL, NULL}
};


int main(void)
{

    (void) puts("Welcome to RIOT!");
    gpio_t pinled = GPIO_PIN( 0 , 2);
    gpio_init( pinled , GPIO_OUT);
    gpio_set( pinled );

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run( shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
