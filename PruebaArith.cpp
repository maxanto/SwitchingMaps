#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow
#include <random>

/**Variables globales, sus valores no cambian a lo largo del programa**/
unsigned char Base = 2; // Es la base a la que le quiero calcular la presición

int main()
{
    float Flotante;
    double Doble;
    long double Largo;

    //Abro los archivos para guardar los resultados
    FILE* FlotanteFile=fopen("Flotante.dat","w");
    FILE* DobleFile=fopen("Doble.dat","w");
    FILE* LargoFile=fopen("Largo.dat","w");

    //Inicio las variables
    Flotante = 1;
    Doble = 1;
    Largo = 1;

    for (int iPrec = 1; iPrec <= 100; iPrec++) //Recorro posiciones después de la coma
    {
        fprintf(FlotanteFile,"%d\t%1.100f\n",iPrec,Flotante + (float)pow(Base,-iPrec));
        fprintf(DobleFile,"%d\t%1.100f\n",iPrec,Doble + (double)pow(Base,-iPrec));
        fprintf(LargoFile,"%d\t%1.100Lf\n",iPrec,Largo + (long double)pow(Base,-iPrec));
    }

    fclose(FlotanteFile);
    fclose(DobleFile);
    fclose(LargoFile);
}
