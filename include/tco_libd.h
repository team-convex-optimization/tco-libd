#ifndef _TCO_LIBD_H_
#define _TCO_LIBD_H_

typedef enum error_t
{
    ERR_OK = 0, /* Weird naming but its done for consistency. */
    ERR_UNK = 1,
    ERR_CRIT,
    ERR_I2C_WRITE,
    ERR_I2C_READ,
    ERR_I2C_CTRL,
    ERR_GPIO_READ,
    ERR_GPIO_WRITE,
    ERR_GPIO_CTRL,
} error_t;

#include "tco_libd_ipc.h"
#include "tco_libd_log.h"
#include "tco_libd_i2c.h"
#include "tco_libd_gpio.h"

#endif /* _TCO_LIBD_H_ */
