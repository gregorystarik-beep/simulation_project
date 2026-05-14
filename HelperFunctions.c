#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Courier.h"
#include "struct.h"
#include "HelperFucntions.h"
void Generate_ID(char ID[10])
{
	const char charset[] = "0123456789";
	int i;
	for (i = 0; i < 9; ++i)
	{
		ID[i] = charset[rand() % 10];
	}
	ID[9] = '\0';
}