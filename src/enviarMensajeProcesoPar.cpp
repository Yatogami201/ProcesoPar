#include "ProcesoPar.h"
#include <unistd.h>

Estado_t enviarMensajeProcesoPar(ProcesoPar_t* procesoPar,
                                const char* mensaje,
                                int longitud) {
    if (!procesoPar || !mensaje || longitud <= 0)
        return E_PAR_INC;

    pthread_mutex_lock(&procesoPar->mutex);
    ssize_t escritos = write(procesoPar->pipe_write, mensaje, longitud);
    pthread_mutex_unlock(&procesoPar->mutex);

    if (escritos != longitud)
        return E_PIPE_FAIL;

    return E_OK;
}
