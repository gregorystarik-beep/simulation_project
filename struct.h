#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum eventType
{
	new_order,
	order_delivered,
	order_pickup,
	order_delayed,
	order_canceled
}eventType;
typedef struct Event
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
}courier;
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
//
//
typedef struct Order
{
	char* food_name;
	char* resturant;
	char id[10];
	char courierID[10];
	char customerID[10];
	double creation_time, delivery_time;
}order;
//
typedef struct Node
{
	struct Node* next;
	void* data;
}Node;

typedef struct {
	/* שעון גלובלי ותורים (כמו שכתבת) */
	double current_time;
	Node* order_queue;
	Node* event_queue;
	Node* customer_queue;

	/* תשתית המרחקים (כמו שכתבת) */
	double** distance_matrix;
	int total_locations;

	/* ניהול השליחים (כמו שכתבת) */
	Node* couriers_list;
	int total_couriers;

	/* --- התוספות המומלצות --- */

	/* קובץ תיעוד מרכזי */
	FILE* log_file;

	/* סטטיסטיקות לסוף הסימולציה */
	int total_orders_handled;      /* סך ההזמנות שטופלו */
	int successful_deliveries;     /* כמה הגיעו ליעדן בהצלחה */
	double total_customer_wait;    /* מצטבר: סך זמן ההמתנה של כל הלקוחות */

} Simulation;