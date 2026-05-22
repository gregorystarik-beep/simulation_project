#pragma once
#include "struct.h"

/* 1. יצירת אירוע חדש (הקצאת זיכרון ואתחול נתונים) */
event* create_event(eventType type, double time, int order_number, void* data);

/* 2. הכנסת אירוע לתור - תור עדיפויות ממוין לפי זמן (מהזמן הקטן לגדול) */
void enqueue_event(Node** head, event* new_event);

/* 3. שליפת האירוע הבא מהתור (תמיד יוציא את האירוע הקרוב ביותר בזמן) */
event* dequeue_event(Node** head);

/* 4. שחרור זיכרון של אירוע בודד */
void free_event(event* specific_event);

/* 5. שחרור כל תור האירועים מהזיכרון (בסיום הסימולציה) */
void free_event_queue(Node** head);

/* 6. פונקציית עזר להדפסת נתוני האירוע (חובה למעקב ודיבאגינג בקובץ הלוג) */
void print_event(event* specific_event, FILE* log_file);

event* peek_event(Node* head);

void print_event_queue(Node* head, FILE* log_file);