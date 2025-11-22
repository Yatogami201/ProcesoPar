#include "ProcesoPar.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

Estado_t lanzarProcesoPar(const char* nombreArchivoEjecutable,
                         const char** listaLineaComando,
                         ProcesoPar_t** procesoPar) {
    if (!nombreArchivoEjecutable || !procesoPar)
        return E_PAR_INC;

    int pipe_to_child[2];
    int pipe_from_child[2];

    if (pipe(pipe_to_child) == -1) return E_PIPE_FAIL;
    if (pipe(pipe_from_child) == -1) {
        close(pipe_to_child[0]);
        close(pipe_to_child[1]);
        return E_PIPE_FAIL;
    }

    pid_t pid = fork();
    if (pid == -1) {
        close(pipe_to_child[0]);
        close(pipe_to_child[1]);
        close(pipe_from_child[0]);
        close(pipe_from_child[1]);
        return E_FORK_FAIL;
    }

    if (pid == 0) { // Hijo
        close(pipe_to_child[1]);
        close(pipe_from_child[0]);

        dup2(pipe_to_child[0], STDIN_FILENO);
        dup2(pipe_from_child[1], STDOUT_FILENO);

        close(pipe_to_child[0]);
        close(pipe_from_child[1]);

        char** args = (char**)listaLineaComando;
        execv(nombreArchivoEjecutable, args);
        _exit(E_EXEC_FAIL);
    }
    else { // Padre
        close(pipe_to_child[0]);
        close(pipe_from_child[1]);

        ProcesoPar_t* pPar = (ProcesoPar_t*)malloc(sizeof(ProcesoPar_t));
        if (!pPar) {
            close(pipe_to_child[1]);
            close(pipe_from_child[0]);
            return E_PAR_INC;
        }

        pPar->pid = pid;
        pPar->pipe_write = pipe_to_child[1];
        pPar->pipe_read = pipe_from_child[0];
        pPar->running = 1;
        pPar->callback = NULL;
        pthread_mutex_init(&pPar->mutex, NULL);

        *procesoPar = pPar;
        return E_OK;
    }
}
