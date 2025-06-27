// programa.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funciones.h"

int buscar_o_agregar(IPInfo ips[], int *total, const char *ip) {
    for (int i = 0; i < *total; i++) {
        if (strcmp(ips[i].ip, ip) == 0)
            return i;
    }
    // Nueva IP
    strcpy(ips[*total].ip, ip);
    ips[*total].intentos_fallidos = 0;
    ips[*total].bloqueada = 0;
    (*total)++;
    return (*total) - 1;
}

void extraer_ip(const char *linea, char *ip_dest) {
    const char *ptr = strstr(linea, "IP: ");
    if (ptr != NULL) {
        sscanf(ptr, "IP: %s", ip_dest);
    }
}

void analizar_log(const char *nombre_archivo) {
    FILE *log = fopen(nombre_archivo, "r");
    if (!log) {
        perror("Error abriendo archivo de log");
        return;
    }

    char archivo_bloqueadas[100], archivo_intentos[100];
    // ✅ Archivos fijos para todos los escaneos
    strcpy(archivo_bloqueadas, "informe_bloqueadas.txt");
    strcpy(archivo_intentos, "informe_intentos.txt");

    FILE *bloqueadas = fopen(archivo_bloqueadas, "a"); // modo append
    FILE *intentos = fopen(archivo_intentos, "a");     // modo append
    if (!bloqueadas || !intentos) {
        perror("Error creando informes");
        fclose(log);
        return;
    }

    // Obtener fecha y hora actual
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Escribir encabezado con fecha y nombre del archivo escaneado
    fprintf(intentos, "\n==== Escaneo realizado el %s | Archivo: %s ====\n", timestamp, nombre_archivo);
    fprintf(bloqueadas, "\n==== Escaneo realizado el %s | Archivo: %s ====\n", timestamp, nombre_archivo);

    IPInfo ips[MAX_IPS];
    int total_ips = 0;
    char linea[MAX_LINEA];

    while (fgets(linea, sizeof(linea), log)) {
        if (strstr(linea, "Status: FAILED")) {
            char ip[40];
            extraer_ip(linea, ip);
            int idx = buscar_o_agregar(ips, &total_ips, ip);
            ips[idx].intentos_fallidos++;
        }
    }

    for (int i = 0; i < total_ips; i++) {
        fprintf(intentos, "IP: %s - Intentos fallidos: %d\n", ips[i].ip, ips[i].intentos_fallidos);
        if (ips[i].intentos_fallidos >= UMBRAL_FALLIDOS) {
            ips[i].bloqueada = 1;
            fprintf(bloqueadas, "🔒 IP bloqueada: %s (intentos fallidos: %d)\n", ips[i].ip, ips[i].intentos_fallidos);
            printf(" IP bloqueada: %s\n", ips[i].ip);
        }
    }

    fclose(log);
    fclose(bloqueadas);
    fclose(intentos);

    printf("Informe actualizado: %s\n", archivo_bloqueadas);
    printf("Resumen de intentos: %s\n", archivo_intentos);
}
