#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++)
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}











    unsigned hash(unsigned x, unsigned i, unsigned B, int tipo)
    {
        if(tipo==0)
        return h_div(x,i,B);

        return h_mul(x,i,B);
    }






    unsigned over_prog( unsigned* coli ,string* tab, string s, unsigned B,int tipo)
    {
        int c1;
        unsigned h, c;
        *coli=0;


        c=converter(s);

        *coli++;
        for(c1=0;tab[h]!=NULL&&c1<B;c1++)// faz o re-hasing ate achar um espaco vago
        {
            h=hash(c,c1,B,tipo);

            if(tab[h]==NULL)
            {
                return h;
            }

            *coli++;//conta as colisoes
        }

        if(c1<B)
        return h;

        return -1;

    }


    unsigned achar(unsigned* ach, string* tab, string s, unsigned B,int tipo)//funcao de busca
    {
        unsigned seta;
        int c1;

        for(c1=0;c1<B;c1++)
        {
            seta=hash(converter(s),c1,B,tipo);//aponta para o hash do buscado e faz o re-hash

            if(tab[seta]==NULL)//se nao tem nada no hash do buscado
            {
                return -1;
            }

            if(!strcmp(s,tab[seta]))
            {
                ach++;// aumenta o indicador de palavras achadas
                return seta;
            }

        }

    return -1;

    }











int main(int argc, char const *argv[])
{
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 50021;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    // cria tabela hash com hash por divisão

    string* tabela=malloc(sizeof(string)*B);

    int c1;

    for(c1=B;c1>0;c1--)//marca os espaços vazios na tabela
    {
        tabela[c1]=NULL;
    }




    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash

        c1= over_prog(&colisoes_h_div,tabela,tabela[i],B,0);
        if(c1==-1)
        {
            printf("deu ruim");

        }
        tabela[c1]=insercoes[i];

    }
    double tempo_insercao_h_div = finaliza_tempo();




    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash

        achar(&encontrados_h_div,tabela, consultas[i],B,0);

    }
    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão

    for(c1=B;c1>0;c1--)
    {
        tabela[c1]=NULL;
    }





    // cria tabela hash com hash por divisão



    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
        for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash

        c1= over_prog(&colisoes_h_mul,tabela,tabela[i],B,1);

        if(c1==-1)
        {
            printf("deu ruim");
        }
        tabela[c1]=insercoes[i];

    }

    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash

        achar(&encontrados_h_mul,tabela, consultas[i],B,1);

    }
    double tempo_busca_h_mul = finaliza_tempo();





    // limpa a tabela hash com hash por multiplicação

    free(tabela);



    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
