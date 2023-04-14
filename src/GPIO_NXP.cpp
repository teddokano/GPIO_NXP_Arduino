#include "GPIO_NXP.h"

/* ******** GPIO_base ******** */

GPIO_base::GPIO_base( uint8_t i2c_address, const int nbits, const uint8_t* ar, uint8_t ai  ) :
	I2C_device( i2c_address ), 
	n_bits( nbits ),
	n_ports( (nbits + 7) / 8 ),
	arp( ar ),
	auto_increment( ai )
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
		pinMode(RESET_PIN, OUTPUT);
		pinMode(ADDR_PIN, OUTPUT);

		digitalWrite( RESET_PIN , 1 );
		digitalWrite( ADDR_PIN , 1 );
		digitalWrite( ADDR_PIN , 0 );
		delay( 1 );
	
		digitalWrite( RESET_PIN , 0 );
		delay( 1 );	// reset time = 500ns(min)
		digitalWrite( RESET_PIN , 1 );
		delay( 1 ); // reset recovery time = 600ns(min)
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
	write_port( OUT, vp );
}

uint8_t GPIO_base::input( int port )
{
	return read_r8( *(arp + IN) + port );
}

uint8_t* GPIO_base::input( uint8_t *vp )
{
	read_port( IN, vp );
	
	return vp;
}

void GPIO_base::config( int port, uint8_t config, uint8_t mask )
{
	if ( mask )
		bit_op8( *(arp + CONFIG) + port, mask, config );

	write_r8( *(arp + CONFIG) + port, config );
}

void GPIO_base::config( uint8_t* vp )
{
	write_port( CONFIG, vp );
}

void GPIO_base::write_port( access_word w, uint8_t* vp )
{
	if ( auto_increment ) {
		reg_w( auto_increment | *(arp + w), vp, n_ports );		
	}
	else {
		for ( int i = 0; i < n_ports; i++ )
			write_r8( *(arp + w) + i, *vp++ );
	}
}

void GPIO_base::write_port16( access_word w, uint16_t* vp )
{
	if ( endian ) {
		uint16_t	temp;
		for ( int i = 0; i < n_ports; i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	}
	
	int	n_bytes	= (n_bits * 2 + 7) / 8;

	if ( auto_increment ) {
		reg_w( auto_increment | *(arp + w), (uint8_t*)vp, n_bytes );		
	}
	else {
		for ( int i = 0; i < n_bytes; i++ )
			write_r8( *(arp + w) + i, *vp++ );
	}
}

uint8_t* GPIO_base::read_port( access_word w, uint8_t* vp )
{
	if ( auto_increment ) {
		reg_r( auto_increment | *(arp + w), vp, n_ports );		
	}
	else {
		for ( int i = 0; i < n_ports; i++ )
			*(vp + i)	= read_r8( *(arp + w) + i );
	}
	
	return vp;
}

uint16_t*  GPIO_base::read_port16( access_word w, uint16_t* vp )
{
	int	n_bytes	= (n_bits * 2 + 7) / 8;
	
	if ( auto_increment ) {
		reg_r( auto_increment | *(arp + w), (uint8_t*)vp, n_bytes );	
	}
	else {
		for ( int i = 0; i < n_bytes; i++ )
			*(vp + i)	= read_r8( *(arp + w) + i );		
	}
		

	if ( endian ) {
		uint16_t	temp;

		for ( int i = 0; i < n_ports; i++ ) {
			temp	= vp[ i ] << 8;
			vp[ i ]	= temp | vp[ i ] >> 8;			
		}
	}
	
	return vp;
}

void GPIO_base::write_port( access_word w, uint8_t value, int port_num )
{
	write_r8( *(arp + w) + port_num, value );
}

void GPIO_base::write_port16( access_word w, uint16_t value, int port_num )
{
	write_r16( *(arp + w) + port_num, value );
}

uint8_t GPIO_base::read_port( access_word w, int port_num )
{
	return read_r8( *(arp + w) + port_num );
}

uint16_t GPIO_base::read_port16( access_word w, int port_num )
{
	return read_r16( *(arp + w) + port_num );
}

void GPIO_base::print_bin( uint8_t v )
{
	Serial.print(" 0b");
	for (int i = 7; 0 <= i; i-- )
		Serial.print(((v >> i) & 0x1) ? "1" : "0");
}


/* ******** PCAL6xxx_base ******** */

PCAL6xxx_base::PCAL6xxx_base( uint8_t i2c_address, const int nbits, const uint8_t arp[], uint8_t ai ) :
	GPIO_base( i2c_address, nbits, arp, ai )
{
}

PCAL6xxx_base::~PCAL6xxx_base()
{
}


/* ******** PCAL6408A ******** */

PCAL6408A::PCAL6408A( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 8, access_ref, 0 )
{
}

PCAL6408A::~PCAL6408A()
{
}

constexpr uint8_t PCAL6408A::access_ref[];


/* ******** PCAL6416A ******** */

PCAL6416A::PCAL6416A( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 16, access_ref, 0 )
{
}

PCAL6416A::~PCAL6416A()
{
}

constexpr uint8_t PCAL6416A::access_ref[];


/* ******** PCAL6524 ******** */

PCAL6524::PCAL6524( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 24, access_ref, 0x80 )
{
}

PCAL6524::~PCAL6524()
{
}

constexpr uint8_t PCAL6524::access_ref[];


/* ******** PCAL6534 ******** */

PCAL6534::PCAL6534( uint8_t i2c_address ) :
	PCAL6xxx_base( i2c_address, 34, access_ref, 0x80 )
{
}

PCAL6534::~PCAL6534()
{
}

constexpr uint8_t PCAL6534::access_ref[];
