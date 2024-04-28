/**
 * @file lvgfx.hpp
 * @author Jordi Gauchía
 * @brief  LOVYANGFX TFT driver
 * @version 0.3.1
 * @date 2024-04
 */

#ifndef LVGFX_HPP
#define LVGFX_HPP

#define LGFX_USE_V1

#include "LovyanGFX.hpp"

class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Touch_XPT2046 _touch_instance;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = HSPI_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 79999999;
            cfg.freq_read = 27000000;
            cfg.spi_3wire = false;
            cfg.use_lock = false;
            cfg.dma_channel = SPI_DMA_CH_AUTO;
            cfg.pin_sclk = 14;
            cfg.pin_mosi = 13;
            cfg.pin_miso = 27;
            cfg.pin_dc = 15;
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs = 2;
            cfg.pin_rst = 32;
            cfg.pin_busy = -1;
            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.memory_width = 240;
            cfg.memory_height = 320;
            cfg.offsetX = 0;
            cfg.offsetY = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = true;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = false;
            _panel_instance.config(cfg);
        }

        {
            auto cfg = _touch_instance.config();
            cfg.x_min = 0;
            cfg.x_max = 319;
            cfg.y_min = 0;
            cfg.y_max = 239;
            cfg.pin_int = 5;
            cfg.bus_shared = true;
            cfg.offset_rotation = 0;
            cfg.spi_host = HSPI_HOST;
            cfg.freq = 1600000;
            cfg.pin_sclk = 14;
            cfg.pin_mosi = 13;
            cfg.pin_miso = 27;
            cfg.pin_cs = 18;
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }
        setPanel(&_panel_instance);
    }
};

#endif