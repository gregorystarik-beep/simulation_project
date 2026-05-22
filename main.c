#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // חובה בשביל ההשהייה הריאליסטית (Sleep)

#include "struct.h"
#include "Simulation.h"
#include "event.h"
#include "Courier.h"
#include "customer.h"
#include "Order.h"
#include "HelperFucntions.h"

int main()
{
    // אתחול מערכת
    srand((unsigned int)time(NULL));
    Simulation* sim = init_simulation(100, 5, "simulation_log.txt");
    if (sim == NULL) return 1;

    init_mock_customers(sim);
    create_test_file_automatically(sim);
    load_data_from_file(sim, "orders.txt");

    printf("--- Starting Realistic Simulation ---\n\n");

    event* curr_event = NULL;
    event* upcoming = NULL;

    // הלולאה רצה אוטומטית - בלי אנטרים ובקצב ריאליסטי (לא מהירות בזק)
    while (sim->event_queue != NULL)
    {
        upcoming = peek_event(sim->event_queue);
        if (upcoming != NULL) {
            // מדפיסים התראה קטנה על מה שעומד לקרות
            printf("[Radar] Next upcoming event is Type %d at T=%.2f...\n", upcoming->type, upcoming->time);
        }
        curr_event = dequeue_event(&sim->event_queue);
        if (curr_event == NULL) break;

        sim->current_time = curr_event->time;

        // הדפסת טקסט ייחודי וריאליסטי לכל סוג של אירוע (לא סתם מילים שחוזרות על עצמן)
        switch (curr_event->type)
        {
        case new_order:
            printf("[Time: %.2f] NEW ORDER: Order #%d was received in the system.\n", sim->current_time, curr_event->order_number);
            handle_new_order(sim, curr_event);
            break;

        case order_pickup:
            printf("[Time: %.2f] PICKUP: Courier is currently picking up Order #%d.\n", sim->current_time, curr_event->order_number);
            handle_order_pickup(sim, curr_event);
            break;

        case order_delivered:
            printf("[Time: %.2f] DELIVERED: Order #%d successfully reached the customer!\n", sim->current_time, curr_event->order_number);
            handle_order_delivered(sim, curr_event);
            break;

        case order_delayed:
            printf("[Time: %.2f] DELAY: Order #%d is experiencing a delay in traffic.\n", sim->current_time, curr_event->order_number);
            handle_order_delayed(sim, curr_event);
            break;

        case order_canceled:
            printf("[Time: %.2f] CANCELED: Order #%d has been canceled.\n", sim->current_time, curr_event->order_number);
            handle_order_canceled(sim, curr_event);
            break;
        }

        // דוחף את הטקסט למסך באותו רגע
        fflush(stdout);

        // --- כאן הקסם --- 
        // המתנה של שנייה וחצי (1500 מילי-שניות) כדי שזה ירוץ בקצב ריאליסטי שתוכל לקרוא
        Sleep(1500);

        free_event(curr_event);
    }

    printf("\n--- Simulation Complete ---\n");

    save_statistics_to_file(sim, "statistics_report.txt");
    free_simulation(sim);

    return 0;
}