#pragma once
#include "struct.h"
#include "Simulation.h"
#include "event.h"
#include "Courier.h"
#include "customer.h"
/* ניהול הסימולציה */
/* ניהול הסימולציה */
/* ניהול הסימולציה */
Simulation* init_simulation(int num_locations, int num_couriers, const char* log_filename);
void free_simulation(Simulation* sim);

/* פונקציות עזר (חובה לפרויקט) */
void load_data_from_file(Simulation* sim, const char* filename);
void save_statistics_to_file(Simulation* sim, const char* filename); /* חובה לדרישת שמירה */
courier* find_closest_courier(Simulation* sim, int resX, int resY);

/* פונקציות טיפול באירועים (Event Handlers) */
void handle_new_order(Simulation* sim, event* current_event);
void handle_order_pickup(Simulation* sim, event* current_event);
void handle_order_delivered(Simulation* sim, event* current_event);
void handle_order_delayed(Simulation* sim, event* current_event);
void handle_order_canceled(Simulation* sim, event* current_event);

void init_mock_customers(Simulation* sim);