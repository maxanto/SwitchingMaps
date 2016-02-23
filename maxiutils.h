/*********************************************************************************
Version 17/10/14
    fixed function normalize_vector for constant vectors

Version 25/09/14
    fixed function normalize_PDF for non integer vectors

Version 01/09/14
    added normalize_PDF
    added PDF_hist2
*********************************************************************************/
#ifndef MAXIUTILS_H_INCLUDED
#define MAXIUTILS_H_INCLUDED

#include <stdio.h>/************************Tiene FILE*/
#include <math.h>/*************************Tiene los logaritmos y la potencia*/
#include <stdlib.h> /**********************Biblioteca para llamar a malloc*/

/*********************************************************************************
Bandt & Pompe PDF is used to make a causal PDF in functions PDF_BP and PDF_BPW
    Reference: Permutation entropy: a natural complexity for time series
               Physical Review Letters 88 (2002) 174102. C. Bandt, B. Pompe

Lemher lexicographic method is used in Bandt & Pompe PDF's.
    Reference: www.keithschwarz.com/interesting/code/?dir=factoradic-permutation

Bandt & Pompe PDF with amplitude contributuions is used in PDF_BPW
    Reference: Weighted permutation entropy: a complexity measure for
               time series incorporating amplitude information.
               Physical Review E 87 (2013) 022911. B. Fadlallah, B. Chen, A. Keil, J. Principe



Due to (problem 1) the functions only can return only one value (or pointer, or struct), and (problem 2)
the pointer returned of a function no have information about the length of the pointed vector, ALL
VECTORS MUST HAVE ITS OWN LENGTH IN THE FIRST POSITION (data_vector[0]=lenght_of_data_vector).

All vectors are allocated in memory after each function in order to be valid out of the subrutine,
is necessary unlock the the memory positions with function free(pointer) in main() function.
************************************************************************************/

double* normalize_vector(double x_vec[]){
/******************************************************
Generates a normalized version of the input vector

Parameters:
    x_vec is the input vector in double presition. The first position must be
    the length of the vector (x_vec[0]=length_x_vec).

Returned value:
    x_norm is the pointer of an allocated vector that contains the normalized
    version of th e input vector. The first position of x_nrom is its own length

Caution:
    The allocated vector needs to be unallocated with free(pointer) outside
    this function.
*******************************************************/
    unsigned long int length=x_vec[0];
    double x_min=x_vec[1],x_max=x_vec[1];
    double* x_norm; //Declare the pointer
        x_norm = (double*) malloc (sizeof(double)*length+1); //Creates the array. It has one more postition at first for the length
        x_norm[0]=length;

    for (unsigned long int i_vec=1;i_vec<=length;i_vec++) {
        if (x_vec[i_vec]<x_min) x_min=x_vec[i_vec];//Find the maximum in order to normalize
        if (x_vec[i_vec]>x_max) x_max=x_vec[i_vec];//Find the minimum in order to normalize
    }

    if (x_max==x_min) //If amplitude is equals zero, vector in non-normalizable
    {
        for (unsigned long int i=1;i<=length;i++) x_norm[i]=0; //The output is a zero vector
    }
    else
    {
        double inv_amplitude=1/(double)(x_max-x_min);
        for (unsigned long int i=1;i<=length;i++)
        {
            x_norm[i]=inv_amplitude*(x_vec[i]-x_min);//Normalizes each component of the vector
            if (x_norm[i]>=0.999999999999999) x_norm[i]=0.999999999999990;
        }
    }


    return x_norm;
}

double* normalize_PDF(double PDF_vec[]){
/******************************************************
Generates a normalized version of the histogram to make a PDF

Parameters:
    PDF_vec is the input vector in double presition. The first position must be
    the length of the vector (PDF_vec[0]=length_PDF_vec).

Returned value:
    PDF_norm is the pointer of an allocated vector that contains the normalized
    version of the input vector. The first position of PDF_nrom is its own length

*******************************************************/
    unsigned long int length=PDF_vec[0];
    double sum=0;

    for (unsigned long int i_vec=1;i_vec<=length;i_vec++) {
        sum+=PDF_vec[i_vec];
    }

    for (unsigned long int i_norm=1;i_norm<=length;i_norm++) {
        PDF_vec[i_norm]=PDF_vec[i_norm]/sum; //Normalizes each component of the vector
    }
}

double* PDF_hist(double x_vec[],unsigned long int bins){
/*******************************************************************************
Generates the Histogram PDF.

Parameters:
    x_vec is the input vector. THE FIRST POSITION OF x_vec IS ITS OWN LENGTH
    bins is the number of bins in the histogram

Returned value:
    PDF_hist_vec is the pointer of an allocated vector that contains the
    normalized version of histogram. THE FIRST POSITION OF PDF_hist_vec IS ITS OWN LENGTH

Caution:
    The allocated vector needs to be unallocated with free(pointer) outside
    this function.
********************************************************************************/

    unsigned long int length=x_vec[0];//The length of data vector

    double* PDF_hist_vec;//Declare the pointer
        PDF_hist_vec = (double*) malloc ( sizeof(double) * (bins+1)); //Creates the array with one more position at first for the length
        PDF_hist_vec[0] = bins;//The first position is the length of data vector
        for (unsigned long int i_hist=1;i_hist<=bins;i_hist++) PDF_hist_vec[i_hist]=0; //Initializes the PDF at zero.

    double* x_norm = normalize_vector(x_vec);//Vector must be normalyzed. x_norm  will be a pointer asociated to an allocated array

    /**Increments the histogram PDF*/
    unsigned long int histogram_index;
    for (unsigned long int i_norm=1;i_norm<=length;i_norm++){//For each position of the normalyzed vector. Are missing the last embedd-1 positions
        histogram_index=floor((bins)*x_norm[i_norm])+1;//Calculate the index for histogram

        PDF_hist_vec[histogram_index]+=1;//Increment the position
    }
    free(x_norm);//Releases the vector allocated by normalize_vector() function.

    normalize_PDF(PDF_hist_vec);

    return PDF_hist_vec;
}

double* PDF_hist2(double x_vec[],unsigned long int bins){ /**Debug* printf("\t\tPDF_hist2\n");/**/
/*******************************************************************************
Generates the Histogram PDF for vectors of integers with known possible values
between 0 and bins

Parameters:
    x_vec is the input vector. THE FIRST POSITION OF x_vec IS ITS OWN LENGTH
    bins is the number of possible values

Returned value:
    PDF_hist_vec is the pointer of an allocated vector that contains the
    normalized version of histogram. THE FIRST POSITION OF PDF_hist_vec IS ITS OWN LENGTH

Caution:
    The allocated vector needs to be unallocated with free(pointer) outside
    this function.
********************************************************************************/

    unsigned long int length=x_vec[0];//The length of data vector

    double* PDF_hist_vec;//Declare the pointer
        PDF_hist_vec = (double*) malloc ( sizeof(double) * (bins+1));// printf("\tmalloc ok\n");//Creates the array with one more position at first for the length
        PDF_hist_vec[0] = bins;//The first position is the length of data vector
        for (unsigned long int i_hist=1;i_hist<=bins;i_hist++) PDF_hist_vec[i_hist]=0; //Initializes the PDF at zero.

    /**Increments the histogram PDF*/
    unsigned long int histogram_index;
    for (unsigned long int i_vec=1;i_vec<=length;i_vec++){//For each position of the normalyzed vector. Are missing the last embedd-1 positions
        histogram_index=floor(x_vec[i_vec]+1); /**Debug* printf("\t\t\thistogram_index[%d]=%d\n",i_vec,histogram_index);/**///Dejo el floor por que pareciera que lo pasa a enteros!!!!!!!!!!!!!!!!!!!!!!!!
        PDF_hist_vec[histogram_index]+=1; /**Debug*printf("\t\t\tPDF_hist_vec[%d]=%le\n",histogram_index,PDF_hist_vec[histogram_index]);/**///Increment the position
    }

    normalize_PDF(PDF_hist_vec);

    return PDF_hist_vec;
}

double* PDF_BP_CS(double x_vec[], unsigned long int embedd){
/*******************************************************************************
Generates Bandt & Pompe PDF.

Parameters:
    x_vec is the input vector. The first position of x_vec is its own length
    embedd is the embedding dimension between 2 and length

Returned value:
    PDF_BP_vec is the pointer of an allocated vector that contains the
    Bandt & Pompe PDF with embedd! bins.

Caution:
    The allocated vector needs to be unallocated.
********************************************************************************/
    unsigned long int length=x_vec[0];

    /**Calculates the length of PDF_BP=embedd! and creates the vector. Creates the factorial basis*/
    unsigned long int BP_length=1;
    unsigned long int factorial_basis[embedd]; factorial_basis[embedd-1]=1;//Inicialize the basis for Lemher lexicographic order
    for (unsigned long int i_emb=2;i_emb<=embedd;i_emb++){
        factorial_basis[embedd-i_emb]=BP_length;
        BP_length*=i_emb;
    }

    double *PDF_BP_vec;//Create the pointer
        PDF_BP_vec=(double*) malloc ( sizeof(double) * (BP_length+1)); //Create the array with one more position at first for length
        PDF_BP_vec[0]=BP_length; //Save the PDF_BP_length in the first position of a vector
        for (unsigned long int i_BP=1;i_BP<=BP_length;i_BP++) PDF_BP_vec[i_BP]=0;

    /**Generates the PDF_BP*/
    unsigned long int ordering_pattern;//Inicializes the ordering pattern for BP and BPW

    for (unsigned long int i_vec=1;i_vec<=length-embedd+1;i_vec++){//For each position of the input vector.
 //       printf("i_vec1=%d\n",i_vec);
        /**Calculate the lexicographic order to embedding vector with Lemher method*/
        ordering_pattern=1;//First index of PDF_BPW is 1, the 0 is reserved for the length. Ordering patern is between 1 and embedd!

        for (unsigned long int i_pos=i_vec;i_pos<i_vec+embedd-1;i_pos++){
            for (unsigned long int i_comp=i_pos+1;i_comp<i_vec+embedd;i_comp++){
                if (x_vec[i_pos]>x_vec[i_comp]) ordering_pattern+=factorial_basis[i_pos-i_vec];
            }
        }
        PDF_BP_vec[ordering_pattern]+=1;//Increments the position
    }

    normalize_PDF(PDF_BP_vec);

    return PDF_BP_vec;
}


double* PDF_BP_SS(double x_vec[], unsigned long int embedd){
/*******************************************************************************
Generates Bandt & Pompe PDF.

Parameters:
    x_vec is the input vector. The first position of x_vec is its own length
    embedd is the embedding dimension between 2 and length

Returned value:
    PDF_BP_vec is the pointer of an allocated vector that contains the
    Bandt & Pompe PDF with embedd! bins.

Caution:
    The allocated vector needs to be unallocated.
********************************************************************************/
    unsigned long int length=x_vec[0];

    /**Calculates the length of PDF_BP=embedd! and creates the vector. Creates the factorial basis*/
    unsigned long int BP_length=1;
    unsigned long int factorial_basis[embedd]; factorial_basis[embedd-1]=1;//Inicialize the basis for Lemher lexicographic order
    for (unsigned long int i_emb=2;i_emb<=embedd;i_emb++){
        factorial_basis[embedd-i_emb]=BP_length;
        BP_length*=i_emb;
    }

    double *PDF_BP_vec;//Create the pointer
        PDF_BP_vec=(double*) malloc ( sizeof(double) * (BP_length+1)); //Create the array with one more position at first for length
        PDF_BP_vec[0]=BP_length; //Save the PDF_BP_length in the first position of a vector
        for (unsigned long int i_BP=1;i_BP<=BP_length;i_BP++) PDF_BP_vec[i_BP]=0;

    /**Generates the PDF_BP*/
    unsigned long int ordering_pattern;//Inicializes the ordering pattern for BP and BPW

    for (unsigned long int i_vec=1;i_vec<=length-embedd+1;i_vec+=embedd){//For each position of the input vector.
 //       printf("i_vec1=%d\n",i_vec);
        /**Calculate the lexicographic order to embedding vector with Lemher method*/
        ordering_pattern=1;//First index of PDF_BPW is 1, the 0 is reserved for the length. Ordering patern is between 1 and embedd!

        for (unsigned long int i_pos=i_vec;i_pos<i_vec+embedd-1;i_pos++){
            for (unsigned long int i_comp=i_pos+1;i_comp<i_vec+embedd;i_comp++){
                if (x_vec[i_pos]>x_vec[i_comp]) ordering_pattern+=factorial_basis[i_pos-i_vec];
            }
        }
        PDF_BP_vec[ordering_pattern]+=1;//Increments the position
    }

    normalize_PDF(PDF_BP_vec);

    return PDF_BP_vec;
}


double* PDF_BPW(double x_vec[],unsigned long int embedd){
/*******************************************************************************
Generates wighted Bandt & Pompe PDF

Parameters:
    x_vec is the input vector. The first position of x_vec is its own length
    embedd is the embedding dimension between 2 and length

Returned value:
    PDF_BPW_vec is the pointer of an allocated vector that contains the
    Weighted Bandt & Pompe PDF with embedd! bins. Its first is its own length.

Caution:
    The allocated vector needs to be unallocated outside this function. free(pointer);

Future improvements:
    Each embedding vector is crossed three times, is possible to cross once.

Problems:
    If input_vector is a zeros, program crashes.
    If amplitude is very small, the result is 1.#IND000000000000e+000 or -1.#IND000000000000e+000
        in each bin of BPW.
    If amplitude and length are small the PDF is poorly normalized.
********************************************************************************/
    unsigned long int length=x_vec[0];

    /**Calculates the length of PDF_BPW=embedd! and creates the vector*/
    unsigned long int BPW_length=1;
    unsigned long int factorial_basis[embedd]; factorial_basis[embedd-1]=1;//Inicialize the basis for Lemher lexicographic order
    for (unsigned long int i_emb=2;i_emb<=embedd;i_emb++){
        factorial_basis[embedd-i_emb]=BPW_length;
        BPW_length*=i_emb;
    }
    double *PDF_BPW_vec;//Create the pointer
        PDF_BPW_vec=(double*) malloc ( sizeof(double) * (BPW_length+1)); //Create the array with one more position ti save the length
        PDF_BPW_vec[0]=BPW_length;
        for (unsigned long int i_BPW=1;i_BPW<=BPW_length;i_BPW++) PDF_BPW_vec[i_BPW]=0;


    /**Generates the PDF_BPW*/
    double weight_BPW;//Inicializes the Weight_BPW
    double media;//Initialize the media of the embedding vector
    unsigned long int ordering_pattern;//Inicializes the ordering pattern for BP and BPW

    for (unsigned long int i_vec=1;i_vec<=length-embedd+1;i_vec++){//For each position of the input vector.

        /**Calculate the lexicographic order to embedding vector with Lemher method*/
        ordering_pattern=1;//First index of PDF_BPW is 1, the 0 is reserved for the length. Ordering patern is between 1 and emedd!
        for (unsigned long int i_pos=i_vec;i_pos<i_vec+embedd-1;i_pos++){
            for (unsigned long int i_comp=i_pos+1;i_comp<i_vec+embedd;i_comp++){
                if (x_vec[i_pos]>x_vec[i_comp]) ordering_pattern+=factorial_basis[i_pos-i_vec];
            }
        }

        /**Calculate the variance=weight_BPW*/
        media=0;//The media of the embedding vector
        weight_BPW=0;//The variance of the embedding vector
        for (unsigned long int i_pos=i_vec;i_pos<i_vec+embedd;i_pos++) media+=x_vec[i_pos];
        media/=embedd;
        for (unsigned long int i_pos=i_vec;i_pos<i_vec+embedd;i_pos++) weight_BPW+=pow(media-x_vec[i_pos],2); //I use 100*x to give more numerical resolution (See section "problems" in header)
        weight_BPW/=embedd; //Real weight is weight/1000000

        /**Inceremants the BPW PDF and the normalization variable*/
        PDF_BPW_vec[ordering_pattern]+=weight_BPW;
    }

    /**Normalize the PDF_BPW*/
    normalize_PDF(PDF_BPW_vec);

    return PDF_BPW_vec;
}

unsigned long int missing_patterns(double PDF_vec[]){
    unsigned long int length=PDF_vec[0];
    unsigned long int MP=0;

    for (unsigned long int i_PDF=1;i_PDF<=length;i_PDF++){
        if (PDF_vec[i_PDF]==0) MP++;
    }
    return MP;
}

double entropy(double PDF_vec[]){
    /***Cambio el código para calcular la entropía sin normalizar**/
    unsigned long int length=PDF_vec[0];
    double S=0;
    double S_e=log2((double)length);
    double H;

    for (unsigned long int i_PDF=1;i_PDF<=length;i_PDF++) if (PDF_vec[i_PDF]!=0) S-=PDF_vec[i_PDF]*log2(PDF_vec[i_PDF]);

    H=S/S_e;

    return H;
}

double disequilibrum(double PDF_vec[]){
/**Cambio el código para calcular el desequilibrio sin normalizar**/
    unsigned long int length=PDF_vec[0];
    double inv_length=1/(double)length;
    double Q;
    double Q0=-(((double)length+1)*log2((double)length+1)-2*(double)length*log2(2*(double)length)+(double)length*log2((double)length))/(2*(double)length);
    double P_prom;
    double S_prom=0;
    double S_eq=log2((double)length);
    double S_p=0;

    for (unsigned long int i_PDF=1;i_PDF<=length;i_PDF++){
        P_prom=0.5*(PDF_vec[i_PDF]+inv_length);
        S_prom-=P_prom*log2(P_prom);

        if (PDF_vec[i_PDF]!=0) S_p-=PDF_vec[i_PDF]*log2(PDF_vec[i_PDF]);
    }

    Q=(S_prom-0.5*S_eq-0.5*S_p)/Q0;

    return Q;
}

unsigned long int find_period(double x_vec[])
{
/******************************************************
Returns lenght of ventor imputs period

Parameters:
    x_vec is the input vector in double presition. The first position must be
    the length of the vector (x_vec[0]=length_x_vec).

Returned value:
    Period is the iterations needed for a loop

Caution:
    Only for unidimentional maps
*******************************************************/

    unsigned long int length=x_vec[0];
    double UltimoValor = x_vec[length];

    for (unsigned long int iVec = length-1; iVec > 0; iVec--)
    {
        if (x_vec[iVec] == UltimoValor)
        {
            return length - iVec;
        }
    }

   return 0;
}

/*double* load_vector(char name_file[]){
    FILE* input = fopen(name_file,"r");
    }*/

void save_vector(double x_vec[],char name_file[]){
    FILE* out = fopen(name_file,"w");
    for (unsigned long int i_vec=1;i_vec<=x_vec[0];i_vec++) fprintf(out,"%.10e\n",x_vec[i_vec]);
    fclose(out);
    }

#endif // MAXIUTILS_H_INCLUDED
