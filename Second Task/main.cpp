#include "mbed.h"
#include "Timer.h"

#define LED1 PA_5
#define LED2 PA_6
#define BUTT1 PB_6
#define BUTT2 PB_6
//#define LIGHT_ON_FLAG (1U << 0)
//#define LIGHT_OFF_FLAG (1U << 1)

using namespace std::chrono;

static DigitalOut led1(LED1);
static DigitalOut led2(LED2);
static DigitalIn butt1(BUTT1);
static DigitalIn butt2(BUTT2);

//static EventFlags eventFlags;
//static Thread thread;
//static InterruptIn mbutt1(BUTTON1);

Timer t;
bool mode = 1;

void light(int part_time, int ost_time){
    if (ost_time == 0) {

        if (part_time > 0) led1.write(mode);
        else led2.write(mode);
    }
}

void lightoff(int part_time, int ost_time) {
    if (ost_time == 0) {
        if (part_time > 0) led1.write(!mode);
        else led2.write(!mode);
    }
}

// main() runs in its own thread in the OS
int main()
{
    t.start();
    
    while (true) {
        t.reset();
    
        int part_time = duration_cast<milliseconds>(t.elapsed_time()).count() / 120 % 2;
        int ost_time = duration_cast<milliseconds>(t.elapsed_time()).count() % 120;

        if (butt1.read()) light(part_time, ost_time);
        if (butt2.read()) lightoff(part_time, ost_time);

        printf("The time taken was %llu milliseconds\n", duration_cast<milliseconds>(t.elapsed_time()).count());
    }
}

