#include "ProcesoPar.h"
#include <unistd.h>

static void* listenerThread(void* arg) {
    ProcesoPar_t* p = (ProcesoPar_t*)arg;
    char buffer[1024];
    ssize_t bytes_read;

    while (p->running) {
        bytes_read = read(p->pipe_read, buffer, sizeof(buffer));
        if (bytes_read > 0 && p->callback) {
            p->callback(buffer, bytes_read);
        } else if (bytes_read <= 0) {
            break;
        }
    }
    return NULL;
}

Estado_t establecerFuncionDeEscucha(ProcesoPar_t* procesoPar,
                                   Estado_t (*f)(const char*, int)) {
    if (!procesoPar || !f)
        return E_PAR_INC;

    procesoPar->callback = f;
    
    if (pthread_create(&procesoPar->listener_thread, NULL, listenerThread, procesoPar) != 0) {
        return E_THREAD_FAIL;
    }

    return E_OK;
}
