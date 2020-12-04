#pragma once
#include <wiringPi.h>
#include <atomic>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <chrono>

/*
    This class serves as the manager for the LED and switch listener. When initialized it will manage the LED and listen for switch states 
    on a detached thread.

    States: 
        WAITGPSFIX  -   indicates system is looking for gps signal
        GPSFIX      -   system has a fixed gps signal / is idle / standby
        RECORDING   -   (post gps fix) system is recording    
        SHUTDOWN    -   indicate operational safe shutdown 
*/

namespace LedSwitch{
    enum STATE {OFF, WAITGPSFIX, GPSFIX, RECORDING, SHUTDOWN};
    constexpr short GPIO_POWER_BUTTON = 5; 
    constexpr short GPIO_LED_PIN = 1; 
    constexpr short POWER_OFF_SECONDS = 3;


    class Manager{
       
        public:
            static Manager* getInstance();
            static void shutdownButtonInterrupt();
            bool isRecording(){ return recording_;}
            void setState(STATE s){Manager::getInstance()->state_ = s;}

        private:
            static Manager* instance;
            Manager();
            void threadRoutine();
            void shutdown();
            void blinkWaitForFix();
            void blinkHasFix();
            void blinkStandbyRecord();
            void blinkIsRecording();
            STATE state_ = LedSwitch::STATE::WAITGPSFIX;
            std::atomic<bool> shutdown_;
            std::thread worker_;
            std::atomic<bool> recording_;
    };
};