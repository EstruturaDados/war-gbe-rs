// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};
// --- Protótipos das Funções ---
void limparBufferEntrada(void);
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// FUNÇÃO: cadastrarTerritorios()
// Lê os dados dos territórios e preenche o vetor dinamicamente
void cadastrarTerritorios(struct Territorio* mapa, int total) {
    for (int i = 0; i < total; i++) {
        printf("--- Cadastrando Território %d ---\n", i + 1);

        printf("Nome do território: ");
        fgets((mapa + i)->nome, sizeof((mapa + i)->nome), stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0';

        printf("Cor do exército (ex: Azul, Verde): ");
        fgets((mapa + i)->cor, sizeof((mapa + i)->cor), stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &((mapa + i)->tropas));
        limparBufferEntrada();
        printf("\n");
    }
}

// FUNÇÃO: exibirMapa()
// Exibe todos os territórios e suas informações
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

// FUNÇÃO: atacar()
// Realiza a simulação de ataque entre dois territórios
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Um território não pode atacar outro do mesmo exército!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("O território atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    printf("\nBATALHA: %s (Exército %s) atacando %s (Exército %s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rand() % 6 + 1; // valor entre 1 e 6
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n✅ O atacante venceu a batalha!\n");

        // Defensor perde uma tropa, atacante ganha uma
        if (defensor->tropas > 0) {
            defensor->tropas -= 1;
            atacante->tropas += 1;
        }

        // Conquista somente se tropas do defensor chegarem a zero
        if (defensor->tropas <= 0) {
            printf("O território %s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = atacante->tropas / 2; // metade das tropas do atacante
            atacante->tropas -= defensor->tropas;
        }

    } else {
        printf("\nO defensor resistiu ao ataque.\n");
        atacante->tropas -= 1; // atacante perde uma tropa
        defensor->tropas += 1;   // defensor ganha uma tropa
    }

    printf("\n--- Estado após a batalha ---\n");
    printf("%s -> Tropas: %d | Exército: %s\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("%s -> Tropas: %d | Exército: %s\n\n", defensor->nome, defensor->tropas, defensor->cor);
}

// FUNÇÃO: liberarMemoria()
// Libera o vetor alocado dinamicamente
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    srand(time(NULL)); // garante aleatoriedade nos dados

    int totalTerritorios;
    printf("Quantos territórios terão no mapa? ");
    scanf("%d", &totalTerritorios);
    limparBufferEntrada();

    // Alocação dinâmica do vetor de territórios
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

    // SIMULAÇÃO DE ATAQUES
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
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("\nJogo encerrado e memória liberada. Ate a proxima!\n");
    return 0;

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}