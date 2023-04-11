#include "GPIO_NXP.h"

/* LEDDriver class ******************************************/

GPIO_base::GPIO_base( uint8_t i2c_address, int n_bits, uint8_t in_r, uint8_t out_r, uint8_t cfg_r ) :
	I2C_device( i2c_address ), 
	N_BITS( n_bits ),
	in_reg( in_r ),
	out_reg( out_r ),
	cfg_reg( cfg_r )
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
	all_reg_w8( out_reg, vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( in_reg + port );
}

void GPIO_base::input( uint8_t *vp )
{
	all_reg_r8( in_reg, vp );
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( cfg_reg + port, mask, config );

	write_r8( cfg_reg + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	all_reg_w8( cfg_reg, vp );	
}

void GPIO_base::all_reg_w8( int reg, uint8_t* vp )
{
	reg_w( 0x80 | reg, vp, N_BITS );
}

void GPIO_base::all_reg_w16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_w( 0x80 | reg, (uint8_t*)vp, N_BITS * 2 );
}

void GPIO_base::all_reg_r8( int reg, uint8_t* vp )
{
	reg_r( 0x80 | reg, vp, N_BITS );
}

void GPIO_base::all_reg_r16( int reg, uint16_t* vp )
{
	//	Expecting little endian
	reg_r( 0x80 | reg, (uint8_t*)vp, N_BITS * 2 );	
}


PCAL6xxx_base::PCAL6xxx_base( 
							 	uint8_t i2c_address, 
							 	int n_bits, 
							 	uint8_t in_r, 
							 	uint8_t out_r, 
							 	uint8_t cfg_r,
								uint8_t latch_r,
								uint8_t pud_en_r,
								uint8_t pud_sel_r,
								uint8_t int_mask_r,
								uint8_t int_status_r
							 ) :
	GPIO_base( i2c_address, n_bits, in_r, out_r, cfg_r ),
	latch_reg( latch_r ),
	pud_en_reg( pud_en_r ),
	pud_sel_reg( pud_sel_r ),
	int_mask_reg( int_mask_r ),
	int_status_reg( int_status_r )
{
}

PCAL6xxx_base::~PCAL6xxx_base()
{
}

PCAL6534::PCAL6534(
					uint8_t i2c_address, 
					int n_bits, 
					uint8_t in_r, 
					uint8_t out_r, 
					uint8_t cfg_r,
					uint8_t latch_r,
					uint8_t pud_en_r,
					uint8_t pud_sel_r,
					uint8_t int_mask_r,
					uint8_t int_status_r
					) : 
	PCAL6xxx_base( i2c_address, n_bits, in_r, out_r, cfg_r, latch_r, pud_en_r, pud_sel_r, int_mask_r, int_status_r )
{	
}

PCAL6534::~PCAL6534()
{
}
