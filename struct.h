#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum eventType
{
	new_order,
	order_deliverd,
	order_pickup,
	order_delyed,
	order_cancled
}eventType;
typedef struct Event
{
	int order_number;
	double time;
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
	int individual_deliveries_count;
}courier;
static double total_busy_time =0;
static  int total_delivers =0;
//
typedef enum Customer_status
{
	WAITING,
	RECIVED
}customer_status;
typedef struct Customer
{
	char* name;
	int id, xCord, yCord;
	customer_status status;
}customer;
static double total_waiting_time =0;
static  int total_customers =0;
//
//
typedef struct Order
{
	char* name;
	int id, courierID, customerID;
	double creation_time;
	double delivery_time;
}order;
//
typedef struct Node
{
	struct Node* next;
	void*data;
}Node;