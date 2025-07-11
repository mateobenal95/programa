#include <stdio.h>
#include <stdlib.h>
#include "zona.h"

int main() {
    struct Zona zonas[5];
    
    // Leer datos desde archivo binario (si existen)
    leerDatos(zonas, "datos.bin");

    int opcion;
    do {
        printf("\n1. Ingresar datos actuales\n2. Tabla historico, estado actual y prediccion\n3. Mostrar datos climaticos y de contaminacion actuales\n4. Alertas\n5. Recomendaciones\n6. Exportar datos\n7. Salir\nOpcion: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1: ingresarDatosActuales(zonas); break;  // Ingresar datos actuales
            case 2: mostrarTablaHistoricoActualPrediccion(zonas); break;  // Tabla histórica, estado actual y predicción
            case 3: mostrarDatosClimaticos(zonas); break; // Mostrar datos climáticos y de contaminación actuales
            case 4: emitirAlertas(zonas); break;
            case 5: generarRecomendaciones(zonas); break;
            case 6: exportarDatos(zonas, "reporte.txt"); break;
        }
    } while(opcion != 7);

    return 0;
}
