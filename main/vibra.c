#include "vibra.h"
#include "driver/gpio.h"

#define VIBRA_GPIO GPIO_NUM_9  // Vaihda jos käytät toista pinniä

void vibra_sensor_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << VIBRA_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,   // riippuu moduulin rakenteesta
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE      // Ei keskeytyksiä tässä
    };
    gpio_config(&io_conf);
}

bool vibra_sensor_triggered(void)
{
    return gpio_get_level(VIBRA_GPIO) == 0;  // SW-420 antaa LOW kun tärinä havaittu
}

