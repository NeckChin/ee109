void encoder_init();
int get_high_temp();
int get_low_temp();
char temp_has_change();
ISR(PCINT1_vect);
ISR(PCINT0_vect);