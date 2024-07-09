#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI

#define SERVO_N SERVO0
bool_t goesForward = false; //Variable global de control para saber si el auto esta yendo para adelante

int main( void )
{
    boardConfig();
    uartConfig(UART_USB,115200);
    
    //CONFIG SENSOR
    ultrasonicSensorConfig(ULTRASONIC_SENSOR_0, ULTRASONIC_SENSOR_ENABLE);
    delay(100);
    
    //CONFIG SERVO
    bool_t value = 0;
    uint8_t servoAngle = 115;
    value = servoConfig(0, SERVO_ENABLE);
    value = servoConfig(SERVO_N, SERVO_ENABLE_OUTPUT);
    value = servoWrite(SERVO_N, servoAngle);
    servoAngle = servoRead(SERVO_N);
    delay(100);
    
    //CONFIG MOTORES
    gpioConfig(T_COL0,GPIO_OUTPUT); //Motor1 Front Left
    gpioConfig(GPIO1,GPIO_OUTPUT);
    gpioConfig(GPIO8,GPIO_OUTPUT); //Motor2 Front Right
    gpioConfig(GPIO3,GPIO_OUTPUT);
    //GPIO0 y GPIO2 estan en uso por el sensor, por lo cual usamos en su lugar T_COL0 y GPIO8 respectivamente
    
    // Hacemos varias lecturas iniciales con el sensor para tener un valor exacto de distancia inicial
    uint32_t distance;
    
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    
    while(TRUE){
        printf("Distance: %d cm.\r\n", distance);
        gpioToggle(LEDR); //Para chequear que funciona
         
        if(distance <= 15){
            moveStop();
            delay(100);
            moveBackward();
            turnRight();
        }else{
            moveForward();
        }

        distance = readPing();
    };
    
    return 0;
}

int readPing(){ 
    delay(70);
    int cm = ultrasonicSensorGetDistance(ULTRASONIC_SENSOR_0, CM);
    if(cm == 0){
        cm = 250;
    }
    return cm;
}

void moveStop(){
    gpioWrite(T_COL0, false);
    gpioWrite(GPIO1, false); 
    gpioWrite(GPIO8, false);
    gpioWrite(GPIO3, false); 
    goesForward = false;
  } 
  
void moveForward(){
    if(!goesForward){
        goesForward=true;
        gpioWrite(T_COL0, true);
        gpioWrite(GPIO1, false); 
        gpioWrite(GPIO8, true);
        gpioWrite(GPIO3, false); 
    }
}

void moveBackward(){
    goesForward=false;
    gpioWrite(T_COL0, false);
    gpioWrite(GPIO1, true); 
    gpioWrite(GPIO8, false);
    gpioWrite(GPIO3, true); 
}  

void turnRight(){
    goesForward = false;
    gpioWrite(T_COL0, true);
    gpioWrite(GPIO1, false);  
    gpioWrite(GPIO8, false);
    gpioWrite(GPIO3, true); 
    delay(500);
    moveStop();
    moveForward();          
} 
