/*
 * AFE4490.c
 *
 *  Created on: Feb 20, 2025
 *      Author: Jawad
 */

#include "AFE4490.h"
#include "main.h" // Ensure GPIO and SPI handles are defined
#include <stdio.h>

extern SPI_HandleTypeDef hspi2; // Modify if using a different SPI instance

// AFE44XX configuration structure
void AFE44XX_Write(uint8_t address, uint32_t data) {
    uint8_t txData[4];
    txData[0] = (address& 0x7F);
    txData[1] = (data >> 16) & 0xFF;
    txData[2] = (data >> 8) & 0xFF;
    txData[3] = data & 0xFF;


     printf("LEDCNTRL Register: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",  txData[0],txData[1],txData[2],txData[3]);

    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, 0);
    HAL_SPI_Transmit(&hspi2, txData, 4, 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}

uint32_t AFE44XX_Read(uint8_t address) {
	uint8_t txData = (address& 0x7F) ;  // Set MSB to indicate read operation
    uint8_t rxData[4] = {0};
    uint32_t result = 0;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
    HAL_SPI_Transmit(&hspi2, &txData, 1, 1000);
    HAL_SPI_Receive(&hspi2, rxData, 3, 1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
    result = ((uint32_t)rxData[0] << 16) | ((uint32_t)rxData[1] << 8) | rxData[2];
    return result;
}



void AFE44XX_Init() {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
    HAL_Delay(500);

    AFE44XX_Write( CONTROL0 , 0x000000);
    AFE44XX_Write( CONTROL0, 0x000008);
     AFE44XX_Write( TIAGAIN, 0x000000); // CF = 5pF, RF = 500kR
     AFE44XX_Write( TIA_AMB_GAIN, 0x000001);
     AFE44XX_Write( LEDCNTRL, 0x001414);
     AFE44XX_Write( CONTROL2, 0x000000); // LED_RANGE=100mA, LED=50mA
     AFE44XX_Write( CONTROL1, 0x010707); // Timers ON, average 3 samples
     AFE44XX_Write( PRPCOUNT, 0X001F3F);
     AFE44XX_Write( LED2STC, 0X001770);
     AFE44XX_Write( LED2ENDC, 0X001F3E);
     AFE44XX_Write( LED2LEDSTC, 0X001770);
     AFE44XX_Write( LED2LEDENDC, 0X001F3F);
     AFE44XX_Write( ALED2STC, 0X000000);
     AFE44XX_Write( ALED2ENDC, 0X0007CE);
     AFE44XX_Write( LED2CONVST, 0X000002);
     AFE44XX_Write( LED2CONVEND, 0X0007CF);
     AFE44XX_Write( ALED2CONVST, 0X0007D2);
     AFE44XX_Write( ALED2CONVEND, 0X000F9F);
     AFE44XX_Write( LED1STC, 0X0007D0);
     AFE44XX_Write( LED1ENDC, 0X000F9E);
     AFE44XX_Write( LED1LEDSTC, 0X0007D0);
     AFE44XX_Write( LED1LEDENDC, 0X000F9F);
     AFE44XX_Write( ALED1STC, 0X000FA0);
     AFE44XX_Write( ALED1ENDC, 0X00176E);
     AFE44XX_Write( LED1CONVST, 0X000FA2);
     AFE44XX_Write( LED1CONVEND, 0X00176F);
     AFE44XX_Write( ALED1CONVST, 0X001772);
     AFE44XX_Write( ALED1CONVEND, 0X001F3F);
     AFE44XX_Write( ADCRSTCNT0, 0X000000);
     AFE44XX_Write( ADCRSTENDCT0, 0X000000);
     AFE44XX_Write( ADCRSTCNT1, 0X0007D0);
     AFE44XX_Write( ADCRSTENDCT1, 0X0007D0);
     AFE44XX_Write( ADCRSTCNT2, 0X000FA0);
     AFE44XX_Write( ADCRSTENDCT2, 0X000FA0);
     AFE44XX_Write( ADCRSTCNT3, 0X001770);
     AFE44XX_Write( ADCRSTENDCT3, 0X001770);
     HAL_Delay(1000);


    printf("Initializing AFE44XX...\r\n");
    AFE44XX_TestReadRegisters( ADCRSTCNT2, LED1CONVEND, LEDCNTRL);
    printf("Initialization complete  .\r\n");
}



void AFE44XX_TestReadRegisters(uint8_t reg1, uint8_t reg2, uint8_t reg3) {
    printf("Testing register reads...\r\n");

    AFE44XX_Write( CONTROL0, 0x000001); // Example write operation to CONTROL0

    // Read and print the value of the first register
    uint32_t value1 = AFE44XX_Read( reg1);
    // Read and print the value of the second register
    uint32_t value2 = AFE44XX_Read( reg2);
    // Read and print the value of the third register
    uint32_t value3 = AFE44XX_Read( reg3);

    printf("Register 0x%02X: 0x%08lX\r\n", reg1, value1);
    printf("Register 0x%02X: 0x%08lX\r\n", reg2, value2);
    printf("Register 0x%02X: 0x%08lX\r\n", reg3, value3);


    printf("Register read test complete.\r\n");
}

