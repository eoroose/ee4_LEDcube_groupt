/*********************************************************************
 *
 *                  EE 4 Project - Code Template
 *
 *********************************************************************
 * Processor:       PIC18F2550
 * Compiler:        XC8 1.35+
 * Author:          Jeroen Van Aken
 * Updated:         17/02/2016
 ********************************************************************/

/** I N C L U D E S *************************************************/
#include "func_audio.h"

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the corresponding .h file

/** P R I V A T E   V A R I A B L E S *******************************/
// 'static' implies that the variable can only be used in this file
// (cfr. 'private' in Java)
static unsigned int timer_preload;
static unsigned char stop;

/** P R I V A T E   P R O T O T Y P E S *****************************/
// 'static' implies that the function can only be used in this file
// (cfr. 'private' in Java)
static void OpenTimer1(unsigned char intEnable);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void AUDIO_init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Initializes all output channels for the AUDIO process
 ********************************************************************/
int all_notes [15] = {C3,B2,A2,G2,F2,E2,D2,C2,B1,A1,G1,F1,E1,D1,C1};
int loser_melody [45] = {C3,C3,C3,B2,B2,B2,A2,A2,A2,G2,G2,G2,F2,F2,F2,E2,E2,E2,D2,D2,D2,C2,C2,C2,B1,B1,B1,A1,A1,A1,G1,G1,G1,F1,F1,F1,E1,E1,E1,D1,D1,D1,C1,C1,C1}; 
int start_melody [58] = {A1o,D1,G1,A1o,C2,F1,A1,A1o,G1,A1o,D2,D2o,G1,D2,C2,A1o,D1,G1,A1o,C2,F1,A1,A1o,G1,A1o,D2,D2o,G1,D2,C2,A1o,D1,G1,A1o,C2,F1,A1,A1o,G1,A1o,D2,D2o,G1,D2,C2,A1o,D1,G1,A1o,A1,C1,F1,G1,F1,G1,F1,G1,A1};
void AUDIO_init() {
    OpenTimer1(TRUE);
    timer_preload = 0;
    stop = 0;
}

/********************************************************************
 * Function:        void AUDIO_play(...)
 * PreCondition:    AUDIO_init()
 * Input:           freq : the frequency you want to play
 * Output:          None
 * Overview:        Plays the frequency on the output pin configured
 *                  in the initialization.
 ********************************************************************/
void AUDIO_play(unsigned int freq) {
    stop = 0;
    timer_preload = freq; 
    T1CONbits.TMR1ON = 1;
}

/********************************************************************
 * Function:        void AUDIO_stop(...)
 * PreCondition:    AUDIO_init()
 * Input:           None
 * Output:          None
 * Overview:        Stops playing the frequency set by AUDIO_play()
 ********************************************************************/
void AUDIO_stop() {
    stop = 1;
}

/********************************************************************
 * Function:        void AUDIO_ISR(void)
 * PreCondition:    AUDIO_init()
 * Input:           None
 * Output:          None
 * Overview:        AUDIO Interrupt service routine will process all
 *                  notes and set the outputs accordingly
 ********************************************************************/
void AUDIO_ISR(void) {
    if (PIR1bits.TMR1IF == 1) {
        if (stop == 1) {
            T1CONbits.TMR1ON = 0;
            AUDIO_OUT = 0;
        } else {
            AUDIO_OUT = (unsigned) !AUDIO_OUT;
            TMR1 = timer_preload; 
        }
        PIR1bits.TMR1IF = 0;
    }
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void OpenTimer1(unsigned char intEnable)
 * PreCondition:    None
 * Input:           intEnable: enable Timer1 interrupt
 * Output:          None
 * Overview:        Will initialize Timer1 given the parameters
 ********************************************************************/
static void OpenTimer1(unsigned char intEnable) {
    T1CONbits.T1RD16 = 1;
    T1CONbits.T1CKPS = 0b00;
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 1;
    T1CONbits.T1SYNC = 1;  
    
    PIE1bits.TMR1IE = intEnable & 0x01u;
    INTCONbits.GIE = (intEnable & 0x01u) | INTCONbits.GIE;
    INTCONbits.PEIE = (intEnable & 0x01u) | INTCONbits.PEIE;
    IPR1bits.TMR1IP = 0;        // priority high, otherwise frequency not stable
    PIR1bits.TMR1IF = 0;      // Clear Timer1 overflow flag
    
}

//EOF-----------------------------------------------------------------
