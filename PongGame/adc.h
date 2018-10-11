
void adcInit();
uint8_t adc_read();
uint8_t mapTocuhValues(char axis);

extern volatile uint16_t touchX;
extern volatile uint16_t touchY;
extern volatile uint16_t aMeter;
extern volatile uint8_t ADCchannel;
extern   uint16_t adcResult;

extern volatile uint16_t paddle2Y;
extern volatile uint16_t paddle1Y;