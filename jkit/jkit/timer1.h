#ifndef TIMER1_H_
#define TIMER1_H_

volatile int pitch;
volatile int top;
volatile int pre_volume;
volatile int volume;

volatile uint8_t play_mode;
volatile uint8_t adc_volume_mode;
volatile uint8_t volume_onoff_mode;
volatile uint8_t hour_flag;
volatile uint8_t min_flag;


int *song_pointer;
unsigned int song_len;
volatile int song_idx;

extern const unsigned int grandpa_song[198];
extern const unsigned int candy[72];
extern const unsigned int SCHOOL_BELL[28];

void set_pwm_duty(uint16_t percent);
void set_pitch();
void set_timer1_CS(int n);
void set_timer1_Fast_PWM(int n);
void change_pitch(char c);

#endif /* TIMER1_H_ */
