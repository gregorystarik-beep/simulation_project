#include "Order.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

order* create_order(char order_id[10], const char* food, const char* rest, char cust_id[10], double current_time)
{
	int len_food = 0, len_rest = 0;
	order* new_order = (order*)malloc(sizeof(order));
	if (new_order == NULL)
	{
		fprintf(stderr, "no order\n");
		return NULL;
	}
	len_food = strlen(food);
	len_rest = strlen(rest);
	new_order->food_name = (char*)calloc((len_food + 1), sizeof(char));
	new_order->resturant = (char*)calloc((len_rest + 1), sizeof(char));
	if (new_order->food_name == NULL || new_order->resturant == NULL)
	{
		free(new_order->food_name);
		free(new_order->resturant);
		free(new_order);
		new_order = NULL;
		return NULL;
	}
	strcpy(new_order->food_name, food);
	strcpy(new_order->resturant, rest);
	strcpy(new_order->courierID, "NONE");
	strcpy(new_order->customerID, cust_id);
	strcpy(new_order->id, order_id);
	new_order->creation_time = current_time;
	new_order->delivery_time = 0;
	return new_order;
}

void assign_courier_to_order(order* specific_order, char cour_id[10])
{
	if (specific_order == NULL)
	{
		return;
	}
	strcpy(specific_order->courierID, cour_id);
}

void free_order(order* specific_order)
{
	if (specific_order != NULL)
	{
		free(specific_order->food_name);
		specific_order->food_name = NULL;
		free(specific_order->resturant);
		specific_order->resturant = NULL;
		free(specific_order);
		specific_order = NULL;
	}
}

void print_order(order* specific_order, FILE* log_file)
{
	if (specific_order == NULL)
	{
		fprintf(stderr, "no data\n");
		return;
	}
	printf("Order ID: %s | Food: %s | Restaurant: %s | Customer: %s | Courier: %s | Created: %.2f | Delivered: %.2f\n",
		specific_order->id,
		specific_order->food_name,
		specific_order->resturant,
		specific_order->customerID,
		specific_order->courierID,
		specific_order->creation_time,
		specific_order->delivery_time);

	if (log_file != NULL)
	{
		fprintf(log_file, "Order ID: %s | Food: %s | Restaurant: %s | Customer: %s | Courier: %s | Created: %.2f | Delivered: %.2f\n",
			specific_order->id,
			specific_order->food_name,
			specific_order->resturant,
			specific_order->customerID,
			specific_order->courierID,
			specific_order->creation_time,
			specific_order->delivery_time);
	}
}
void enqueue_order(Node** head, order* new_order)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL)
	{
		fprintf(stderr, "NO QUEUE\n");
		return;
	}
	Node* curr = *head;
	new_node->data = new_order;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	while (curr->next != NULL)
	{
		curr = curr->next;
	}
	curr->next = new_node;
}
order* dequeue_order(Node** head)
{
	Node* temp = NULL;
	order* first_order = NULL;
	if (*head == NULL)
	{
		fprintf(stderr, "no orders\n");
		return NULL;
	}
	temp = *head;
	first_order = (order*)temp->data;
	*head = (*head)->next;
	free(temp);
	temp = NULL;
	return first_order;
}
order* find_order_by_id(Node* head, char id[10])
{
	Node* curr = head;
	int res = 0;
	order* curr_order = NULL;
	while (curr != NULL)
	{
		curr_order = (order*)curr->data;
		res = strcmp(curr_order->id, id);
		if (res == 0)
		{
			return curr_order;
		}
		curr = curr->next;
	}
	return NULL;
}
void free_order_queue(Node** head)
{
	Node* curr = *head;
	Node* next_node = NULL;
	order* temp = NULL;
	while (curr != NULL)
	{
		temp = (order*)curr->data;
		next_node = curr->next;
		free_order(temp);
		curr->data = NULL;
		free(curr);
		curr = NULL;
		curr = next_node;
	}
	*head = NULL;
}
void print_order_queue(Node* head, FILE* log_file) 
{
	Node* curr = head;
	order* o = NULL;
	printf("\n========== SIMULATION RESULTS ==========\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "\n========== SIMULATION RESULTS ==========\n");
	}
	while (curr != NULL)
	{
		o = (order*)curr->data;
		print_order(o, log_file);
		curr = curr->next;
	}
	printf("==================================================\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "==================================================\n");
	}
}