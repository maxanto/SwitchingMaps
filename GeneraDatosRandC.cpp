#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow, floor
#include "maxiutils.h"  //tiene los cuantificadores
#include <time.h>   //time
#include <random>

using namespace std;

int main()
{
    mt19937_64 gen(0);
    uniform_real_distribution<double> dist(0, 1);

    unsigned long int NIter = 10000000; // Es el largo de cada atractor
    double Bases[] = {2, 10}; // Vector con las bases que quiero probar
    double Precisions[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30}; //Contiene todas las precisiones que voy barriendo
    unsigned long int Bins = 65536; // Cantidad de bines del histograma
    unsigned long int DimEmb = 6; // Dimensión de embedding para MP, BP y BPW

    double* Map; //Declare the pointer
    Map = (double*) malloc (sizeof(double)*NIter+1); //Creates the array. It has one more postition at first for the length
    Map[0]=NIter; //Save the vector length in the first position

    double* OriginMap; //Declare the pointer
    OriginMap = (double*) malloc (sizeof(double)*NIter+1); //Creates the array. It has one more postition at first for the length
    OriginMap[0]=NIter; //Save the vector length in the first position

    int NBases = sizeof(Bases)/sizeof(Bases[0]); // Cantidad de bases
    int NPrecisions = sizeof(Precisions)/sizeof(Precisions[0]); // Cantidad de precisiones

    char StrAux[32]; // Acá armo los nombres de los archivos
    double Scale; // Es la escala que utilizo para multiplicar y dividir en el floor
    double InvScale; // Guardo acá la inversa de la escala para cambiar una división por una multiplicación en cada iteración del mapa

    double Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, MP; // Vectores en donde van guardados los cuantificadores

    for (unsigned long int iMap = 1; iMap < NIter; iMap++) // Va riterando el mapa logístico
    {
        OriginMap[iMap] =  dist(gen); // Mapa logístico, x[n] = r*x[n-1]*(1-x[n-1]), caótico con r=4
    } // Acá ya tengo el atractor guardado en el vector Map
    printf("Mapa creado"); // Para debuguear

    for (int iBases = 0; iBases <  NBases; iBases++) // Va recorriendo el vector de bases
    {
        printf("\tBase = %d/%d\n", (int)iBases+1, (int)NBases); // Para debuguear

        sprintf(StrAux,"RandB%.0f.dat", Bases[iBases]); //Arma el nombre del archivo para esta base
        FILE* Results=fopen(StrAux,"w"); // Abre archivo de resultados

        for (int iPrecisions = 0; iPrecisions < NPrecisions; iPrecisions++) //Recorro el vector de precisiones
        {
            printf("\t\tPrecision = %d/%d\n", (int)iPrecisions+1, (int)NPrecisions); // Para debuguear

            Scale = pow(Bases[iBases],Precisions[iPrecisions]); // calculo el valor de la escala para redondear. pow sirve para double

            for (unsigned long int iMap = 1; iMap <= NIter; iMap++) // Va riterando el vector para redondearlo
            {
                Map[iMap] =  floor(Scale*OriginMap[iMap])/Scale; // Mapa logístico, x[n] = r*x[n-1]*(1-x[n-1]), caótico con r=4
            } // Acá ya tengo el atractor guardado en el vector Map


                //Period = find_period(Map); //Tengo que hacer esta función!!!!!!!!!!!
//printf("\t\t\t\tMapa en memoria\n"); // Para debuguear
                double* PDFhist = PDF_hist(Map,Bins); // Genera el histograma de patrones de órden
//printf("\t\t\t\tArmado histograma de valores\n"); // Para debuguear
                Hhist = entropy(PDFhist); // Le calcula la entropía y la suma para el promedio
//printf("\t\t\t\tHhist=%le\n",Hhist); // Para debuguear
                Qhist = disequilibrum(PDFhist); // Le calcula el desequilibrio
//printf("\t\t\t\tQhist=%le\n",Qhist); // Para debuguear
                Chist = Hhist*Qhist; // Le calcula la complejidad
//printf("\t\t\t\tChist=%le\n",Chist); // Para debuguear
                free(PDFhist); // Libera el vector que contiene al histograma
//printf("\t\t\t\tLiberada la memoria ocupada por PDFhist\n"); // Para debuguear

                double* PDFbp = PDF_BP_CS(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbp = entropy(PDFbp); // Le calcula la entropía
                Qbp = disequilibrum(PDFbp); // Le calcula el desequilibrio
                Cbp = Hbp*Qbp; // Le calcula la complejidad
                free(PDFbp); // Libera el vector que contiene al histograma

                double* PDFbpw = PDF_BPW(Map,DimEmb); // Genera el histograma de patrones de órden
                Hbpw = entropy(PDFbpw); // Le calcula la entropía
                Qbpw = disequilibrum(PDFbpw); // Le calcula el desequilibrio
                Cbpw = Hbpw*Qbpw; // Le calcula la complejidad
                free(PDFbpw); // Libera el vector que contiene al histograma

            fprintf(Results,"%.0f\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%.8e\t%d\t\n", Precisions[iPrecisions], Hhist, Qhist, Chist, Hbp, Qbp, Cbp, Hbpw, Qbpw, Cbpw, (unsigned int)MP); //Guarda los valores en el archivo de salida, escribo la condición inicial para evaluar el comportamiento del rand()
        }
        fclose(Results); // Cierra el archivo de salida, queda un archivo por base para este oscilador
    }
}
