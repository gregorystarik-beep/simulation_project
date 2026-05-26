#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> //for (Sleep)
#include <string.h>
#include "struct.h"
#include "Simulation.h"
#include "event.h"
#include "Courier.h"
#include "customer.h"
#include "Order.h"
#include "HelperFucntions.h"

int main()
{
    // //Initializing system
    FILE* log_file = NULL;
    srand((unsigned int)time(NULL));
    Simulation* sim = init_simulation(100, 5, "simulation_log.txt");
    if (sim == NULL) return 1;

    init_mock_customers(sim);
    create_test_file_automatically(sim);
    load_data_from_file(sim, "orders.txt");

    log_file = fopen("simulation_log.txt", "a");
    if (log_file == NULL)
    {
        fprintf(stderr, "couldnt open log\n");
        return 1;
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//windows commends to give us control on terminal
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    system("cls"); //cleans screen for map

    printf("--- Starting Delivery DES Terminal Simulation ---\n");
    printf("Legend: [.] Empty | [R] Restaurant | [C] Customer | [I] Idle Courier | [B] Busy Courier\n\n");

    fprintf(log_file,"--- Starting Delivery DES Terminal Simulation ---\n");
    fprintf(log_file,"Legend: [.] Empty | [R] Restaurant | [C] Customer | [I] Idle Courier | [B] Busy Courier\n\n");
    event* curr_event = NULL;
    event* upcoming = NULL;

    //runs on events 
    while (sim->event_queue != NULL)
    {
        upcoming = peek_event(sim->event_queue);//lets us see the upcoming event 
        if (upcoming != NULL) {
            // when an event is incomming 
            printf("[Radar] Next upcoming event is Type %d at T=%.2f...\n", upcoming->type, upcoming->time);
            fprintf(log_file,"[Radar] Next upcoming event is Type %d at T=%.2f...\n", upcoming->type, upcoming->time);
        }
        curr_event = dequeue_event(&sim->event_queue);
        if (curr_event == NULL) break;

        sim->current_time = curr_event->time;
        GetConsoleScreenBufferInfo(hConsole, &csbi);//saves the info before drawing the map again 
        COORD log_pos = csbi.dwCursorPosition;

        //
        COORD map_pos = { 0, csbi.srWindow.Top };//creates new map where the old one was
        SetConsoleCursorPosition(hConsole, map_pos);

        // drawing the map and the commend are for the map to clear it self and update live while running
        draw_city_map(sim);

        //
        if (log_pos.Y < map_pos.Y + 14) {//saves the first 14 lines for map
            log_pos.Y = map_pos.Y + 14;
        }
        SetConsoleCursorPosition(hConsole, log_pos);
        //after drawing the map go dwon and keep wrtiting logs
        switch (curr_event->type)
        {
        case new_order:
            printf("[Time: %.2f] NEW ORDER: Order #%d was received in the system.\n", sim->current_time, curr_event->order_number);

            fprintf(log_file,"[Time: %.2f] NEW ORDER: Order #%d was received in the system.\n", sim->current_time, curr_event->order_number);

            handle_new_order(sim, curr_event);
            break;

        case order_pickup:
            printf("[Time: %.2f] PICKUP: Courier is currently picking up Order #%d.\n", sim->current_time, curr_event->order_number);

            fprintf(log_file,"[Time: %.2f] PICKUP: Courier is currently picking up Order #%d.\n", sim->current_time, curr_event->order_number);

            handle_order_pickup(sim, curr_event);
            break;

        case order_delivered:
            printf("[Time: %.2f] DELIVERED: Order #%d successfully reached the customer!\n", sim->current_time, curr_event->order_number);

            fprintf(log_file,"[Time: %.2f] DELIVERED: Order #%d successfully reached the customer!\n", sim->current_time, curr_event->order_number);

            handle_order_delivered(sim, curr_event);
            break;

        case order_delayed:
            printf("[Time: %.2f] DELAY: Order #%d is experiencing a delay in traffic.\n", sim->current_time, curr_event->order_number);

            fprintf(log_file,"[Time: %.2f] DELAY: Order #%d is experiencing a delay in traffic.\n", sim->current_time, curr_event->order_number);

            handle_order_delayed(sim, curr_event);
            break;

        case order_canceled:
            printf("[Time: %.2f] CANCELED: Order #%d has been canceled.\n", sim->current_time, curr_event->order_number);

            fprintf(log_file,"[Time: %.2f] CANCELED: Order #%d has been canceled.\n", sim->current_time, curr_event->order_number);

            handle_order_canceled(sim, curr_event);
            break;
        }

        // prints texts live to buffer
        fflush(stdout);
        fflush(log_file);

        //slows down the running so we can read
        Sleep(1500);

        free_event(curr_event);
    }

    printf("\n--- Simulation Complete ---\n");

    fprintf(log_file,"\n--- Simulation Complete ---\n");

    fclose(log_file);
    save_statistics_to_file(sim, "statistics_report.txt");
    free_simulation(sim);//frees the simulation

    return 0;
}