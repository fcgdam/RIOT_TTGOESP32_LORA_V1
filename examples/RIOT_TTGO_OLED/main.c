#include <stdio.h>
#include <string.h>

#include "board.h"
#include "thread.h"
#include "shell.h"
#include "shell_commands.h"

#include "xtimer.h"

#include "periph/gpio.h"
#include "periph/i2c.h"
#include "u8g2.h"

/**
 * @brief   RIOT-OS pin maping of U8g2 pin numbers to RIOT-OS GPIO pins.
 * @note    To minimize the overhead, you can implement an alternative for
 *          u8x8_gpio_and_delay_riotos.
 *          The TTGO SSD1306 OLED screen uses I2C and has the reset pin
 *          at GPIO 16.
 */
static gpio_t pins[] = {
    [U8X8_PIN_RESET] = OLED_RESET_PIN     // See the board.h for the TTGO ESP32 Lora Oled pin reset definition
};

/**
 * @brief   Bit mapping to indicate which pins are set.
 */
static uint32_t pins_enabled = (
    (1 << U8X8_PIN_RESET)
);

u8g2_t u8g2;

/**
  @brief   RIOT-OS logo, 64x32 pixels at 8 pixels per byte.
 */
static const uint8_t logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x3C,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1E, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0E, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xF0, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x1E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3C, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xF0, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0xF8,
    0x30, 0x3C, 0x3F, 0xC0, 0x00, 0x0C, 0x77, 0xF0, 0x38, 0x7E, 0x3F, 0xC0,
    0x00, 0x7E, 0x73, 0xC0, 0x38, 0xE7, 0x06, 0x00, 0x00, 0xFC, 0x71, 0x00,
    0x38, 0xE3, 0x06, 0x00, 0x01, 0xF0, 0x70, 0x00, 0x38, 0xE3, 0x06, 0x00,
    0x01, 0xC0, 0x70, 0x00, 0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x70, 0xC0,
    0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x71, 0xE0, 0x38, 0xE3, 0x06, 0x00,
    0x03, 0x80, 0x70, 0xE0, 0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x70, 0xF0,
    0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x70, 0x70, 0x38, 0xE3, 0x06, 0x00,
    0x03, 0x80, 0xF0, 0x78, 0x38, 0xE3, 0x06, 0x00, 0x03, 0xC1, 0xE0, 0x3C,
    0x38, 0xE7, 0x06, 0x00, 0x01, 0xE3, 0xE0, 0x3C, 0x38, 0x7E, 0x06, 0x00,
    0x01, 0xFF, 0xC0, 0x1C, 0x30, 0x3C, 0x06, 0x00, 0x00, 0x7F, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

uint32_t screen = 0;

void OLed_Init(void) {
    puts("Initializing OLED");

    printf("Initializing I2C display at address 0x%02x.", OLED_I2C_ADDR);
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_riotos_hw_i2c, u8x8_gpio_and_delay_riotos);

    u8g2_SetPins(&u8g2, pins, pins_enabled);
    u8g2_SetDevice(&u8g2, I2C_DEV(0));
    u8g2_SetI2CAddress(&u8g2, OLED_I2C_ADDR);

    /* initialize the display */
    puts("Initializing display.");

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);

    puts("OLED initialized");
}

void OLed_Test(void) {
    int loop = 15;
    while (loop--) {
        puts("Drawing...");
        u8g2_FirstPage(&u8g2);

        do {
            u8g2_SetDrawColor(&u8g2, 1);
            u8g2_SetFont(&u8g2, u8g2_font_helvB12_tf);

            switch (screen) {
                case 0:
                    u8g2_DrawStr(&u8g2, 12, 22, "THIS");
                    break;
                case 1:
                    u8g2_DrawStr(&u8g2, 24, 22, "IS");
                    break;
                case 2:
                    u8g2_DrawBitmap(&u8g2, 0, 0, 8, 32, logo);
                    break;
            }
        } 
        while (u8g2_NextPage(&u8g2));

        /* show screen in next iteration */
        screen = (screen + 1) % 3;

        /* sleep a little */
        puts("Sleeping...");
        xtimer_sleep(1);
    }  
}

static void oled_cmd_usage(void) {
    puts("Usage: oled init | test ");
}

static int oled_cmd(int argc, char **argv)
{
    if ( argc < 2 ) {
        oled_cmd_usage();
        return 1;
    }
    
    if ( strcmp(argv[1],"init") == 0 )
        OLed_Init();

    if ( strcmp(argv[1],"test") == 0 )
        OLed_Test();

    return 0;    
}

static int draw_cmd(int argc, char **argv) {
    OLed_Test();    
    return 0;
}

static const shell_command_t shell_commands[] = {
    {"oled" , "Oled commands" , oled_cmd },
    {"test" , "Test output on the oled" , draw_cmd },
    { NULL , NULL , NULL}
};

int main(void)
{
    //gpio_t Oled_Reset = GPIO_PIN(0,16);

    puts("Welcome to RIOT!");

    puts ("Initial Oled testing");
    OLed_Init();
    OLed_Test();
    
/*puts("Reseting OLED:");
    gpio_init( Oled_Reset , GPIO_OUT);
    gpio_clear( Oled_Reset);
    xtimer_usleep(50);
    gpio_set(Oled_Reset) ;
*/
   

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}