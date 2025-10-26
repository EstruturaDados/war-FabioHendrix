// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para usar malloc, free, rand, srand (memoria, numeros pseudoaletorios)
#include <string.h> // Para usar strcpy (copiar strings)
#include <time.h>   // Para usar time (gerar semente aleatória)

// Struct do territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Funções
void cadastrarTerritorios(struct Territorio* mapa, int quantidade);
void exibirTerritorios(struct Territorio* mapa, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);


int main() {
    // Inicializa o gerador de números aleatórios. Isso garante que os "dados" rolem números diferentes a cada execução.
    srand(time(NULL));

    int quantidadeTerritorios = 0;
    struct Territorio* mapa = NULL; // Ponteiro para o vetor de territórios

    // Etapa 1: Alocação dinamica
    printf("--- WAR - NÍVEL AVENTUREIRO ---\n");
    printf("Digite a quantidade de territórios para o mapa: ");
    scanf("%d", &quantidadeTerritorios);

    // Aloca memória para o vetor de territórios usando malloc.
    mapa = (struct Territorio*) malloc(quantidadeTerritorios * sizeof(struct Territorio));

    // Verificação de segurança: se a memória não pôde ser alocada, o programa encerra.
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memória para o mapa!\n");
        return 1; // Retorna 1 para indicar um erro
    }

    // Etapa 2: Menu e cadastro
    cadastrarTerritorios(mapa, quantidadeTerritorios);

    int opcao = 0;
    while (opcao != 9) {
        exibirTerritorios(mapa, quantidadeTerritorios);

        printf("\n--- MENU DE AÇÕES ---\n");
        printf("1 - Atacar\n");
        printf("9 - Sair do Jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int idAtacante, idDefensor;
            printf("\nEscolha o território atacante (pelo número): ");
            scanf("%d", &idAtacante);
            printf("Escolha o território defensor (pelo número): ");
            scanf("%d", &idDefensor);

            // Ajusta os IDs para o índice do vetor (ex: usuário digita 1, o índice é 0)
            idAtacante--;
            idDefensor--;

            // --- Validações do Ataque ---
            if (idAtacante < 0 || idAtacante >= quantidadeTerritorios || idDefensor < 0 || idDefensor >= quantidadeTerritorios) {
                printf("\n*** ERRO: Número de território inválido! ***\n\n");
            } else if (idAtacante == idDefensor) {
                printf("\n*** ERRO: Um território não pode atacar a si mesmo! ***\n\n");
            } else if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
                printf("\n*** ERRO: Você não pode atacar um território da sua própria cor! ***\n\n");
            } else if (mapa[idAtacante].tropas <= 1) {
                printf("\n*** ERRO: O território atacante precisa de mais de 1 tropa para atacar! ***\n\n");
            } else {
                // Se tudo estiver certo, chama a função de ataque passando os ponteiros
                printf("\n--- INICIANDO BATALHA! ---\n");
                atacar(&mapa[idAtacante], &mapa[idDefensor]);
            }
        }
    }

    // Etapa Final: liberar memoria
    liberarMemoria(mapa);
    printf("\nJogo finalizado. Memória liberada com sucesso!\n");

    return 0;
}

/**
 * @brief Solicita ao usuário os dados de cada território.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param quantidade Número de territórios a serem cadastrados.
 */
void cadastrarTerritorios(struct Territorio* mapa, int quantidade) {
    printf("\n--- CADASTRO DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf("%s", mapa[i].nome);
        printf("Cor do exército: ");
        scanf("%s", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

/**
 * @brief Exibe os dados de todos os territórios cadastrados.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param quantidade Número de territórios a serem exibidos.
 */
void exibirTerritorios(struct Territorio* mapa, int quantidade) {
    printf("\n\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[(%d)] Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

/**
 * @brief Simula um ataque entre dois territórios.
 * @param atacante Ponteiro para o território que está atacando.
 * @param defensor Ponteiro para o território que está defendendo.
 */
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Simula a rolagem de um dado de 6 lados para cada um
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("-> Atacante (%s) rolou o dado: %d\n", atacante->nome, dadoAtaque);
    printf("-> Defensor (%s) rolou o dado: %d\n", defensor->nome, dadoDefesa);

    // Compara os resultados dos dados
    if (dadoAtaque > dadoDefesa) {
        printf("\n*** VITÓRIA DO ATACANTE! ***\n");
        printf("O território %s foi conquistado!\n", defensor->nome);

        // Transfere a cor do exército para o território conquistado
        strcpy(defensor->cor, atacante->cor);

        // Metade das tropas do atacante se movem para o novo território
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0) tropasMovidas = 1; // Garante que pelo menos 1 tropa se mova

        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

    } else {
        printf("\n*** VITÓRIA DO DEFENSOR! ***\n");
        printf("O ataque ao território %s falhou!\n", defensor->nome);

        // O atacante perde 1 tropa pela derrota na batalha
        if (atacante->tropas > 1) {
            atacante->tropas--;
        }
    }
}

/**
 * @brief Libera a memória que foi alocada dinamicamente para o mapa.
 * @param mapa Ponteiro para a memória a ser liberada.
 */
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}
