
//
//  Header.h
//  Projeto ED
//
//  Created by João Pereira.
//  Copyright © 2019 Pedro pereira. All rights reserved.
//

#ifndef estrutura
#define estrutura


#include <stdio.h>
#include <stdlib.h>



//_________DADOS DE CADA PIXEL_______________
typedef struct pixel
{
    int R;
    int G;
    int B;
    int visitado;
}PIXEL;



//________DADOS DE CADA IMAGEM_______________
typedef struct imagem{
    int total;      //NLinhas * NColunas
    int linhas;
    int colunas;
    int canais;
    PIXEL *dados;
    char nome[100];
}IMAGEM;



//_________NÓ QUE GUARDA A POSIÇÃO DE CADA PIXEL PERTENCENTE AO BLOB______________
typedef struct nodo{
    struct nodo* seguinte;
    int info; //posicao da informacao do pixel
}NODO_BLOB;



//__________LISTA QUE GUARDA DADOS DE CADA BLOB________________
typedef struct blob
{
    NODO_BLOB *inicio;
    int conta;
    double mediar, mediag, mediab;                             //Médias
    double desviopadrao_r, desviopadrao_g, desviopadrao_b;     //Desvio Padrao
    int linhacentral, colunacentral;//linha e coluna central do blob
}BLOB;



//__________NÓ QUE GUARDA LISTAS DE BLOBS____________
typedef struct nolista{
     BLOB *Elementos;
     struct nolista *seguinte;
}NO_LISTA_BLOB;



//__________LISTA QUE GUARDA AS LISTAS DOS NÓS DA LISTA DE BLOBS
typedef struct ListadeZona{
    NO_LISTA_BLOB *inicio;
    int Elementos;
    
}LISTA_BLOB;



#endif /* estrutura */
