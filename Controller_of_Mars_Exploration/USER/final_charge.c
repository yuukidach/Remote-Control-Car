#include "final_charge.h"

#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

uint32_t last_sonic, now_sonic;
float last_yaw, now_yaw;
uint8_t inner_turn, outer_turn;

void dirDef(uint8_t dir) {
    if (dir) {
        inner_turn = TURNRIGHT;
        outer_turn = TURNLEFT;
    } else {
        inner_turn = TURNLEFT;
        outer_turn = TURNRIGHT;
    }
}


void turnInner(void) {
    while(getYaw(&last_yaw));
    setSpeed(inner_turn, TURN_PWM, TURN_PWM);
    now_yaw = 0;
    while (now_yaw < 85) {
        while(getYaw(&now_yaw));
        now_yaw = now_yaw > last_yaw ? now_yaw-last_yaw : last_yaw - now_yaw;

#if (__DEBUG__ == __ON__)
        printf("In turnInner()\n");
#endif
        
    } 
    stopTheCar();
}


void turnOuter(void) {
    while(getYaw(&last_yaw));
    setSpeed(outer_turn, TURN_PWM, TURN_PWM);
    now_yaw = 0;
    while (now_yaw < 85) {
        while(getYaw(&now_yaw));
        now_yaw = now_yaw > last_yaw ? now_yaw-last_yaw : last_yaw - now_yaw;
    } 
    stopTheCar();
}


void upwardSlope(void) {             
    setSpeed(FORWARDS, 60, 60);
    delay_ms(850);
    stopTheCar();
    now_sonic = Ten_Times_Trig(MIDDLE_TRIGGER);
    while (now_sonic < 4000 && now_sonic > 4100) {
        if (now_sonic < 4000) {
            setSpeed(BACKWARDS, 15, 15);
            delay_ms(10);
            now_sonic = Ten_Times_Trig(MIDDLE_TRIGGER);
        } else if (now_sonic > 4100) {
            setSpeed(FORWARDS, 15, 15);
            delay_ms(10);
            now_sonic = Ten_Times_Trig(MIDDLE_TRIGGER);
        }
    }
}


void Final_Charge (uint8_t _direction) { 
    dirDef(_direction);
    
    turnInner();

    setSpeed(FORWARDS, 22, 22);
    delay_ms(700);
    stopTheCar();
    
#if (__DEBUG__ == __ON__)
    printf("Forward 1 was over!\n");
#endif
    
    turnOuter();
    
    do {
        setSpeed(FORWARDS, 22, 22);
    } while (Ten_Times_Trig(MIDDLE_TRIGGER) > 2000);
    
    turnOuter();
    
    upwardSlope();
    
    stopTheCar();
    
    while(1);
}
