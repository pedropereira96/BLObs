#include "estrutura.h"
#include <stdio.h>
#include <string.h>
#include <math.h>







//--------CRIA A IMAGEM, COM TODOS OS DADOS LIMPOS----------------------------------------------------
 IMAGEM *criar_imagem(){
    IMAGEM *imagem = (IMAGEM *)malloc(sizeof(IMAGEM));
    imagem->linhas = 0;
    imagem->colunas = 0;
    imagem->canais = 0;
    imagem->nome[0]='\0';
    imagem->dados = NULL;
    return imagem;
}



// Cria a Lista de BLOB
LISTA_BLOB *criarlistadeBlob(){
    LISTA_BLOB *lb=(LISTA_BLOB *)malloc(sizeof(LISTA_BLOB));
    lb->inicio=NULL;
    lb->Elementos=0;
   
    return lb;
}




//--------DESTROI A IMAGEM------------------------------------------------------------
void DestruirImagem(IMAGEM *imagem)
{
    free(imagem->dados);
    free(imagem);
}

//liberta da memória todos os nodos de blob, que existam
void LibertarBlob(BLOB *b)
{
    NODO_BLOB *P;
    NODO_BLOB *Aux;
    
    for ( P = b->inicio; P; P = Aux )
    {
        Aux = P->seguinte;
        free(P);
        
    }
    free(b);
}


//liberta da memória as listas de nodos de blob, que existam
void  DestruirLB(LISTA_BLOB *lb)
{
    NO_LISTA_BLOB *Aux;
    NO_LISTA_BLOB *P;
    
    for ( P = lb->inicio; P; P = Aux )
    {
        Aux = P->seguinte;
        LibertarBlob(P->Elementos);
    }
    free(lb);
}







//----------------------Adicionar DADOS--------------------------



//Cria o BLob de Inicio
BLOB *AddBlob(){
    BLOB *b;
    b=(BLOB *)malloc(sizeof(BLOB));
    
    b->inicio = NULL;
    b->conta=0;
    b->mediar = b->mediag = b->mediab = 0;
    b->desviopadrao_r = b->desviopadrao_g = b->desviopadrao_b=0;
    
    return b;
}


//Adiciona ao blobs ordenados
void  AddOrdenadoBlob(LISTA_BLOB *lb, BLOB *b)
{
    NO_LISTA_BLOB *No;
    NO_LISTA_BLOB *P;
    NO_LISTA_BLOB *ant;
    
    ant = 0;
    P = lb->inicio;
    No = (NO_LISTA_BLOB *)malloc(sizeof(NO_LISTA_BLOB));
    
    No->Elementos = b;
    No->seguinte = NULL;
    lb->Elementos++;
    if ( P )
    {
        while ( P && P->Elementos->conta >= b->conta )
        {
            ant = P;
            P = P->seguinte;
        }
        if ( ant )
        {
            ant->seguinte = No;
            No->seguinte = P;
        }
        else
        {
            No->seguinte = lb->inicio;
            lb->inicio = No;
        }
    }
    else
    {
        lb->inicio = No;
    }
}


//Insere por ordem ao blob caso o pixel pertença aos dados pretendidos
void InseriremPrimeiroBlob(BLOB *b, int posicao)
{
    NODO_BLOB *Nob;
   
    b->conta++;
    Nob = (NODO_BLOB *)malloc(sizeof(NODO_BLOB));
    Nob->info = posicao;
    Nob->seguinte= b->inicio;
    b->inicio = Nob;
}








//------------ENCONTRAR PIXEL EM CRITERIOS-----------------------------------------------



// Verifica se está dento dos criterios
int cor_teste(IMAGEM *imagem, int posicao,int R,int G,int B,int D){
    if ( posicao < 0 || imagem->total <= posicao ) //Verifica se a posição nao está fora da imagem
        return 0;
    if ( abs(imagem->dados[posicao].R - R) <= D )     //Verifica se está R está dentro dos critérios
        if ( abs(imagem->dados[posicao].G - G) <= D )   //Verifica se está G está dentro dos critérios
            if( abs(imagem->dados[posicao].B - B) <= D )  //Verifica se está B está dentro dos critérios
                return 1;
    return 0;
}


//Algoritmo principal do blob, percorre o pixel correspondente a toda a sua volta
//Encontrando algum pixel que corresponde aos critérios, ele vê a sua direita, baixo, esquerda e cima
void Percorre_Blob( BLOB *b, IMAGEM *imagem, int posicao, int R, int G, int B, int D)
{
    int coluna;
    int linha;
    
    if ( imagem->dados[posicao].visitado != 1 )
    {
        linha = posicao / imagem->colunas;
        coluna = posicao % imagem->colunas;
        
        if ( linha >= 0 && imagem->linhas > linha && coluna >= 0 && imagem->colunas > coluna )
        {
            InseriremPrimeiroBlob(b, posicao);
            imagem->dados[posicao].visitado = 1;
            
            
            if (cor_teste(imagem, posicao + 1, R, G, B, D) == 1)                    //Testa o pixel da direita
                Percorre_Blob(b, imagem, posicao + 1, R, G, B, D);
            
            if (cor_teste(imagem, posicao + imagem->colunas, R, G, B, D) == 1)      //Testa o pixel de baixo
                Percorre_Blob(b, imagem, posicao + imagem->colunas, R, G, B, D);
            
            if (cor_teste(imagem, posicao - 1, R, G, B, D) == 1)                    //Testa o pixel da esquerda
                Percorre_Blob(b, imagem, posicao - 1, R, G, B, D);
            
            if (cor_teste(imagem, posicao - imagem->colunas, R, G, B, D) == 1)      //Testa o pixel de cima
                Percorre_Blob(b, imagem, posicao - imagem->colunas, R, G, B, D);
        }
    }
}



//procura o primeiro pixel encontrado em cada blob, que depois de encontrar chamo a função marcar_zona que procura pelo blob todo
LISTA_BLOB *CalcularBlob(IMAGEM *imagem, int R, int G, int B, int D){
    BLOB *b;
    LISTA_BLOB *lb;
    int posicao=0;
    lb=criarlistadeBlob();
    while(imagem->total > posicao)
    {
        if(imagem->dados[posicao].visitado!=1){
            if(cor_teste(imagem, posicao, R,G,B,D)==1){
                b=AddBlob();
                Percorre_Blob(b,imagem,posicao,R,G,B,D);
                AddOrdenadoBlob(lb,b);
            }
            imagem->dados[posicao].visitado=1;
        }
        posicao++;
    }
    return lb;
}



//------------------------Mostrar DADOS----------------------------------------
void MostrarListaBlob(IMAGEM *imagem, LISTA_BLOB *b)
{
    NO_LISTA_BLOB *nlb;
    
    if ( imagem && b )
    {
        for ( nlb = b->inicio; nlb; nlb = nlb->seguinte )
        {
            printf("Na imagem %s centro (%d, %d) %d Pixeis tem DP (R - %.3f, G - %.3f, B - %-3f)\n", imagem->nome,nlb->Elementos->linhacentral, nlb->Elementos->colunacentral, nlb->Elementos->conta,nlb->Elementos->desviopadrao_r, nlb->Elementos->desviopadrao_g, nlb->Elementos->desviopadrao_b);
//
        }
    }
}







//------------------------DESVIOS PADROES----------------------------------------

//calcula o desvio padrao de cada blob e a linha e coluna de cada blob
void CalcularDPBLOB(IMAGEM *imagem, BLOB *blob)
{
    NODO_BLOB *nb;
    NODO_BLOB *Pa;
    float soma_r = 0.0;      //soma para a média
    float soma_g = 0.0;      //soma para a média
    float soma_b = 0.0;      //soma para a média
    float soma_Rc, soma_Gc, soma_Bc;  //soma de (valor-media)^2
    int imColunas = 0;
    int imLinhas = 0;
    
    if ( blob->conta )
    {
        for ( nb = blob->inicio; nb; nb = nb->seguinte )
        {

            imLinhas += nb->info / imagem->colunas;
            imColunas += nb->info % imagem->colunas;
            soma_r = (long double)imagem->dados[nb->info].R + soma_r;
            soma_g = (long double)imagem->dados[nb->info].G + soma_g;
            soma_b = (long double)imagem->dados[nb->info].B + soma_b;
        }
        blob->linhacentral = imLinhas / blob->conta;
        blob->colunacentral = imColunas / blob->conta;
        blob->mediar = soma_r / (long double)blob->conta;
        blob->mediag = soma_g / (long double)blob->conta;
        blob->mediab = soma_b / (long double)blob->conta;
        soma_Rc = 0.0;
        soma_Gc = 0.0;
        soma_Bc = 0.0;
                for ( Pa = blob->inicio; Pa; Pa = Pa->seguinte )
        {
            soma_Rc = soma_Rc + ((long double)imagem->dados[Pa->info].R - blob->mediar) * ((long double)imagem->dados[Pa->info].R - blob->mediar);
            
            soma_Gc = soma_Gc + ((long double)imagem->dados[Pa->info].G - blob->mediag) * ((long double)imagem->dados[Pa->info].G - blob->mediag);
            
            soma_Bc = soma_Bc + ((long double)imagem->dados[Pa->info].B - blob->mediab) * ((long double)imagem->dados[Pa->info].B - blob->mediab);
        }
        
        blob->desviopadrao_r = sqrt(soma_Rc / (long double)blob->conta);
        blob->desviopadrao_g = sqrt(soma_Gc / (long double)blob->conta);
        blob->desviopadrao_b = sqrt(soma_Bc / (long double)blob->conta);
    }
}



//calcula o desvio oadrao de todos os blobs
void  CalcularDP(IMAGEM *imagem, LISTA_BLOB *b)
{
    NO_LISTA_BLOB *nb;
    
    if ( imagem && b )
    {
        for ( nb = b->inicio; nb; nb = nb->seguinte )
            CalcularDPBLOB(imagem, nb->Elementos);
    }
}

void CalcularMenorDesviosPadrao(IMAGEM *imagem, LISTA_BLOB *b, int *linha_blob_menor_desvio_padrao, int *coluna_blob_menor_desvio_padrao, int *NPixeis_Blob_Menor_DP, double *DPmenor, char *ImgcomDPmenor)
{
    double desv_p;
    NO_LISTA_BLOB *lb;
    
    if ( imagem && b )
    {
        for ( lb = b->inicio; lb; lb = lb->seguinte )
        {
            desv_p = sqrt(lb->Elementos->desviopadrao_r * lb->Elementos->desviopadrao_r + lb->Elementos->desviopadrao_g * lb->Elementos->desviopadrao_g + lb->Elementos->desviopadrao_b * lb->Elementos->desviopadrao_b);
            
            if ( *DPmenor > (long double)desv_p )
            {
                *DPmenor = desv_p;
                *linha_blob_menor_desvio_padrao = lb->Elementos->linhacentral;
                *coluna_blob_menor_desvio_padrao = lb->Elementos->colunacentral;
                *NPixeis_Blob_Menor_DP = lb->Elementos->conta;
                strcpy(ImgcomDPmenor, imagem->nome);
            }
        }
    }
}











// -------------LER DO FICHEIRO E FAZER TODOS OS CALCULOS E RESULTADOS------------------------------------------
int LerFicheiro(char *nf,int R,int G,int B,int D){
    IMAGEM *imagem;
    LISTA_BLOB *lb;
    
    
    char nome_mais_blob[50];
    double desvio_padrao_menor = 9999;
    char nome_dp_menor[50];
    int quant_pixel_blob_menor_dp =-999;    //Quantidade de pixeis do menor desvio padrao
    int coluna_blob_menor_desvio_padrao =-999;
    int linha_blob_menor_desvio_padrao =-999;
    int mais_blob = -999;
    int dd=0;
    int coluna1 = 0,linha1 = 0, coluna2 = 0, linha2 = 0;
    char nomemaiordistancia[50];
    
    
    FILE *f = fopen(nf,"r");
    if (!f) {
        printf("\nErro a abrir ficheiro!");
        return 0;
    }
    
//      Percorre até ao final do ficheiro
    while(!feof(f)){
        
        imagem = criar_imagem();

        fscanf(f,"%s",imagem->nome);
        fscanf(f,"%d %d %d",&(imagem->linhas),&(imagem->colunas), &(imagem->canais));

        imagem->dados = (PIXEL *)malloc(sizeof(PIXEL)*imagem->linhas*imagem->colunas);
        
        imagem->total=imagem->linhas*imagem->colunas;
        for (int i=0; i<(imagem->linhas)*(imagem->colunas);i++){
            fscanf(f,"%d",&(imagem->dados[i].R));
            fscanf(f,"%d",&(imagem->dados[i].G));
            fscanf(f,"%d",&(imagem->dados[i].B));
            imagem->dados[i].visitado=0;
        }
        
        lb=CalcularBlob(imagem,R,G,B,D);
        CalcularDP(imagem,lb);
        
        
        
        
        
        
        CalcularMenorDesviosPadrao(imagem,lb, &linha_blob_menor_desvio_padrao, &coluna_blob_menor_desvio_padrao,&quant_pixel_blob_menor_dp, &desvio_padrao_menor, nome_dp_menor);
        
        MostrarListaBlob(imagem, lb);
        
        if ( lb->Elementos > mais_blob )
        {
            mais_blob = lb->Elementos;
            strcpy(nome_mais_blob, imagem->nome);
        }
        
        
        
        DestruirImagem(imagem);
        DestruirLB(lb);
        lb = 0;
        
        }
        
    
    fclose(f);
   
    if ( mais_blob >= 0 )
        printf("\n\nImagem com mais BLOBS é [%s] com %d BLOBS\n", nome_mais_blob, mais_blob);
    if ( quant_pixel_blob_menor_dp >= 0 )
        printf("\nMenor DP é o BLOB (%d, %d) com %d Pixeis e DP (%f) da imagem [%s]\n", linha_blob_menor_desvio_padrao, coluna_blob_menor_desvio_padrao, quant_pixel_blob_menor_dp, desvio_padrao_menor, nome_dp_menor);

    return 0;
}


