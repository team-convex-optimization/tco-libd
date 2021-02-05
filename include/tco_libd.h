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

#endif /* _TCO_LIBD_H_ */
