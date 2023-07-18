#ifndef __PINOUT_H
#define __PINOUT_H

#include "PCF8574.h"



/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * pinout::instance().setup();
 * 
 * From global application loop you must call:
 * pinout::instance().loop();
 */
class pinout {
public: // Public area for added variables and functions
    // PINS
    #define DOUT 4
    #define CLK 3
    #define EXT_BATTERY_V A0
    #define WATCHDOG_PIN D6
    #define MIRROR_R A1
    #define MIRROR_G A3
    #define MIRROR_B A5
    int EN_12V = D8; // supplies unregulated battery voltage to pump's relays
    int I2C_POWER = D7;
    bool I2C[4];
    bool solar_on;
    bool charging;
    bool fb_RS_PWR;
    
    void rs485_toggle(bool);
    
protected: // Protected area for added variables and functions
    unsigned long wd_last_checked_time = 0;
    unsigned long wd_check_freq = 2 * 60 * 1000;

    void I2cSetup();
    void servingWatchdog();
    void pinSetups();
    void i2cCheckFeedbacks();


public:
    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use pinout::instance() to instantiate the singleton.
     */
    static pinout &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use pinout::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * 
     * You typically use pinout::instance().loop();
     */
    void loop();


protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use pinout::instance() to instantiate the singleton.
     */
    pinout();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~pinout();

    /**
     * This class is a singleton and cannot be copied
     */
    pinout(const pinout&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    pinout& operator=(const pinout&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static pinout *_instance;

};
#endif  /* __PINOUT_H */
