// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para usar malloc, free, rand, srand (memoria, numeros pseudoaletorios)
#include <string.h> // Para strcpy, strcmp
#include <time.h>   // Para time

// Struct do territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

struct Jogador {
    char cor[10];
    char* missao; // Ponteiro para a string da missão (alocada dinamicamente)
};

// Area de Missões
// Usei 'const' para indicar que esses dados não devem ser alterados.
const char* MISSOES[] = {
    "CONQUISTAR 10 TERRITORIOS",
    "CONQUISTAR O TERRITORIO A",
    "ELIMINAR O EXERCITO VERMELHO",
    "ELIMINAR O EXERCITO AZUL",
    "TER 30 TROPAS NO TOTAL"
};
const int TOTAL_MISSOES = 5;


// Funções
void cadastrarTerritorios(struct Territorio* mapa, int quantidade);
void exibirMapa(struct Territorio* mapa, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes);
int verificarMissao(struct Jogador* jogador, struct Territorio* mapa, int totalTerritorios);
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int totalJogadores);


int main() {
    srand(time(NULL));

    int totalTerritorios = 0;
    int totalJogadores = 2; // Jogo para 2 jogadores (Azul vs Vermelho)
    struct Territorio* mapa = NULL;
    struct Jogador* jogadores = NULL;

    printf("--- WAR - NÍVEL MESTRE ---\n");
    printf("Digite a quantidade de territórios para o mapa: ");
    scanf("%d", &totalTerritorios);

    // Alocação de memoria
    mapa = (struct Territorio*) malloc(totalTerritorios * sizeof(struct Territorio));
    jogadores = (struct Jogador*) malloc(totalJogadores * sizeof(struct Jogador));

    if (mapa == NULL || jogadores == NULL) {
        printf("Erro: Falha ao alocar memória inicial!\n");
        return 1;
    }

    // Inicializando jogadores e Missões
    // Jogador 1
    strcpy(jogadores[0].cor, "AZUL");
    atribuirMissao(&jogadores[0].missao, MISSOES, TOTAL_MISSOES);
    // Jogador 2
    strcpy(jogadores[1].cor, "VERMELHO");
    atribuirMissao(&jogadores[1].missao, MISSOES, TOTAL_MISSOES);

    printf("\n--- MISSÕES SECRETAS DISTRIBUÍDAS! ---\n");
    printf("Jogador 1 (AZUL), sua missão é: %s\n", jogadores[0].missao);
    printf("Jogador 2 (VERMELHO), sua missão é: %s\n", jogadores[1].missao);
    printf("Boa sorte, generais!\n");

    cadastrarTerritorios(mapa, totalTerritorios);

    int gameOver = 0;
    int jogadorAtual = 0; // Começa com o jogador 0 (AZUL)

    while (!gameOver) {
        exibirMapa(mapa, totalTerritorios);

        printf("\n--- TURNO DO JOGADOR %d (%s) ---\n", jogadorAtual + 1, jogadores[jogadorAtual].cor);
        printf("Sua missão: %s\n", jogadores[jogadorAtual].missao);

        int opcao;
        printf("1 - Atacar | 9 - Desistir\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 9) {
            gameOver = 1;
            printf("\nO jogador %s desistiu!\n", jogadores[jogadorAtual].cor);
            continue;
        }

        int idAtacante, idDefensor;
        printf("Escolha seu território para atacar (pelo número): ");
        scanf("%d", &idAtacante);
        printf("Escolha o território inimigo (pelo número): ");
        scanf("%d", &idDefensor);

        idAtacante--; idDefensor--;

        // Verificações de erros
        if (idAtacante < 0 || idAtacante >= totalTerritorios || idDefensor < 0 || idDefensor >= totalTerritorios) {
            printf("\n*** ERRO: Território inválido! ***\n");
        } else if (strcmp(mapa[idAtacante].cor, jogadores[jogadorAtual].cor) != 0) {
            printf("\n*** ERRO: Você só pode atacar com um território da sua cor! ***\n");
        } else if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
             printf("\n*** ERRO: Você não pode atacar um território aliado! ***\n");
        } else if (mapa[idAtacante].tropas <= 1) {
             printf("\n*** ERRO: Precisa de mais de 1 tropa para atacar! ***\n");
        } else {
            atacar(&mapa[idAtacante], &mapa[idDefensor]);

            // VERIFICAÇÃO DE MISSÃO APÓS O ATAQUE
            if (verificarMissao(&jogadores[jogadorAtual], mapa, totalTerritorios)) {
                gameOver = 1;
                exibirMapa(mapa, totalTerritorios);
                printf("\n\n============================================\n");
                printf("!!! VITÓRIA DO JOGADOR %s !!!\n", jogadores[jogadorAtual].cor);
                printf("Ele cumpriu a missão: '%s'\n", jogadores[jogadorAtual].missao);
                printf("============================================\n");
            }
        }
        // Troca o turno do jogador
        jogadorAtual = (jogadorAtual + 1) % totalJogadores;
    }

    // Liberação da memoria alocada
    liberarMemoria(mapa, jogadores, totalJogadores);
    printf("\nJogo finalizado. Toda a memória foi liberada!\n");

    return 0;
}

/**
 * @brief Aloca memória para a missão de um jogador e sorteia uma do banco de missões.
 * @param destino Ponteiro para o ponteiro da missão do jogador (passagem por referência).
 * @param missoes Vetor com as descrições das missões.
 * @param totalMissoes Tamanho do vetor de missões.
 */
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // Aloca memória suficiente para copiar a string da missão sorteada (+1 para o '\0')
    *destino = (char*) malloc(strlen(missoes[indiceSorteado]) + 1);
    if (*destino != NULL) {
        strcpy(*destino, missoes[indiceSorteado]);
    }
}

/**
 * @brief Verifica se a missão de um jogador foi cumprida.
 * @param jogador Ponteiro para o jogador atual.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param totalTerritorios Quantidade total de territórios no mapa.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(struct Jogador* jogador, struct Territorio* mapa, int totalTerritorios) {
    // Verifica qual é a missão do jogador usando strcmp e aplicamos a regra.

    if (strcmp(jogador->missao, "CONQUISTAR 10 TERRITORIOS") == 0) {
        int contador = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) {
                contador++;
            }
        }
        return (contador >= 10);
    }
    else if (strcmp(jogador->missao, "ELIMINAR O EXERCITO VERMELHO") == 0) {
        // Esta missão só pode ser do jogador AZUL.
        if (strcmp(jogador->cor, "AZUL") != 0) return 0;
        
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "VERMELHO") == 0) {
                return 0; // Se encontrou algum território vermelho, a missão não é cumprida.
            }
        }
        return 1; // Se o loop terminou e não achou nenhum, a missão é cumprida
    }
    else if (strcmp(jogador->missao, "ELIMINAR O EXERCITO AZUL") == 0) {
        // Esta missão só pode ser do jogador VERMELHO.
        if (strcmp(jogador->cor, "VERMELHO") != 0) return 0;

        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "AZUL") == 0) {
                return 0; // Encontrou um território azul.
            }
        }
        return 1; // Não há mais territórios azuis.
    }
    
    // Lógica para outras missões seria adicionada aqui com mais 'else if'.
    // Por enquanto, missões não implementadas nunca são completadas.
    return 0;
}

/**
 * @brief Libera toda a memória alocada dinamicamente durante o jogo.
 * @param mapa Ponteiro para o mapa de territórios.
 * @param jogadores Ponteiro para o vetor de jogadores.
 * @param totalJogadores Quantidade de jogadores.
 */
void liberarMemoria(struct Territorio* mapa, struct Jogador* jogadores, int totalJogadores) {
    // Libera a string da missão de cada jogador
    for (int i = 0; i < totalJogadores; i++) {
        free(jogadores[i].missao);
    }
    // Libera o vetor de jogadores
    free(jogadores);
    // Libera o mapa de territórios
    free(mapa);
}

//Funções do desafio anterior (sem modificações)
void cadastrarTerritorios(struct Territorio* mapa, int quantidade) {
    printf("\n--- CADASTRO DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf("%s", mapa[i].nome);
        printf("Cor inicial: ");
        scanf("%s", mapa[i].cor);
        printf("Tropas iniciais: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirMapa(struct Territorio* mapa, int quantidade) {
    printf("\n\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[(%d)] Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("-> %s (Atacante) rolou o dado: %d\n", atacante->nome, dadoAtaque);
    printf("-> %s (Defensor) rolou o dado: %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\n*** VITÓRIA DO ATACANTE! O território %s foi conquistado! ***\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0) tropasMovidas = 1;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;
    } else {
        printf("\n*** VITÓRIA DO DEFENSOR! O ataque falhou! ***\n");
        if (atacante->tropas > 1) {
            atacante->tropas--;
        }
    }
}
