#pragma once
#include "struct.h"
#include <stdio.h>

// creating a new order
order* create_order(char order_id[10], const char* food, const char* rest, char cust_id[10], double current_time);

// assigning a courier to a new order
void assign_courier_to_order(order* specific_order, char cour_id[10]);

// prints the order
void print_order(order* specific_order, FILE* log_file);

//frees memory 
void free_order(order* specific_order);



/* queue algorithm  */
void enqueue_order(Node** head, order* new_order);

/* using fifo */
order* dequeue_order(Node** head);

/* searching by orders ID  */
order* find_order_by_id(Node* head, char id[10]);

/*prints to log the order queue  */
void print_order_queue(Node* head, FILE* log_file);

/*frees rhe list */
void free_order_queue(Node** head);