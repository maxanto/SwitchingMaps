#include <stdio.h>  //sprintf, FILE
#include <string.h> //strcpy, strcat
#include <math.h>   //pow
#include <random>

/**Variables globales, sus valores no cambian a lo largo del programa**/
unsigned long long int NIC = 100; // Es la cantidad de condiciones iniciales diferentes de los que se larga el atractor.

/************HASTA ACÁ LLEGUÉ 3/2/2016*********************/

int main(){
    float x_vec_s[20000];
    double x_vec_d[20000];
    double rand[1000];
    double digits;
    char serie_str[32];
    char P_str[8];
    char j_str[8];
    FILE* list_out=fopen("list.dat","w");


    /*********Lee un archivo con números random generado con matlab*****/
    FILE* rand_in=fopen("rand.dat","r");
    for (int i_rnd=0;i_rnd<1000;i_rnd++){
        fscanf(rand_in,"%le",&rand[i_rnd]);
    }
    fclose(rand_in);

    for (int P=1;P<=15;P++){
        digits=pow(10,(double)P);

        for (int j=0;j<1000;j++){
            printf("%i\t%i\n",P,j);
            /*Genera el nombre del archivo*/
            sprintf(P_str,"%i",P);
            sprintf(j_str,"%i",j+1);
            strcpy(serie_str, "32_P");
            strcat(serie_str,P_str);
            strcat(serie_str,"_j");
            strcat(serie_str,j_str);
            strcat(serie_str,".dat");

            fprintf(list_out,"%s\n",serie_str);//Escribe el nombre del archivo en la lista
            FILE* tent_32=fopen(serie_str,"w");//Abre el archivo para escibir el atractor

            /*Carga las condiciones iniciales*/
            x_vec_s[0]=floor(digits*rand[j])/digits;

            /*Itera el mapa en 32 bits*/
            for (int i_vec=0;i_vec<20000;i_vec++){
                fprintf(tent_32,"%.20E\n",x_vec_s[i_vec]);
                if (x_vec_s[i_vec]<0.5) x_vec_s[i_vec+1]=2*x_vec_s[i_vec];
                else x_vec_s[i_vec+1]=2*(floor(digits*(1-x_vec_s[i_vec]))/digits);
            }
            fprintf(tent_32,"%.20E",x_vec_s[19999]);
            fclose(tent_32);

        }
    }
    fclose(list_out);

    /**Para ver como levanta los datos con presiciones distintas**
    x_vec_s[0]=rand[0];
    x_vec_d[0]=rand[0];

    printf("%.16e\n",rand[0]);
    printf("%.16e\n",x_vec_s[0]);
    printf("%.16e\n",x_vec_d[0]);
/**/


/**Para ver como itera los datos con presisciones distintas**
    FILE* SD_out=fopen("SimpleDecimal.dat","w");
    FILE* SH_out=fopen("SimpleHexa.dat","w");
    FILE* DD_out=fopen("DobleDecimal.dat","w");
    FILE* DH_out=fopen("DobleHexa.dat","w");


    for (int i_vec=0;i_vec<98;i_vec++){

        if (x_vec_s[i_vec]<.5) x_vec_s[i_vec+1]=2*x_vec_s[i_vec];
        else x_vec_s[i_vec+1]=2*(1-x_vec_s[i_vec]);
        fprintf(SD_out,"%.20E\n",x_vec_s[i_vec]);
        fprintf(SH_out,"%.20A\n",x_vec_s[i_vec]);

        if (x_vec_d[i_vec]<.5) x_vec_d[i_vec+1]=2*x_vec_d[i_vec];
        else x_vec_d[i_vec+1]=2*(1-x_vec_d[i_vec]);
        fprintf(DD_out,"%.20E\n",x_vec_d[i_vec]);
        fprintf(DH_out,"%.20A\n",x_vec_d[i_vec]);

    }

    fprintf(SD_out,"%.20E\n",x_vec_s[99]);
    fprintf(SH_out,"%.20A\n",x_vec_s[99]);

    fprintf(DD_out,"%.20lE\n",(double)x_vec_d[99]);
    fprintf(DH_out,"%.20A\n",(double)x_vec_d[99]);

    fclose(SD_out);
    fclose(SH_out);
    fclose(DD_out);
    fclose(DH_out);
/**/
}
