#pragma once
#include <stdio.h>
#include "Courier.h"
#include "struct.h"
#include "HelperFucntions.h"
double total_busy_time =0.0;//0 for global variables 
int total_delivers =0;
Node* init_couriers(int num_couriers)//allocates a specific ammount of couriers
{
	Node* head = NULL;
	int i;
	Node* new_node = NULL;
	courier* temp = NULL;
	courier* new_courier = NULL;
	for (i = 0; i < num_couriers; ++i)//runs from zero the how many couries we have and puts it in a linked list
	{
		new_node = (Node*)malloc(sizeof(Node));
		if (new_node == NULL)
		{
			fprintf(stderr,"NO couriers\n");
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
		new_courier->distance = 0.0;
		new_courier->individual_busy_time = 0;
		new_courier->individual_deliveries_count = 0;
		new_courier->status = IDLE;
		new_courier->currentX = rand() % 100;
		new_courier->currentY = rand() % 100;
		new_node->next = head;
		head = new_node;
	}//Initializing all parameters
	return head;
}
courier* find_idle_courier(Node* head)//find a free courier
{
	Node* curr = head;
	courier* temp_cour = NULL;
	while (curr != NULL)
	{
		temp_cour = (courier*)curr->data;
		if (temp_cour->status == IDLE)//if he is not busy then we found him him and can give him a new order
		{
			temp_cour->status = BUSY;
			return temp_cour;
		}
		curr = curr->next;
	}//else couldnt find a free courier
	return NULL;
}
courier* find_courier_by_id(Node* head, char id[10])//searches for a specific couriers 
{
	Node* curr = head;
	int res = 0;
	courier* curr_courier = NULL;
	while (curr != NULL)
	{
		curr_courier = (courier*)curr->data;
		res = strcmp(id, curr_courier->id);//using strcmp to see if the ID matches
		if (res == 0)
		{
			return curr_courier;
		}
		curr = curr->next;
	}
	return NULL;
}
Node* remove_single_courier(Node* head, char target_id[])//removes a single courier from list after he ended his shift
{
	int res = 0;
	Node* curr = head;
	Node* prev = NULL;
	courier* temp = NULL;
	if (head == NULL)return head;
	temp = (courier*)curr->data;
	res = strcmp(target_id, temp->id);
	if (res == 0)
	{
		head = curr->next;
		free((courier*)curr->data);
		curr->data = NULL;
		free(curr);
		curr = NULL;
		return head;
	}
	prev = curr;
	curr = curr->next;
	while (curr != NULL)
	{
		temp = (courier*)curr->data;
		res = strcmp(target_id, temp->id);
		if (res == 0)
		{
			prev->next = curr->next;
			free(curr->data);
			curr->data = NULL;
			free(curr);
			curr = NULL;
			return head;
		}
		prev = curr;
		curr = curr->next;
	}
	return head;//uses delete algorithm in linked list
}
void free_all_couriers(Node* head)
{
	Node* curr = head;
	Node* next_node = NULL;
	while (curr != NULL)
	{
		next_node = curr->next;
		free(curr->data);
		curr->data = NULL;
		free(curr);
		curr = NULL;
		curr = next_node;
	}//frees all couriers in the end of the day
}
void update_courier_after_delivery(courier* specific_courier, double delivery_time, double delivery_distance,int destX, int destY)
{
	specific_courier->status = IDLE;
	specific_courier->currentX = destX;
	specific_courier->currentY = destY;
	specific_courier->individual_deliveries_count++;
	specific_courier->individual_busy_time += delivery_time;
	total_busy_time += delivery_time;
	specific_courier->distance += delivery_distance;
	total_delivers++;
}//after a courier finished his delivery we update his stats
void print_courier(courier* specific_courier, FILE* log_file)
{
	const char* status = "UNKNOWN";

	if (specific_courier == NULL)
	{
		fprintf(stderr, "no data\n");
		return;
	}
	switch (specific_courier->status)//using switch case with enums so we can print the status
	{
	case IDLE:
		status = "IDLE";
		break;
	case BUSY:
		status = "BUSY";
		break;
	default:
		status = "UNKNOWN";
		break;
	}
	printf("Courier ID: %s | Status: %s | Deliveries: %d | Busy Time: %.2f min | Distance: %.2f\n",
		specific_courier->id,
		status,
		specific_courier->individual_deliveries_count,
		specific_courier->individual_busy_time,
		specific_courier->distance);

	if (log_file != NULL)
	{
		fprintf(log_file, "Courier ID: %s | Status: %s | Deliveries: %d | Busy Time: %.2f min | Distance: %.2f\n",
			specific_courier->id,
			status,
			specific_courier->individual_deliveries_count,
			specific_courier->individual_busy_time,
			specific_courier->distance);
	}
}
void print_couriers_stats(Node* head, FILE* log_file)
{
	Node* curr = head;
	courier* c = NULL;
	printf("\n========== SIMULATION RESULTS ==========\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "\n========== SIMULATION RESULTS ==========\n");
	}
	while (curr != NULL)
	{
		c = (courier*)curr->data;
		print_courier(c, log_file);
		curr = curr->next;
	}
	printf("-------------------------------------------------------------------------\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "-------------------------------------------------------------------------\n");
	}

	printf("Total deliveries made: %d\n", total_delivers);
	if (log_file != NULL)
	{
		fprintf(log_file, "Total deliveries made: %d\n", total_delivers);
	}
	printf("Total busy time : %2.lf\n", total_busy_time);
	if (log_file != NULL)
	{
		fprintf(log_file, "Total busy time : %2.lf\n", total_busy_time);
	}
	printf("========================================\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "========================================\n");
	}
}