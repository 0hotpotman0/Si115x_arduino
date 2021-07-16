#include <Arduino.h>
#include "Si115X.h"

Si115X::Si115X() {
	//empty constructor
}

/**
 * Configures a channel at a given index
 */
void Si115X::config_channel(uint8_t index, uint8_t *conf){
    int len = sizeof(conf);
  
    if(len != 4 || index < 0 || index > 5)
      return;

    int inc = index * len;
    
    param_set(Si115X::ADCCONFIG_0 + inc, conf[0]);
    param_set(Si115X::ADCSENS_0 + inc, conf[1]);
    param_set(Si115X::ADCPOST_0 + inc, conf[2]);
    param_set(Si115X::MEASCONFIG_0 + inc, conf[3]);   
}

/**
 * Writes data over i2c
 */
void Si115X::write_data(uint8_t addr, uint8_t *data, size_t len){
    Wire.beginTransmission(addr);
    Wire.write(data, len);
    Wire.endTransmission();
}

/**
 * Reads data from a register over i2c
 */
int Si115X::read_register(uint8_t addr, uint8_t reg, int bytesOfData){
    int val = -1;
  
    Si115X::write_data(addr, &reg, sizeof(reg));
    Wire.requestFrom(addr, bytesOfData);
  
    if(Wire.available())
      val = Wire.read();
	
    return val;
}

/**
 * param set as shown in the datasheet
 */
void Si115X::param_set(uint8_t loc, uint8_t val){
    uint8_t packet[2];
    int r;
    int cmmnd_ctr;

    do {
        cmmnd_ctr = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1);
      
        packet[0] = Si115X::HOSTIN_0;
        packet[1] = val;
        Si115X::write_data(Si115X::DEVICE_ADDRESS, packet, sizeof(packet));
      
        packet[0] = Si115X::COMMAND;
        packet[1] = loc | (0B10 << 6);
        Si115X::write_data(Si115X::DEVICE_ADDRESS, packet, sizeof(packet));
      
        r = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1);	    
    } while(r > cmmnd_ctr); 
}

/**
 * param query as shown in the datasheet
 */
int Si115X::param_query(uint8_t loc){
    int result = -1;
    uint8_t packet[2];
    int r;
    int cmmnd_ctr;

    do {
        cmmnd_ctr = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1);
	    
        packet[0] = Si115X::COMMAND;
        packet[1] = loc | (0B01 << 6);
	    
        Si115X::write_data(Si115X::DEVICE_ADDRESS, packet, sizeof(packet));
	    
        r = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1);
    } while(r > cmmnd_ctr);
	
    result = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_1, 1);
	
    return result;
}

/**
 * Sends command to the command register
 */
void Si115X::send_command(uint8_t code){
    uint8_t packet[2];
    int r;
    int cmmnd_ctr;
    do {
        cmmnd_ctr = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1);
	    
        packet[0] = Si115X::COMMAND;
        packet[1] = code;
	    
        Si115X::write_data(Si115X::DEVICE_ADDRESS, packet, sizeof(packet));
	    
        r = Si115X::read_register(Si115X::DEVICE_ADDRESS, Si115X::RESPONSE_0, 1); 
    } while(r > cmmnd_ctr);
}

/**
 * Returns int given a byte array
 */
int Si115X::get_int_from_bytes(uint8_t *data, size_t len){
    int result = 0;
    int shift = 8 * len;
	
    for(int i = 0; i < len; i++){
        shift -= 8;
        result |= ((data[i] << shift) & (0xFF << shift));
    }
	
    return result;
}
