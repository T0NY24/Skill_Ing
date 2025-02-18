#include <stdio.h>
#include <stdlib.h>

int* asignar_memoria(int valor) {
    int* ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        fprintf(stderr, "Error asignando memoria");
        exit(EXIT_FAILURE);
    }
    *ptr = valor;
    return ptr;
}

void mostrar_informacion(int* ptr) {
    printf("\nDireccion almacenada en puntero: %p", (void*)ptr);
    printf("\nValor almacenado: %d\n", *ptr);
}

void liberar_memoria(int* ptr) {
    free(ptr);
    ptr = NULL;
}

int main() {
    int* puntero = asignar_memoria(5);
    
    printf("=== Estado inicial ===");
    mostrar_informacion(puntero);
    
    printf("\n=== Modificacion ===");
    *puntero = 15;
    mostrar_informacion(puntero);
    
    liberar_memoria(puntero);
    return 0;
}