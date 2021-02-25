#include <stdio.h>
#include <stdint.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "tco_libd.h"

#define I2C_PATH "/dev/i2c-"

int i2c_port_open(int interface_id)
{
    char path[sizeof(I2C_PATH) + 1] = I2C_PATH "%i";
    sprintf(path, path, interface_id);
    return open(path, O_RDWR);
}

i2c_error_t i2c_cmd_write(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t data, uint8_t data_send)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }
    if (data_send)
    {
        if (i2c_smbus_write_byte_data(i2c_port_fd, cmd, data) != 0)
        {
            log_error("Failed to send command and data");
            log_error("i2c_smbus_write_byte_data: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }
    else
    {
        if (i2c_smbus_write_byte(i2c_port_fd, cmd) != 0)
        {
            log_error("Failed to send command");
            log_error("i2c_smbus_write_byte: %s", strerror(errno));
            return ERR_I2C_WRITE;
        }
    }

    return ERR_OK;
}

i2c_error_t i2c_cmd_read(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t *output)
{
    if (ioctl(i2c_port_fd, I2C_SLAVE, i2c_addr) < 0)
    {
        log_error("Failed to send the slave address");
        log_error("ioctl: %s", strerror(errno));
        return ERR_I2C_CTRL;
    }
    int const output_tmp = i2c_smbus_read_byte_data(i2c_port_fd, cmd);
    if (output_tmp < 0)
    {
        log_error("Failed to send command and read the response");
        log_error("i2c_smbus_read_byte: %s", strerror(errno));
        return ERR_I2C_READ;
    }
    *output = output_tmp;
    return ERR_OK;
}
