## Wiring

    Pinout (ESP32)

    SD CARD         PCM1808 (ADC)      PCM5102 (DAC)
    ---------------------------------------------------
    VCC  3,3v       VCC   3,3v-5v      VCC   3,3v
    GND  GND        GND   GND          GND   GND 
    CS   GPIO4      BCK   GPIO26       SCK   GPIO0
    MISO GPIO19     OUT   GPIO35       BCK   GPIO26 
    SCK  GPIO12     LRC   GPIO33       DIN   GPIO25
    MOSI GPIO23     SCK   GPIO0        LCK   GPIO33