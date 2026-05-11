
#include <stdio.h>
#include "Courier.h"
#include "struct.h"
#include "HelperFucntions.h"
Node* init_couriers(int num_couriers)
{
	Node* head = NULL;
	int i;
	Node* new_node = NULL;
	courier* temp = NULL;
	courier* new_courier = NULL;
	for (i = 0; i < num_couriers; ++i)
	{
		new_node = (Node*)malloc(sizeof(Node));
		if (new_node == NULL)
		{
			printf("NO couriers\n");
			free(new_node);
			new_node = NULL;
			return NULL;
		}
		temp = (courier*)malloc(sizeof(courier));
		if (temp == NULL)
		{
			printf("NO couriers\n");
			free(temp);
			free(new_node);
			new_node = NULL;
			temp = NULL;
			return NULL;
		}
		new_node->data = temp;
		new_courier = temp;
		Generate_ID(new_courier->id);
		new_courier->status = IDLE;
		new_node->next = head;
		head = new_node;
	}
	return head;
}
courier* find_idle_courier(Node* head)
{
	Node* curr = head;
	courier* temp_cour = NULL;
	while (curr != NULL)
	{
		temp_cour = (courier*)curr->data;
		if (temp_cour->status == IDLE)
		{
			return temp_cour;
		}
		curr = curr->next;
	}
	return NULL;
}