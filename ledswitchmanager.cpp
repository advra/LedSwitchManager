#include "ledswitchmanager.hpp"

namespace LedSwitch{

    // init static default value
    Manager* Manager::instance = 0;

    Manager::Manager(){
        std::cout << "Manager::Manager()" << std::endl;
        shutdown_ = false;
        recording_ = false;
        state_ = LedSwitch::STATE::WAITGPSFIX;

        wiringPiSetup () ;
        pinMode(GPIO_LED_PIN, OUTPUT);
        pinMode(GPIO_POWER_BUTTON, INPUT);
        pullUpDnControl(GPIO_POWER_BUTTON, PUD_UP);
        wiringPiISR(GPIO_POWER_BUTTON, INT_EDGE_FALLING, LedSwitch::Manager::shutdownButtonInterrupt); 
    }

    Manager* Manager::getInstance(){
        if (instance == 0){
	    std::cout << "Initializing Manager for first time" << std::endl;
            instance = new Manager();
            std::cout << "Init worker thread" << std::endl;
            instance->worker_ = std::thread(&LedSwitch::Manager::threadRoutine, instance);
            instance->worker_.detach();
        }
        return instance;
    }

    void Manager::shutdown(){
        Manager::getInstance()->shutdown_ = true;
        Manager::getInstance()->recording_ = false;
    }

    void Manager::shutdownButtonInterrupt(){
        std::cout << "Manager::shutdownInterrupt()" << std::endl;
        LedSwitch::STATE previousState =  Manager::getInstance()->state_;
        int64_t timer;
        int64_t interruptTime;
        bool cancel_ = false;

        while(1){
            if(digitalRead(GPIO_POWER_BUTTON) == LOW){
                previousState =  Manager::getInstance()->state_;
                Manager::getInstance()->setState(LedSwitch::SHUTDOWN);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                std::cout << "Push detected" << std::endl;
                // If interrupts come faster than 200ms, assume it's a bounce and ignore
                timer =  std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + std::chrono::seconds(POWER_OFF_SECONDS).count();
                while(digitalRead(GPIO_POWER_BUTTON) == LOW){
                    std::cout << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
                    if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() >= timer){
                        if(digitalRead(GPIO_POWER_BUTTON) == LOW){
                            // poweroff
                            std::cout << "Powering Off!" << std::endl;
                            digitalWrite (GPIO_LED_PIN,  LOW);
                            Manager::getInstance()->shutdown_ = true;
                            Manager::getInstance()->recording_ = false; // perhaps have some checks to make sure consumer queues are empty?
                            system("sudo poweroff");
                        }
                    }
                }
                std::cout << "Released" << std::endl;
                Manager::getInstance()->setState(previousState);
            }
        }
    }

    void Manager::threadRoutine(){

        while(!shutdown_){
            
            if(state_ == LedSwitch::STATE::WAITGPSFIX){
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(250);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(1250);
            }else if(state_ == LedSwitch::STATE::GPSFIX){
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(250);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(250);
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(250);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(750);
            }else if(state_ == LedSwitch::STATE::RECORDING){
                digitalWrite (GPIO_LED_PIN,  HIGH);
            }else if(state_ == LedSwitch::STATE::SHUTDOWN){
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(25);
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  LOW);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  HIGH);
                delay(50);
                digitalWrite (GPIO_LED_PIN,  LOW);
            }else{

            }
        }
    }
}
