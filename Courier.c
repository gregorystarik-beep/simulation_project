
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
			temp_cour->status = BUSY;
			return temp_cour;
		}
		curr = curr->next;
	}
	return NULL;
}
Node* remove_single_courier(Node* head, char target_id[])
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
	return head;
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
	}
}
void update_courier_after_delivery(courier* specific_courier, double delivery_time)
{
	specific_courier->status = IDLE;
	specific_courier->individual_deliveries_count++;
	specific_courier->individual_busy_time += delivery_time;
	total_busy_time += delivery_time;
	total_delivers++;
}
void print_couriers_stats(Node*head,FILE*log_file)
{
	Node* curr = head;
	courier* c = NULL;
	printf("\n========== SIMULATION RESULTS ==========\n");
	fprintf(log_file, "\n========== SIMULATION RESULTS ==========\n");
	while (curr != NULL)
	{
		c = (courier*)curr->data;
		printf("Courier ID: %s | Deliveries: %d | Busy Time: %.2f min\n",
			c->id, c->individual_deliveries_count, c->individual_busy_time);

		fprintf(log_file, "Courier ID: %s | Deliveries: %d | Busy Time: %.2f min\n",
			c->id, c->individual_deliveries_count, c->individual_busy_time);
		curr = curr->next;
	}
	printf("-------------------------------------------------------------------------\n");
	fprintf(log_file,"-------------------------------------------------------------------------\n");

	printf("Total deliveries made: %d\n", total_delivers);
	fprintf(log_file, "Total deliveries made: %d\n", total_delivers);

	printf("Total busy time : %2.lf\n", total_busy_time);
	fprintf(log_file, "Total busy time : %2.lf\n", total_busy_time);

	printf("========================================\n");
	fprintf(log_file, "========================================\n");
}