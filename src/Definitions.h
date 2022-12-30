#define DISPLAY_ADDR 0x3C // I2C Address - use 0x3C or 0x3D depending on your display -- Display connects A4 A5
#define ENC_CLK 2 // Encoder CLK Input Pin
#define ENC_DT 3 // Encoder DT Input Pin
#define ENC_SW 4 // Encoder Switch Input Pin
#define ENC_TOL 4 // 4 Clicks of Tolerance
#define START_BTN 5 // Start Button Input Pin
#define RUN_BTN 7 // continious run button
#define SSR 6 // Start Button Input Pins
#define SINGLEDOSE_DEFAULT_TIME 2.50 // Default Single Dose Time in S
#define DOUBLEDOSE_DEFAULT_TIME 5.00 // Default Double Dose Time in S
#define roundf round        //define to use math round
#define START_BTN_LED 9     //PWM
#define RUN_BTN_LED 10      //PWM
#define fadeTime 700.0      //float Fading multiplier (700 looks good)
#define flashTime 250       //time between flashes in ms