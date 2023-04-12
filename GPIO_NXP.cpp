#include "GPIO_NXP.h"

#define	BYTE_ORDER	LITTLE_ENDIAN

/* LEDDriver class ******************************************/

GPIO_base::GPIO_base( uint8_t i2c_address, const int nbits, const reg_references* rr ) :
	I2C_device( i2c_address ), 
	n_bits( nbits ),
	rrp( rr )
{
	constexpr uint16_t	i	= 0x0001;
	uint8_t*			tp	= (uint8_t*)(&i);

	endian	= tp[ 0 ];	//	'true' if LittleEndian
}

GPIO_base::~GPIO_base()
{
}

void GPIO_base::begin( board env )
{
	if ( env ) {
		//	NEED INVESTIGATION LATER
		//	NO ENOUGH CURRENT ON PIN8 OUTPUT?
		//	BECAUSE IT CANNOT ASSERT LOW (LITTLE LOWER VOLTAGE)
		//	SAME THING HAPPENS ON OTHER PCAL6XXX-ARD BOARDS
		//	THE SIGNAL CAN BE LOW WHEN THE ARD BOARD IS NOT CONNECTED
		//	INSTEAD OF USING THIS MECHANISM, USE "I2C_device::scan()" TO CONFIRM THE TARGET ADDRESS
		
#if 0		
		digitalWrite( RESET_PIN , 0 );
		delay( 1 );	// reset time = 500ns(min)
		digitalWrite( RESET_PIN , 1 );
		delay( 1 ); // reset recovery time = 600ns(min)
#endif
	}
}


void GPIO_base::output( int port, uint8_t value, uint8_t mask )
{
	if ( mask )
		bit_op8( rrp->output + port, mask, value );

	write_r8( rrp->output + port, value );
}

void GPIO_base::output( uint8_t *vp )
{
	write_ports( rrp->output, vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( rrp->input + port );
}

void GPIO_base::input( uint8_t *vp )
{
	read_ports( rrp->input, vp );
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( rrp->config + port, mask, config );

	write_r8( rrp->config + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	write_ports( rrp->config, vp );	
}

void GPIO_base::write_ports( int reg, uint8_t* vp )
{
	reg_w( 0x80 | reg, vp, (n_bits + 7) / 8 );
}

void GPIO_base::write_ports16( int reg, uint16_t* vp )
{
	if ( endian ) {
		uint16_t	temp;
		for ( int i = 0; i < ((n_bits + 7) / 8); i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	}

	reg_w( 0x80 | reg, (uint8_t*)vp, (n_bits * 2 + 7) / 8 );		
}

void GPIO_base::read_ports( int reg, uint8_t* vp )
{
	reg_r( 0x80 | reg, vp, (n_bits + 7) / 8 );
}

void GPIO_base::read_ports16( int reg, uint16_t* vp )
{
	reg_r( 0x80 | reg, (uint8_t*)vp, (n_bits * 2 + 7) / 8 );	

	if ( endian ) {
		uint16_t	temp;

		for ( int i = 0; i < ((n_bits + 7) / 8); i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	} 
}

PCAL6xxx_base::PCAL6xxx_base( uint8_t i2c_address, const int nbits, const reg_references* rrp ) :
	GPIO_base( i2c_address, nbits, rrp )
{
}

PCAL6xxx_base::~PCAL6xxx_base()
{
}


PCAL6534::PCAL6534( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 34, &reg_ref )
{
}

PCAL6534::~PCAL6534()
{
}

constexpr reg_references PCAL6534::reg_ref;
