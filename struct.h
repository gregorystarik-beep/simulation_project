#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum eventType//enum for the type of ebent 
{
	new_order,
	order_delivered,
	order_pickup,
	order_delayed,
	order_canceled
}eventType;
typedef struct Event//event struct 
{
	int order_number;
	double time;
	void* data;
	eventType type;
}event;
//
typedef enum Courier_status
{
	IDLE,
	BUSY
}Courier_status;
typedef struct Courier
{
	char id[10];
	Courier_status status;
	double individual_busy_time;
	double distance;
	int individual_deliveries_count;
	int currentX;
	int currentY;
}courier;//couriers ID  his status his distance and x y coordinates
static double total_busy_time;
static  int total_delivers;
//
typedef enum Customer_status
{
	WAITING,
	RECEIVED,
	CANCELED
}customer_status;
typedef struct Customer
{
	char* name;
	char id[10];
	int xCord, yCord;
	int resX, resY;
	double arrival_time;
	customer_status status;
}customer;
static double total_waiting_time;
static  int total_customers;
//name of the customer his ID  the coordinates of the restaurant he ordered from and his coordinates
//
typedef struct Order
{
	char* food_name;
	char* resturant;
	char id[10];
	char courierID[10];
	char customerID[10];
	double creation_time, delivery_time;
}order;//keeps name the id number of the order
//
typedef struct Node
{
	struct Node* next;
	void* data;
}Node;

typedef struct {
	//global watch to see the time in simulation
	double current_time;
	Node* order_queue;
	Node* event_queue;
	Node* customer_queue;

	//helps us to keep track distance
	double** distance_matrix;
	int total_locations;

	//all our lists for the simulation
	Node* couriers_list;
	int total_couriers;


	//our file where put the logs of the simulation
	FILE* log_file;

	/* statistics for the end of the simulation*/
	int total_orders_handled;      /* total orders */
	int successful_deliveries;     /* the that got the customer */
	double total_customer_wait;    /* how many customer were */

} Simulation;