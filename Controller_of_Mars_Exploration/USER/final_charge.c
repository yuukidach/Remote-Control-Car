#include "final_charge.h"

uint32_t last_time, now ;
uint32_t inner_trigger, middle_trigger = GPIO_Pin_7, outer_trigger;

/** @breif Define innner and outer pins
  * @param dir: Direction at part 3.
  * @retval None
  */
void pinsDef(uint8_t dir) {
    if (dir) {         // Turn right at part 3
        inner_trigger = GPIO_Pin_8 ;
        outer_trigger = GPIO_Pin_9 ;
    } else {                   // Turn left at part 3
        inner_trigger = GPIO_Pin_9 ; 
        outer_trigger = GPIO_Pin_8 ;
    }
}


/** @breif Turn a bit to make sure the car is facing to the slope.
  * @param dir: Direction at part 3.
  * @retval None
  */
void correctDir(uint8_t dir) {
    uint32_t tryLeft = 0;
    
    last_time = Ten_Times_Trig (middle_trigger);
    
    if (dir) 
        setSpeed (TURNRIGHT, TURN_PWM, TURN_PWM);                             // Deside whether to turn left or turn right.
    else 
        setSpeed (TURNLEFT , TURN_PWM, TURN_PWM);
    delay_ms (INTERVAL << 1);
    tryLeft = Ten_Times_Trig(middle_trigger);               // Insure the data is right.
    tryLeft = Ten_Times_Trig(middle_trigger);
    tryLeft = Ten_Times_Trig(middle_trigger);
    if (tryLeft < last_time) dir = 1;
    else dir = 0;
    
    do {
        if (dir) 
            setSpeed (TURNRIGHT, TURN_PWM, TURN_PWM);
        else
            setSpeed (TURNLEFT, TURN_PWM, TURN_PWM);
        delay_ms ( INTERVAL ) ;
        stopTheCar();
        now = Ten_Times_Trig (inner_trigger) ;
        
#if (__DEBUG__ == __ON__)
        printf ( "Correct Direction : now = %u\r\n" , now ) ;
        printf ( "Correct Direction : late_time = %u\r\n" , now ) ;
#endif
        
        if (last_time < now + 2000) last_time = now;
    } while ( last_time == now) ;
}


void turnInner(uint8_t dir) {
    do {
        if (dir)
            setSpeed (TURNLEFT , 20, 20);
        else
            setSpeed (TURNRIGHT, 20, 20);
        delay_ms(INNER_TIME);
        //stopTheCar();
        now = Ten_Times_Trig (middle_trigger) ;

#if (__DEBUG__ == __ON__)
        printf ( "Turn inner : now = %u\r\n" , now ) ;
#endif 
        
    } while (now < INNER_DIS) ;                         // Make sure the car has turned over the slope.
    if (dir)
            setSpeed (TURNRIGHT, 20, 20);
        else
            setSpeed (TURNLEFT , 20, 20);
        delay_ms(INNER_TIME);
}


void forwardToSlope(void) {
    setSpeed (FORWARDS, 15, 15);
    delay_ms (FORWARD_I_TIME);
    //stopTheCar();
    do {
        setSpeed (FORWARDS, 10, 10);
        delay_ms ( INTERVAL ) ;
        stopTheCar();
        now = Ten_Times_Trig (outer_trigger);
    
#if (__DEBUG__ == __ON__)
        printf ( "Forward I : now = %u\r\n" , now ) ;
#endif 
        
    } while (now > FORWARD_I_DIS);
}


void turnOuter(uint8_t dir) {
    if (dir)
        setSpeed (TURNRIGHT, TURN_PWM, TURN_PWM) ;
    else
        setSpeed (TURNLEFT , TURN_PWM, TURN_PWM) ;
    delay_ms (OUTER_TIME_I);
    //stopTheCar ( ) ;
    last_time = Ten_Times_Trig (inner_trigger) ;
    do {
        if (dir)
            setSpeed (TURNRIGHT, TURN_PWM, TURN_PWM);
        else
            setSpeed (TURNLEFT , TURN_PWM, TURN_PWM);
        delay_ms ( INTERVAL ) ;
        //stopTheCar () ;
        now = Ten_Times_Trig (inner_trigger);
    
#if (__DEBUG__ == __ON__)
        printf ( "Turn Outer : now = %u\r\n" , now );
        printf ("Turn Outer : last_time = %u\r\n", last_time);
#endif
        
        if (last_time > now + 1000 || last_time < now + 1000) last_time = now;
        if (now > 65530) break;
    } while ( last_time == now ) ;
    if (dir)
        setSpeed (TURNRIGHT, TURN_PWM, TURN_PWM) ;
    else
        setSpeed (TURNLEFT , TURN_PWM, TURN_PWM) ;
    delay_ms (INTERVAL+10);
}


void upwardSlope(void) {
    setSpeed (FORWARDS, 40, 40);
    delay_ms (FORWARD_II_TIME) ;
    setSpeed (FORWARDS, 10, 10);
    delay_ms (300);
    stopTheCar();
    now = Ten_Times_Trig (middle_trigger);
    if (now > FORWARD_II_DIS) {
        do {
            setSpeed(FORWARDS, 4, 4);
            delay_ms(INTERVAL - 10);
            stopTheCar();
            now = Ten_Times_Trig(middle_trigger);   
        } while (now > FORWARD_II_DIS);
    } else if (now < FORWARD_II_DIS) {
        do {
            setSpeed(BACKWARDS, 4, 4);
            delay_ms(INTERVAL - 10);
            stopTheCar();
            now = Ten_Times_Trig(middle_trigger);   
        } while (now < FORWARD_II_DIS);
    }
    
}


void Final_Charge (uint8_t _direction) {
    // Define innner and outer pins
    pinsDef(_direction);
    
    turnInner(_direction);
    forwardToSlope();
    turnOuter(_direction);
    //correctDir(_direction);
    upwardSlope();
    
    while(1);
}
