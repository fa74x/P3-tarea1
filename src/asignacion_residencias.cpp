/*
Módulo de implementación de `asignacion_residencias'.

Laboratorio de Programación 3.
InCo-FIng-UDELAR
*/

#include "../include/asignacion_residencias.h"
#include "../include/lista.h"
#include <cstddef>
#include <unistd.h>


Asignacion asignarResidencias(nat m, nat *C, nat n, nat **hPrefs, nat **ePrefs)
{
  //DECLARACION DE ESTRUCTURAS NECESARIAS
  Asignacion res = crear_asignacion();
  Lista hospitales_libres = crear_lista();
  nat ultima_propuesta[m];
  int h_de_e[n];
  nat **ranking = new nat*[n];


  //INICIALIZACION DE ESTRUCTURAS
  for (nat i = 0; i < m; i++) {
    insertar(i, hospitales_libres);
    ultima_propuesta[i] = 0;
  }
  for (nat i = 0; i < n; i++)
    h_de_e[i] = -1;
  for (nat i = 0; i < n; i++) {
    ranking[i] = new nat[m];
    for (nat j = 0; j < m; j++)
      ranking[i][ePrefs[i][j]] = j;  
  }



  //ALGORITMO
  while (!es_vacia_lista(hospitales_libres))
  {
    nat hospital_propone = primero(hospitales_libres);
    nat estudiante = hPrefs[hospital_propone][ultima_propuesta[hospital_propone]];
  
    if (h_de_e[estudiante] == -1) {
      h_de_e[estudiante] = hospital_propone;
      C[hospital_propone]--;

    } else {
      nat hospital_actual = h_de_e[estudiante];

      if (ranking[estudiante][hospital_actual] > ranking[estudiante][hospital_propone]) {
        h_de_e[estudiante] = hospital_propone;
        C[hospital_propone]--;
        C[hospital_actual]++;

        bool hospital_actual_libre = pertenece_a_lista(hospital_actual,hospitales_libres);
        if (C[hospital_actual] > 0 && !hospital_actual_libre)
          insertar(hospital_actual, hospitales_libres);
        else if (C[hospital_actual] == 0 && hospital_actual_libre)
          remover(hospital_actual,hospitales_libres);
      }
    }

    ultima_propuesta[hospital_propone]++;
    bool hospital_propone_libre = pertenece_a_lista(hospital_propone, hospitales_libres);

    if (C[hospital_propone] == 0 && hospital_propone_libre)
        remover(hospital_propone, hospitales_libres);
    else if (C[hospital_propone] > 0 && !hospital_propone_libre)
        insertar(hospital_propone, hospitales_libres);

  }


  //DESTRUCCION DE ESTRUCTURAS DINAMICAS
  destruir_lista(hospitales_libres);
  for (nat i = 0; i < n; i++)
    delete[] ranking[i];
  delete[] ranking;



  //CARGO LOS PARES A LA ESTRUCTURA TIPO ASIGNACION
  for (nat i = 0; i < n; i++)
  {
    nat h = h_de_e[i];
    par nuevo = {i, h};
    insertar_par(nuevo, res);
  }

  return res;
}
