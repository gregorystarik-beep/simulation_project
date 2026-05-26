#include "struct.h"
#include "event.h"
event* create_event(eventType type, double time, int order_number, void* data)//creates our event with void* so we can assign any veriable
{
	event* current_event = (event*)malloc(sizeof(event));
	if (current_event == NULL)
	{
		fprintf(stderr, "event was not created\n");
		return NULL;
	}
	current_event->type = type;
	current_event->time = time;
	current_event->order_number = order_number;
	current_event->data = data;
	return current_event;
}//Initializing all parameters
void enqueue_event(Node** head, event* new_event)//Priority Queue works like normal queue but is orderded that first item is smallest item or in our case the clossets event using min heap
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	Node* curr = *head;
	if (new_node == NULL)
	{
		fprintf(stderr, "node was not created\n");
		return;
	}
	new_node->data = new_event;
	new_node->next = NULL;
	if (*head == NULL||new_event->time<((event*)((*head)->data))->time)//checks to see if the new event smaller then head
	{
		new_node->next = *head;
		*head = new_node;
		return;
	}
	while ((curr->next != NULL)&&(((event*)((curr)->next->data))->time) <=new_event->time)//run in loop while we are not pointing to null and the new event is smaller then the current one
	{
		curr = curr->next;
	}
	new_node->next = curr->next;
	curr->next = new_node;
}
event* dequeue_event(Node** head)//works like normal dequeue because we sorted the queue in enqueue_event(Priority Queue)
{
	Node* temp = NULL;
	event* first_event = NULL;
	if (*head == NULL)
	{
		fprintf(stderr, "no event queue");
		return NULL;
	}
	temp = *head;
	first_event = (event*)temp->data;
	*head = (*head)->next;
	free(temp);
	temp = NULL;
	return first_event;
}
event* peek_event(Node* head)//lets us peak to see which is the first event
{
	if (head == NULL)
	{
		fprintf(stderr, "no event\n");
		return NULL;
	}
	return (event*)head->data;
}
void free_event(event* specific_event)
{
	if (specific_event != NULL)
	{
		free(specific_event);
		specific_event = NULL;
	}
}
void free_event_queue(Node** head)
{
	Node* curr = *head;
	Node* next_node = NULL;
	event* temp = NULL;
	while (curr != NULL)
	{
		temp = (event*)curr->data;
		next_node = curr->next;
		free_event(temp);
		curr->data = NULL;
		free(curr);
		curr = NULL;
		curr = next_node;
	}
	*head = NULL;
}
void print_event(event* specific_event, FILE* log_file)
{
	const char* status = "UNKNOWN";
	if (specific_event == NULL)
	{
		fprintf(stderr, "no data\n");
		return;
	}
	switch (specific_event->type)
	{
	case new_order:
		status = "new_order";
		break;
	case order_delivered:
		status = "order_delivered";
		break;
	case order_pickup:
		status = "order_pickup";
		break;
	case order_delayed:
		status = "order_delayed";
		break;
	case order_canceled:
		status = "order_canceled";
		break;
	default:
		status = "UNKNOWN";
		break;
	}
	printf("Time: %.2f | Order Number: %d | Event Type: %s\n",
		specific_event->time,
		specific_event->order_number,
		status);
	if (log_file != NULL)
	{
		fprintf(log_file,"Time: %.2f | Order Number: %d | Event Type: %s\n",
			specific_event->time,
			specific_event->order_number,
			status);
	}
}
void print_event_queue(Node* head, FILE* log_file)
{
	Node* curr = head;
	event* e = NULL;
	printf("\n========== SIMULATION RESULTS ==========\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "\n========== SIMULATION RESULTS ==========\n");
	}
	while (curr != NULL)
	{
		e = (event*)curr->data;
		print_event(e, log_file);
		curr = curr->next;
	}
	printf("==================================================\n");
	if (log_file != NULL)
	{
		fprintf(log_file, "==================================================\n");
	}
}

