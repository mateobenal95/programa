// zona.h

// Estructura Zona
struct Zona {
    char nombre[50];
    float historico[30];
    float actual;
    float temperatura;
    float viento;
    float humedad;
    float limite; // Limite de PM2.5 aceptable para la zona
};

// Declaracion de las funciones
void inicializarZonas(struct Zona *zonas);
void mostrarEstadoActual(struct Zona *zonas);
void mostrarDatosClimaticos(struct Zona *zonas);
void calcularPromedios(struct Zona *zonas);
float prediccionSiguienteDia(float *hist, int n, float actual);
void emitirAlertas(struct Zona *zonas);
void generarRecomendaciones(struct Zona *zonas);
void exportarDatos(struct Zona *zonas, const char *archivo);
void leerDatos(struct Zona *zonas, const char *archivo);  // Declaracion de leerDatos
void mostrarTablaHistoricoActualPrediccion(struct Zona *zonas);
void ingresarDatosActuales(struct Zona *zonas);  // Nueva funcion para ingresar datos
void actualizarArchivo(struct Zona *zonas, const char *archivo);  // Nueva funcion para actualizar el archivo binario
