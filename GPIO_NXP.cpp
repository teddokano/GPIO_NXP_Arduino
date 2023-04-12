#include "GPIO_NXP.h"

/* LEDDriver class ******************************************/

GPIO_base::GPIO_base( uint8_t i2c_address, int nbits, const reg_references* rr ) :
	I2C_device( i2c_address ), 
	n_bits( nbits ),
	rrp( rr )
{
}

GPIO_base::~GPIO_base()
{
}

void GPIO_base::output( int port, uint8_t value, uint8_t mask )
{
	if ( mask )
		bit_op8( rrp->output + port, mask, value );

	write_r8( rrp->output + port, value );
}

void GPIO_base::output( uint8_t *vp )
{
	all_port_w8( rrp->output, vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( rrp->input + port );
}

void GPIO_base::input( uint8_t *vp )
{
	all_port_r8( rrp->input, vp );
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( rrp->config + port, mask, config );

	write_r8( rrp->config + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	all_port_w8( rrp->config, vp );	
}

void GPIO_base::all_port_w8( int reg, uint8_t* vp )
{
	reg_w( 0x80 | reg, vp, n_bits );
}

void GPIO_base::all_port_w16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_w( 0x80 | reg, (uint8_t*)vp, n_bits * 2 );
}

void GPIO_base::all_port_r8( int reg, uint8_t* vp )
{
	reg_r( 0x80 | reg, vp, n_bits );
}

void GPIO_base::all_port_r16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_r( 0x80 | reg, (uint8_t*)vp, n_bits * 2 );	
}

PCAL6534::PCAL6534( uint8_t i2c_address ) :
	GPIO_base( i2c_address, 34, &rr )
{
}

PCAL6534::~PCAL6534()
{
}

#if 1
#if 1
const reg_references PCAL6534::rr;
#else
const reg_references PCAL6534::rr = {
	Input_Port_0,
	Output_Port_0,
	Polarity_Inversion_port_0,
	Configuration_port_0,
	Input_latch_register_port_0,
	Pull_up_pull_down_enable_register_port_0,
	Pull_up_pull_down_selection_register_port_0,
	Interrupt_mask_register_port_0,
	Interrupt_status_register_port_0,
};
#endif
#endif
