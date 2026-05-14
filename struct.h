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
	double distance;
	int individual_deliveries_count;
}courier;
static double total_busy_time;
static  int total_delivers;
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
	int resX, resY;
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
	int id, courierID, customerID;
	double delivery_time;
}order;
//
typedef struct Node
{
	struct Node* next;
	void* data;
}Node;
