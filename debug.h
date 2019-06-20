/* Debug Header */

#ifndef DEBUG_H
#define DEBUG_H

/* Includes-------------------------------------------------------------*/
#include "ODriveLib.h"
#include "options.h"

/* Constants -----------------------------------------------------------*/

/* Functions------------------------------------------------------------*/
class Debug {
public:
    Debug(ODriveClass& odrive) : odrive_(odrive) {}

    void serviceDebug();

private:
    ODriveClass& odrive_;
};

#endif //DEBUG_H
