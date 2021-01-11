#include "DLE_BH1750.h"
#include "Arduino.h"
#define I2C_TIMEOUT 0
#define I2C_NOINTERRUPT 0
#define I2C_FASTMODE 0
#define FAC 1
#define I2C_CPUFREQ (F_CPU/FAC)

/* Corresponds to A4/A5 - the hardware I2C pins on Arduinos */
/* Adjust to your own liking */

/*Only need update 4 line following for your hardware*/
#define SDA_PORT PORTE
#define SDA_PIN 3
#define SCL_PORT PORTG
#define SCL_PIN 5



#define I2C_FASTMODE 0


#include <SoftI2CMaster.h>
#include <avr/io.h>


DLE_BH1750::DLE_BH1750(){  
  
 }
  
uint8_t DLE_BH1750::begin(int addr)
{
    #if FAC != 1
    CPUSlowDown(FAC);
    #endif
    if(i2c_init())
    {
        BH1750address = addr;
        if(i2c_start((BH1750address<<1) | I2C_WRITE))
        {
            i2c_write(0x10);
            i2c_stop();
            return 1;
        }
    }
    
    return 0;
}


uint8_t DLE_BH1750::Read(uint16_t *buffData)
{
    uint8_t buff[2] = {0};
    
    if(i2c_start((BH1750address<<1) | I2C_READ))
    {
        buff[0] = i2c_read(false);
        delay(1);
        buff[1] = i2c_read(false);
        delay(1);
        i2c_stop();
        *buffData = ((buff[0]<<8)|buff[1])/1.2;
        return 1;
    }
    return 0;
}
  
void DLE_BH1750::CPUSlowDown(int fac) {
  // slow down processor by a fac
    CLKPR = _BV(CLKPCE);
    CLKPR = _BV(CLKPS1) | _BV(CLKPS0);
}
