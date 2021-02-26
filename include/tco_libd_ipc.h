#ifndef _TCO_LIBD_IPC_H_
#define _TCO_LIBD_IPC_H_

#include <semaphore.h>
#include <stdint.h>

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

#endif /* _TCO_LIBD_IPC_H_ */