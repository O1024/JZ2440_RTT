/**
 * \file
 *
 * \brief SAM Serial Peripheral Interface Driver for SAMB
 *
 * Copyright (C) 2015-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

/**
 * \defgroup asfdoc_samb_drivers_spi_group SAM Serial Peripheral Interface (SPI) Driver
 *
 * This driver for Atmel&reg; | SMART ARM-based microcontrollers provides 
 * an interface for the configuration and management of the module in 
 * its SPI mode to transfer SPI data frames. The following driver API modes 
 * are covered by this manual:
 *
 * - Polled APIs
 * \if SPI_CALLBACK_MODE
 * - Callback APIs
 * \endif
 *
 * The following peripheral is used by this module:
 * - SPI (Serial Peripheral Interface)
 *
 * The following devices can use this module:
 *  - Atmel | SMART SAM B11
 *
 * The outline of this documentation is as follows:
 * - \ref asfdoc_samb_drivers_spi_prerequisites
 * - \ref asfdoc_samb_drivers_spi_module_overview
 * - \ref asfdoc_samb_drivers_spi_special_considerations
 * - \ref asfdoc_samb_drivers_spi_extra_info
 * - \ref asfdoc_samb_drivers_spi_examples
 * - \ref asfdoc_samb_drivers_spi_api_overview
 *
 * \section asfdoc_samb_drivers_spi_prerequisites Prerequisites
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_samb_drivers_spi_module_overview Module Overview
 * \subsection asfdoc_samb_drivers_spi_bus SPI Bus Connection
 * In \ref asfdoc_sam0_spi_connection_example "the figure below", the
 * connection between one master and one slave is shown.
 *
 * \anchor asfdoc_sam0_spi_connection_example
 * \image html spi_block_diagram.svg "SPI Block Diagram"
 *
 * The different lines are as follows:
 * - \b MISO Master Input Slave Output. The line where the data is shifted
 *   out from the slave and into the master.
 * - \b MOSI Master Output Slave Input. The line where the data is shifted
 *   out from the master and into the slave.
 * - \b SCK Serial Clock. Generated by the master device.
 * - \b SS Slave Select. To initiate a transaction, the master must pull this
 *      line low.
 *
 * If the bus consists of several SPI slaves, they can be connected in parallel
 * and the SPI master can use general I/O pins to control separate SS lines to
 * each slave on the bus.
 *
 * It is also possible to connect all slaves in series. In this configuration,
 * a common SS is provided to \c N slaves, enabling them simultaneously. The
 * MISO from the \c N-1 slaves is connected to the MOSI on the next slave. The
 * \c N<SUP>th</SUP> slave connects its MISO back to the master. For a
 * complete transaction, the master must shift \c N+1 characters.
 *
 * \subsection asfdoc_samb_drivers_spi_chsize SPI Character Size
 * The SPI character size is configurable to eight bits.
 *
 * \subsection asfdoc_samb_drivers_spi_master_mode Master Mode
 * When configured as a master, the SS pin will be configured as an output.
 *
 * \subsubsection asfdoc_samb_drivers_spi_master_mode_data_transfer Data Transfer
 * Writing a character will start the SPI clock generator, and
 * the character is transferred to the shift register when the shift
 * register is empty.
 * Once this is done, a new character can be written.
 * As each character is shifted out from the master, a character is shifted in
 * from the slave. If the receiver is enabled, the data is moved to the receive
 * buffer at the completion of the frame and can be read.
 *
 * \subsection asfdoc_samb_drivers_spi_slave_mode Slave Mode
 * When configured as a slave, the SPI interface will remain inactive with MISO
 * tri-stated as long as the SS pin is driven high.
 *
 * \subsubsection asfdoc_samb_drivers_spi_slave_mode_data_transfer_slave Data Transfer
 * The data register can be updated at any time.
 * As the SPI slave shift register is clocked by SCK, a minimum of three SCK
 * cycles are needed from the time new data is written, until the character is
 * ready to be shifted out. If the shift register has not been loaded with
 * data, the current contents will be transmitted.
 *
 * If constant transmission of data is needed in SPI slave mode, the system
 * clock should be faster than SCK.
 *
 * \subsection asfdoc_samb_drivers_spi_data_modes Data Modes
 * There are four combinations of SCK phase and polarity with respect to
 * serial data. \ref asfdoc_samb_drivers_spi_mode_table "The table below" shows the
 * clock polarity (CPOL) and clock phase (CPHA) in the different modes.
 * <i>Leading edge</i> is the first clock edge in a clock cycle and
 * <i>trailing edge</i> is the last clock edge in a clock cycle.
 *
 * \anchor asfdoc_samb_drivers_spi_mode_table
 * <table>
 *   <caption>SPI Data Modes</caption>
 *   <tr>
 *      <th>Mode</th>
 *      <th>CPOL</th>
 *      <th>CPHA</th>
 *      <th>Leading Edge</th>
 *      <th>Trailing Edge</th>
 *   </tr>
 *   <tr>
 *      <td> 0 </td>
 *      <td> 0 </td>
 *      <td> 0 </td>
 *      <td> Rising, Sample </td>
 *      <td> Falling, Setup </td>
 *   </tr>
 *   <tr>
 *      <td> 1 </td>
 *      <td> 0 </td>
 *      <td> 1 </td>
 *      <td> Rising, Setup </td>
 *      <td> Falling, Sample </td>
 *   </tr>
 *   <tr>
 *      <td> 2 </td>
 *      <td> 1 </td>
 *      <td> 0 </td>
 *      <td> Falling, Sample </td>
 *      <td> Rising, Setup </td>
 *   </tr>
 *   <tr>
 *      <td> 3 </td>
 *      <td> 1 </td>
 *      <td> 1 </td>
 *      <td> Falling, Setup </td>
 *      <td> Rising, Sample </td>
 *   </tr>
 * </table>
 *
 * \section asfdoc_samb_drivers_spi_special_considerations Special Considerations
 * The pin MUX settings must be configured properly, as not all settings
 * can be used in different modes of operation.
 *
 * \section asfdoc_samb_drivers_spi_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_samb_drivers_spi_extra. This includes:
 *  - \ref asfdoc_samb_drivers_spi_extra_acronyms
 *  - \ref asfdoc_samb_drivers_spi_extra_dependencies
 *  - \ref asfdoc_samb_drivers_spi_extra_errata
 *  - \ref asfdoc_samb_drivers_spi_extra_history
 *
 * \section asfdoc_samb_drivers_spi_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_samb_drivers_spi_exqsg.
 *
 * \section asfdoc_samb_drivers_spi_api_overview API Overview
 * @{
 */

#include <stdint.h>
#include <string.h>
#include <compiler.h>
#include <conf_spi.h>
#include <gpio.h>
#include <system_sam_b.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (CONF_SPI_MASTER_ENABLE == false) && (CONF_SPI_SLAVE_ENABLE == false)
#error "Not possible compile SPI driver, invalid driver configuration. Make sure that either/both CONF_SPI_MASTER_ENABLE/CONF_SPI_SLAVE_ENABLE is set to true."
#endif

#  ifndef SPI_TIMEOUT
/** SPI timeout value */
#  define SPI_TIMEOUT 10000
#  endif

#  ifndef PINMUX_UNUSED
/** Unused pinmux. */
#  define PINMUX_UNUSED 0xFFFFFFFF
#  endif

#  if SPI_CALLBACK_MODE == true
/** Prototype for the device instance. */
struct spi_module;
/** Type of the callback functions */
typedef void (*spi_callback_t)(struct spi_module *const module);

/**
 * \brief SPI Callback enum
 *
 * Callbacks for SPI callback driver.
 *
 * \note For slave mode, these callbacks will be called when a transaction
 * is ended by the master pulling Slave Select high.
 *
 */
enum spi_callback {
	/** Callback for buffer transmitted */
	SPI_CALLBACK_BUFFER_TRANSMITTED,
	/** Callback for buffer received */
	SPI_CALLBACK_BUFFER_RECEIVED,
	/** Callback for buffers transceived */
	SPI_CALLBACK_BUFFER_TRANSCEIVED,
	/** Callback for error */
	SPI_CALLBACK_ERROR,
#  if !defined(__DOXYGEN__)
	/** Number of available callbacks */
	SPI_CALLBACK_N,
#  endif
};

#  if !defined(__DOXYGEN__)
/** Prototype for the interrupt handler */
extern struct spi_module *_spi_instances[SPI_INST_NUM];
extern void spi_rx0_isr_handler(void);
extern void spi_tx0_isr_handler(void);
extern void spi_rx1_isr_handler(void);
extern void spi_tx1_isr_handler(void);
/**
 * \brief SPI transfer directions
 */
enum _spi_direction {
	/** Transfer direction is read. */
	SPI_DIRECTION_READ,
	/** Transfer direction is write. */
	SPI_DIRECTION_WRITE,
	/** Transfer direction is read and write. */
	SPI_DIRECTION_BOTH,
	/** No transfer. */
	SPI_DIRECTION_IDLE,
};
#  endif
#  endif

/**
 * \brief SPI transfer modes enum
 *
 * SPI transfer mode.
 */
enum spi_transfer_mode {
	/** Mode 0. Leading edge: rising, sample. Trailing edge: falling, setup */
	SPI_TRANSFER_MODE_0 = 0,
	/** Mode 1. Leading edge: rising, setup. Trailing edge: falling, sample */
	SPI_TRANSFER_MODE_1 = SPI_CONFIGURATION_SCK_PHASE_1,
	/** Mode 2. Leading edge: falling, sample. Trailing edge: rising, setup */
	SPI_TRANSFER_MODE_2 = SPI_CONFIGURATION_SCK_POLARITY_1,
	/** Mode 3. Leading edge: falling, setup. Trailing edge: rising, sample */
	SPI_TRANSFER_MODE_3 = SPI_CONFIGURATION_SCK_PHASE_1 | \
						SPI_CONFIGURATION_SCK_POLARITY_1,
};

/**
 * \brief SPI modes enum
 *
 * SPI mode selection.
 */
enum spi_mode {
	/** Master mode */
	SPI_MODE_MASTER         = 1,
	/** Slave mode */
	SPI_MODE_SLAVE          = 0,
};

/**
 * \brief SPI data order enum
 *
 * SPI data order.
 *
 */
enum spi_data_order {
	/** The MSB of the data is transmitted first */
	SPI_DATA_ORDER_MSB      = 0,
	/** The LSB of the data is transmitted first */
	SPI_DATA_ORDER_LSB      = SPI_CONFIGURATION_LSB_FIRST_ENABLE_1,
};

/**
 * \brief SPI module clock input
 *
 * SPI module clock.
 *
 */
enum spi_clock_input {
	/** source from clock input 0 26MHz*/
	SPI_CLK_INPUT_0 = 0,
	/** source from clock input 1 13MHz */
	SPI_CLK_INPUT_1,
	/** source from clock input 2 6.5MHz*/
	SPI_CLK_INPUT_2,
	/** source from clock input 3 3MHz*/
	SPI_CLK_INPUT_3,
};

/**
 * \brief  SPI driver software device instance structure.
 *
 *  SPI driver software instance structure, used to retain software state
 * information of an associated hardware module instance.
 *
 * \note The fields of this structure should not be altered by the user
 *       application; they are reserved for module-internal use only.
 */
struct spi_module {
#  if !defined(__DOXYGEN__)
	/** Hardware module */
	Spi *hw;
	/** Module lock */
	volatile uint8_t locked;
	/** SPI mode */
	enum spi_mode mode;
	/** Transmit dummy data when receiving*/
	uint8_t tx_dummy_byte;
#if SPI_CALLBACK_MODE == true
	/** Direction of transaction */
	volatile enum _spi_direction dir;
	/** Array to store callback function pointers in */
	spi_callback_t callback[SPI_CALLBACK_N];
	/** Buffer pointer to where the next received character will be put */
	volatile uint8_t *rx_buffer_ptr;
	/** Buffer pointer to where the next character will be transmitted from
	**/
	volatile uint8_t *tx_buffer_ptr;
	/** Remaining characters to receive */
	volatile uint16_t remaining_rx_buffer_length;
	/** Remaining dummy characters to send when reading */
	volatile uint16_t remaining_dummy_buffer_length;
	/** Remaining characters to transmit */
	volatile uint16_t remaining_tx_buffer_length;
	/** Bit mask for callbacks registered */
	uint8_t registered_callback;
	/** Bit mask for callbacks enabled */
	uint8_t enabled_callback;
	/** Holds the status of the ongoing or last operation */
	volatile enum status_code status;
#  endif
#  endif
};

/**
 * \brief SPI peripheral slave instance structure
 *
 * SPI peripheral slave software instance structure, used to configure the
 * correct SPI transfer mode settings for an attached slave. See
 * \ref spi_select_slave.
 */
struct spi_slave_inst {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Address recognition enabled in slave device */
	uint8_t address_enabled;
	/** Address of slave device */
	uint8_t address;
};

/**
 * \brief SPI peripheral slave configuration structure
 *
 * SPI Peripheral slave configuration structure
 */
struct spi_slave_inst_config {
	/** Pin to use as Slave Select */
	uint8_t ss_pin;
	/** Enable address */
	bool address_enabled;
	/** Address of slave */
	uint8_t address;
};

/**
 * \brief SPI configuration structure
 *
 * Configuration structure for an SPI instance. This structure should be
 * initialized by the \ref spi_get_config_defaults function before being
 * modified by the user application.
 */
struct spi_config {
	/** SPI mode */
	enum spi_mode mode;
	/** Data order */
	enum spi_data_order data_order;
	/** Transfer mode */
	enum spi_transfer_mode transfer_mode;
	/** clock source to use */
	enum spi_clock_input clock_source;
	/** clock divider value to use*/
	uint8_t clock_divider;
	/** SPI PAD pin number */
	uint32_t pin_number_pad[4];
	/** SPI PAD pinmux selection */
	uint32_t pinmux_sel_pad[4];
};

/**
 * \name Initialization functions
 * @{
 */

enum status_code spi_init(
		struct spi_module *const module,
		Spi *const hw,
		const struct spi_config *const config);

void spi_reset(struct spi_module *const module);

void spi_slave_inst_get_config_defaults(
		struct spi_slave_inst_config *const config);

void spi_get_config_defaults(struct spi_config *const config);

void spi_attach_slave(
		struct spi_slave_inst *const slave,
		struct spi_slave_inst_config *const config);

/** @} */

/**
 * \name Enable/Disable
 * @{
 */

void spi_enable(struct spi_module *const module);
void spi_disable(struct spi_module *const module);

/** @} */

/**
 * \name Lock/Unlock
 * @{
 */

enum status_code spi_lock(struct spi_module *const module);
void spi_unlock(struct spi_module *const module);

/** @} */


/**
 * \name Read/Write
 * @{
 */

enum status_code spi_write(
		struct spi_module *module,
		uint8_t tx_data);

enum status_code spi_write_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint16_t length);

enum status_code spi_read(
		struct spi_module *const module,
		uint8_t *rx_data);

enum status_code spi_read_buffer_wait(
		struct spi_module *const module,
		uint8_t *rx_data,
		uint16_t length,
		uint8_t dummy);

enum status_code spi_transceive_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data);

enum status_code spi_transceive_buffer_wait(
		struct spi_module *const module,
		uint8_t *tx_data,
		uint8_t *rx_data,
		uint16_t length);

enum status_code spi_select_slave(
		struct spi_module *const module,
		struct spi_slave_inst *const slave,
		bool select);

/** @} */

/** @}*/

#ifdef __cplusplus
}
#endif

/**
 * \page asfdoc_samb_drivers_spi_extra Extra Information for SPI Driver
 *
 * \section asfdoc_samb_drivers_spi_extra_acronyms Acronyms
 * The table below presents the acronyms used in this module:
 *
 * <table>
 *	<tr>
 *		<th>Acronym</th>
 *		<th>Description</th>
 *	</tr>
 *	<tr>
 *		<td>SPI</td>
 *		<td>Serial Peripheral Interface</td>
 *	</tr>
 *	<tr>
 *		<td>SCK</td>
 *		<td>Serial Clock</td>
 *	</tr>
 *	<tr>
 *		<td>MOSI</td>
 *		<td>Master Output Slave Input</td>
 *	</tr>
 *	<tr>
 *		<td>MISO</td>
 *		<td>Master Input Slave Output</td>
 *	</tr>
 *	<tr>
 *		<td>SS</td>
 *		<td>Slave Select</td>
 *	</tr>
 * </table>
 *
 * \section asfdoc_samb_drivers_spi_extra_dependencies Dependencies
 * There are no dependencies related to this driver.
 *
 *
 * \section asfdoc_samb_drivers_spi_extra_errata Errata
 * There are no errata related to this driver.
 *
 * \section asfdoc_samb_drivers_spi_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial Release</td>
 *	</tr>
 * </table>
 */

/**
 * \page asfdoc_samb_drivers_spi_exqsg Examples for SPI Driver
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_samb_drivers_spi_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that QSGs can be compiled as a standalone application or be
 * added to the user application.
 *
 * - \subpage asfdoc_samb_spi_master_basic_use
 * - \subpage asfdoc_samb_spi_slave_basic_use
 * \if SPI_CALLBACK_MODE
 * - \subpage asfdoc_samb_spi_master_callback_use
 * - \subpage asfdoc_samb_spi_slave_callback_use
 * \endif
 */
 
 /**
 *
 * \page asfdoc_samb_drivers_spi_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>A</td>
 *		<td>09/2015</td>
 *		<td>Initial release</td>
 *	</tr>
 * </table>
 */

#endif //SPI_H_INCLUDED

