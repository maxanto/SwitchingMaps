#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow, floor
#include "maxiutils.h"  //tiene los cuantificadores
#include <time.h>   //time
#include <random>
#include <conio.h> //getch

using namespace std; // Agrego el espacio de nombres del estandar de C++, zafo de poner std:: adelante de cada línea.

int main()
{
    double Bases[] = {2, 10}; // Vector con las bases que quiero probar
    double Precisions[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}; //Contiene todas las precisiones que voy barriendo
    unsigned int NInitialConditions = 1000; // Es la cantidad de condiciones iniciales diferentes de los que se larga el atractor.
    unsigned long int NIter = 3000000; // Es el largo de cada atractor

    unsigned long int Bins = 65536; // Cantidad de bines del histograma
    unsigned long int DimEmb = 6; // Dimensión de embedding para MP, BP y BPW

    mt19937_64 gen(0); // Creo un objeto del tipo mt19937 en presición double con semilla en 0
    uniform_real_distribution<double> dist(0, 1); // Defino el tipo de distribución

    double InitialConditions[NInitialConditions]; // Declaro el vector de condiciones iniciales

    double* Map; //Declare the pointer
    Map = (double*) malloc (sizeof(double)*NIter+1); //Creates the array. It has one more postition at first for the length
    Map[0]=NIter; //Save the vector length in the first position

    int NBases = sizeof(Bases)/sizeof(Bases[0]); // Cantidad de bases
    int NPrecisions = sizeof(Precisions)/sizeof(Precisions[0]); // Cantidad de precisiones

    char StrAux[32]; // Acá armo los nombres de los archivos
    double Scale; // Es la escala que utilizo para multiplicar y dividir en el floor
    double InvScale; // Guardo acá la inversa de la escala para cambiar una división por una multiplicación en cada iteración del mapa

    double Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw;

    unsigned long int MP, Period; // Vectores en donde van guardados los cuantificadores

    for (int iInitialCondition = 0; iInitialCondition < NInitialConditions; iInitialCondition++) // Lleno un vector de condiciones iniciales
    {
        InitialConditions[iInitialCondition] = dist(gen); // Sortea el primer valor del mapa, es una variable uniformemente distribuída entre 0 y 1
    }
    printf("Generated %lu initial conditions\n", NInitialConditions);

    sprintf(StrAux,"P.dat"); //Arma el nombre del archivo para guardar la precisiones
    FILE* P=fopen(StrAux,"w"); // Abre archivo de resultados
    for (int iPrecisions = 0; iPrecisions < NPrecisions; iPrecisions++) //Recorro el vector de precisiones
    {
        fprintf(P, "%.0f\n", Precisions[iPrecisions]);
    }
    fclose(P);

    for (int iBases = 0; iBases <  NBases; iBases++) // Va recorriendo el vector de bases
    {
        printf("\tBase = %d/%d\n", (int)iBases+1, (int)NBases); // Para debuguear

        sprintf(StrAux,"HhistLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* HhistResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"QhistLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* QhistResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"ChistLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* ChistResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"HbpLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* HbpResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"CbpLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* QbpResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"QbpLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* CbpResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"HbpwLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* HbpwResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"CbpwLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* CbpwResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"QbpwLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* QbpwResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"MPLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* MPResults=fopen(StrAux,"w"); // Abre archivo de resultados

        sprintf(StrAux,"PeriodLogB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* PeriodResults=fopen(StrAux,"w"); // Abre archivo de resultados

        for (int iPrecisions = 0; iPrecisions < NPrecisions; iPrecisions++) //Recorro el vector de precisiones
        {
            printf("\t\tPrecision = %d/%d\n", (int)iPrecisions+1, (int)NPrecisions); // Para debuguear

            Scale = pow(Bases[iBases],Precisions[iPrecisions]); // calculo el valor de la escala para redondear. pow sirve para double
            InvScale = 1/Scale;

            for (unsigned long int iInitialCondition = 0; iInitialCondition < NInitialConditions; iInitialCondition++) // Va sorteando condiciones iniciales
            {

                Map[1] = InvScale*floor(Scale*InitialConditions[iInitialCondition]); // floorl sirve para long double, como son mapas positivos puedo usar floor en vez de round

                for (unsigned long int iMap = 1; iMap < NIter; iMap++) // Va riterando el mapa logístico
                {
                    Map[iMap+1] =  4*InvScale*floor(Scale*Map[iMap]*(1-Map[iMap])); // Mapa logístico, x[n] = r*x[n-1]*(1-x[n-1]), caótico con r=4. Ni la resta ni la multiplicación por un entero generan fraccionarios
                } // Acá ya tengo el atractor guardado en el vector Map

                double* PDFhist = PDF_hist(Map,Bins); // Genera el histograma de patrones de órden
                Hhist = entropy(PDFhist); // Le calcula la entropía y la suma para el promedio
                Qhist = disequilibrum(PDFhist); // Le calcula el desequilibrio
                Chist = Hhist*Qhist; // Le calcula la complejidad
                free(PDFhist); // Libera el vector que contiene al histograma

                double* PDFbp = PDF_BP_CS(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbp = entropy(PDFbp); // Le calcula la entropía
                Qbp = disequilibrum(PDFbp); // Le calcula el desequilibrio
                Cbp = Hbp*Qbp; // Le calcula la complejidad
                MP = missing_patterns(PDFbp);
                free(PDFbp); // Libera el vector que contiene al histograma

                double* PDFbpw = PDF_BPW(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbpw = entropy(PDFbpw); // Le calcula la entropía
                Qbpw = disequilibrum(PDFbpw); // Le calcula el desequilibrio
                Cbpw = Hbpw*Qbpw; // Le calcula la complejidad
                free(PDFbpw); // Libera el vector que contiene al histograma

                Period = find_period(Map);

                fprintf(HhistResults,"\t%.8e",Hhist);
                fprintf(QhistResults,"\t%.8e",Qhist);
                fprintf(ChistResults,"\t%.8e",Chist);
                fprintf(HbpResults,"\t%.8e",Hbp);
                fprintf(QbpResults,"\t%.8e",Qbp);
                fprintf(CbpResults,"\t%.8e",Cbp);
                fprintf(HbpwResults,"\t%.8e",Hbpw);
                fprintf(QbpwResults,"\t%.8e",Cbpw);
                fprintf(CbpwResults,"\t%.8e",Qbpw);
                fprintf(MPResults,"\t%ld",MP);
                fprintf(PeriodResults,"\t%ld",Period);
            }

            fprintf(HhistResults,"\n",Hhist);
            fprintf(QhistResults,"\n",Qhist);
            fprintf(ChistResults,"\n",Chist);
            fprintf(HbpResults,"\n",Hbp);
            fprintf(QbpResults,"\n",Qbp);
            fprintf(CbpResults,"\n",Cbp);
            fprintf(HbpwResults,"\n",Hbpw);
            fprintf(QbpwResults,"\n",Cbpw);
            fprintf(CbpwResults,"\n",Qbpw);
            fprintf(MPResults,"\n",MP);
            fprintf(PeriodResults,"\n",Period);

        }

        fclose(HhistResults);
        fclose(QhistResults);
        fclose(ChistResults);
        fclose(HbpResults);
        fclose(QbpResults);
        fclose(CbpResults);
        fclose(HbpwResults);
        fclose(QbpwResults);
        fclose(CbpwResults);
        fclose(MPResults);
        fclose(PeriodResults);

    }
    printf("\nPresionar una tecla");
    getch(); //Avisa que terminó y espera una tecla
}
