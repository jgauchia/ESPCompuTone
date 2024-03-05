# ESP32 Based Digital recorder
* Note: Under development


Prototype:

<img src="ESPCompuTone.jpg" width="25%" height="25%">



### Specifications

*   ESP32-S2 (WROVER or WROOM)
*   SD Card
*   ILI9341 TFT with touch
*   PCM5102 DAC
*   PCM1808 ADC



### Wiring

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
                                                                            


### Firmware install

Please install first [PlatformIO](http://platformio.org/) open source ecosystem for IoT development compatible with **Arduino** IDE and its command line tools (Windows, MacOs and Linux). Also, you may need to install [git](http://git-scm.com/) in your system. 

After the first run, load the icons and assets with:

```bash
pio run --target uploadfs
```


### TODO

- [ ] Recording (Wav Format)
- [ ] File transfer over WiFi
- [ ] Other file support (MP3, TAP, TZX...)
- [ ] Configuration Screen
- [ ] Splash screen
- [ ] PSRAM support


### Credits

*   [ESP32 SDCard Wave File Example](https://github.com/atomic14/esp32_sdcard_audio) thanks to [@atomic14](https://github.com/atomic14)