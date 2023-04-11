/** GPIO operation library for Arduino
 *
 *  @class  LEDDriver
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_LED_DRIVER_NXP_ARD_H
#define ARDUINO_LED_DRIVER_NXP_ARD_H

#include <Arduino.h>
#include <stdint.h>

#include <I2C_device.h>
#include <SPI.h>

/** GPIO_base class
 *	
 *  @class GPIO_base
 *
 *	LEDDriver class is a base class for all LED drivers
 *	All actual device class will be derived from this class
 */

class GPIO_base : public I2C_device
{
public:
	enum board {
		NONE,
		ARDUINO_SHIELD,
	};
	enum io_config {
		OUTPUT,
		INPUT,
	};
	
	GPIO_base( uint8_t i2c_address, uint8_t n_bits );
	virtual ~GPIO_base();

	void output( int port, uint8_t value );
	void output( uint8_t *value );
	uint8_t output( int port );
	void output( uint8_t *value );
	void config( int port, uint8_t config  );

protected:
	const	uint8_t n_bits;
};



/** PCA995x class
 *	
 *  @class PCA995x
 *
 *	Base class to abstract behavior of PCA995x series
 */

class PCA995x : public LEDDriver
{
public:
	PCA995x( uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe = 8 );
	virtual ~PCA995x();
	
	virtual void begin( float current =  0.1, board env = NONE, bool buffered = false );
	virtual void init( float current )	= 0;

	/** Set IREFALL value (current setting for all channels)
	 *
	 * @param value current value in float (0.0 ~ 1.0)
	 */
	void irefall( uint8_t iref );

protected:
	const uint8_t reg_IREF;
	const uint8_t reg_IREFALL;
};



class PCA995x_I2C : public PCA995x, public I2C_device
{
public:
	PCA995x_I2C( uint8_t i2c_address, uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe = 8 );
	virtual ~PCA995x_I2C();

	void reg_access( uint8_t reg, uint8_t val  );
	void reg_access( uint8_t reg, uint8_t *vp, int len );
};



class PCA995x_SPI : public PCA995x
{
public:
	PCA995x_SPI( uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe = 9 );
	virtual ~PCA995x_SPI();

	void txrx( uint8_t *data, int size );

	void reg_access( uint8_t reg, uint8_t val );
	void reg_access( uint8_t reg, uint8_t *vp, int len );
	void reg_w( uint8_t reg, uint8_t val );
	void reg_w( uint8_t reg, uint8_t *vp, int len );
	uint8_t reg_r( uint8_t reg );
	void reg_r( uint8_t reg, uint8_t *vp, int len );
	void write_r8( uint8_t reg, uint8_t val );
	uint8_t read_r8( uint8_t reg );

	/** Set IREFALL value (current setting for all channels)
	 *
	 * @param value current value in float (0.0 ~ 1.0)
	 */
	void irefall( uint8_t iref );

	void pwm( uint8_t ch, float value );
	void pwm( float* values );
};



/** PCA9955B class
 *	
 *  @class PCA9955B

 *  About PCA9955B:
 *    https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/led-drivers/16-channel-fm-plus-ic-bus-57-ma-20-v-constant-current-led-driver:PCA9955BTW
 */

class PCA9955B : public PCA995x_I2C
{
public:
	/** Number of channels */
	const static uint8_t n_channel	= 16;
	
	/** Name of the PCA9955B registers */
	enum reg_num {
		MODE1, MODE2, 
		LEDOUT0, LEDOUT1, LEDOUT2, LEDOUT3, 
		GRPPWM, GRPFREQ, 
		PWM0,  PWM1,  PWM2,   PWM3,   PWM4,   PWM5,   PWM6,   PWM7, 
		PWM8,  PWM9,  PWM10,  PWM11,  PWM12,  PWM13,  PWM14,  PWM15, 
		IREF0, IREF1, IREF2,  IREF3,  IREF4,  IREF5,  IREF6,  IREF7, 
		IREF8, IREF9, IREF10, IREF11, IREF12, IREF13, IREF14, IREF15, 
		RAMP_RATE_GRP0, STEP_TIME_GRP0, HOLD_CNTL_GRP0, IREF_GRP0, 
		RAMP_RATE_GRP1, STEP_TIME_GRP1, HOLD_CNTL_GRP1, IREF_GRP1, 
		RAMP_RATE_GRP2, STEP_TIME_GRP2, HOLD_CNTL_GRP2, IREF_GRP2, 
		RAMP_RATE_GRP3, STEP_TIME_GRP3, HOLD_CNTL_GRP3, IREF_GRP3, 
		GRAD_MODE_SEL0, GRAD_MODE_SEL1, 
		GRAD_GRP_SEL0, GRAD_GRP_SEL1, GRAD_GRP_SEL2, GRAD_GRP_SEL3, 
		GRAD_CNTL, 
		OFFSET, 
		SUBADR1, SUBADR2, SUBADR3, ALLCALLADR, 
		PWMALL, IREFALL, 
		EFLAG0, EFLAG1, EFLAG2, EFLAG3, 
	};
	
    /** Create a PCA9955B instance connected to specified I2C pins with specified address
     *
     * @param i2c_address I2C-bus address (default: (0xBC>>1))
     */
	PCA9955B( uint8_t i2c_address = (0xBC >> 1) );
	virtual ~PCA9955B();

	/** Initializing device
	 *
	 * @param value current value in float (0.0 ~ 1.0)
	 */
	void init( float current );
};

class PCA9956B : public PCA995x_I2C
{
public:
	/** Number of channels */
	const static uint8_t n_channel	= 24;
	
	/** Name of the PCA9955B registers */
	enum reg_num {
		MODE1, MODE2,
		LEDOUT0, LEDOUT1, LEDOUT2, LEDOUT3, LEDOUT4, LEDOUT5,
		GRPPWM, GRPFREQ,
		PWM0,  PWM1,  PWM2,  PWM3,  PWM4,  PWM5,
		PWM6,  PWM7,  PWM8,  PWM9,  PWM10, PWM11,
		PWM12, PWM13, PWM14, PWM15, PWM16, PWM17,
		PWM18, PWM19, PWM20, PWM21, PWM22, PWM23,
		IREF0,  IREF1,  IREF2,  IREF3,  IREF4,  IREF5,
		IREF6,  IREF7,  IREF8,  IREF9,  IREF10, IREF11,
		IREF12, IREF13, IREF14, IREF15, IREF16, IREF17,
		IREF18, IREF19, IREF20, IREF21, IREF22, IREF23,
		OFFSET,
		SUBADR1, SUBADR2, SUBADR3, ALLCALLADR,
		PWMALL, IREFALL,
		EFLAG0, EFLAG1, EFLAG2, EFLAG3, EFLAG4, EFLAG5
	};
	/** Create a PCA9955B instance connected to specified I2C pins with specified address
	 *
	 * @param i2c_address I2C-bus address (default: (0x02>>1))
	 */
	PCA9956B( uint8_t i2c_address = (0x02 >> 1) );
	virtual ~PCA9956B();

	/** Initializing device
	 *
	 * @param value current value in float (0.0 ~ 1.0)
	 */
	void init( float current );
};

class PCA9957 : public PCA995x_SPI
{
public:
	/** Number of channels */
	const static uint8_t n_channel	= 24;
	
	/** Name of the PCA9957 registers */
	enum reg_num {
		MODE1, MODE2,
		EFLAG0,  EFLAG1,  EFLAG2,  EFLAG3,  EFLAG4,  EFLAG5,
		LEDOUT0, LEDOUT1, LEDOUT2, LEDOUT3, LEDOUT4, LEDOUT5,
		GRPPWM, GRPFREQ,
		PWM0,  PWM1,  PWM2,  PWM3,  PWM4,  PWM5,
		PWM6,  PWM7,  PWM8,  PWM9,  PWM10, PWM11,
		PWM12, PWM13, PWM14, PWM15, PWM16, PWM17,
		PWM18, PWM19, PWM20, PWM21, PWM22, PWM23,
		IREF0,  IREF1,  IREF2,  IREF3,  IREF4,  IREF5,
		IREF6,  IREF7,  IREF8,  IREF9,  IREF10, IREF11,
		IREF12, IREF13, IREF14, IREF15, IREF16, IREF17,
		IREF18, IREF19, IREF20, IREF21, IREF22, IREF23,
		RAMP_RATE_GRP0, STEP_TIME_GRP0, HOLD_CNTL_GRP0, IREF_GRP0,
		RAMP_RATE_GRP1, STEP_TIME_GRP1, HOLD_CNTL_GRP1, IREF_GRP1,
		RAMP_RATE_GRP2, STEP_TIME_GRP2, HOLD_CNTL_GRP2, IREF_GRP2,
		RAMP_RATE_GRP3, STEP_TIME_GRP3, HOLD_CNTL_GRP3, IREF_GRP3,
		RAMP_RATE_GRP4, STEP_TIME_GRP4, HOLD_CNTL_GRP4, IREF_GRP4,
		RAMP_RATE_GRP5, STEP_TIME_GRP5, HOLD_CNTL_GRP5, IREF_GRP5,
		GRAD_MODE_SEL0, GRAD_MODE_SEL1, GRAD_MODE_SEL2,
		GRAD_GRP_SEL0,  GRAD_GRP_SEL1,  GRAD_GRP_SEL2,
		GRAD_GRP_SEL3,  GRAD_GRP_SEL4,  GRAD_GRP_SEL5,
		GRAD_GRP_SEL6,  GRAD_GRP_SEL7,  GRAD_GRP_SEL8,
		GRAD_GRP_SEL9,  GRAD_GRP_SEL10, GRAD_GRP_SEL11,
		GRAD_CNTL0, GRAD_CNTL1,
		OFFSET,
		PWMALL, IREFALL
	};
	
	/** Create a PCA9957 instance connected to specified I2C pins with specified address
	 *
	 * @param i2c_address I2C-bus address (default: (0x02>>1))
	 */
	PCA9957();
	virtual ~PCA9957();

	/** Initializing device
	 *
	 * @param value current value in float (0.0 ~ 1.0)
	 */
	void init( float current );
};

#endif //	ARDUINO_LED_DRIVER_NXP_ARD_H
