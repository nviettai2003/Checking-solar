#include "BME.h"

// Cac he so bu (compensation parameters)
int32_t t_fine;

// Cac he so tu cam bien
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1;
int16_t dig_H2, dig_H3;
int16_t dig_H4, dig_H5, dig_H6;

void BME280_ReadRawData(uint32_t* temperature, uint32_t* pressure, uint32_t* humidity) 
{
    uint8_t tempData[3];
    uint8_t pressData[3];
    uint8_t humData[2];

    // Doc du lieu nhiet do
    I2C1_ReadData(BME280_ADDRESS, BME280_REG_TEMP_MSB, tempData, 3);
    *temperature = ((uint32_t)(tempData[0] << 12) | (tempData[1] << 4) | (tempData[2] >> 4));

    // Doc du lieu ap suat
    I2C1_ReadData(BME280_ADDRESS, BME280_REG_PRESS_MSB, pressData, 3);
    *pressure = ((uint32_t)(pressData[0] << 12) | (pressData[1] << 4) | (pressData[2] >> 4));

    // Doc du lieu do am
    I2C1_ReadData(BME280_ADDRESS, BME280_REG_HUM_MSB, humData, 2);
    *humidity = ((uint32_t)(humData[0] << 8) | humData[1]);
}

void BME280_ReadCompensationParams(void) 
{
    uint8_t data[24];
    
    // Doc cac he so tu cam bien
    I2C1_ReadData(BME280_ADDRESS, 0x88, data, 24);
    
    dig_T1 = (data[1] << 8) | data[0];
    dig_T2 = (data[3] << 8) | data[2];
    dig_T3 = (data[5] << 8) | data[4];
    dig_P1 = (data[7] << 8) | data[6];
    dig_P2 = (data[9] << 8) | data[8];
    dig_P3 = (data[11] << 8) | data[10];
    dig_P4 = (data[13] << 8) | data[12];
    dig_P5 = (data[15] << 8) | data[14];
    dig_P6 = (data[17] << 8) | data[16];
    dig_P7 = (data[19] << 8) | data[18];
    dig_P8 = (data[21] << 8) | data[20];
    dig_P9 = (data[23] << 8) | data[22];
    
    // Ð?c h? s? bù d? ?m
    I2C1_ReadData(BME280_ADDRESS, 0xA1, &dig_H1, 1);
    I2C1_ReadData(BME280_ADDRESS, 0xE1, data, 7);
    dig_H2 = (data[1] << 8) | data[0];
    dig_H3 = data[2];
    dig_H4 = (data[3] << 4) | (data[4] & 0x0F);
    dig_H5 = (data[5] << 4) | ((data[4] >> 4) & 0x0F);
    dig_H6 = data[6];
}
uint32_t BME280_CompensateTemperature(int32_t adc_T) 
{
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    int32_t T = (t_fine * 5 + 128) >> 8; // Tr? v? nhi?t d? tính b?ng d? C
	T /= 100;
	return (uint32_t)T;
}

uint32_t BME280_CompensatePressure(int32_t adc_P) 
{
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) {
        return 0; // Tranh chia cho 0
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
	p /= 25600;
    return (uint32_t)p; // Tra ve ap suat tinh bang Pa
}	

uint32_t BME280_CompensateHumidity(int32_t adc_H)
{
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    uint32_t H = (uint32_t)(v_x1_u32r >> 12);
	H /= 1024;
	return (uint32_t)H;
}

void BME280_ReadCompensatedData(int32_t* temperature, uint32_t* pressure, uint32_t* humidity)
{
    uint32_t adc_T, adc_P, adc_H;
    
    // Doc du lieu tho cam bien
    BME280_ReadRawData(&adc_T, &adc_P, &adc_H);
    
    // Chuyen du lieu tho sang du lieu thuc te
    *temperature = BME280_CompensateTemperature(adc_T);
    *pressure = BME280_CompensatePressure(adc_P);
    *humidity = BME280_CompensateHumidity(adc_H);
}

void BME280_Init(void) 
{
	// Dat do phan giai cua do am truoc khi bat cam bien
	I2C1_WriteData(BME280_ADDRESS, BME280_REG_CTRL_HUM, 0x01); // Dat oversampling cho do am là x1
	
	BME280_ReadCompensationParams();
	// Cau hinh che do do
	I2C1_WriteData(BME280_ADDRESS, BME280_REG_CTRL_MEAS, 0x27); // Dat che do binh thuong, ti le lay mau cua nhiet do va ap suat la 1
	// Cau hinh bo loc va thoi gian standby
	I2C1_WriteData(BME280_ADDRESS, BME280_REG_CONFIG, 0xA0); // Dat thoi gian cho la 100ms, bo loc tat
}
