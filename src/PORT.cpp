#include "PORT.h"

PORT::PORT( GPIO_base* gpio_ptr, int port_num, uint8_t _mask )
{
	devp	= gpio_ptr;
	pn		= port_num;
	mask	= _mask;
}

void PORT::config( uint8_t config )
{
	devp->config( pn, config );
}

PORT& PORT::operator=( uint8_t v )
{
	devp->output( pn, v );
	return *this;
}

PORT& PORT::operator=( PORT& )
{
	return *this;
}

PORT::operator int()
{
	return devp->input( pn );
}

GPIO_PORT::GPIO_PORT( GPIO_base* gpio_ptr, int port_num, uint8_t _mask ) : PORT( gpio_ptr, port_num, _mask ){}
