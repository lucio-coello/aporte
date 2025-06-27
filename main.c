// main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"

void mostrar_menu() {
    printf("\n===== DETECTOR DE FUERZA BRUTA =====\n");
    printf("1. Escanear archivo de log\n");
    printf("2. Salir\n");
    printf("Seleccione una opcion: ");
}

int main() {
    int opcion;
    char archivo[100];

    while (1) {
        mostrar_menu();
        scanf("%d", &opcion);
        getchar(); 
        switch (opcion) {
            case 1:
                printf("Ingrese el nombre del archivo a escanear: ");
                fgets(archivo, sizeof(archivo), stdin);
                archivo[strcspn(archivo, "\n")] = '\0'; 
                FILE *test = fopen(archivo, "r");
                if (!test) {
                    printf(" El archivo '%s' no existe o no se puede abrir.\n", archivo);
                } else {
                    fclose(test);
                    analizar_log(archivo);
                }
                break;

            case 2:
                printf("Saliendo del programa.\n");
                return 0;

            default:
                printf(" Opcion no valida. Intente de nuevo.\n");
        }
    }

    return 0;
}