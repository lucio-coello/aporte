// funciones.h
#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_LINEA 512
#define MAX_IPS 200
#define UMBRAL_FALLIDOS 5

typedef struct {
    char ip[40];
    int intentos_fallidos;
    int bloqueada;
} IPInfo;

int buscar_o_agregar(IPInfo ips[], int *total, const char *ip);
void extraer_ip(const char *linea, char *ip_dest);
void analizar_log(const char *nombre_archivo);

#endif
