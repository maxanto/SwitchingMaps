# SwitchingMaps
Código para generar el experimento del paper switching maps

El código hace lo siguiente:

  - Primero genera un vector de condiciones iniciales (ICV) de longitud NIC.
  - Después recorre un vector de bases (B=[2,10]). Las bases utilizadas son 2 y 10 en este caso.
    - Para cada base recorre un vector P (ver paper)
      - Para cada P recorre el vector de condiciones iniciales
        - Itera NI veces el mapa logístico (MAP)
        - Calcula cuantificadores (H, Hbp, MP, Hbpw)
        - Guarda un renglón (P, H, Hbp, MP, Hbpw) del archivo de salida logB10.dat
     - Vuelve a recorrer el vector P y repite los pasos anteriores para los nueve mapas:
        - logístico, odd logístico, even logístico, tent, odd tent, even tent, switch, odd switch, even switch
 
Para sortear el vector de condiciones iniciales podemos usar la función rand() de la librería estándar o la función uniform_real_distribution() de la librería <random>. El primer caso tiene una distribución uniforme en el intervalo [0 RAND_MAX], al segundo se le pasa el intervalo en los parámetros. En GCC RAND_MAX = 2147483647 = 2^(32-1)-1 (compilador usado para los resultados obtenidos en el paper), pero puede ser tan bajo como 32767 en otras librerías, lo que lo hace un mal generador en algunos casos. En cambio en la librería <random>, la función uniform_real_distribution() garantiza una buenas propiedades estadísticas y repetibilidad del experimento si se usa el motor mersenne_twister (más información en http://www.cplusplus.com/reference/random/mersenne_twister_engine/).

El resultado de este programa es un archivo *.dat por cada base por cada atractor. En este caso son 18 archivos (9 atractores y 2 bases).
Cada uno de los archivos tiene cinco columnas separadas por tabulaciones (P, H, Hbp, MP, Hbpw) y tantas filas como aritméticas en el vector P.
