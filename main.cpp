#include "mbed.h"
#include "hcsr04.h"
#include <cstdio>
//#include "rtos.h"



//using namespace std;

HCSR04 sens1(D3, D4); // Trigger(DO), Echo(PWMIN)
HCSR04 sens2(D5, D6); // Trigger(DO), Echo(PWMIN)
UnbufferedSerial pc(USBTX, USBRX, 9600);

typedef struct sensor_data_{
    int ind_num;
    int distance;
} sensor_data; 

void send_data(HCSR04 *sensor, int num);

int main(){
    pc.format(8, BufferedSerial::None, 1);
    Thread th1,th2;
    th1.start([=] { send_data(&sens1, 1); });
    th2.start([=] { send_data(&sens2, 2); });
    th1.join();
    th2.join();
    return 0;
}

void send_data(HCSR04 *sensor, int num){
    int dist;
    sensor_data data;
    data.ind_num = num;
    sensor->start();
    
    while(true){
        dist = sensor->get_dist_cm();
        data.distance = dist;
        printf("%d %d\n", data.ind_num,dist);
        // pc.write(&dist, 1);
        // pc.write("\n", 1);
        ThisThread::sleep_for(200ms);
    }
}