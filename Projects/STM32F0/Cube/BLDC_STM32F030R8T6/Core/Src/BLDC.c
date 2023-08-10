/*
 * BLDC.c
 *
 *  Created on: Aug 8, 2023
 *      Author: kzhou
 */

#include "BLDC.h"


void driving_test()
{
	static int i=1;
	switch(6)
	{
	case 1:
		AHBL_ON;
		printf("AB");
		break;
	case 2:
		CHBL_ON;
		printf("CB");
		break;
	case 3:
		CHAL_ON;
		printf("CA");
		break;
	case 4:
		BHAL_ON;
		printf("BA");
		break;
	case 5:
		BHCL_ON;
		printf("BC");
		break;
	case 6:
		AHCL_ON;
		printf("AC");
		break;
	default:
		break;
	}
	i= i==6? 1:i+1;
}
