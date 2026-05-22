#pragma once
#pragma once
#include "struct.h"
#include <stdio.h>

// יצירת הזמנה חדשה (הקצאת זיכרון דינמי למבנה ולמחרוזות)
order* create_order(char order_id[10], const char* food, const char* rest, char cust_id[10], double current_time);

// קישור שליח להזמנה (עדכון ה-courierID ברגע ששליח לוקח את ההזמנה)
void assign_courier_to_order(order* specific_order, char cour_id[10]);

// הדפסת פרטי ההזמנה (למסך ולקובץ הלוג)
void print_order(order* specific_order, FILE* log_file);

// שחרור הזיכרון של ההזמנה (כולל המחרוזות של שם האוכל והמסעדה)
void free_order(order* specific_order);



/* הכנסת הזמנה חדשה לסוף תור ההמתנה */
void enqueue_order(Node** head, order* new_order);

/* הוצאת ההזמנה הראשונה מתחילת תור ההמתנה (כדי להעביר לשליח) */
order* dequeue_order(Node** head);

/* חיפוש הזמנה בתור לפי מזהה ההזמנה */
order* find_order_by_id(Node* head, char id[10]);

/* הדפסת כל ההזמנות שנמצאות כרגע בתור (למסך ולקובץ לוג) */
void print_order_queue(Node* head, FILE* log_file);

/* שחרור מלא של כל הזיכרון שהוקצה לתור ולהזמנות שבו */
void free_order_queue(Node** head);