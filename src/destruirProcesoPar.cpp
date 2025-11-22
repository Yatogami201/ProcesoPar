#include "ProcesoPar.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

Estado_t destruirProcesoPar(ProcesoPar_t* procesoPar) {
    if (!procesoPar)
        return E_PAR_INC;

    // Primero cerrar pipes para desbloquear lecturas/escrituras
    close(procesoPar->pipe_write);
    close(procesoPar->pipe_read);

    // Luego indicar que debe terminar
    procesoPar->running = 0;

    // Esperar al hilo de escucha
    pthread_join(procesoPar->listener_thread, NULL);

    // Terminar proceso hijo
    kill(procesoPar->pid, SIGTERM);
    waitpid(procesoPar->pid, NULL, 0);

    pthread_mutex_destroy(&procesoPar->mutex);
    free(procesoPar);
    
    return E_OK;
}
