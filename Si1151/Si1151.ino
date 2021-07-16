#include "Si115X.h"

Si115X si1151;

/**
 * Setup for configuration
 */
void setup()
{
    uint8_t conf[4];

    Wire.begin();
    Serial.begin(115200);

    // Send start command
    si1151.send_command(Si115X::START);

    // Configure CHAN_LIST, enable channel 1
    si1151.param_set(Si115X::CHAN_LIST, 0B000010);

    /*
     * Configure timing parameters
     */
    si1151.param_set(Si115X::MEASRATE_H, 0);
    si1151.param_set(Si115X::MEASRATE_L, 1);  // 1 for a base period of 800 us
    si1151.param_set(Si115X::MEASCOUNT_0, 5); 
    si1151.param_set(Si115X::MEASCOUNT_1, 10);
    si1151.param_set(Si115X::MEASCOUNT_2, 10);
    si1151.param_set(Si115X::THRESHOLD0_L, 200);
    si1151.param_set(Si115X::THRESHOLD0_H, 0);

    /*
     * IRQ_ENABLE
     */
    int data1 = si1151.read_register(Si115X::DEVICE_ADDRESS, Si115X::IRQ_STATUS, 1);
    Serial.print("IRQ_STATUS11 = ");
    Serial.println(data1);   
     
    Wire.beginTransmission(Si115X::DEVICE_ADDRESS);
    Wire.write(Si115X::IRQ_ENABLE);
    Wire.write(0B000010);
    Wire.endTransmission();

    data1 = si1151.read_register(Si115X::DEVICE_ADDRESS, Si115X::IRQ_STATUS, 1);
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
    si1151.config_channel(1, conf);

    // /*
    //  * Configuation for channel 3
    //  */
    conf[0] = 0B00000000;
    conf[1] = 0B00000010, 
    conf[2] = 0B00000001;
    conf[3] = 0B11000001;
    si1151.config_channel(3, conf);

}

/**
 * Loops and reads data from registers
 */
void loop()
{   
    si1151.send_command(Si115X::FORCE);
    uint8_t data[3];
    data[0] = si1151.read_register(Si115X::DEVICE_ADDRESS, Si115X::HOSTOUT_0, 1);
    data[1] = si1151.read_register(Si115X::DEVICE_ADDRESS, Si115X::HOSTOUT_1, 1);
    si1151.send_command(Si115X::PAUSE);
    Serial.println(data[0] * 256 + data[1]);
    delay(100);
}
