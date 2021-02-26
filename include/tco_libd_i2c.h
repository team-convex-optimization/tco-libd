#ifndef _TCO_LIBD_I2C_H_
#define _TCO_LIBD_I2C_H_

#include <stdint.h>
#include "tco_libd.h"

/**
 * @brief Open an I2C port.
 * @param interface_id  The interface to open. E.g. 1 means i2c-1 will be opened (if it exists).
 * @param fd_loc Location where the file descriptor will be written.
 * @return Status code.
 */
error_t i2c_port_open(uint8_t const interface_id, int *fd_loc);

/**
 * @brief Send a command with data. E.g. Send a register address, and write to its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param data Pointer to the data that will be sent to the slave. If there is no data, NULL is
 * accepted.
 * @param data_size Indicates the number of bytes that will be read from "data" and sent to the
 * slave in a row after sending the command.
 * @return Status code.
 */
error_t i2c_cmd_write(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const data, uint8_t const data_size);

/**
 * @brief Send a command and receive output. E.g. Send a register address, and read its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param output Pointer where the output will be written. Undefined on failure.
 * @param output_size The number of bytes that are expected to be sent back by the slave.
 * @return Status code.
 */
error_t i2c_cmd_read(int const i2c_port_fd, uint8_t const i2c_addr, uint8_t const cmd, uint8_t *const output, uint8_t const output_size);

#endif /* _TCO_LIBD_I2C_H_ */
