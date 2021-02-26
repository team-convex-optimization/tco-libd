#ifndef _TCO_LIBD_I2C_H_
#define _TCO_LIBD_I2C_H_

#include <stdint.h>

/* I2C status codes */
typedef enum i2c_error_t
{
    ERR_OK = 0, /* Weird naming but its done for consistency. */
    ERR_UNK = 1,
    ERR_CRIT,
    ERR_I2C_WRITE,
    ERR_I2C_READ,
    ERR_I2C_CTRL
} i2c_error_t;

/**
 * @brief Open an I2C port.
 * @param interface_id  The interface to open. E.g. 1 means i2c-1 will be opened (if it exists).
 * @return File descriptor of the opened port on success, -1 on failure.
 */
int i2c_port_open(uint8_t interface_id);

/**
 * @brief Send a command with data. E.g. Send a register address, and write to its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param data The data that will be sent to the slave.
 * @param data_send When >0 data will be sent after command, when =0 data will be ignored.
 * @return Status code
 */
i2c_error_t i2c_cmd_write(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t data, uint8_t data_send);

/**
 * @brief Send a command and receive output. E.g. Send a register address, and read its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param output Pointer where the output will be written. Undefined on failure.
 * @return Status code
 */
i2c_error_t i2c_cmd_read(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t *output);

#endif /* _TCO_LIBD_I2C_H_ */
