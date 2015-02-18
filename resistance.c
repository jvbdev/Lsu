/* Kenan Rahmanovic*/

#include <stdio.h>

#include "resistance.h"



#define ERROR_CALC -1



float calc_serial(int count, float* array);
float calc_parallell(int count, float* array);



float calc_resistance(int count, char conn, float *array)
{	
	int i = 0;
	int result = 0;

	if (conn == 's' || conn == 'S') {
		conn = 'S';
	}
	else if (conn == 'p' || conn == 'P') {
		conn = 'P';
	}
	else {
		printf("Fel bokstav som anger kopplingen : %c.\n", conn);		
		return ERROR_CALC;
	}

	if (count <= 1 && conn == 'P') {
		printf("Du behöver minst två resistorer för att parallellkoppla : %d.\n", count);		
		return ERROR_CALC;
	}
	
	if (count != 0 && array != NULL) {				
		for (i = 0; i < count; i++) {
			if (array[i] < 0 || (conn == 'P' && array[i] == 0)) {
				printf("Fel resistorvärde som parameter : %f\n", array[i]);
				result = ERROR_CALC;				
				break;						
			}
		}
		
	}
	else {
		printf("Kolla att antal resistorer är fler än 0 och array pekar mot rätt minne.\n");
		return ERROR_CALC;
	}

	if (result != ERROR_CALC) {
		if (conn == 'S') {
			result = calc_serial(count, array);
		}
		else {
			result = calc_parallell(count, array);
		}
	}

	return result;
}



float calc_serial(int count, float* array)
{
	int i = 0;
	float result = 0;

	for (i = 0; i < count; i++) {
		result += array[i];
	}

	return result;
}



float calc_parallell(int count, float* array)
{
	int i = 0;
	float result = 0;
	float dividend = 1;
	float divisor = 0;

	for (i = 0; i < count; i++) {
		dividend *= array[i];
		divisor += array[i];
	}

	result = dividend / divisor;
	
	return result;
}

