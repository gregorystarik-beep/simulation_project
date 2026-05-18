#include "customer.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
double total_waiting_time =0.0;
int total_customers = 0;
customer* create_customer(char* name, char id[10], int xCord, int yCord, int resX, int resY)
{
	int len = 0;
	customer* new_cust = (customer*)malloc(sizeof(customer));
	if (new_cust == NULL)return NULL;
	len = strlen(name);
	new_cust->name = (char*)calloc((len + 1), sizeof(char));
	if (new_cust->name == NULL)
	{
		free(new_cust);
		new_cust = NULL;
		return NULL;
	}
	strcpy(new_cust->name, name);
	strcpy(new_cust->id, id);
	new_cust->xCord = xCord;
	new_cust->yCord = yCord;
	new_cust->resX = resX;
	new_cust->resY = resY;
	new_cust->status = WAITING;
	return new_cust;
}
void free_customer(customer* specific_customer)
{
	if (specific_customer != NULL)
	{
		free(specific_customer->name);
		specific_customer->name = NULL;
		free(specific_customer);
		specific_customer = NULL;
	}
}
double calculate_distance_time(customer* specific_customer)
{
	double res = sqrt(pow((specific_customer->resX - specific_customer->xCord), 2) + pow((specific_customer->resY - specific_customer->yCord), 2));
	return res;
}

double calculate_delivery_time(customer* specific_customer)
{
	double res = calculate_distance_time(specific_customer);
	double speed = (rand() % 81) + 20;
	double time = res / speed;
	return time;
}
void update_customer_status(customer* specific_customer, customer_status new_status)
{
	if (specific_customer != NULL)
	{
		specific_customer->status = new_status;
	}
}
void enqueue_customer(Node** head, customer* new_cust)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	Node* current = *head;
	if (new_node == NULL)return;
	new_node->data = new_cust;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_node;
}
customer* dequeue_customer(Node** head)
{
	Node* temp = NULL;
	customer* first_cust = NULL;
	if (*head == NULL)return NULL;
	temp = *head;
	first_cust = (customer*)temp->data;
	*head = (*head)->next;
	free(temp);
	temp = NULL;
	return first_cust;
}
customer* find_customer_by_id(Node* head, char id[10])
{
	Node* curr = head;
	int res = 0;
	customer* curr_cust = NULL;
	while (curr != NULL)
	{
		curr_cust = (customer*)curr->data;
		res = strcmp(id, curr_cust->id);
		if (res == 0)
		{
			return curr_cust;
		}
		curr = curr->next;
	}
	return NULL;
}
void free_customer_queue(Node** head)
{
	Node* curr = *head;
	Node* next_node = NULL;
	customer* cust = NULL;
	while (curr != NULL)
	{
		cust = (customer*)curr->data;
		next_node = curr->next;
		free_customer(cust);
		curr->data = NULL;
		free(curr);
		curr = NULL;
		curr = next_node;
	}
	*head = NULL;
}
void print_customer(customer* specific_customer, FILE* log_file)
{
	char* status = "UNKNOWN";
	if (specific_customer == NULL)return;
	switch (specific_customer->status)
	{
	case WAITING:
		status = "WAITING";
		break;
	case RECEIVED:
		status = "RECEIVED";
		break;
	default:
		status = "UNKNOWN";
		break;
	}
	printf("Customer ID : %s | Name : %s | Status : %s | Start : (%d, %d) | Dest : (%d, %d)\n", 
		specific_customer->id,
		specific_customer->name,
		status, specific_customer->resX, 
		specific_customer->resY,
		specific_customer->xCord, 
		specific_customer->yCord);
	if (log_file != NULL)
	{
		fprintf(log_file,"Customer ID : %s | Name : %s | Status : %s | Start : (%d, %d) | Dest : (%d, %d)\n",
			specific_customer->id,
			specific_customer->name,
			status, specific_customer->resX,
			specific_customer->resY,
			specific_customer->xCord,
			specific_customer->yCord);
	}
}
void print_customer_queue(Node* head, FILE* log_file)
{
	double avg_waiting_time = 0.0;
	Node* curr = head;
	customer* c = NULL;
	printf("\n========== SIMULATION RESULTS ==========\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "\n========== SIMULATION RESULTS ==========\n");
	}
	while (curr != NULL)
	{
		c = (customer*)curr->data;
		print_customer(c, log_file);
		curr = curr->next;
	}
	printf("-------------------------------------------------------------------------\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "-------------------------------------------------------------------------\n");
	}
	if (total_customers > 0)
	{
		avg_waiting_time = total_waiting_time / total_customers;
	}
	printf("Total Customers Served: %d\n", total_customers);
	printf("Total Waiting Time:     %.2f min\n", total_waiting_time);
	printf("Average Waiting Time:   %.2f min\n", avg_waiting_time);
	if (log_file != NULL)
	{
		fprintf(log_file, "Total Customers Served: %d\n", total_customers);
		fprintf(log_file, "Total Waiting Time:     %.2f min\n", total_waiting_time);
		fprintf(log_file, "Average Waiting Time:   %.2f min\n", avg_waiting_time);
	}
	printf("==================================================\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "==================================================\n");
	}
}
