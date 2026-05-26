#pragma once
#include "struct.h"

//Allocates memory for the couriers array according to the requested amount..
//Allocates dynamic memory for each courier's name, assigns an ID, sets initial status to free, and resets statistical counters.
Node* init_couriers(int num_couriers);

 //searches for idle courier 
courier* find_idle_courier(Node* head);

//after a courier finishes delivery we update back to IDLE.
void update_courier_after_delivery(courier* specific_courier, double delivery_time, double delivery_distance, int destX, int destY);
//end the shift of courier
Node* remove_single_courier(Node* head, char target_id[]);
//prints the statistics of all couriers
void print_couriers_stats(Node* head, FILE* log_file);

void print_courier(courier* specific_courier, FILE* log_file);

void free_all_couriers(Node* head);

courier* find_courier_by_id(Node* head, char id[10]);