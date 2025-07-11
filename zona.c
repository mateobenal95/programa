#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zona.h"

// Definimos la constante LIMITE_PM25_OMS_2025
const float LIMITE_PM25_OMS_2025 = 15.0f; // Limite OMS 2025 para PM2.5 (24h): 15 ug/m3

// Predicción para el siguiente día usando el promedio de los últimos 30 días y el valor actual
float prediccionSiguienteDia(float *hist, int n, float actual) {
    float suma = 0;
    for(int i = 0; i < n; i++) {
        suma += hist[i];
    }
    float promedio = suma / n;
    // Predicción simple: promedio entre el promedio histórico y el valor actual
    return (promedio + actual) / 2.0f;
}

// Función para inicializar las zonas
void inicializarZonas(struct Zona *zonas) {
    strcpy(zonas[0].nombre, "Centro-Norte (Cotocollao)");
    for(int i = 0; i < 30; i++) zonas[0].historico[i] = 17.86;
    zonas[0].actual = 10.5;
    zonas[0].temperatura = 18.0;
    zonas[0].viento = 2.0;
    zonas[0].humedad = 60.0;
    zonas[0].limite = 15.0f;

    strcpy(zonas[1].nombre, "Carapungo");
    for(int i = 0; i < 30; i++) zonas[1].historico[i] = 18.6;
    zonas[1].actual = 8.0;
    zonas[1].temperatura = 19.0;
    zonas[1].viento = 2.5;
    zonas[1].humedad = 58.0;
    zonas[1].limite = 15.0f;

    strcpy(zonas[2].nombre, "Tumbaco");
    for(int i = 0; i < 30; i++) zonas[2].historico[i] = 13.04;
    zonas[2].actual = 6.0;
    zonas[2].temperatura = 20.0;
    zonas[2].viento = 3.0;
    zonas[2].humedad = 55.0;
    zonas[2].limite = 15.0f;

    strcpy(zonas[3].nombre, "Centro Historico");
    for(int i = 0; i < 30; i++) zonas[3].historico[i] = 25.0;
    zonas[3].actual = 28.0;
    zonas[3].temperatura = 17.0;
    zonas[3].viento = 2.2;
    zonas[3].humedad = 62.0;
    zonas[3].limite = 15.0f;

    strcpy(zonas[4].nombre, "Guamani");
    for(int i = 0; i < 30; i++) zonas[4].historico[i] = 17.6;
    zonas[4].actual = 7.5;
    zonas[4].temperatura = 18.5;
    zonas[4].viento = 2.8;
    zonas[4].humedad = 59.0;
    zonas[4].limite = 15.0f;
}

// Función para ingresar datos actuales de contaminación, temperatura, viento y humedad
void ingresarDatosActuales(struct Zona *zonas) {
    int zonaSeleccionada;
    
    printf("\nSeleccione una zona (0 a 4):\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i, zonas[i].nombre);
    }
    printf("Opcion: ");
    scanf("%d", &zonaSeleccionada);

    if (zonaSeleccionada < 0 || zonaSeleccionada >= 5) {
        printf("Opcion invalida. Por favor, seleccione una zona valida.\n");
        return;
    }

    printf("\nIngrese los datos para la zona: %s\n", zonas[zonaSeleccionada].nombre);
    printf("Contaminacion actual (ug/m3): ");
    scanf("%f", &zonas[zonaSeleccionada].actual);
    printf("Temperatura actual (C): ");
    scanf("%f", &zonas[zonaSeleccionada].temperatura);
    printf("Viento actual (m/s): ");
    scanf("%f", &zonas[zonaSeleccionada].viento);
    printf("Humedad actual (%%): ");
    scanf("%f", &zonas[zonaSeleccionada].humedad);

    // Actualizamos el archivo binario después de ingresar los datos
    actualizarArchivo(zonas, "datos.bin");
}

// Función para actualizar el archivo binario con los datos actuales
void actualizarArchivo(struct Zona *zonas, const char *archivo) {
    FILE *f = fopen(archivo, "wb");
    if(!f) {
        printf("No se pudo abrir el archivo para actualizar.\n");
        return;
    }
    fwrite(zonas, sizeof(struct Zona), 5, f);  // Guardar los datos binarios al archivo
    fclose(f);
    printf("Datos actualizados en %s\n", archivo);
}

// Función para mostrar la tabla histórica, actual y predicción
void mostrarTablaHistoricoActualPrediccion(struct Zona *zonas) {
    printf("\n%-30s %-25s %-15s %-20s %-20s %-20s\n",
           "Zona", "Historico (ug/m3)", "Actual", "Prediccion manana", "Estado Historico", "Estado Actual");

    for(int i = 0; i < 5; i++) {
        float suma = 0;
        for(int j = 0; j < 30; j++) suma += zonas[i].historico[j];
        float promedio = suma / 30;
        float pred = prediccionSiguienteDia(zonas[i].historico, 30, zonas[i].actual);

        // Verificamos si el valor supera el limite para estado histórico
        const char *estadoHistorico = (promedio > zonas[i].limite) ? "Excede" : "OK";

        // Verificamos si el valor actual supera el limite para estado actual
        const char *estadoActual = (zonas[i].actual > zonas[i].limite) ? "Excede" : "OK";

        // Imprimir las columnas de estado histórico y estado actual correctamente
        printf("%-30s %-25.2f %-15.2f %-20.2f %-20s %-20s\n",
               zonas[i].nombre, promedio, zonas[i].actual, pred, estadoHistorico, estadoActual);
    }
}

// Función para mostrar los datos climáticos
void mostrarDatosClimaticos(struct Zona *zonas) {
    printf("\n%-30s %-20s %-20s %-20s %-20s\n", "Zona", "Contaminacion (ug/m3)", "Temperatura (C)", "Viento (m/s)", "Humedad (%)");
    for(int i = 0; i < 5; i++) {
        printf("%-30s %-20.2f %-20.2f %-20.2f %-20.2f\n", zonas[i].nombre, zonas[i].actual, zonas[i].temperatura, zonas[i].viento, zonas[i].humedad);
    }
}

// Función para emitir alertas si los niveles de contaminación exceden los límites
void emitirAlertas(struct Zona *zonas) {
    printf("\n--- Alertas Preventivas ---\n");
    for(int i = 0; i < 5; i++) {
        float pred = prediccionSiguienteDia(zonas[i].historico, 30, zonas[i].actual);
        if(zonas[i].actual > zonas[i].limite || pred > zonas[i].limite) {
            printf("ALERTA: %s supera el limite aceptable (Actual: %.2f, Prediccion: %.2f, Limite: %.2f)\n", zonas[i].nombre, zonas[i].actual, pred, zonas[i].limite);
        }
    }
}

// Función para generar recomendaciones si la contaminación excede los límites
void generarRecomendaciones(struct Zona *zonas) {
    printf("\n--- Recomendaciones de Mitigacion ---\n");
    for(int i = 0; i < 5; i++) {
        float pred = prediccionSiguienteDia(zonas[i].historico, 30, zonas[i].actual);
        if(zonas[i].actual > zonas[i].limite || pred > zonas[i].limite) {
            printf("%-30s (Limite: %.2f ug/m3):\n", zonas[i].nombre, zonas[i].limite);
            if (i == 0) {
                // Centro-Norte (Cotocollao)
                printf("- Implementar restricciones de circulacion de vehiculos durante las horas pico, especialmente en los dias de mayor contaminacion.\n");
                printf("- Fomentar el uso de transporte publico y bicicletas para reducir el trafico y las emisiones vehiculares.\n");
                printf("- Incrementar la plantacion de arboles y la creacion de areas verdes para mejorar la calidad del aire.\n");
                printf("- Establecer campanas de concientizacion sobre los efectos del aire contaminado, especialmente en ninos y personas vulnerables.\n");
                printf("- Implementar medidas para reducir las emisiones de los sectores industriales cercanos.\n");
            } else if (i == 1) {
                // Carapungo
                printf("- Promover el uso de vehiculos electricos y otras alternativas sostenibles en la comunidad.\n");
                printf("- Establecer dias de restriccion vehicular para mejorar la calidad del aire en dias con altos niveles de contaminacion.\n");
                printf("- Mejorar la infraestructura de transporte publico limpio y accesible para que los residentes puedan desplazarse sin depender de vehiculos particulares.\n");
                printf("- Aumentar los esfuerzos de arborizacion urbana y crear jardines comunitarios en las zonas mas afectadas por la contaminacion.\n");
                printf("- Fortalecer el control de las emisiones de vehiculos y promover inspecciones periodicas de los automoviles.\n");
            } else if (i == 2) {
                // Tumbaco
                printf("- Incentivar el teletrabajo y clases virtuales en dias con niveles de contaminacion elevados, para reducir la exposicion de la poblacion al aire contaminado.\n");
                printf("- Mejorar la infraestructura de transporte publico con opciones de transporte mas eficientes y menos contaminantes, como autobuses a gas o hibridos, para reducir las emisiones de gases contaminantes.\n");
                printf("- Fomentar la reforestacion y el mantenimiento de areas verdes para capturar contaminantes y mejorar la calidad del aire.\n");
                printf("- Establecer medidas para reducir la contaminacion de las actividades agricolas cercanas, como el uso controlado de pesticidas y fertilizantes.\n");
                printf("- Realizar campanas de sensibilizacion para que los residentes adopten practicas de movilidad mas sostenibles.\n");
            } else if (i == 3) {
                // Centro Historico
                printf("- Desarrollar una zona de bajas emisiones para restringir el acceso de vehiculos de combustion en el area central durante las horas de mayor contaminacion.\n");
                printf("- Impulsar el uso de vehiculos electricos y otros medios de transporte no contaminantes dentro del Centro Historico.\n");
                printf("- Mejorar la red de bicicletas y peatonalizacion en las calles mas congestionadas para reducir la emision de contaminantes y promover estilos de vida saludables.\n");
                printf("- Promover la restauracion y mantenimiento de los edificios historicos con tecnicas y materiales que minimicen la contaminacion.\n");
                printf("- Implementar un sistema de monitoreo de la calidad del aire en tiempo real para alertar a los residentes y visitantes sobre los niveles de contaminacion.\n");
            } else if (i == 4) {
                // Guamani
                printf("- Establecer controles mas estrictos sobre las emisiones industriales cercanas para reducir los niveles de PM2.5 en la zona.\n");
                printf("- Fomentar la adopcion de tecnologias limpias en los sectores industriales para minimizar su impacto en la calidad del aire.\n");
                printf("- Implementar restricciones de trafico en horas de mayor contaminacion y promover el uso de transporte publico mas eficiente y limpio.\n");
                printf("- Aumentar las areas verdes y espacios recreativos en la zona para ayudar a reducir la concentracion de contaminantes en el aire.\n");
                printf("- Desarrollar programas educativos para la comunidad sobre los riesgos de la contaminacion y como minimizar su exposicion.\n");
            }
        }
    }
}

// Función para exportar los datos a un archivo
void exportarDatos(struct Zona *zonas, const char *archivo) {
    FILE *f = fopen(archivo, "w");
    if(!f) { printf("No se pudo crear el archivo de reporte.\n"); return; }
    fprintf(f, "Zona,Actual,Promedio30d,PrediccionManana\n");
    for(int i = 0; i < 5; i++) {
        float suma = 0;
        for(int j = 0; j < 30; j++) suma += zonas[i].historico[j];
        float promedio = suma / 30;
        float pred = prediccionSiguienteDia(zonas[i].historico, 30, zonas[i].actual);
        fprintf(f, "%s,%.2f,%.2f,%.2f\n", zonas[i].nombre, zonas[i].actual, promedio, pred);
    }
    fclose(f);
}

// Función para leer los datos desde un archivo binario
void leerDatos(struct Zona *zonas, const char *archivo) {
    FILE *f = fopen(archivo, "rb");
    if(f) {
        fread(zonas, sizeof(struct Zona), 5, f);  // Leer los datos binarios del archivo
        fclose(f);
        printf("Datos leidos desde %s\n", archivo);
    } else {
        printf("No se encontraron datos, inicializando valores por defecto...\n");
        inicializarZonas(zonas);  // Si no existe el archivo, inicializamos los datos
    }
}
