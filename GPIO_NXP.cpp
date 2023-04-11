#include "LEDDriver.h"

/* LEDDriver class ******************************************/

LEDDriver::LEDDriver( uint8_t n_ch, uint8_t PWM_r, uint8_t oe ) :
	n_channel( n_ch ), reg_PWM( PWM_r ), oe_pin( oe ), bp( NULL )
{
}

LEDDriver::~LEDDriver()
{
	if ( bp )
		delete[]	bp;
}

void LEDDriver::pwm( uint8_t ch, float value )
{
	if ( bp ) {
		bp[ ch ]	= (uint8_t)(value * 255.0);
	}
	else {
		reg_access( reg_PWM + ch, (uint8_t)(value * 255.0) );		
	}
}

void LEDDriver::pwm( float* values )
{
	if ( bp ) {
		for ( int i = 0; i < n_channel; i++ )
			bp[ i ]	= (uint8_t)(values[ i ] * 255.0);
	}
	else {
		uint8_t	v[ n_channel ];
		for ( int i = 0; i < n_channel; i++ )
			v[ i ]	= (uint8_t)(values[ i ] * 255.0);

		reg_access( 0x80 | reg_PWM, v, n_channel );
	}
}

void LEDDriver::flush( void )
{
	if ( bp )
		reg_access( 0x80 | reg_PWM, bp, n_channel );
}

void LEDDriver::buffer_enable( bool flag )
{
	if ( bp ) {
		delete[]	bp;
		bp	= NULL;		
	}
	
	if ( flag ) {
		bp	= new uint8_t[ n_channel ];
		for ( int i = 0; i < n_channel; i++ )
			bp[ i ]	= 0x00;
	}
	
}


/* PCA995x class ******************************************/

PCA995x::PCA995x( uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe ) : 
	LEDDriver( n_ch, PWM_r, oe ), reg_IREF( IREF_r ), reg_IREFALL( IREFALL_r )
{
	//  do nothing.
	//  leave it in default state.
}

PCA995x::~PCA995x()
{
}

void PCA995x::begin( float current, board env, bool bflag )
{
	init( current );
	
	if ( env ) {
		pinMode( oe_pin, OUTPUT );
		digitalWrite( oe_pin , 0 );
	}
	
	buffer_enable( bflag );
}

void PCA995x::irefall( uint8_t iref )
{
	reg_access( reg_IREFALL, iref );
}



/* PCA995x_I2C class ******************************************/

PCA995x_I2C::PCA995x_I2C( uint8_t i2c_address, uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe ) : 
	PCA995x( n_ch, PWM_r, IREF_r, IREFALL_r, oe ), 
	I2C_device( i2c_address )
{
	//  do nothing.
	//  leave it in default state.
}

PCA995x_I2C::~PCA995x_I2C()
{
}

void PCA995x_I2C::reg_access( uint8_t reg, uint8_t val )
{
	write_r8( reg, val );
}

void PCA995x_I2C::reg_access( uint8_t reg, uint8_t *vp, int len )
{
	reg_w( 0x80 | reg, vp, len );
}



/* PCA995x_SPI class ******************************************/

PCA995x_SPI::PCA995x_SPI( uint8_t n_ch, uint8_t PWM_r, uint8_t IREF_r, uint8_t IREFALL_r, uint8_t oe ) : 
	PCA995x( n_ch, PWM_r, IREF_r, IREFALL_r, oe )
{
	//  do nothing.
	//  leave it in default state.
}

PCA995x_SPI::~PCA995x_SPI()
{
}

void PCA995x_SPI::txrx( uint8_t *data, int size )
{
	digitalWrite( SS, LOW );
	SPI.transfer( data, size );
	digitalWrite( SS, HIGH );
}

void PCA995x_SPI::reg_access( uint8_t reg, uint8_t val )
{
	uint8_t data[]	= { reg, val };

	txrx( data, sizeof( data ) );
}

void PCA995x_SPI::reg_access( uint8_t reg, uint8_t *vp, int len )
{
	uint8_t data[ len * 2 ];
	
	for ( int i = 0; i < len; i++ ) {
		data[ i * 2 + 0 ]	= reg + (i * 2);
		data[ i * 2 + 1 ]	= vp[ i ];
	}
	
	for ( int i = 0; i < len * 2; i += 2 )
		txrx( data + i, 2 );
}

void PCA995x_SPI::reg_w( uint8_t reg, uint8_t val )
{
	reg_access( reg << 1, val );
}

void PCA995x_SPI::reg_w( uint8_t reg, uint8_t *vp, int len )
{
	reg_access( reg << 1, vp, len );
}

uint8_t PCA995x_SPI::reg_r( uint8_t reg )
{
	uint8_t	data[ 2 ]	= { 0xFF, 0xFF };
	
	reg_access( (reg << 1) | 0x01, 0xFF );
	txrx( data, sizeof( data ) );

	return data[ 1 ];
}

void PCA995x_SPI::reg_r( uint8_t reg, uint8_t *vp, int len )
{
	for ( int i = 0; i < len; i++ ) {
		*vp++	= reg_r( ((reg + i) << 1) | 0x01 );
	}
}

void PCA995x_SPI::write_r8( uint8_t reg, uint8_t val )
{
	reg_w( reg, val );
}

uint8_t PCA995x_SPI::read_r8( uint8_t reg )
{
	return reg_r( reg );
}

void PCA995x_SPI::irefall( uint8_t iref )
{
	write_r8( reg_IREFALL, iref );
}

void PCA995x_SPI::pwm( uint8_t ch, float value )
{
	write_r8( reg_PWM + ch, (uint8_t)(value * 255.0) );
}

void PCA995x_SPI::pwm( float* values )
{
	uint8_t	v[ n_channel ];
	for ( int i = 0; i < n_channel; i++ )
		v[ i ]	= (uint8_t)(values[ i ] * 255.0);

	reg_w( reg_PWM, v, n_channel );
}








/* PCA9955B class ******************************************/
PCA9955B::PCA9955B( uint8_t i2c_address ) : 
	PCA995x_I2C( i2c_address, 16, PCA9955B::PWM0, PCA9955B::IREF0, PCA9955B::IREFALL )
{
}

PCA9955B::~PCA9955B()
{
}

void PCA9955B::init( float current )
{
	uint8_t	init[]	= { 0xAA, 0xAA, 0xAA, 0xAA };
	reg_w( 0x80 | LEDOUT0, init, sizeof( init ) );
	write_r8( PWMALL, 0x00 );
	
	irefall( (uint8_t)(current * 255.0) );
}



/* PCA9956B class ******************************************/
PCA9956B::PCA9956B( uint8_t i2c_address ) : 
	PCA995x_I2C( i2c_address, 24, PCA9956B::PWM0, PCA9956B::IREF0, PCA9956B::IREFALL )
{
}

PCA9956B::~PCA9956B()
{
}

void PCA9956B::init( float current )
{
	uint8_t	init[]	= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
	reg_w( 0x80 | LEDOUT0, init, sizeof( init ) );
	write_r8( PWMALL, 0x00 );
	
	irefall( (uint8_t)(current * 255.0) );
}



/* PCA9957 class ******************************************/
PCA9957::PCA9957() : 
	PCA995x_SPI( 24, PCA9957::PWM0, PCA9957::IREF0, PCA9957::IREFALL )
{
}

PCA9957::~PCA9957()
{
}

void PCA9957::init( float current )
{
	uint8_t	init[]	= { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

	digitalWrite( SS, HIGH );

	write_r8( MODE2,  0x18 );
	write_r8( PWMALL, 0x00 );
	reg_w( LEDOUT0, init, sizeof( init ) );
	
	irefall( (uint8_t)(current * 255.0) );
	
}



