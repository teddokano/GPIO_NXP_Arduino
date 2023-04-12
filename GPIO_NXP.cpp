#include "GPIO_NXP.h"

/* LEDDriver class ******************************************/

GPIO_base::GPIO_base( uint8_t i2c_address, const properties* pp ) :
	I2C_device( i2c_address ), 
	N_BITS( pp->bits ),
	in_reg( pp->input ),
	out_reg( pp->output ),
	cfg_reg( pp->config )
{
}

GPIO_base::~GPIO_base()
{
}

void GPIO_base::output( int port, uint8_t value, uint8_t mask )
{
	if ( mask )
		bit_op8( out_reg + port, mask, value );

	write_r8( out_reg + port, value );
}

void GPIO_base::output( uint8_t *vp )
{
	all_port_w8( out_reg, vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( in_reg + port );
}

void GPIO_base::input( uint8_t *vp )
{
	all_port_r8( in_reg, vp );
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( cfg_reg + port, mask, config );

	write_r8( cfg_reg + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	all_port_w8( cfg_reg, vp );	
}

void GPIO_base::all_port_w8( int reg, uint8_t* vp )
{
	reg_w( 0x80 | reg, vp, N_BITS );
}

void GPIO_base::all_port_w16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_w( 0x80 | reg, (uint8_t*)vp, N_BITS * 2 );
}

void GPIO_base::all_port_r8( int reg, uint8_t* vp )
{
	reg_r( 0x80 | reg, vp, N_BITS );
}

void GPIO_base::all_port_r16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_r( 0x80 | reg, (uint8_t*)vp, N_BITS * 2 );	
}

PCAL6534::PCAL6534( uint8_t i2c_address ) :
	GPIO_base( i2c_address, &ppty )
{
}

PCAL6534::~PCAL6534()
{
}
