/** PORT: a interfacing class for GPIO operation library, Arduino
 *
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_GPIO_NXP_ARD_PORT_H
#define ARDUINO_GPIO_NXP_ARD_PORT_H

#include <GPIO_NXP.h>

/** PORT class
 *	
 *  @class PORT
 *
 *	Simple interface for GPIO devices
 */
class PORT {
public:
	/** Mask setting
	 * 
	 * 	Bit mask for output. This is being initialized to 0 if _mask is not given to constructor
	 */
	uint8_t	mask;

	/** Constractor
	 * 
	 * @param gpio_ptr 	Pointer to GPIO device instance
	 * @param port_num	Port number
	 * @param mask		(option) bit mask for output
	 */
	PORT( GPIO_base* gpio_ptr, int port_num, uint8_t _mask = 0x00 );

	/** Config port
	 * 
	 *	Basic GPIO port access function for single port configuration
	 *
	 * @param config	8 bit value. 0 for output, 1 for input
	 */
	void	config( uint8_t config );
	
	/** A short hand for setting pins
	 */
	PORT&	operator=( uint8_t v );
	PORT&	operator=( PORT& rhs );

	/** A short hand for reading pins
	 */
	operator	int();
	
private:
	GPIO_base	*devp;
	int			pn;
};

class GPIO_PORT : public PORT {
public:
	using PORT::operator=;
	GPIO_PORT( GPIO_base* gpio_ptr, int port_num, uint8_t _mask = 0x00 );
};

#endif //	ARDUINO_GPIO_NXP_ARD_PORT_H
