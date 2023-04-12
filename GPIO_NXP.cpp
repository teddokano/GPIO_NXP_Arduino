#include "GPIO_NXP.h"

#define	BYTE_ORDER	LITTLE_ENDIAN

/* LEDDriver class ******************************************/

GPIO_base::GPIO_base( uint8_t i2c_address, const int nbits, const uint8_t* ar ) :
	I2C_device( i2c_address ), 
	n_bits( nbits ),
	arp( ar )
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
		bit_op8( *(arp + OUT) + port, mask, value );

	write_r8( *(arp + OUT) + port, value );
}

void GPIO_base::output( uint8_t *vp )
{
	write_ports( *(arp + OUT), vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( *(arp + IN) + port );
}

void GPIO_base::input( uint8_t *vp )
{
	read_ports( *(arp + IN), vp );
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( *(arp + CONFIG) + port, mask, config );

	write_r8( *(arp + CONFIG) + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	write_ports( *(arp + CONFIG), vp );	
}

void GPIO_base::write_ports( access_word w, uint8_t* vp )
{
	reg_w( 0x80 | *(arp + w), vp, (n_bits + 7) / 8 );
}

void GPIO_base::write_ports16( access_word w, uint16_t* vp )
{
	if ( endian ) {
		uint16_t	temp;
		for ( int i = 0; i < ((n_bits + 7) / 8); i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	}

	reg_w( 0x80 | *(arp + w), (uint8_t*)vp, (n_bits * 2 + 7) / 8 );		
}

void GPIO_base::read_ports( access_word w, uint8_t* vp )
{
	reg_r( 0x80 | *(arp + w), vp, (n_bits + 7) / 8 );
}

void GPIO_base::read_ports16( access_word w, uint16_t* vp )
{
	reg_r( 0x80 | *(arp + w), (uint8_t*)vp, (n_bits * 2 + 7) / 8 );	

	if ( endian ) {
		uint16_t	temp;

		for ( int i = 0; i < ((n_bits + 7) / 8); i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	} 
}

PCAL6xxx_base::PCAL6xxx_base( uint8_t i2c_address, const int nbits, const uint8_t arp[] ) :
	GPIO_base( i2c_address, nbits, arp )
{
}

PCAL6xxx_base::~PCAL6xxx_base()
{
}


PCAL6534::PCAL6534( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 34, access_ref )
{
}

PCAL6534::~PCAL6534()
{
}

constexpr uint8_t PCAL6534::access_ref[];
