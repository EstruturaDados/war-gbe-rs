#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ESTRUTURA: Território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// VETOR DE MISSÕES
char* missoesDisponiveis[] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas do exercito Vermelho",
    "Controlar ao menos 4 territorios",
    "Possuir mais de 15 tropas somadas",
    "Conquistar 2 territorios de cores diferentes"
};
int totalMissoes = 5;

// FUNÇÃO: limparBufferEntrada()
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// FUNÇÃO: cadastrarTerritorios()
void cadastrarTerritorios(struct Territorio* mapa, int total) {
    for (int i = 0; i < total; i++) {
        printf("--- Cadastrando Território %d ---\n", i + 1);

        printf("Nome do território: ");
        fgets((mapa + i)->nome, sizeof((mapa + i)->nome), stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0';

        printf("Cor do exército (ex: Azul, Verde, Vermelho): ");
        fgets((mapa + i)->cor, sizeof((mapa + i)->cor), stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &((mapa + i)->tropas));
        limparBufferEntrada();
        printf("\n");
    }
}

// FUNÇÃO: exibirMapa()
void exibirMapa(struct Territorio* mapa, int total) {
    printf("\n============================================\n");
    printf("MAPA DO MUNDO - ESTADO ATUAL:\n");
    printf("============================================\n\n");

    for (int i = 0; i < total; i++) {
        printf("%d. %s (Exército %s, Tropas: %d)\n",
               i + 1,
               (mapa + i)->nome,
               (mapa + i)->cor,
               (mapa + i)->tropas);
    }
}

// FUNÇÃO: atribuirMissao()
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    *destino = (char*) malloc((strlen(missoes[indice]) + 1) * sizeof(char));
    if (*destino == NULL) {
        printf("Erro ao alocar memoria para missao.\n");
        exit(1);
    }
    strcpy(*destino, missoes[indice]);
}

// FUNÇÃO: exibirMissao()
void exibirMissao(char* missao) {
    printf("\nSua missão secreta é:\n-> %s\n\n", missao);
}

// FUNÇÃO: verificarMissao()
int verificarMissao(char* missao, struct Territorio* mapa, int total, char* corJogador) {
    int territoriosDaCor = 0;
    int totalTropas = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosDaCor++;
            totalTropas += mapa[i].tropas;
        }
    }

    if (strcmp(missao, "Controlar ao menos 4 territorios") == 0 && territoriosDaCor >= 4)
        return 1;

    if (strcmp(missao, "Possuir mais de 15 tropas somadas") == 0 && totalTropas > 15)
        return 1;

    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0 && territoriosDaCor >= 3)
        return 1;

    if (strcmp(missao, "Eliminar todas as tropas do exercito Vermelho") == 0) {
        int tropasVermelhas = 0;
        for (int i = 0; i < total; i++)
            if (strcmp(mapa[i].cor, "Vermelho") == 0)
                tropasVermelhas += mapa[i].tropas;
        if (tropasVermelhas == 0)
            return 1;
    }

    if (strcmp(missao, "Conquistar 2 territorios de cores diferentes") == 0 && territoriosDaCor >= 2)
        return 1;

    return 0;
}

// FUNÇÃO: atacar()
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Um território não pode atacar outro do mesmo exército!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("O território atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    printf("\nBATALHA: %s (%s) atacando %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nO atacante venceu a batalha!\n");

        if (defensor->tropas > 0) {
            defensor->tropas -= 1;
            atacante->tropas += 1;
        }

        if (defensor->tropas <= 0) {
            printf("O território %s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2;
            atacante->tropas -= defensor->tropas;
        }

    } else {
        printf("\nO defensor resistiu ao ataque.\n");
        atacante->tropas -= 1;
        defensor->tropas += 1;
    }

    printf("\n--- Estado após a batalha ---\n");
    printf("%s -> Tropas: %d | Exército: %s\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("%s -> Tropas: %d | Exército: %s\n\n", defensor->nome, defensor->tropas, defensor->cor);
}

// FUNÇÃO: liberarMemoria()
void liberarMemoria(struct Territorio* mapa, char** missoes, int totalJogadores, char** coresJogadores) {
    free(mapa);
    for (int i = 0; i < totalJogadores; i++)
        free(missoes[i]);
    free(missoes);
    free(coresJogadores);
}

// FUNÇÃO PRINCIPAL
int main() {
    srand(time(NULL));

    int totalTerritorios;
    printf("Quantos territórios terão no mapa? ");
    scanf("%d", &totalTerritorios);
    limparBufferEntrada();

    struct Territorio* mapa = (struct Territorio*) malloc(totalTerritorios * sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    printf("\n=================================\n");
    printf("CADASTRO DOS TERRITÓRIOS\n");
    printf("=================================\n\n");

    cadastrarTerritorios(mapa, totalTerritorios);
    exibirMapa(mapa, totalTerritorios);

    // SISTEMA DE JOGADORES E MISSÕES
    int totalJogadores = 2;
    char** missoesJogadores = (char**) malloc(totalJogadores * sizeof(char*));
    char** coresJogadores = (char**) malloc(totalJogadores * sizeof(char*));

    coresJogadores[0] = "Azul";
    coresJogadores[1] = "Verde";

    for (int j = 0; j < totalJogadores; j++) {
        atribuirMissao(&missoesJogadores[j], missoesDisponiveis, totalMissoes);
        printf("\nJogador %d (%s): ", j + 1, coresJogadores[j]);
        exibirMissao(missoesJogadores[j]);
    }

    // LOOP PRINCIPAL DE JOGO
    int opcao;
    do {
        printf("\nDeseja realizar um ataque? (1-Sim / 0-Não): ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        if (opcao == 1) {
            int iAtacante, iDefensor;
            printf("Escolha o número do território atacante: ");
            scanf("%d", &iAtacante);
            printf("Escolha o número do território defensor: ");
            scanf("%d", &iDefensor);
            limparBufferEntrada();

            if (iAtacante < 1 || iAtacante > totalTerritorios ||
                iDefensor < 1 || iDefensor > totalTerritorios ||
                iAtacante == iDefensor) {
                printf("Escolha inválida.\n");
                continue;
            }

            atacar(&mapa[iAtacante - 1], &mapa[iDefensor - 1]);
            exibirMapa(mapa, totalTerritorios);

            // Verifica se algum jogador venceu
            for (int j = 0; j < totalJogadores; j++) {
                if (verificarMissao(missoesJogadores[j], mapa, totalTerritorios, coresJogadores[j])) {
                    printf("\nJOGADOR %d (%s) CUMPRIU SUA MISSÃO!\n", j + 1, coresJogadores[j]);
                    printf("Missão: %s\n", missoesJogadores[j]);
                    liberarMemoria(mapa, missoesJogadores, totalJogadores, coresJogadores);
                    printf("\nJogo encerrado.\n");
                    return 0;
                }
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missoesJogadores, totalJogadores, coresJogadores);
    printf("\nJogo encerrado e memória liberada. Até a próxima!\n");
    return 0;
}
