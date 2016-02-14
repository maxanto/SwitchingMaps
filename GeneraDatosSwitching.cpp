#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow, floor
#include "maxiutils.h"  //tiene los cuantificadores
#include <time.h>   //time

int main()
{
    unsigned long int NIter = 2;//100; // Es el largo de cada atractor
    unsigned int NInitialConditions = 10; // Es la cantidad de condiciones iniciales diferentes de los que se larga el atractor.
    double Bases[] = {2, 10}; // Vector con las bases que quiero probar
    double Precisions[] = {1, 2};//, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
    unsigned long int Bins = 10; // Cantidad de bines del histograma
    unsigned long int DimEmb = 3; // Dimensión de embedding para MP, BP y BPW

    double* Map; //Declare the pointer
    Map = (double*) malloc (sizeof(double)*NIter+1); //Creates the array. It has one more postition at first for the length
    Map[0]=NIter; //Save the vector length in the first position

    int NBases = sizeof(Bases)/sizeof(Bases[0]); // Cantidad de bases
    int NPrecisions = sizeof(Precisions)/sizeof(Precisions[0]); // Cantidad de precisiones

    char StrAux[32]; // Acá armo los nombres de los archivos
    double Scale; // Es la escala que utilizo para multiplicar y dividir en el floor
    double InvScale; // Guardo acá la inversa de la escala para cambiar una división por una multiplicación en cada iteración del mapa

    double Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, MP, Period; // Vectores en donde van guardados los cuantificadores


        for (int iBases = 0; iBases <  NBases; iBases++) // Va recorriendo el vector de bases
        {
            sprintf(StrAux,"LogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
            FILE* Results=fopen(StrAux,"w"); // Abre archivo de resultados

    for (int iInitialCondition = 0; iInitialCondition < NInitialConditions; iInitialCondition++) // Va sorteando condiciones iniciales
    {
        double InitialCondition = (double)rand()/(double)RAND_MAX; // Sortea el primer valor del mapa, es una variable uniformemente distribuída entre 0 y 1

            for (int iPrecisions = 0; iPrecisions < NPrecisions; iPrecisions++) //Recorro el vector de precisiones
            {
                Scale = pow(Bases[iBases],Precisions[iPrecisions]); // powl sirve para long double
                Map[1] = floor(Scale*InitialCondition)/Scale; // floorl sirve para long double, como son mapas positivos puedo usar floor en vez de round

                for (unsigned long int iMap = 1; iMap < NIter; iMap++) // Va riterando el mapa logístico
                {
                    Map[iMap+1] =  4*InvScale*floorl(Scale*Map[iMap]*(Map[iMap]-1)); // Mapa logístico, x[n] = r*x[n-1]*(1-x[n-1]), caótico con r=4
                } // Acá ya tengo el atractor guardado en el vector Map

                //Period = find_period(Map); //Tengo que hacer esta función!!!!!!!!!!!

                double* PDFhist=PDF_hist(Map,Bins); // Genera el histograma de patrones de órden
                Hhist=entropy(PDFhist); // Le calcula la entropía
                Qhist=disequilibrum(PDFhist); // Le calcula el desequilibrio
                Chist=Hhist*Qhist; // Le calcula la complejidad
                free(PDFhist); // Libera el vector que contiene al histograma

                double* PDFbp=PDF_BP_CS(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbp=entropy(PDFbp); // Le calcula la entropía
                Qbp=disequilibrum(PDFbp); // Le calcula el desequilibrio
                Cbp=Hbp*Qbp; // Le calcula la complejidad
                free(PDFbp); // Libera el vector que contiene al histograma

                double* PDFbpw=PDF_BPW(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbpw=entropy(PDFbpw); // Le calcula la entropía
                Qbpw=disequilibrum(PDFbpw); // Le calcula el desequilibrio
                Cbpw=Hbpw*Qbpw; // Le calcula la complejidad
                free(PDFbpw); // Libera el vector que contiene al histograma


        }
    }
                //Acá tengo que hacer un promedio para todas las condiciones iniciales sorteadas, los cuantificadores de arriba tengo que guardarlos en un vactor
                fprintf(Results,"%.0f\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%d%d\n", Precisions[iPrecisions], Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, (unsigned long int)MP, (unsigned long int)Period); //Guarda los valores en el archivo de salida, escribo la condición inicial para evaluar el comportamiento del rand()
            }
            fclose(Results); // Cierra el archivo de salida, queda un archivo por base para este oscilador
}
