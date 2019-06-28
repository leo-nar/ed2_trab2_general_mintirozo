#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "int");

    int * inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    int aux;

    encontrados=0;
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        for(int k=0;k<N;k++){
            if(consultas[i]==entradas[k]){
                encontrados++;
                if(k!=0){
                    aux=entradas[k];
                    entradas[k]=entradas[k-1];
                    entradas[k-1]=aux;
                }
                break;
            }
        }
    }

    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
