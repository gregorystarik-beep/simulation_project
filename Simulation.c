#include "struct.h"
#include "Simulation.h"
#include "event.h"
#include "Courier.h"
#include "customer.h"
#include "Order.h"
#include "HelperFucntions.h"
#include <math.h>
Simulation* init_simulation(int num_locations, int num_couriers, const char* log_filename)//we creates our Simulation
{
	int i,j;
	Simulation* sim = (Simulation*)malloc(sizeof(Simulation));
	if (sim == NULL)
	{
		fprintf(stderr, "ERROR\n");
		return NULL;
	}
	sim->current_time = 0.0;
	sim->event_queue = NULL;
	sim->order_queue = NULL;
	sim->customer_queue = NULL;
	sim->total_couriers = 0;
	sim->total_orders_handled = 0;
	sim->successful_deliveries = 0;
	sim->total_customer_wait = 0.0;
	sim->total_locations = num_locations;
	sim->total_couriers = num_couriers;
	sim->distance_matrix = (double**)malloc(sim->total_locations * sizeof(double*));
	//Initializing all couriers parameters
	if (sim->distance_matrix == NULL)
	{
		fprintf(stderr, "no allocation\n");
		free(sim);
		sim = NULL;
		return NULL;
	}
	for (i = 0; i < sim->total_locations; ++i)
	{
		sim->distance_matrix[i] = (double*)malloc(sim->total_locations * sizeof(double));
		if (sim->distance_matrix[i] == NULL)
		{
			for (j = 0; j < i; ++j)
			{
				free(sim->distance_matrix[j]);
			}
			free(sim->distance_matrix);
			sim->distance_matrix = NULL;
			free(sim);
			sim = NULL;
			fprintf(stderr, "Error allocating matrix row\n");
			return NULL;
		}
	}//allocating  memory 
	for (i = 0; i < sim->total_locations; ++i)
	{
		for (j = 0; j < sim->total_locations; ++j)
		{
			sim->distance_matrix[i][j] = sqrt((double)(i * i + j * j));
		}
	}
	sim->couriers_list = init_couriers(num_couriers);//opening file so we can write in it
	sim->log_file = fopen(log_filename, "w");
	if (sim->log_file == NULL)
	{
		for (j = 0; j < sim->total_locations; ++j)
		{
			free(sim->distance_matrix[j]);
		}
		free(sim->distance_matrix);
		sim->distance_matrix = NULL;
		free_all_couriers(sim->couriers_list);
		sim->couriers_list = NULL;
		free(sim);
		sim = NULL;
		fprintf(stderr, "file could not open\n");
		return NULL;
	}
	return sim;
}
void handle_new_order(Simulation* sim, event* current_event)//handles a new order
{
	event* pickup_event = NULL;
	double time = 0.0;
	double pickup_time = 0.0;
	order* my_order = (order*)current_event->data;
	customer* cust;
	cust = find_customer_by_id(sim->customer_queue,my_order->customerID);//searches the customer who ordered
	courier* found_courier = find_closest_courier(sim, cust->resX, cust->resY);//using a function that lets us find the clossest courier
	if (found_courier != NULL)
	{
		found_courier->status = BUSY;//after that we change status to busy
		assign_courier_to_order(my_order, found_courier->id);//we assign him the order
		time = calculate_delivery_time(cust);
		pickup_time=sim->current_time + time;//and update the pickup time
		pickup_event = create_event(order_pickup, pickup_time, atoi(my_order->id), my_order);//creating new event 
		if (pickup_event == NULL)
		{
			fprintf(stderr, "no event created\n");
			return;
		}
		enqueue_event(&sim->event_queue, pickup_event);//enqueuing it 
	}
	else
	{
		enqueue_order(&sim->order_queue, my_order);
	}
}
void handle_order_pickup(Simulation* sim, event* current_event)
{
	order* my_order = (order*)current_event->data;
	customer* my_cust = NULL;
	double travel_time = 0.0,event_time=0.0;
	int chance = rand() % 100;
	event* next_event = NULL;
	my_cust = find_customer_by_id(sim->customer_queue, my_order->customerID);
	if (my_cust != NULL)
	{
		travel_time = calculate_delivery_time(my_cust);//we calculate how much time it will take to get to the customer
		event_time = travel_time + sim->current_time;
		if (chance < 15)//usinh rand to create a chance the a order is delayed or canceled
		{
			next_event = create_event(order_delayed, event_time, atoi(my_order->id), my_order);
		}
		else if (chance >= 15 && chance < 25)
		{
			next_event = create_event(order_canceled, event_time, atoi(my_order->id), my_order);
		}
		else
		{
			next_event = create_event(order_delivered, event_time, atoi(my_order->id), my_order);
		}
		if (next_event == NULL)
		{
			fprintf(stderr, "no event created\n");
			return;
		}
		enqueue_event(&sim->event_queue, next_event);
	}
}
void handle_order_delivered(Simulation* sim, event* current_event)
{
	order* my_order = (order*)current_event->data;
	courier* my_courier = NULL;
	customer* my_customer = NULL;
	double distance = 0.0, wait_time = 0.0, pickup_time = 0.0;
	order* waiting_order = NULL;
	customer* waiting_cust = NULL;
	event* new_pickup_event = NULL;
	my_customer = find_customer_by_id(sim->customer_queue, my_order->customerID);
	my_courier = find_courier_by_id(sim->couriers_list, my_order->courierID);
	//Initializing all parameters
	if (my_customer != NULL && my_courier != NULL)
	{
		my_order->delivery_time = sim->current_time;
		update_customer_status(my_customer, RECEIVED);//we update the customer status
		sim->successful_deliveries++;//add to statistics
		sim->total_orders_handled++;
		wait_time = sim->current_time - my_order->creation_time;
		sim->total_customer_wait += wait_time;
		distance = calculate_distance_time(my_customer);
		update_courier_after_delivery(my_courier, wait_time, distance, my_customer->xCord, my_customer->yCord);
		if (my_courier->individual_deliveries_count >= 3)//after 3 deliveries a couries ends his shift 
		{
			printf("  -> [SHIFT END] Courier %s finished 3 deliveries and is going home.\n", my_courier->id);

			// deletes him from linked list 
			sim->couriers_list = remove_single_courier(sim->couriers_list, my_courier->id);

			// total couriers is down by one
			sim->total_couriers--;
		}
		else
		{
			if (sim->order_queue != NULL)
			{
				waiting_order = dequeue_order(&sim->order_queue);
				assign_courier_to_order(waiting_order, my_courier->id);
				my_courier->status = BUSY;
				waiting_cust = find_customer_by_id(sim->customer_queue, waiting_order->customerID);
				pickup_time = sim->current_time + calculate_delivery_time(waiting_cust);
				new_pickup_event = create_event(order_pickup, pickup_time, atoi(waiting_order->id), waiting_order);
				if (new_pickup_event == NULL)
				{
					fprintf(stderr, "no event created\n");
					return;
				}
				enqueue_event(&sim->event_queue, new_pickup_event);
			}
		}
	}
}
void handle_order_delayed(Simulation* sim, event* current_event)
{
	order* my_order = (order*)current_event->data;
	double delay_time = 15.0;//we add 15 minutes 
	double new_delivery_time = 0.0;
	event* delayed_event = NULL;
	new_delivery_time = sim->current_time + delay_time;//and add it to the new delivery time
	delayed_event = create_event(order_delivered, new_delivery_time, atoi(my_order->id), my_order);
	if (delayed_event == NULL)
	{
		fprintf(stderr, "no event created\n");
		return;
	}
	enqueue_event(&sim->event_queue, delayed_event);
}
void handle_order_canceled(Simulation* sim, event* current_event)
{
	order* my_order = (order*)current_event->data;
	customer* my_cust = NULL;
	courier* my_courier = NULL;
	order* waiting_order = NULL;
	customer* waiting_cust = NULL;
	event* new_pickup_event = NULL;
	double pickup_time = 0.0;
	my_cust = find_customer_by_id(sim->customer_queue,my_order->customerID);
	if (my_cust == NULL)
	{
		fprintf(stderr, "No customer\n");
		return;
	}
	update_customer_status(my_cust, CANCELED);
	if (strcmp(my_order->courierID, "NONE") != 0)//if order gets canceled we find for the couriers a new order and create a new pickup event
	{
		my_courier = find_courier_by_id(sim->couriers_list, my_order->courierID);
		my_courier->status = IDLE;
		if (sim->order_queue != NULL)
		{
			waiting_order = dequeue_order(&sim->order_queue);
			assign_courier_to_order(waiting_order, my_courier->id);
			my_courier->status = BUSY;
			waiting_cust = find_customer_by_id(sim->customer_queue, waiting_order->customerID);
			pickup_time = sim->current_time + calculate_delivery_time(waiting_cust);
			new_pickup_event = create_event(order_pickup, pickup_time, atoi(waiting_order->id), waiting_order);
			if (new_pickup_event == NULL)
			{
				fprintf(stderr, "no event created\n");
				return;
			}
			enqueue_event(&sim->event_queue, new_pickup_event);
		}
	}
}
void load_data_from_file(Simulation* sim, const char* filename)//loading data from  file
{
	char order_id[10];
	char food[50];
	char restaurant[50];
	char cust_id[10];
	double creation_time = 0.0;
	order* n_order = NULL;
	event* new_event = NULL;
	FILE* fin = fopen(filename ,"r");
	if (fin == NULL)
	{
		fprintf(stderr, "cannot open file\n");
		return;
	}
	while (fscanf(fin, "%s %s %s %s %lf", order_id, food, restaurant, cust_id, &creation_time) == 5)//we scan for order id restaurant and customer's id and creation time of order
	{
		n_order=create_order(order_id, food, restaurant, cust_id, creation_time);
		if (n_order == NULL)
		{
			fprintf(stderr, "no order was created\n");
			return;
		}
		new_event= create_event(new_order, creation_time, atoi(order_id), n_order);//and create a new event 
		if (new_event == NULL)
		{
			fprintf(stderr, "no event created\n");
			return;
		}
		enqueue_event(&sim->event_queue, new_event);
	}
	fclose(fin);
}
void init_mock_customers(Simulation* sim)//creates for us customers and uses the ID function from HelperFunctions
{
	char genrated_id[10];
	customer* new_cust = NULL;
	int i;
	const char* names_list[] = { "Dan", "Yael", "Ron", "Tamar", "Avi", "Sara", "Gal", "Roy", "Maya", "Eli" };
	for (i = 0; i < 10; ++i)
	{
		Generate_ID(genrated_id);
		new_cust = create_customer(names_list[i], genrated_id, rand() % 100, rand() % 100, rand() % 100, rand() % 100);
		enqueue_customer(&sim->customer_queue, new_cust);
	}
}
courier* find_closest_courier(Simulation* sim, int resX, int resY)
{
	Node* curr = sim->couriers_list;
	courier* closest = NULL;
	courier* c = NULL;
	double min_distance = 9999999.0;
	double curr_distance;
	int dx, dy;
	while (curr != NULL)
	{
		c = (courier*)curr->data;
		if (c->status == IDLE)
		{
			dx = abs(c->currentX - resX);//the dx is the current x of couries minus the location of the restaurant
			dy = abs(c->currentY - resY);//the dy is the current y of couries minus the location of the restaurant
			if (dx >= sim->total_locations) dx = sim->total_locations - 1;//checks out of bounds
			if (dy >= sim->total_locations) dy = sim->total_locations - 1;
			curr_distance = sim->distance_matrix[dx][dy];//then puts location in matrix
			if (curr_distance < min_distance)
			{
				min_distance = curr_distance;//and updates the min distance if its smaller
				closest = c;
			}
		}
		curr = curr->next;
	}
	if (closest == NULL)
	{
		fprintf(stderr, "couldnt find a close courier\n");
		return NULL;
	}
	return closest;
}
void save_statistics_to_file(Simulation* sim, const char* filename)
{
	double avg_wait = 0.0;
	Node* curr = NULL;
	courier* c = NULL;
	FILE* fin = fopen(filename, "w");
	if (fin == NULL)
	{
		fprintf(stderr, "couldnt open file\n");
		return;
	}
	if (sim->successful_deliveries > 0)
	{
		avg_wait = sim->total_customer_wait / sim->successful_deliveries;
	}
	fprintf(fin, "--- Simulation Final Statistics ---\n");
	fprintf(fin, "Total Orders Handled: %d\n", sim->total_orders_handled);
	fprintf(fin, "Successful Deliveries: %d\n", sim->successful_deliveries);
	fprintf(fin, "Average Wait Time: %.2f min\n", avg_wait);
	fprintf(fin, "-----------------------------------\n\n");
	fprintf(fin, "--- Couriers Summary ---\n");
	curr = sim->couriers_list;
	while (curr != NULL)
	{
		c = (courier*)curr->data;
		fprintf(fin, "Courier ID: %s | Deliveries: %d | Busy Time: %.2f min | Distance: %.2f\n",
			c->id,
			c->individual_deliveries_count,
			c->individual_busy_time,
			c->distance);
		curr = curr->next;
	}
	fclose(fin);
}
void free_simulation(Simulation* sim)
{
	int i;
	if (sim == NULL)
	{
		fprintf(stderr, "simulation not active\n");
		return;
	}
	if (sim->log_file != NULL)
	{
		fclose(sim->log_file);
	}
	free_all_couriers(sim->couriers_list);
	free_customer_queue(&sim->customer_queue);
	free_event_queue(&sim->event_queue);
	free_order_queue(&sim->order_queue);
	for (i = 0; i < sim->total_locations; ++i)
	{
		free(sim->distance_matrix[i]);
		sim->distance_matrix[i] = NULL;
	}
	free(sim->distance_matrix);
	sim->distance_matrix = NULL;
	free(sim);
	sim = NULL;
}
void draw_city_map(Simulation* sim)
{
	char map[10][10];
	int i, j, x, y;
	Node* curr = NULL;
	courier* cour = NULL;
	customer* cust = NULL;

	// creates map
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			map[i][j] = '.';
		}
	}
	curr = sim->customer_queue;
	while (curr != NULL)
	{
		cust = (customer*)curr->data;
		x = cust->resX/10;//using x and y as indexes on matrix
		y = cust->resY/10;
		if (x >= 0 && x < 10 && y >= 0 && y < 10)map[x][y] = 'R';//puts Restaurant if its in matrix
		if (cust->status == WAITING)
		{
			x = cust->xCord / 10;
			y = cust->yCord / 10;
			if (x >= 0 && x < 10 && y >= 0 && y < 10) map[x][y] = 'C';//puts customer
		}
		curr = curr->next;
	}
	curr = sim->couriers_list;
	while (curr != NULL)
	{
		cour = (courier*)curr->data;
		x = cour->currentX / 10;
		y = cour->currentY / 10;//same to courier
		if (x >= 0 && x < 10 && y >= 0 && y < 10)
		{
			if (cour->status == IDLE)map[x][y] = 'I';//but here we are using  his status
			else map[x][y] = 'B';
		}
		curr = curr->next;
	}
	printf("\n============= CITY MAP (T=%.2f) =============\n", sim->current_time);
	for (i = 0; i < 10; i++)
	{
		printf("  ");
		for (j = 0; j < 10; j++)
		{
			printf("%c  ", map[i][j]);
		}
		printf("\n");
	}
	printf("===============================================\n\n");
}


