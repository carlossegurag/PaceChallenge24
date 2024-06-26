/*******************************************************************************************
Authors: Carlos Segura, Lazaro Lugo, Edison Serrano, Gara Miranda 
Description: functions related to random numbers.
********************************************************************************************/


#include <stdlib.h>
#include <iostream>

int getRandomInteger0_N(int n){
  return (int) ((n + 1.0)*rand()/(RAND_MAX+1.0));
}

double generateRandomDouble0_Max(double maxValue){
    return (double)(rand()) / RAND_MAX * maxValue;
}
