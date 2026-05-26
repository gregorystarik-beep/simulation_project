#pragma once
#include "struct.h"
#include "HelperFucntions.h"
#include "Courier.h"
customer* create_customer(char* name, char id[10], int xCord, int yCord, int resX, int resY);

//frees a  specific customer after he got his order 
void free_customer(customer* specific_customer);

// updates his status
void update_customer_status(customer* specific_customer, customer_status new_status);
//calculate distance from the restaurant to him
double calculate_distance_time(customer* specific_customer);


//calculate delivery time from the restaurant to him
double calculate_delivery_time(customer* specific_customer);

// הדפסת פרטי לקוח
void print_customer(customer* specific_customer, FILE* log_file);




// using queue data base fro customers
void enqueue_customer(Node** head, customer* new_cust);
customer* dequeue_customer(Node** head);

// finding customer through his ID
customer* find_customer_by_id(Node* head, char id[10]);

// prints statistics
void print_customer_queue(Node* head, FILE* log_file);

// frees queue at the end of the day
void free_customer_queue(Node** head);

