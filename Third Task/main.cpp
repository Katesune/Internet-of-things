#include "mbed.h"
#include <string>
#include <cstdio>
//#include "easy-connect.h"
// #include "MQTTNetwork.h"
// #include "MQTTmbed.h"
// #include "MQTTClient.h"

#define TX PA_9
#define RX PA_10

#define PHOTOR PA_6
#define FIRSTPORT PB_11
#define SECONDPORT PB_10

#define BAUDRATE 9600

static AnalogIn photor(PHOTOR);
static AnalogIn firstport(FIRSTPORT);
static AnalogIn secondport(SECONDPORT);

static BufferedSerial uart(TX, RX, BAUDRATE);

FileHandle *mbed::mbed_override_console(int fd) {
    return &uart;
}  

// void send(const char *ch) {
//     while ()
// }

// main() runs in its own thread in the OS
int main()
{
    int count = 0;
    int gray_mass1[4];
    int gray_mass2[4];

    while (true) {

        if (count!=4) {
            gray_mass1[count] = firstport.read_u16();
            gray_mass2[count] = secondport.read_u16();
            count++;
        }


        if (count==4) {
            float encoder_value1 = 0;
            float encoder_value2 = 0;
            for (int i=0; i<4; i++) {
                encoder_value1 += gray_mass1[i] * pow(2,count);
                encoder_value2 += gray_mass2[i] * pow(2,count);

            }
            encoder_value1 *= 45;
            encoder_value2 *= 45;

            int encoder_value = (int)((encoder_value+encoder_value1)/2);
            if (encoder_value1>encoder_value2) encoder_value *=-1;

            float photor_value = 5*( (pow(10, 4) ) / ( photor.read()+ pow(10, 4) ) );
            photor_value = photor_value*10/10;

            printf("Ph %f Enk %d", photor_value, encoder_value);
        }

        //encoder_value = pow(2, encoder_value);


        // printf("%d\  %d\n", photor.read_u16());
        // printf("%d\  %d\n", photor_value);
        // printf("%d  %d\n", firstport.read_u16(), secondport.read_u16());
    
        //printf("%d\  %d\n" ,photor_value);
        //printf("%d\  %d\nЗначение энкодера - " ,encoder_value);

        // string answer(
        //     "Значение фоторезистора - " + to_string(photor_value) + "/n" +
        //     "Значение энкодера - " + to_string(encoder_value) + "/n"
        //     ); 

        ThisThread::sleep_for(5ms);
    }
}

