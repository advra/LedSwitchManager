#include "ledswitchmanager.hpp"
#include <unistd.h>

int main( int argc, char** argv )
{
    // Changes states of the LED
    LedSwitch::Manager::getInstance()->setState(LedSwitch::WAITGPSFIX);
    sleep(5);
    LedSwitch::Manager::getInstance()->setState(LedSwitch::GPSFIX);
    sleep(5);
    LedSwitch::Manager::getInstance()->setState(LedSwitch::RECORDING);
    sleep(5);
    LedSwitch::Manager::getInstance()->setState(LedSwitch::OFF);
    while(1){ /* wait forever until power shutdown */ }
}