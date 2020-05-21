/** I N C L U D E S *************************************************/
#include "config.h"
#include <stdlib.h>

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static enum {FSM_ASKING,FSM_START1,FSM_BEEP1,FSM_WAIT1,FSM_DEAD1,
        FSM_START2,FSM_WAIT2,FSM_COUNT2,FSM_ALARM2,FSM_DEAD2,
        FSM_START3,FSM_SEQ_ON3,FSM_SEQ_OFF3,FSM_WAIT3,FSM_USER3,FSM_CHECK3,FSM_DEAD3
            } current_state;
static int unsigned time;
static int unsigned game_time;
static char signed led_switch;
static char signed next_led;
static int unsigned score;
static char unsigned status;
static int unsigned count;
static char signed loser;
static char unsigned melody_index;
static char signed game3seq[200];
static int unsigned game_lenght;
static int unsigned wait_time;
static int unsigned index;
/********************************************************************
 * Function:        void fsm_cube_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_cube_init(void) {
    current_state = FSM_ASKING;
    time = 0;
    led_switch=0;
    game_time = 0;
    score = 0;
    next_led = 0;
    count = 0;
    melody_index=0;
}


/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/
void fsm_cube(void) {
    
    switch (current_state) { 
        case FSM_ASKING :
            if ((BUT1==TRUE)&&(BUT0==TRUE)){
                current_state = FSM_START1;
            }
            else if((BUT2==TRUE)&&(BUT3==TRUE)){
                current_state = FSM_START2;
            }
            else if((BUT4==TRUE)&&(BUT5==TRUE)){
                current_state = FSM_START3;
            }
            else{
                time++;
                AUDIO_play(start_melody[melody_index]);
                if(time>=200){
                    time=0;
                    melody_index++;
                    LEDS = 0x00;
                    fire_led(led_switch);
                    fire_led(led_switch+1);
                    led_switch+=2;
                    if(led_switch>5){
                        led_switch=0;
                    }
                    if(melody_index==58){
                        melody_index = 0;
                    }
                }
            }
            break;
//***************************************************************************//
        case FSM_START1 :
            score = 0;
            game_time = 0;
            wait_time = 3000;
            if(buttonpressed()==FALSE){
                srand(time);
                current_state = FSM_WAIT1;
            }
            break;
        case FSM_BEEP1:
            fire_led(next_led);
            AUDIO_play(all_notes[next_led]);
            game_time++;
            if(checkbutton()==next_led){ 
                score++;
                game_time = 0;
                current_state = FSM_WAIT1;
            }else if((game_time>=wait_time)||(buttonpressed()==TRUE)){
                game_time = 0;
                current_state = FSM_DEAD1;
            }
            break;
        case FSM_WAIT1:
            AUDIO_stop();
            LEDS = 0x00;
            game_time++;
            if (game_time>=(wait_time/10))
            {
                next_led = rand()%6;
                wait_time -= 50;
                if(wait_time==100){
                    wait_time=150;
                }
                game_time = 0;
                current_state = FSM_BEEP1;
            }
            break;
        case FSM_DEAD1 :
            AUDIO_stop();
            game_time++;
            if(score==0)
            {
                time=0;
                current_state = FSM_ASKING;
            }
            if(game_time==400){
                LEDS=0xFC;
            }
            if(game_time==700){
                score--;
                game_time = 0;
                LEDS=0x00;
            }
            break;
//***************************************************************************//
        case FSM_START2 : 
            AUDIO_stop();
            loser=-1;
            game_time=0;
            status=0;
            melody_index = 0;
            current_state = FSM_WAIT2;
            break;
        case FSM_WAIT2 :
            game_time++;
            if (status==0){
                LEDS = 0x00;
            }else
                {
                    fire_led(0);
                    fire_led(1);
                    fire_led(2);
                    fire_led(3);
            }   
            if (game_time==500){
                game_time=0;
                status = abs(status-1);
            }
            if((BUT1==TRUE)&&(BUT0==TRUE)&&(BUT2==TRUE)&&(BUT3==TRUE)&&(BUT4==FALSE)&&(BUT5==FALSE))
            {
                game_time=0;
                current_state = FSM_COUNT2;
                count=10+rand()%5;
            }
            break;
        case FSM_COUNT2 :
            game_time++;    
            if (game_time==800){
                count--;
                status = abs(status-1);
                game_time=0;
            }
           
            if(status ==1){
                AUDIO_play(C3);
                fire_led(4);
                fire_led(5);
            }else{
                    AUDIO_stop();
                    LEDS=0x00;
            }
            if(BUT0==FALSE)       
            {
                game_time=0;
                loser = 0;
                current_state = FSM_DEAD2;
            }else if(BUT1==FALSE)       
                {
                game_time=0;    
                loser = 1;
                current_state = FSM_DEAD2;
            }else if(BUT2==FALSE)       
            {
                game_time=0;    
                loser = 2;
                current_state = FSM_DEAD2;
            }else if(BUT3==FALSE)       
            {
                game_time=0;    
                loser = 3;
                current_state = FSM_DEAD2;
            }
            if(count==0){
                game_time = 0;
                current_state = FSM_ALARM2;
            }
            break;
        case FSM_ALARM2 :
            game_time++;
            if (game_time==100){
                status = abs(status-1);
                game_time = 0;
            }
            if(status == 1){
                AUDIO_play(C3);
                LEDS = 0x00;
            }else{
                LEDS = 0xFC;
                AUDIO_play(C1);
            }
            if((BUT0==FALSE)&&(BUT1==FALSE)&&(BUT2==FALSE)&&(BUT3==TRUE)){
                melody_index=0;
                game_time = 0;
                loser = 3;
                current_state = FSM_DEAD2; 
            }else if((BUT0==FALSE)&&(BUT1==FALSE)&&(BUT2==TRUE)&&(BUT3==FALSE)){
                melody_index=0;
                game_time = 0;
                loser = 2;
                current_state = FSM_DEAD2;
            }else if((BUT0==FALSE)&&(BUT1==TRUE)&&(BUT2==FALSE)&&(BUT3==FALSE)){
                melody_index=0;
                game_time = 0;
                loser = 1;
                current_state = FSM_DEAD2;
            }else if((BUT0==TRUE)&&(BUT1==FALSE)&&(BUT2==FALSE)&&(BUT3==FALSE)){
                melody_index=0;
                game_time = 0;
                loser = 0;
                current_state = FSM_DEAD2;
            }
            break;
        case FSM_DEAD2:
            game_time++;
            AUDIO_play(loser_melody[melody_index]);
            if(game_time==100){
                game_time = 0;
                status = abs(status-1);
                melody_index++;
                if(melody_index==45){
                    time = 0;
                    melody_index = 0;
                    current_state = FSM_ASKING;
                }
            }
            if(status==0){
                fire_led(loser);    
            }else{
                LEDS = 0x00;
            }
            break;       
//***************************************************************************//
        case FSM_START3 :
            AUDIO_stop();
            game_time=0;
            count=0;
            game_lenght=1;
            if(buttonpressed()==FALSE){
                srand(time);
                current_state = FSM_SEQ_OFF3;
            }
            break;
        case FSM_SEQ_OFF3:
            game_time++;
            LEDS=0x00;
            AUDIO_stop();
            if(game_time==500){
                game_time = 0;
                game3seq[count]=rand()%6;
                current_state = FSM_SEQ_ON3;
            }
            break;
        case FSM_SEQ_ON3:
            AUDIO_play(all_notes[game3seq[count]]);
            fire_led(game3seq[count]);
            game_time++;
            if(game_time==1000){
                game_time=0;
                count++;
                if(count==game_lenght){
                    count=0;
                    current_state = FSM_USER3;
                }else{
                    current_state = FSM_SEQ_OFF3;
                }
            }
            break;
        case FSM_USER3: 
            if(buttonpressed()==FALSE){
                current_state = FSM_WAIT3;
            }
            break;
        case FSM_WAIT3:
            AUDIO_stop();
            LEDS=0x00;
            game_time++;
            if(buttonpressed()==TRUE){
                game_time=0;
                current_state = FSM_CHECK3;
            }else if(game_time==2000){
                game_time=0;
                current_state = FSM_DEAD3;
            }
            break;
        case FSM_CHECK3 : 
            if(checkbutton()==game3seq[count]){
                fire_led(checkbutton());
                AUDIO_play(all_notes[checkbutton()]);
                count++;
                if (count==game_lenght){
                    game_time=0;
                    game_lenght++;
                    count=0;
                    current_state=FSM_SEQ_OFF3;
                }else{
                    game_time=0;
                    current_state=FSM_USER3;
                }
            }else
            {
                game_time=0;
                current_state=FSM_DEAD3; 
            }
        case FSM_DEAD3:
            AUDIO_stop();
            game_time++;
            if(game_lenght==0)
            {
                time=0;
                current_state = FSM_ASKING;
            }
            if(game_time==200){
                LEDS=0xFC;
            }
            if(game_time==400){
                game_lenght--;
                game_time = 0;
                LEDS=0x00;
            }
            break;
//***************************************************************************//
        default:
            current_state = FSM_ASKING;
    }
}
void fire_led(char ledint){
    switch(ledint){
        case 0:
                LED0=TRUE;
                break;
            case 1:
                LED1=TRUE;
                break;
            case 2:
                LED2=TRUE;
                break;
            case 3:
                LED3=TRUE;
                break;
            case 4:
                LED4=TRUE;
                break;
            case 5:
                LED5=TRUE;
                break;
            default:
                LEDS = 0x00;
        }
    }
char buttonpressed(void){
    if((BUT0==TRUE)||(BUT1==TRUE)||(BUT2==TRUE)||(BUT3==TRUE)||(BUT4==TRUE)||(BUT5==TRUE)){
        return TRUE;
    }else{
        return FALSE;
    }
} 
char checkbutton(void){
        if(BUT0==TRUE)
        {
            return 0;
        }
        else if(BUT1==TRUE)
        {
            return 1;
        }
        else if(BUT2==TRUE)
        {
            return 2;
        }
        else if(BUT3==TRUE)
        {
            return 3;
        }
        else if(BUT4==TRUE)
        {
            return 4;
        }
        else if(BUT5==TRUE)
        {
            return 5;
        }
        else
        {
            return -1;
        }
    }

