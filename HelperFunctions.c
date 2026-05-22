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
void create_test_file_automatically(Simulation*sim)
{
	FILE* f = fopen("orders.txt", "w");
	if (f == NULL) return;

	// מאגר אופציות להגרלה כדי שזה ייראה אמיתי
	const char* foods[] = { "Pizza", "Sushi", "Burger", "Salad", "Pasta" };
	const char* rests[] = { "Dominos", "Japanika", "McDonalds", "Green", "Nonna" };
	int num_options = 5;
	int rand_food;
	int random_index;
	int i, j;
	customer* selected_cust = NULL;
	Node* curr = NULL;

	double current_time = 8.00; // מתחילים ב-8 בבוקר

	// נייצר 50 הזמנות שפרוסות לאורך היום
	for (i = 1; i <= 50; i++)
	{
		curr = sim->customer_queue;
		rand_food = rand() % num_options; // הגרלת סוג אוכל
		random_index = rand() % 10; // הגרלת ID של לקוח (נניח שיש לנו לקוחות עם ID בין 1 ל-10)
		for (j = 0; j < random_index; ++j)
		{
			curr = curr->next;
		}
		selected_cust = (customer*)curr->data;
		// כותבים את ההזמנה לקובץ
		// מבנה: מספר_הזמנה סוג_אוכל מסעדה תעודת_לקוח זמן
		fprintf(f, "%d %s %s %s %.2f\n",
			i, foods[rand_food], rests[rand_food], selected_cust->id, current_time);

		// מקדמים את הזמן להזמנה הבאה (בין 0 ל-20 דקות בערך אחרי ההזמנה הקודמת)
		current_time += ((double)(rand() % 30)) / 100.0;
	}

	fclose(f);
}