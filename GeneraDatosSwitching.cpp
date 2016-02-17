#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow, floor
#include "maxiutils.h"  //tiene los cuantificadores
#include <time.h>   //time

int main()
{
    unsigned long int NIter = 10000; // Es el largo de cada atractor
    unsigned int NInitialConditions = 2; // Es la cantidad de condiciones iniciales diferentes de los que se larga el atractor.
    double Bases[] = {2, 10}; // Vector con las bases que quiero probar
    double Precisions[] = {3, 4}; //, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}; //Contiene todas las precisiones que voy barriendo
    unsigned long int Bins = 1024; // Cantidad de bines del histograma
    unsigned long int DimEmb = 6; // Dimensión de embedding para MP, BP y BPW

    double InitialConditions[NInitialConditions]; // Declaro el vector de condiciones iniciales

    double* Map; //Declare the pointer
    Map = (double*) malloc (sizeof(double)*NIter+1); //Creates the array. It has one more postition at first for the length
    Map[0]=NIter; //Save the vector length in the first position

    int NBases = sizeof(Bases)/sizeof(Bases[0]); // Cantidad de bases
    int NPrecisions = sizeof(Precisions)/sizeof(Precisions[0]); // Cantidad de precisiones

    char StrAux[32]; // Acá armo los nombres de los archivos
    double Scale; // Es la escala que utilizo para multiplicar y dividir en el floor
    double InvScale; // Guardo acá la inversa de la escala para cambiar una división por una multiplicación en cada iteración del mapa

    double Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, MP, Period; // Vectores en donde van guardados los cuantificadores

    for (int iInitialCondition = 0; iInitialCondition < NInitialConditions; iInitialCondition++) // Lleno un vector de condiciones iniciales
    {
        InitialConditions[iInitialCondition] = (double)rand()/(double)RAND_MAX; // Sortea el primer valor del mapa, es una variable uniformemente distribuída entre 0 y 1
    }
    printf("Generated %lu initial conditions\n", NInitialConditions);

    for (int iBases = 0; iBases <  NBases; iBases++) // Va recorriendo el vector de bases
    {
        printf("\tBase = %d/%d\n", (int)iBases+1, (int)NBases); // Para debuguear

        sprintf(StrAux,"LogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* Results=fopen(StrAux,"w"); // Abre archivo de resultados

        for (int iPrecisions = 0; iPrecisions < NPrecisions; iPrecisions++) //Recorro el vector de precisiones
        {
            printf("\t\tPrecision = %d/%d\n", (int)iPrecisions+1, (int)NPrecisions); // Para debuguear

            Scale = pow(Bases[iBases],Precisions[iPrecisions]); // calculo el valor de la escala para redondear. pow sirve para double

            Hhist = 0;
            Qhist = 0;
            Chist = 0;
            Hbp = 0;
            Qbp = 0;
            Cbp = 0;
            Hbpw = 0;
            Qbpw = 0;
            Cbpw = 0;
            MP = 0;
            Period = 0;

            for (unsigned long int iInitialCondition = 0; iInitialCondition < NInitialConditions; iInitialCondition++) // Va sorteando condiciones iniciales
            {
                printf("\t\t\tCondicion inicial = %d/%d\n", (int)iInitialCondition+1, (int)NInitialConditions); // Para debuguear

                Map[1] = floor(Scale*InitialConditions[iInitialCondition])/Scale; // floorl sirve para long double, como son mapas positivos puedo usar floor en vez de round

                for (unsigned long int iMap = 1; iMap < NIter; iMap++) // Va riterando el mapa logístico
                {
                    Map[iMap+1] =  4*InvScale*floorl(Scale*Map[iMap]*(Map[iMap]-1)); // Mapa logístico, x[n] = r*x[n-1]*(1-x[n-1]), caótico con r=4
                } // Acá ya tengo el atractor guardado en el vector Map


                //Period = find_period(Map); //Tengo que hacer esta función!!!!!!!!!!!
printf("\t\t\t\tMapa en memoria\n"); // Para debuguear
                double* PDFhist = PDF_hist(Map,Bins); // Genera el histograma de patrones de órden
printf("\t\t\t\tArmado histograma de valores\n"); // Para debuguear
                Hhist = Hhist + entropy(PDFhist); // Le calcula la entropía y la suma para el promedio
printf("\t\t\t\tHhist=%le\n",Hhist); // Para debuguear
                Qhist = Qhist + disequilibrum(PDFhist); // Le calcula el desequilibrio
printf("\t\t\t\tQhist=%le\n",Qhist); // Para debuguear
                Chist = Chist + Hhist*Qhist; // Le calcula la complejidad
printf("\t\t\t\tChist=%le\n",Chist); // Para debuguear
                free(PDFhist); // Libera el vector que contiene al histograma
printf("\t\t\t\tLiberada la memoria ocupada por PDFhist\n"); // Para debuguear

                double* PDFbp = PDF_BP_CS(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbp = Hbp + entropy(PDFbp); // Le calcula la entropía
                Qbp = Qbp + disequilibrum(PDFbp); // Le calcula el desequilibrio
                Cbp = Cbp + Hbp*Qbp; // Le calcula la complejidad
                free(PDFbp); // Libera el vector que contiene al histograma

                double* PDFbpw = PDF_BPW(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbpw = Hbpw + entropy(PDFbpw); // Le calcula la entropía
                Qbpw = Qbpw + disequilibrum(PDFbpw); // Le calcula el desequilibrio
                Cbpw = Cbpw + Hbpw*Qbpw; // Le calcula la complejidad
                free(PDFbpw); // Libera el vector que contiene al histograma
            }
            Hhist = Hhist/(double)NInitialConditions;
            Qhist = Qhist/(double)NInitialConditions;
            Chist = Chist/(double)NInitialConditions;
            Hbp = Hbp/(double)NInitialConditions;
            Qbp = Qbp/(double)NInitialConditions;
            Cbp = Cbp/(double)NInitialConditions;
            Hbpw = Hbpw/(double)NInitialConditions;
            Qbpw = Qbpw/(double)NInitialConditions;
            Cbpw = Cbpw/(double)NInitialConditions;
            MP = MP/(double)NInitialConditions;
            Period = Period/(double)NInitialConditions;
            fprintf(Results,"%.0f\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%d\t%d\n", Precisions[iPrecisions], Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, (unsigned long int)MP, (unsigned long int)Period); //Guarda los valores en el archivo de salida, escribo la condición inicial para evaluar el comportamiento del rand()
        }
        fclose(Results); // Cierra el archivo de salida, queda un archivo por base para este oscilador
    }
}
