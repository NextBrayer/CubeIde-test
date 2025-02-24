/*
 * heart.h
 *
 *  Created on: Feb 24, 2025
 *      Author: Jawad
 */

#ifndef INC_HEART_H_
#define INC_HEART_H_



void initStatHRM (void);
void statHRMAlgo (unsigned long ppgData);

void updateWindow(unsigned long *peakWindow, unsigned long Y, unsigned char n);
unsigned char chooseRate(unsigned char *rate);
void updateHeartRate(unsigned char *rate, unsigned int freq, unsigned int last);
unsigned long findMax(unsigned long *X);
unsigned long findMin(unsigned long *X);







#endif /* INC_HEART_H_ */
