#pragma once
#include "struct.h"

/* creating new event ) */
event* create_event(eventType type, double time, int order_number, void* data);

/* using priority queue for which event is the most important ) */
void enqueue_event(Node** head, event* new_event);

/*  dequeuing the clossest evenrt */
event* dequeue_event(Node** head);

/* free a single event */
void free_event(event* specific_event);

/* frees al events*/
void free_event_queue(Node** head);

/* 6. print the event to the log file */
void print_event(event* specific_event, FILE* log_file);
//to see which event is first now
event* peek_event(Node* head);

void print_event_queue(Node* head, FILE* log_file);