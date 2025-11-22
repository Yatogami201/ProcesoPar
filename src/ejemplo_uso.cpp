#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ProcesoPar.h"

Estado_t mi_funcion_escucha(const char* mensaje, int longitud) {
    printf("\n[PADRE] Mensaje del hijo (%d bytes): ", longitud);
    write(STDOUT_FILENO, mensaje, longitud);
    printf("\n");
    return E_OK;
}

int main() {
    ProcesoPar_t* proceso;
    
    // ✅ Usar un programa que SÍ lea de stdin y escriba a stdout
    const char* comando[] = {"cat", NULL};
    
    printf("[PADRE] Lanzando proceso 'cat'...\n");
    Estado_t estado = lanzarProcesoPar("/bin/cat", comando, &proceso);
    if (estado != E_OK) {
        printf("Error al lanzar proceso: %d\n", estado);
        return 1;
    }
    
    printf("[PADRE] Estableciendo función de escucha...\n");
    establecerFuncionDeEscucha(proceso, mi_funcion_escucha);
    
    // ✅ Enviar mensajes que cat procesará y devolverá
    const char* mensajes[] = {
        "Hola proceso hijo\n",
        "Este es un mensaje de prueba\n",
        "Adios\n"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("[PADRE] Enviando mensaje %d...\n", i+1);
        enviarMensajeProcesoPar(proceso, mensajes[i], strlen(mensajes[i]));
        sleep(1);
    }
    
    printf("[PADRE] Destruyendo proceso par...\n");
    destruirProcesoPar(proceso);
    printf("[PADRE] Proceso terminado correctamente.\n");
    
    return 0;
}
