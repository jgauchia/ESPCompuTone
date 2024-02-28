## Wiring

    Pinout (ESP32)

    SD CARD         PCM1808 (ADC)      PCM5102 (DAC)     PCF8574            ILI9341
    -------------------------------------------------------------------------------------------
    VCC  3,3v       VCC   3,3v-5v      VCC   3,3v        VCC    3,3v        VCC    3,3v
    GND  GND        GND   GND          GND   GND         GND    GND         GND    GND
    CS   GPIO4      BCK   GPIO26       SCK   GPIO0       SDA    GPIO21      LED    3,3v
    MISO GPIO19     OUT   GPIO35       BCK   GPIO26      SCL    GPIO22      MISO   GPIO27
    SCK  GPIO12     LRC   GPIO33       DIN   GPIO25                         SCK    GPIO14
    MOSI GPIO23     SCK   GPIO0        LCK   GPIO33                         MOSI   GPIO13
                                                                            DC     GPÌO15
                                                                            RST    GPIO32
                                                                            CS     GPIO2
                                                                            TCS    GPIO18   
                                                                            TIRQ   GPIO5
                                                                            