#include "Si1153.h"

Si1153 si1153;

/**
 * Setup for configuration
 */
void setup()
{
    uint8_t conf[4];

    Wire.begin();
    Serial.begin(115200);

    // Send start command
    si1153.send_command(Si1153::START);

    // Configure CHAN_LIST, enable channel 1 and 3
    si1153.param_set(Si1153::CHAN_LIST, 0B001010 );

    /*
     * Configure timing parameters
     */
    si1153.param_set(Si1153::MEASRATE_H, 0);
    si1153.param_set(Si1153::MEASRATE_L, 1);  // 1 for a base period of 800 us
    si1153.param_set(Si1153::MEASCOUNT_0, 5); 
    si1153.param_set(Si1153::MEASCOUNT_1, 10);
    si1153.param_set(Si1153::MEASCOUNT_2, 10);

    si1153.param_set(Si1153::THRESHOLD0_L, 200);
    si1153.param_set(Si1153::THRESHOLD0_H, 0);

    /*
     * IRQ_ENABLE
     */
    int data1 = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::IRQ_STATUS, 1);
    Serial.print("IRQ_STATUS11 = ");
    Serial.println(data1);   
     
    Wire.beginTransmission(Si1153::DEVICE_ADDRESS);
    Wire.write(Si1153::IRQ_ENABLE);
    Wire.write(0B001010 >> 2);
    Wire.endTransmission();

    data1 = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::IRQ_STATUS, 1);
    Serial.print("IRQ_STATUS22 = ");
    Serial.println(data1);   
    
    if (Wire.endTransmission() == 0)
    {
        Serial.println("OK1");
    }
    else
    {
        Serial.println("OK2");
        Serial.println(Wire.endTransmission());
    }

    /*
     * Configuration for channel 1
     */
    conf[0] = 0B00000000;
    conf[1] = 0B00000010, 
    conf[2] = 0B00000001;
    conf[3] = 0B11000001;
    si1153.config_channel(1, conf);

    // /*
    //  * Configuation for channel 3
    //  */
    conf[0] = 0B00000000;
    conf[1] = 0B00000010, 
    conf[2] = 0B00000001;
    conf[3] = 0B11000001;
    si1153.config_channel(3, conf);

}

/**
 * Loops and reads data from registers
 */
void loop()
{   
    si1153.send_command(Si1153::FORCE);
    uint8_t data[3];
    data[0] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_0, 1);
    data[1] = si1153.read_register(Si1153::DEVICE_ADDRESS, Si1153::HOSTOUT_1, 1);
    si1153.send_command(Si1153::PAUSE);
    Serial.println(data[0] * 256 + data[1]);
    delay(100);
}
