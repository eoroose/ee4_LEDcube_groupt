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
#include "config.h"

/** D E F I N E S ***************************************************/

/** P U B L I C   V A R I A B L E S *********************************/
// in order for the variable to be used in other file, it also has to
// be declared as 'extern' in the config.h file
unsigned char led1_output;

/** P R I V A T E   V A R I A B L E S *******************************/

/** P R I V A T E   P R O T O T Y P E S *****************************/
static void init(void);
static void OpenTimer0(unsigned char intEnable);
static unsigned char timed_to_1ms(void);

/********************************************************************/
/** P U B L I C   D E C L A R A T I O N S ***************************/
/********************************************************************
 * Function:        void main(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Main program entry point
 ********************************************************************/
void main(void) {
	init();						//initialize the system
	while(timed_to_1ms()) {
         
        //**** put here a reference to one or more FSM's
        fsm_cube();
	}
}

/********************************************************************/
/** P R I V A T E   D E C L A R A T I O N S *************************/
/********************************************************************
 * Function:        void init(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Overview:        Init is a centralize initialization routine.              
 *                  It will initialze all IO and variables, also the
 *                  hardware_init() for the hardware functions should
 *                  be called here.              
 ********************************************************************/
static void init(void) {
    LATA = 0x00;                // Reset all IO
    LATB = 0x00;
    LATC = 0x00;
    OpenTimer0(FALSE);          // start timer for 1ms loop
    OSCCONbits.IRCF = 0b111;    // put internal clock in 7th gear
    
	// Configure I/O as input (1) or output (0)
    ANSELCbits.ANSC7 = 0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC2 = 0;
    TRISAbits.TRISA0 = 0;       // RA0 first analog input or digital IO
    TRISAbits.TRISA1 = 0;       // RA1 second analog input or digital IO
    TRISAbits.TRISA2 = 0;       // RA2 third analog input or digital IO
    TRISAbits.TRISA3 = 0;       // RA3 fourth analog input or digital IO
    TRISAbits.TRISA4 = 0;       // RA4 digital output only
    TRISAbits.TRISA5 = 0;       // RA5 fifth analog input or digital IO
    
    TRISBbits.TRISB0 = 1;       // IO expander CS
    TRISBbits.TRISB1 = 1;       // IO expander clock
    TRISBbits.TRISB2 = 0;       // ...
    TRISBbits.TRISB3 = 0;       // ...
    TRISBbits.TRISB4 = 0;       // ...
    TRISBbits.TRISB5 = 0;       // ...
    TRISBbits.TRISB6 = 0;       // ...
    TRISBbits.TRISB7 = 0;       // ...
    
    TRISCbits.TRISC0 = 1;       // Pushbutton on the µC PCB
    TRISCbits.TRISC1 = 1;       // LED2 on the µC PCB
    TRISCbits.TRISC2 = 1;       // LED1 on the µC PCB
    TRISCbits.TRISC6 = 1;       // ...
    TRISCbits.TRISC7 = 1;       // IO expander data out
    
    //PWM_duty[0] = 0;
    fsm_cube_init();
       
#if ENABLE_SERVO == TRUE
    SERVO_init();
#endif
#if ENABLE_ADC == TRUE
    ADC_init();
#endif
#if ENABLE_IOEXP == TRUE
    IOEXP_init();
#endif
#if ENABLE_AUDIO == TRUE
    AUDIO_init();
#endif  
}

/********************************************************************
 * Function:        void OpenTimer0(unsigned char intEnable)
 * PreCondition:    None
 * Input:           intEnable: enable Timer0 interrupt
 * Output:          None
 * Overview:        Will initialize Timer0 given the parameters
 ********************************************************************/
static void OpenTimer0(unsigned char intEnable) {
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b011;
    TMR0H = 0;                // Reset Timer0 to 0x0000
    TMR0L = 7;
    
    INTCONbits.TMR0IE = intEnable & 0x01u;
    INTCONbits.GIE = (intEnable & 0x01u) | INTCONbits.GIE;
    INTCONbits.TMR0IF = 0;      // Clear Timer0 overflow flag
}

/********************************************************************
 * Function:        void timed_to_1ms(void)
 * PreCondition:    OpenTimer0(FALSE)
 * Input:           None
 * Output:          None
 * Overview:        Stays in a while loop, doing NOP until Timer0 
 *                  generates an overflow
 ********************************************************************/
static unsigned char timed_to_1ms(void) {
	while(!INTCONbits.TMR0IF){Nop();};
    TMR0L = 7;     // tuned to 1ms
	INTCONbits.TMR0IF = 0;
    return 1;
}

/********************************************************************
 * Function:        void interrupt_handler(void)
 * PreCondition:    GIE need to be set
 * Input:           None
 * Output:          None
 * Overview:        This routine is called whenever an interrupt 
 *                  condition is  reached
 ********************************************************************/	
void interrupt interrupt_handler(void) {
#if ENABLE_AUDIO == TRUE
    AUDIO_ISR();
#endif
#if ENABLE_SERVO == TRUE
    SERVO_ISR();
#endif
#if ENABLE_ADC == TRUE
    ADC_ISR();
#endif
}
//EOF-------------------------------------------------------------------------
