#ifndef _TCO_LIBD_H_
#define _TCO_LIBD_H_

#include <stdint.h>
#include <semaphore.h>

/**
 * @brief Map shared memory object into memory with its associated semaphore.
 * @note This function expects that logger has been initialized.
 * @param shmem_name Name of the shared memory object
 * @param shmem_sem_name Name of the semaphore
 * @param permissions Shared memory page mapping access permissions. Valid values are O_RDONLY,
 * O_WRONLY, O_RDWR from fcntl.h.
 * @param shmem Location where a pointer to the shared memory will be written
 * @param shmem_sem Location where a pointer to the shared memory semaphore will be written
 * @return 0 on success and -1 on failure
 */
int shmem_map(
    const char *shmem_name,
    const uint32_t shmem_size,
    const char *shmem_sem_name,
    int permissions,
    void **shmem,
    sem_t **shmem_sem);

/* Log level flags */
enum log_flags
{
    LOG_OFF = 0,
    LOG_ERROR = 1 << 0,
    LOG_INFO = 1 << 1,
    LOG_DEBUG = 1 << 2,
};

/**
 * @brief Initializes the logger. Call to this function is necessary to use logging functions.
 * @param caller Name of the caller which is included in every log entry.
 * @param log_path Path where to store the log.
 * @return 0 on success, -1 on failure. On failure, calls to log functions lead to undefined
 * behavior.
 */
int log_init(char *caller, const char *log_path);

/**
 * @brief Deinitializes the logger. It will close the log file and reset the caller name.
 * @return 0 on success, -1 on failure. On failure, the caller name is guaranteed to persist.
 */
int log_deinit();

void log_error(const char *msg, ...);
void log_info(const char *msg, ...);
void log_debug(const char *msg, ...);

/* I2C status codes */
typedef enum i2c_error_t
{
    ERR_OK = 0, /* Weird naming but its done for consistency. */
    ERR_UNK = 1,
    ERR_CRIT,
    ERR_NOMEM,
    ERR_I2C_WRITE,
    ERR_I2C_READ,
    ERR_I2C_CTRL
} i2c_error_t;

/**
 * @brief Open an I2C port.
 * @param interface_id  The interface to open. E.g. 1 means i2c-1 will be opened (if it exists).
 * @return File descriptor of the opened port on success, -1 on failure.
 */
int i2c_port_open(int interface_id);

/**
 * @brief Send a command with data. E.g. Send a register address, and write to its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param data The data that will be sent to the slave.
 * @return Status code
 */
i2c_error_t i2c_cmd_write(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t data);

/**
 * @brief Send a command and receive output. E.g. Send a register address, and read its contents.
 * @param i2c_port_fd File descriptor of the i2c port.
 * @param cmd The byte that will be interpreted by the slave.
 * @param output Pointer where the output will be written. Undefined on failure.
 * @return Status code
 */
i2c_error_t i2c_cmd_read(int i2c_port_fd, uint8_t i2c_addr, uint8_t cmd, uint8_t *output);

#endif /* _TCO_LIBD_H_ */
