/** GPIO operation library for Arduino
 *
 *  @class  LEDDriver
 *  @author Tedd OKANO
 *
 *  Released under the MIT license License
 */

#ifndef ARDUINO_GPIO_NXP_ARD_H
#define ARDUINO_GPIO_NXP_ARD_H

#include <Arduino.h>
#include <stdint.h>

#include <I2C_device.h>

/** GPIO_base class
 *	
 *  @class GPIO_base
 *
 *	LEDDriver class is a base class for all LED drivers
 *	All actual device class will be derived from this class
 */

typedef struct register_references {
	const uint8_t	input;
	const uint8_t	output;
	const uint8_t	polarity;
	const uint8_t	config;
	const uint8_t	latch;
	const uint8_t	pud_en;
	const uint8_t	pud_sel;
	const uint8_t	int_mask;
	const uint8_t	int_status;
} reg_references;


class GPIO_base : public I2C_device
{
public:
	enum board {
		NONE,
		ARDUINO_SHIELD,
	};
#ifdef OUTPUT
#undef OUTPUT
#endif
#ifdef INPUT
#undef INPUT
#endif
	enum io_config {
		OUTPUT,
		INPUT,
	};

	const int	n_bits;
	
	GPIO_base( uint8_t i2c_address, const int nbits, const reg_references* prpty );
	virtual ~GPIO_base();

	void begin( board env );
	
	void output( int port, uint8_t value, uint8_t mask = 0 );
	void output( uint8_t *vp );
	uint8_t input( int port );
	void input( uint8_t *vp );
	void config( int port, uint8_t config, uint8_t mask = 0 );
	void config( uint8_t* vp );

	void write_ports( int reg, uint8_t* vp );
	void write_ports16( int reg, uint16_t* vp );
	void read_ports( int reg, uint8_t* vp );
	void read_ports16( int reg, uint16_t* vp );

private:
	const reg_references*	rrp;
	
	static constexpr int RESET_PIN	= 8;
	static constexpr int ADDR_PIN	= 9;
};

class PCAL6xxx_base : public GPIO_base
{
public:
	PCAL6xxx_base( uint8_t i2c_address, const int nbits, const reg_references* prpty );
	virtual ~PCAL6xxx_base();
};


class PCAL6534 : public PCAL6xxx_base
{
public:
	/** Number of channels */
	constexpr static uint8_t n_channel	= 34;
	
	/** Name of the PCA9955B registers */
	enum reg_num {
		Input_Port_0,  Input_Port_1,  Input_Port_2,  Input_Port_3,  Input_Port_4,
		Output_Port_0, Output_Port_1, Output_Port_2, Output_Port_3, Output_Port_4,
		Polarity_Inversion_port_0, Polarity_Inversion_port_1, Polarity_Inversion_port_2, Polarity_Inversion_port_3, Polarity_Inversion_port_4,
		Configuration_port_0, Configuration_port_1, Configuration_port_2, Configuration_port_3, Configuration_port_4,
		Output_drive_strength_register_port_0A	= 0x30, Output_drive_strength_register_port_0B,
		Output_drive_strength_register_port_1A, Output_drive_strength_register_port_1B,
		Output_drive_strength_register_port_2A, Output_drive_strength_register_port_2B,
		Output_drive_strength_register_port_3A, Output_drive_strength_register_port_3B,
		Output_drive_strength_register_port_4A, reserved0,
		Input_latch_register_port_0, Input_latch_register_port_1, Input_latch_register_port_2, Input_latch_register_port_3, Input_latch_register_port_4,
		Pull_up_pull_down_enable_register_port_0, Pull_up_pull_down_enable_register_port_1, Pull_up_pull_down_enable_register_port_2, Pull_up_pull_down_enable_register_port_3, Pull_up_pull_down_enable_register_port_4,
		Pull_up_pull_down_selection_register_port_0, Pull_up_pull_down_selection_register_port_1, Pull_up_pull_down_selection_register_port_2, Pull_up_pull_down_selection_register_port_3, Pull_up_pull_down_selection_register_port_4,
		Interrupt_mask_register_port_0, Interrupt_mask_register_port_1, Interrupt_mask_register_port_2, Interrupt_mask_register_port_3, Interrupt_mask_register_port_4,
		Interrupt_status_register_port_0, Interrupt_status_register_port_1, Interrupt_status_register_port_2, Interrupt_status_register_port_3, Interrupt_status_register_port_4,
		Output_port_configuration_register,
		Interrupt_edge_register_port_0A, Interrupt_edge_register_port_0B,
		Interrupt_edge_register_port_1A, Interrupt_edge_register_port_1B,
		Interrupt_edge_register_port_2A, Interrupt_edge_register_port_2B,
		Interrupt_edge_register_port_3A, Interrupt_edge_register_port_3B,
		Interrupt_edge_register_port_4A, reserved1,
		Interrupt_clear_register_port_0, Interrupt_clear_register_port_1, Interrupt_clear_register_port_2, Interrupt_clear_register_port_3, Interrupt_clear_register_port_4,
		Input_status_port_0, Input_status_port_1, Input_status_port_2, Input_status_port_3, Input_status_port_4,
		Individual_pin_output_port_0_configuration_register, Individual_pin_output_port_1_configuration_register, Individual_pin_output_port_2_configuration_register, Individual_pin_output_port_3_configuration_register, Individual_pin_output_port_4_configuration_register,
		Switch_debounce_enable_0, Switch_debounce_enable_1,
		Switch_debounce_count,
	};
	
	PCAL6534( uint8_t i2c_address = (0x44 >> 1) + 0 );
	virtual ~PCAL6534();

	static constexpr reg_references reg_ref = {
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
};

#endif //	ARDUINO_GPIO_NXP_ARD_H
