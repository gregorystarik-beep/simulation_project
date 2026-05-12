#pragma once
#include "struct.h"
// מקצה זיכרון למערך השליחים בהתאם לכמות המבוקשת.
// מקצה זיכרון דינמי לשם של כל שליח, מעניק מזהה (ID), מגדיר סטטוס התחלתי כפנוי ומאפס את המונים הסטטיסטיים.
Node* init_couriers(int num_couriers);

// סורקת את מערך השליחים ובודקת את הסטטוס של כל אחד מהם.
// מחזירה מצביע לשליח הראשון שנמצא פנוי (COURIER_IDLE), או NULL אם כולם עסוקים.
courier* find_idle_courier(Node* head);

// מופעלת כאשר שליח מסיים את המשלוח.
// מעבירה את הסטטוס שלו בחזרה לפנוי, מקדמת את מונה המשלוחים ב-1, ומוסיפה את זמן השליחות לסך זמן העבודה שלו.
void update_courier_after_delivery(courier* specific_courier, double delivery_time);
// עוברת על מערך השליחים ומשחררת בצורה נקייה את הזיכרון הדינמי שהוקצה לשמות (char*).
// לאחר מכן משחררת את הזיכרון של המערך עצמו כדי למנוע דליפות זיכרון בסיום התוכנית.
Node* remove_single_courier(Node* head, char target_id[]);
// פונקציה להדפסת הסטטיסטיקות של כל השליחים בסיום הסימולציה
void print_couriers_stats(Node* head, FILE* log_file);

void free_all_couriers(Node* head);
