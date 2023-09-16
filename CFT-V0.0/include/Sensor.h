#include <HCSR04.h>
int trigger = 4, echo = 5, nilaiKurasHarian = 10, nilaiKurasMingguan = 80, nilaiPengisianPenuh = 0;
float offset = 36.03;
HCSR04 hc(trigger, echo);