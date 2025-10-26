#include <stdio.h>
#include <string.h>


//Este Struct armazena informações sobre os territorios do jogo.
struct territorio {
    char nome[30]; 
    char cor[10];
    int tropas;

};

//iniciando a coleta de dados

int main(){
     
    //Declaração de um vetor de territorios que comporta a criação de 5 territorios*/
    struct territorio territorios[5];
    int i; //Variaveis de controle para laços em loop

    //1. CADASTRO DOS TERRITORIOS
    printf("--- CADASTRO DE TERRITÓRIOS ---\n");
    for(i = 0; i < 5; i++) {
        printf("---------- Cadastrando territorio %d----------", i + 1);

        //Inicio do cadastro
        printf("Digite o nome do territorio (uma palavra): ");
        scanf("%s", territorios[i].nome);

        printf("Digite o nome da cor do territorio (uma palavra): ");
        scanf("%s", territorios[i].cor);

        printf("Digite o numero de tropas do territorio: ");
        scanf("%d", &territorios[i].tropas);
    }

    //Exibição dos dados

     printf("\n--- DADOS DOS TERRITÓRIOS CADASTRADOS ---\n");
     for(i = 0; i < 5; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
     }

     return 0;
    

}


