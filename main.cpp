/*
 */


#include <avr/io.h>
#include <util/delay.h>

#include "vectors.h"
#include "animation.h"
#include "serial.h"
#include "taskmanager.h"
#include "utils.h"
#include "config.h"
#include "gamma.h"
#include "battery.h"
#include "implementation.h"
#include "arduinowrapper.h"
#include "XBee.h"


uart xbeeSerial(&USARTD0, 57600);
ISR (USARTD0_RXC_vect){ xbeeSerial.rxInterrupt(); }
ISR (USARTD0_DRE_vect){ xbeeSerial.txInterrupt(); }

uart debug(&USARTC1, 9600);
ISR (USARTC1_RXC_vect){ debug.rxInterrupt(); }
ISR (USARTC1_DRE_vect){ debug.txInterrupt(); }



void test_debug();

void gammaCorrection(led& currentLed);
void processData(char* data, uint8_t len);
void xbee_api_callback(ZBRxResponse rx);


vec vectors[NUMBER_OF_VECTORS];
bool runAnimation ;

uint32_t nextFrame;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();

int main(void)
{
    // Set sys clock to 16 Mhz
    // -if you change this, remember to change F_CUP define too
    set32MHzClock(CLK_PSADIV_2_gc);

    //Enable all interrupts
    PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	sei();


    userImplementationSetup();

    batteryManager::initialize();
    batteryManager::highCurrentCharging(false);
    taskManager::initialize();
    taskManager::registerTask(batteryManager::readLevel, 100, 0);
    //taskManager::registerTask(&test_debug, 100, 0);


    debug.sendStringPgm(PSTR("Partyhat version 0.1 \n"));

    Stream xbeeStream(xbeeSerial);

    PORTD.DIRSET = PIN4_bm;
    PORTD.OUTSET = PIN4_bm;
    _delay_ms(20);
    PORTD.OUTCLR = PIN4_bm;

    xbee.begin(xbeeStream);

    nextFrame = taskManager::getTimeMs() ;
    selectAnimation(1);
    runAnimation = true;

    char data[64];
    uint8_t dataLength = 0;
    uint8_t recivedBytes = 0;

    while(1){
        xbee.readPacket();
        if (xbee.getResponse().isAvailable())
        {
            // got something
            if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
            {
                // got a zb rx packet
                xbee.getResponse().getZBRxResponse(rx);
                xbee_api_callback(rx);
            }
        }


        // Echo debug characters
        if(debug.dataAvailable())
        {
            char c = debug.getChar();

            if(dataLength == 0) dataLength = c;
            else
            {
                data[recivedBytes] = c;
                recivedBytes++;
            }

            if(dataLength == recivedBytes)
            {
                processData(data, dataLength);
                dataLength = 0;
                recivedBytes = 0;
            }

        }



        if( taskManager::getTimeMs() >= nextFrame)
        {
            uint32_t time = taskManager::getTimeMs();
            nextFrame = time + 20;

            if(runAnimation) updateAnimation(time);

            for (int ledIndex = 0; ledIndex < number_of_leds; ledIndex++)
            {
                updateLed(leds[ledIndex], vectors, NUMBER_OF_VECTORS);
                gammaCorrection(leds[ledIndex]);
            }

            userImplementationUpdate();
        }



        /// main loop:
        /// -get data from xbee
        /// -prosess data
        /// -update vect"ors in animation handler    DONE
        /// -get rgb values from vectors            DONE
        /// -gamma correction + ect.                DONE
        /// -set rgb values to all leds             REFERENCE IMPLEMENTATION DONE

        /// Backgound tasks:
        /// -batterymonitor                         DONE

    }

    return 0;
}


void test_debug()
{

    debug.sendInt(batteryManager::getBatteryLevel());
    debug.sendString(" mV\n\r");

}


void gammaCorrection(led& currentLed)
{
        currentLed.r = gamma256to256(currentLed.r);
        currentLed.g = gamma256to256(currentLed.g);
        currentLed.b = gamma256to256(currentLed.b);
}


void processData(char* data, uint8_t len)
{

    //No data, shouldn't happen
    if(!len) return;

    switch (data[0])
    {

    //echo
    case 0x00:
        for (uint8_t i = 1; i < len; i++)
            debug.sendChar(data[i]);
        break;

    //Set animation
    case 0x01:
        selectAnimation(data[1]);
        break;

    //set vectors manualy
    case 0x02:

        break;


    case 0xFF:
        userImplementationCommunication(data + 1, len - 1);
        break;

    default:
        break;
    }

}


void xbee_api_callback(ZBRxResponse rx)
{
    // Check first byte
    debug.sendHex(rx.getData(0));
    switch(rx.getData(0))
    {
        // TODO: Define a sane baseline protocol, for testing we have this dummy RGB setter
        case 0x0:
        {
            TCC0.CCA = rx.getData(3);
            TCC0.CCB = rx.getData(2);
            TCC0.CCC = rx.getData(1);

            debug.sendHex(rx.getData(1));
            debug.sendHex(rx.getData(2));
            debug.sendHex(rx.getData(3));

            break;
        }
        case 0x1:
        {

            TCC0.CCD = rx.getData(3);
            TCC1.CCA = rx.getData(1);
            TCC1.CCB = rx.getData(2);
            break;
        }
        case 0x2:
        {
            selectAnimation(rx.getData(1));
            break;
        }
        case 0x3:
        {
            runAnimation = true;
            break;
        }
        case 0x4:
        {
            runAnimation = false;
            break;
        }
        case 0x5:
        {
            selectAnimation(rx.getData(1));
            runAnimation = true;
            break;
        }
        case 0x58: // Ascii X
        {
            // Your extended protocol goes here
            break;
        }
    }
}
