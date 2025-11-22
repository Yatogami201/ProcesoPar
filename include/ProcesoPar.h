#ifndef PROCESOPAR_H
#define PROCESOPAR_H

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

typedef unsigned int Estado_t;

#define E_OK 0
#define E_PAR_INC 1
#define E_FORK_FAIL 2
#define E_PIPE_FAIL 3
#define E_EXEC_FAIL 4
#define E_THREAD_FAIL 5

typedef struct ProcesoPar {
    pid_t pid;
    int pipe_write;
    int pipe_read;
    int running;
    Estado_t (*callback)(const char*, int);
    pthread_t listener_thread;
    pthread_mutex_t mutex;
} ProcesoPar_t;

Estado_t lanzarProcesoPar(const char* nombreArchivoEjecutable,
                         const char** listaLineaComando,
                         ProcesoPar_t** procesoPar);

Estado_t destruirProcesoPar(ProcesoPar_t* procesoPar);

Estado_t enviarMensajeProcesoPar(ProcesoPar_t* procesoPar,
                                const char* mensaje,
                                int longitud);

Estado_t establecerFuncionDeEscucha(ProcesoPar_t* procesoPar,
                                   Estado_t (*f)(const char*, int));

#endif
