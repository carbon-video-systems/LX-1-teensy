/* Debug Source */

/* Includes-------------------------------------------------------------*/
#include "calibration.h"
#include "debug.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
void Debug::serviceDebug()
{
    char command = SerialUSB.read();

    switch(command){
        case 's':
            SerialUSB.println("Saving configuration");
            odrive_.SaveConfiguration();
            delay(100);
            break;
        case 'e':
            SerialUSB.println("Erasing configuration");
            odrive_.EraseConfiguration();
            delay(100);
            break;
        case 'r':
            SerialUSB.println("Rebooting");
            odrive_.Reboot();
            delay(1000);
            odrive_startup_sequence(odrive_);
            break;
        case '\n':
            break;
        case '\r':
            break;
        default:
            SerialUSB.println("Invalid command please try again");
            break;
    }
}
