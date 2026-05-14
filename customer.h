#pragma once
#include "struct.h"
#include "HelperFucntions.h"
#include "Courier.h"
customer* create_customer(char* name, char id[10], int xCord, int yCord, int resX, int resY);

// שחרור הזיכרון של לקוח ספציפי (כולל השם)
void free_customer(customer* specific_customer);

// עדכון סטטוס ללקוח
void update_customer_status(customer* specific_customer, customer_status new_status);
double calculate_distance_time(customer* specific_customer);


// חישוב המרחק/זמן בין נקודה מסוימת ללקוח
double calculate_delivery_time(customer* specific_customer);

// הדפסת פרטי לקוח
void print_customer(customer* specific_customer, FILE* log_file);


// ==========================================
// 3. הצהרות: ניהול תור הלקוחות (רשימה מקושרת - מבוסס Head בלבד)
// ==========================================

// הוספת לקוח לסוף התור (ריצה עד סוף הרשימה והוספה)
void enqueue_customer(Node** head, customer* new_cust);

// שליפת הלקוח הראשון בתור (הוצאה מהראש ועדכון ה-head)
customer* dequeue_customer(Node** head);

// חיפוש לקוח בתור לפי תעודת זהות
customer* find_customer_by_id(Node* head, char id[10]);

// הדפסת כל הלקוחות שממתינים כרגע בתור
void print_customer_queue(Node* head, FILE* log_file);

// מחיקת כל התור ושחרור הזיכרון של כל הלקוחות בתוכו
void free_customer_queue(Node** head);

