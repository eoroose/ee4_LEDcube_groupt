/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         24/02/2017
 ********************************************************************/
/** I N C L U D E S *************************************************/
#include "config.h"
#include <stdlib.h>

/** D E F I N E S ***************************************************/
#define PUSHED 0

/** P R I V A T E   V A R I A B L E S *******************************/
static enum {FSM_IDLE,FSM_ON,FSM_OFF,FSM_CALC
            } current_state;
static int unsigned oncount;
static int unsigned offcount;
static int unsigned global_count;
static int unsigned led_count;
static int unsigned speed;
/********************************************************************
 * Function:        void fsm_game_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        This init sets the start state for this FSM and
 *                  may initialize some counters          
 ********************************************************************/
void fsm_game_init(void) {
	current_state = FSM_IDLE;
    global_count=0;
    offcount=0;
    oncount =0;
    led_count = 0;
    speed = 500;
}

/********************************************************************
 * Function:        void fsm_game(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        An implementation for a simple reaction game
 ********************************************************************/
void fsm_game(void) {
    
    switch (current_state) { 
        case FSM_IDLE :
        // *** outputs ***
            if (PRG_BUTTON == PUSHED){
                current_state = FSM_CALC;
            }
            break;
        case FSM_ON : 
            LED1_OUT = 1;
            oncount--;
            if (oncount==0){
                led_count-=1;
                current_state= FSM_OFF;
                offcount=speed;
            }         
            break;
        case FSM_OFF :
            LED1_OUT = 0;
            offcount-=1;
            if (offcount==0){
                if(led_count == 0){
                    current_state = FSM_IDLE;
                }
                else
                {
                    current_state = FSM_ON;
                    oncount = speed;
                }
            }
              
            break;       
        case FSM_CALC : 
            global_count+=1;
            oncount=speed;
            current_state = FSM_ON;
            led_count = global_count;
            break;
        default:
            current_state = FSM_IDLE;
            break;
         
    }
    
}