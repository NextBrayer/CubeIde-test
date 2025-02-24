/*
 * hear.c
 *
 *  Created on: Feb 24, 2025
 *      Author: Jawad
 */


/*
 * hrm.c
 *
 * Provides AFE4900 HRM API
 *
 * Copyright (C) 2016 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
*/

#include "heart.h"
#include <stdio.h>


unsigned long peakWindowHP[21], lastOnsetValueLED1, lastPeakValueLED1;
unsigned char HR[12];
unsigned char temp;
unsigned int lastPeak, lastOnset;
unsigned long movingWindowHP;
unsigned char ispeak = 0;
unsigned char movingWindowCount, movingWindowSize, smallest, foundPeak, totalFoundPeak;
unsigned int freq;
unsigned long currentRatio = 0;
unsigned char HeartRate = 0;

void initStatHRM(void) {
    unsigned char i;

    lastPeak = 0;
    lastOnset = 0;
    movingWindowHP = 0;
    movingWindowCount = 0;

    for (i = 20; i >= 1; i--)
        peakWindowHP[i - 1] = 0;

    for (i = 12; i >= 1; i--)
        HR[i - 1] = 0;

    freq = 125;
    movingWindowSize = freq / 50;
    smallest = freq * 60 / 220;
    foundPeak = 0;
    totalFoundPeak = 0;
    HeartRate = 0;
}

void statHRMAlgo(unsigned long ppgData) {
    unsigned char i;
    movingWindowHP += ppgData;

    if (movingWindowCount > movingWindowSize) {
        movingWindowCount = 0;
        updateWindow(peakWindowHP, movingWindowHP, movingWindowSize + 1);
        movingWindowHP = 0;
        ispeak = 0;

        if (lastPeak > smallest) {
            ispeak = 1;
            for (i = 10; i >= 1; i--) {
                if (peakWindowHP[10] < peakWindowHP[10 - i]) ispeak = 0;
                if (peakWindowHP[10] < peakWindowHP[10 + i]) ispeak = 0;
            }
            if (ispeak) {
                lastPeakValueLED1 = findMax(peakWindowHP);
                totalFoundPeak++;
                if (totalFoundPeak > 2) {
                    updateHeartRate(HR, freq, lastPeak);
                }
                ispeak = 1;
                lastPeak = 0;
                foundPeak++;
            }
        }

        if ((lastOnset > smallest) && (ispeak == 0)) {
            ispeak = 1;
            for (i = 10; i >= 1; i--) {
                if (peakWindowHP[10] > peakWindowHP[10 - i]) ispeak = 0;
                if (peakWindowHP[10] > peakWindowHP[10 + i]) ispeak = 0;
            }
            if (ispeak) {
                lastOnsetValueLED1 = findMin(peakWindowHP);
                totalFoundPeak++;
                lastOnset = 0;
                foundPeak++;
            }
        }

        if (foundPeak > 2) {
            foundPeak = 0;
            temp = chooseRate(HR);
            if ((temp > 40) && (temp < 220)) {
                HeartRate = temp;
                printf("heart rate is %d \r\n", HeartRate);

            }
        }
    }
    movingWindowCount++;
    lastOnset++;
    lastPeak++;
}

void updateWindow(unsigned long *peakWindow, unsigned long Y, unsigned char n) {
    unsigned char i;
    for (i = 20; i >= 1; i--) {
        peakWindow[i] = peakWindow[(unsigned char)(i-1)];
    }
    peakWindow[0] = (Y / n);
}

unsigned char chooseRate(unsigned char *rate) {
    unsigned char max, min, i, nb;
    unsigned int sum, fullsum;
    max = rate[0];
    min = rate[0];
    sum = 0;
    nb = 0;
    for (i = 7; i >= 1; i--) {
        if (rate[i - 1] > 0) {
            if (rate[(unsigned int)(i-1)] > max) max = rate[(unsigned int)(i-1)];
            if (rate[(unsigned int)(i-1)] < min) min = rate[(unsigned int)(i-1)];
            sum += rate[(unsigned int)(i-1)];
            nb++;
        }
    }

    if (nb > 2)
        fullsum = (sum - max - min) * 10 / (nb - 2);
    else
        fullsum = (sum) * 10 / (nb);

    sum = fullsum / 10;
    if (fullsum - sum * 10 > 4) sum++;
    return sum;
}

void updateHeartRate(unsigned char *rate, unsigned int freq, unsigned int last) {
    unsigned char i;
    i = 60 * freq / last;
    if ((i > 40) && (i < 220)) {
        for (i = 11; i >= 1; i--) {
            rate[i] = rate[(unsigned char)(i-1)];
        }
        rate[0] = 60 * freq / last;
    }
}

unsigned long findMax(unsigned long *X) {
    unsigned long res = X[8];
    unsigned char i;
    for (i = 12; i >= 9; i--) {
        if (res < X[i]) res = X[i];
    }
    return res;
}

unsigned long findMin(unsigned long *X) {
    unsigned long res = X[8];
    unsigned char i;
    for (i = 12; i >= 9; i--) {
        if (res > X[i]) res = X[i];
    }
    return res;
}
