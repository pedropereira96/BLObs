#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>   /* para usar caracteres acentuados   */
#include "estrutura.h"
#include "ficheiro_funcoes.c"

extern void LerFicheiro();


int TesteFicheiro(char nome[50]){
    FILE *f=fopen(nome, "r");
    if(f==NULL){
        printf("\nOcorreu erros na abertura de ficheiro.");
        printf("\nPor favor introduza novamente o nome do ficheiro!\n\n");
        return 0;
    }
        fclose(f);
    return 1;
    }


void menu(){
    char FileName[50];
    int R,G,B,D;
    int x;
    int op=-1;
    do{
    do{
        printf("Qual o ficheiro a abrir?\n");
        scanf("%s", FileName);
        x=TesteFicheiro(FileName);
    }while (x!=1);
    
    do {
        printf("Qual o R?\n");
        scanf("%d", &R);
    } while ((R<0) || (R>=255));
    
    do {
        printf("Qual o G?\n");
        scanf("%d", &G);
    } while ((G<0) || (G>=255));
    
    do {
        printf("Qual o B?\n");
        scanf("%d", &B);
    } while ((B<0) || (B>=255));
    
    do {
        printf("Qual o D?\n");
        scanf("%d", &D);
    } while ((D<0) || (D>=255));
    
        
      LerFicheiro(FileName,R,G,B,D);
        
        printf("\n\n\t\tPRESS ENTER!");
        getchar();
        getchar();
        for (int p=0;p<20;p++) {printf("\n");}
        printf("#----------------------------#\n");
        printf("|         Opção              |\n");
        printf("#----------------------------#\n");
        printf("|                            |\n");
        printf("|    1- Iniciar Novamente    |\n");
        printf("|    0 - Sair                |\n");
        printf("|                            |\n");
        printf("#----------------------------#\n");
        do{
        scanf("%d", &op);
        }while(op>1 || op<0);
    }while (op==1);
    
}

int main(){
    menu();
}
