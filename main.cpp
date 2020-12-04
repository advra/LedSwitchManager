#include "ledswitchmanager.hpp"
#include <stdio.h>

int main( int argc, char** argv )
{
    LedSwitch::Manager::getInstance()->setState(LedSwitch::WAITGPSFIX);
}