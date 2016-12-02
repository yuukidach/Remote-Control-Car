/**
  ******************************************************************************
  * @author        Yuuki_Dach
  * @version       V1.1.0
  * @date          02-December-2016
  * @description   Functions to finish part3
  ******************************************************************************
  * @attention
  *
  * THIS IS THE FINAL PART OF OUR COMPETITION. ALL WE NEED TO DO IS GOING UP A 
  * SLOPE WHICH HAS A INCLINATION OF 30 DEGREE.
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */

#include "final_charge.h"

#include "mechanical_arm.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

uint32_t last_sonic, now_sonic;
float last_yaw, now_yaw;
uint8_t inner_turn, outer_turn, dir_at_part3;

void dirDef(uint8_t dir) {
    if (dir) {
        inner_turn = TURNLEFT;
        outer_turn = TURNRIGHT;
    } else {
        inner_turn = TURNRIGHT;
        outer_turn = TURNLEFT;
    }
}


/**
  * @brief Calibrate the direction after the part 2 was ended.
  */
void calibrateDir(float forward_yaw) {
    stopTheCar();
    float part2yaw;
    while (getYaw(&part2yaw));
    part2yaw -= forward_yaw;
    if (part2yaw > 0) {
        setSpeed(TURNRIGHT, TURN_PWM, TURN_PWM);
        while (part2yaw > 2) {
            getYaw(&part2yaw);
            part2yaw -= forward_yaw;
        }
    } else if (part2yaw < 0) {
        setSpeed(TURNLEFT , TURN_PWM, TURN_PWM);
        while (part2yaw < -2) {
            getYaw(&part2yaw);
            part2yaw -= forward_yaw;
        }
    }
    stopTheCar();
    
    uint32_t dis_from_slop = multiTrig(MIDDLE_TRIGGER);
    while (dis_from_slop > 350) {
        setSpeed(FORWARDS, 30, 30);
        dis_from_slop = multiTrig(MIDDLE_TRIGGER);
    }
    //delay_ms(200);
    stopTheCar();

#if (__DEBUG__ == __ON__)
    printf("Now, go straight end.\r\n");
#endif 

}


void getDirRight(float forward_yaw) {
    while(getYaw(&now_yaw));
    setSpeed(inner_turn, TURN_PWM, TURN_PWM);
    if (now_yaw < forward_yaw) {
        while (forward_yaw - now_yaw > -87)
            getYaw(&now_yaw);
    } else if (now_yaw > forward_yaw) {
        while (forward_yaw - now_yaw < 87)
            getYaw(&now_yaw);
    }
    stopTheCar();
}


void turnInner(float forward_yaw) {
    while(getYaw(&now_yaw));
    setSpeed(inner_turn, TURN_PWM, TURN_PWM);
    now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    while (now_yaw < 82) {
        while(getYaw(&now_yaw));
        now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    } 
    stopTheCar();
    
#if (__DEBUG__ == __ON__)
    printf("turnInner end part 1. \r\n");
#endif 
    
    uint32_t dir_after_turn = multiTrig(MIDDLE_TRIGGER);            
    if (dir_after_turn < 3300) {                                    // If the car turn to a wrong direction
        dir_at_part3 = 1 - dir_at_part3;
        dirDef(dir_at_part3);
        getDirRight(forward_yaw);                                   // Try to get the car into right direction 
    
#if (__DEBUG__ == __ON__)
    printf("End getDirRight(). \r\n");
#endif 
        
    }
    
    stopTheCar();
}


void turnOuter1(float forward_yaw) {
    while (getYaw(&now_yaw));
    setSpeed(outer_turn, TURN_PWM, TURN_PWM);
    now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    while (now_yaw > 2) {
        while(getYaw(&now_yaw));
        now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    } 
    stopTheCar();
}

void turnOuter2(float forward_yaw) {
    while (getYaw(&now_yaw));
    setSpeed(outer_turn, TURN_PWM, TURN_PWM);
    now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    while (now_yaw < 82) {
        while(getYaw(&now_yaw));
        now_yaw = now_yaw > forward_yaw ? now_yaw - forward_yaw : forward_yaw - now_yaw;
    } 
    stopTheCar();
}


void upwardSlope(void) {             
    setSpeed(FORWARDS, 80, 80);
    delay_ms(1000);
    stopTheCar();
    now_sonic = multiTrig(MIDDLE_TRIGGER);
    
    if (now_sonic > 65530) now_sonic = clrSonic();

#if (__DEBUG__ == __ON__)
    printf("sonic: %5d\r\n", now_sonic);
#endif
    
    while (now_sonic < 900 || now_sonic > 1300) {
        if (now_sonic < 900) {
            setSpeed(BACKWARDS, 40, 40);
            delay_ms(10);
            now_sonic = multiTrig(MIDDLE_TRIGGER);
            if (now_sonic > 65530) now_sonic = clrSonic();
            
#if (__DEBUG__ == __ON__)
            printf("sonic: %5d\r\n", now_sonic);
#endif
            
        } else if (now_sonic > 1300) {
            setSpeed(FORWARDS, 40, 40);
            delay_ms(10);
            now_sonic = multiTrig(MIDDLE_TRIGGER);
            if (now_sonic > 65530) now_sonic = clrSonic();

#if (__DEBUG__ == __ON__)
            printf("sonic: %5d\r\n", now_sonic);
#endif
            
        }
    }
}


uint32_t clrSonic(void) {
    stopTheCar();
    delay_ms(20);
    return multiTrig(MIDDLE_TRIGGER);
}


void finishPart3(uint8_t _dir, float forward_yaw) { 
    dir_at_part3 = _dir;
    dirDef(dir_at_part3);
    calibrateDir(forward_yaw);
    turnInner(forward_yaw);

    setSpeed(FORWARDS, 90, 90);
    delay_ms(410);
    stopTheCar();
    
#if (__DEBUG__ == __ON__)
    printf("Forward 1 was over!\n");
#endif
    
    turnOuter1(forward_yaw);
    
    setSpeed(FORWARDS, 30, 30);
    uint32_t tmp_dis = multiTrig(MIDDLE_TRIGGER);
    while (tmp_dis > 360) {
        tmp_dis = multiTrig(MIDDLE_TRIGGER);
        if (tmp_dis > 65530) tmp_dis = clrSonic();
        
#if (__DEBUG__ == __ON__)
        printf("Sonic F: %5d", tmp_dis);
        printf("Now the car is dashing to the slope. \r\n");
#endif
        
    }
    stopTheCar();
    
    turnOuter2(forward_yaw);   
    upwardSlope();    
    stopTheCar();
    putBallDown();
    while(1);
}

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/


