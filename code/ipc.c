#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/mman.h>

#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#include "tco_libd.h"

int shmem_map(
    const char *shmem_name,
    const uint32_t shmem_size,
    const char *shmem_sem_name,
    const int permissions,
    void **shmem,
    sem_t **shmem_sem)
{
    int page_permissions = 0;
    if (permissions == O_RDONLY)
    {
        page_permissions = PROT_READ;
    }
    else if (permissions == O_WRONLY)
    {
        page_permissions = PROT_WRITE;
    }
    else if (permissions == O_RDWR)
    {
        page_permissions = PROT_READ | PROT_WRITE;
    }
    else
    {
        log_error("Invalid permissions.\n");
        return -1;
    }

    int shmem_fd = shm_open(shmem_name, permissions, 0666);
    if (shmem_fd == -1)
    {
        log_error("shm_open: %s", strerror(errno));
        log_error("Failed to open the shared memory object");
        return -1;
    }
    *shmem = (struct tco_shmem_data_control *)mmap(0, shmem_size, page_permissions, MAP_SHARED, shmem_fd, 0);
    if (*shmem == MAP_FAILED)
    {
        log_error("mmap: %s", strerror(errno));
        log_error("Failed to map the shared memory object into memory");
        return -1;
    }
    if (close(shmem_fd) == -1) /* No longer needed. */
    {
        log_error("close: %s", strerror(errno)); /* Not a critical error. */
    }

    *shmem_sem = sem_open(shmem_sem_name, 0);
    if (*shmem_sem == SEM_FAILED)
    {
        log_error("sem_open: %s", strerror(errno));
        log_error("Failed to open the semaphore associated with the shared memory object '%s'", shmem_name);
        return -1;
    }
    return EXIT_SUCCESS;
}
