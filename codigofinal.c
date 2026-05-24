#include <stdio.h>
#define ASIGNATURAS 3

int leer_estudiantes(int defecto);
void limpiar_buffer(void);
int leer_calificaciones(int est, float (*cal)[ASIGNATURAS]);
void calcular_por_estudiante(int est, float (*cal)[ASIGNATURAS],
                             float *promE, float *maxE, float *minE);
void calcular_por_asignatura(int est, float (*cal)[ASIGNATURAS],
                             float *promA, float *maxA, float *minA,
                             int *aprob, int *repro, int *estMax, int *estMin);
void mostrar_resultados(int est,
                        float *promE, float *maxE, float *minE,
                        float *promA, float *maxA, float *minA,
                        int *aprob, int *repro, int *estMax, int *estMin);

int main(void) 
{
    int estudiantes = leer_estudiantes(5);

    float calificaciones[estudiantes][ASIGNATURAS];
    float promedioEstudiante[estudiantes];
    float promedioAsignatura[ASIGNATURAS];
    float maxEstudiante[estudiantes], minEstudiante[estudiantes];
    float maxAsignatura[ASIGNATURAS], minAsignatura[ASIGNATURAS];
    int aprobados[ASIGNATURAS], reprobados[ASIGNATURAS];
    int estudianteMaxAsignatura[ASIGNATURAS], estudianteMinAsignatura[ASIGNATURAS];

    leer_calificaciones(estudiantes, calificaciones);

    calcular_por_estudiante(estudiantes, calificaciones,
                            promedioEstudiante, maxEstudiante, minEstudiante);

    calcular_por_asignatura(estudiantes, calificaciones,
                            promedioAsignatura, maxAsignatura, minAsignatura,
                            aprobados, reprobados, estudianteMaxAsignatura, estudianteMinAsignatura);

    mostrar_resultados(estudiantes,
                       promedioEstudiante, maxEstudiante, minEstudiante,
                       promedioAsignatura, maxAsignatura, minAsignatura,
                       aprobados, reprobados, estudianteMaxAsignatura, estudianteMinAsignatura);

    return 0;
}

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

int leer_estudiantes(int defecto) {
    int estudiantes, r;
    do {
        printf("Numero por defecto de estudiantes: %d\n", defecto);
        printf("Ingrese nuevo numero de estudiantes (0 para usar %d): ", defecto);
        r = scanf("%d", &estudiantes);
        if (r != 1) {
            printf("Entrada invalida. Debe ser un numero.\n");
            limpiar_buffer();
            estudiantes = -1; // fuerza repetir
        } else {
            if (estudiantes == 0) estudiantes = defecto;
            if (estudiantes <= 0) {
                printf("Valor invalido. Intente otra vez.\n");
            }
        }
    } while (estudiantes <= 0);
    return estudiantes;
}

int leer_calificaciones(int est, float (*cal)[ASIGNATURAS]) {
    for (int i = 0; i < est; i++) {
        for (int j = 0; j < ASIGNATURAS; j++) {
            float v;
            int r;
            do {
                printf("Ingrese la calificacion del estudiante %d en la asignatura %d (0-10): ", i+1, j+1);
                r = scanf("%f", &v);
                if (r != 1) {
                    printf("Entrada invalida. Debe ser un numero.\n");
                    limpiar_buffer();
                    v = -1.0f; // fuerza repetir
                } else if (v < 0.0f || v > 10.0f) {
                    printf("Valor fuera de rango. Intente otra vez.\n");
                }
            } while (v < 0.0f || v > 10.0f);
            *(*(cal+i)+j) = v;  // acceso con punteros
        }
    }
    return 0;
}

void calcular_por_estudiante(int est, float (*cal)[ASIGNATURAS],
                             float *promE, float *maxE, float *minE) {
    for (int i = 0; i < est; i++) {
        float suma = 0.0f;
        *(maxE+i) = *(*(cal+i));
        *(minE+i) = *(*(cal+i));
        for (int j = 0; j < ASIGNATURAS; j++) {
            float v = *(*(cal+i)+j);
            suma += v;
            if (v > *(maxE+i)) *(maxE+i) = v;
            if (v < *(minE+i)) *(minE+i) = v;
        }
        *(promE+i) = suma / ASIGNATURAS;
    }
}

void calcular_por_asignatura(int est, float (*cal)[ASIGNATURAS],
                             float *promA, float *maxA, float *minA,
                             int *aprob, int *repro, int *estMax, int *estMin) {
    for (int j = 0; j < ASIGNATURAS; j++) {
        float suma = 0.0f;
        *(maxA+j) = *(*(cal+0)+j);
        *(minA+j) = *(*(cal+0)+j);
        *(estMax+j) = 1;
        *(estMin+j) = 1;
        *(aprob+j) = *(repro+j) = 0;
        for (int i = 0; i < est; i++) {
            float v = *(*(cal+i)+j);
            suma += v;
            if (v > *(maxA+j)) { *(maxA+j) = v; *(estMax+j) = i+1; }
            if (v < *(minA+j)) { *(minA+j) = v; *(estMin+j) = i+1; }
            if (v >= 6.0f) (*(aprob+j))++; else (*(repro+j))++;
        }
        *(promA+j) = suma / est;
    }
}

void mostrar_resultados(int est,
                        float *promE, float *maxE, float *minE,
                        float *promA, float *maxA, float *minA,
                        int *aprob, int *repro, int *estMax, int *estMin) {
    printf("\n--- Resultados ---\n");
    for (int i = 0; i < est; i++) {
        printf("Estudiante %d -> Promedio: %.2f | Max: %.2f | Min: %.2f\n",
               i+1, *(promE+i), *(maxE+i), *(minE+i));
    }
    for (int j = 0; j < ASIGNATURAS; j++) {
        printf("Asignatura %d -> Promedio: %.2f | Max: (%.2f, Estudiante %d) | Min: (%.2f, Estudiante %d) | Aprobados: %d | Reprobados: %d\n",
               j+1, *(promA+j), *(maxA+j), *(estMax+j),
               *(minA+j), *(estMin+j), *(aprob+j), *(repro+j));
    }
}
