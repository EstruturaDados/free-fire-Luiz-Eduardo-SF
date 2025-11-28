/*
 * CODIGO FEITO POR: LUIZ EDUARDO DA SILVA FERNANDES
 * ESSE RECADO EQUIVALE AO MESMO DA ATIVADE WAR
 * ESPERO QUE CORRIJAM E GOSTEM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Para usar o tipo bool

// --- Constantes e Tipos de Dados ---
#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

// Enumeração para os critérios de ordenação (Nível Mestre)
typedef enum {
    POR_NOME = 1,
    POR_TIPO = 2,
    POR_PRIORIDADE = 3
} CriterioOrdenacao;

// Struct para representar cada item na mochila (Nível Mestre com prioridade)
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // De 1 a 5
} Item;

// Variáveis Globais de Estado
int totalItens = 0; // Contador de itens atualmente na mochila
bool estaOrdenadoPorNome = false; // Flag para a Busca Binária

// --- Protótipos das Funções ---
void exibirMenu();
void adicionarItem(Item mochila[]);
void removerItem(Item mochila[]);
void listarItens(const Item mochila[]);
void buscarSequencial(const Item mochila[]);
void ordenarMochila(Item mochila[], int* comparacoes);
void buscarBinaria(const Item mochila[]);

// Funções Auxiliares
int buscarItemPorNome(const Item mochila[], const char* nomeBusca);
void trocar(Item* a, Item* b);

// ----------------------------------------------------
// --- FUNÇÕES DE UTILIDADE E AUXILIARES ---
// ----------------------------------------------------

// Troca o conteúdo de duas structs Item. Usada no Insertion Sort.
void trocar(Item* a, Item* b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// Retorna o índice do item ou -1 se não encontrado.
// Usada pelas funções 'removerItem' e 'buscarSequencial'.
int buscarItemPorNome(const Item mochila[], const char* nomeBusca) {
    int i;
    for (i = 0; i < totalItens; i++) {
        // Conceito Adicionado: Comparação de strings (strcmp)
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            return i; // Item encontrado
        }
    }
    return -1; // Item não encontrado
}

// ----------------------------------------------------
// --- FUNCIONALIDADES BÁSICAS (NÍVEL NOVATO) ---
// ----------------------------------------------------

// Exibe as opções do menu interativo.
void exibirMenu() {
    printf("\n\n=== Mochila de Sobrevivência (Nível Mestre) ===\n");
    printf("1. Adicionar Item\n");
    printf("2. Remover Item por nome\n");
    printf("3. Listar Itens (Tabela)\n");
    printf("4. Buscar Item por nome (Sequencial)\n");
    printf("5. Ordenar Mochila\n");
    printf("6. Buscar Item por nome (Binária)\n");
    printf("0. Sair e Escapar da Ilha\n");
    printf("==============================================\n");
    printf("Escolha uma opção: ");
}

// Adiciona um novo item à mochila.
void adicionarItem(Item mochila[]) {
    if (totalItens >= MAX_ITENS) {
        printf("\n⚠️ Mochila cheia! Capacidade máxima (%d) atingida.\n", MAX_ITENS);
        return;
    }

    Item novoItem;
    
    // Leitura dos dados
    printf("\n--- Adicionar Item ---\n");
    printf("Nome do item: ");
    scanf("%29s", novoItem.nome); // Limita a leitura para evitar buffer overflow
    
    printf("Tipo (Ex: Arma, Cura, Munição): ");
    scanf("%19s", novoItem.tipo);
    
    printf("Quantidade: ");
    while (scanf("%d", &novoItem.quantidade) != 1 || novoItem.quantidade <= 0) {
        printf("Quantidade inválida. Digite um número positivo: ");
        while (getchar() != '\n'); // Limpa o buffer de entrada
    }
    
    // Nível Mestre: Leitura da Prioridade
    printf("Prioridade (1 a 5, sendo 5 a mais alta): ");
    while (scanf("%d", &novoItem.prioridade) != 1 || novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("Prioridade inválida. Digite um valor entre 1 e 5: ");
        while (getchar() != '\n');
    }

    // Adiciona o item ao vetor
    mochila[totalItens] = novoItem;
    totalItens++;
    
    // Atualiza a flag de ordenação (a inserção quebra a ordem)
    estaOrdenadoPorNome = false; 
    
    printf("\n✅ Item '%s' adicionado com sucesso.\n", novoItem.nome);
}

// Remove um item da mochila pelo nome.
void removerItem(Item mochila[]) {
    if (totalItens == 0) {
        printf("\n⚠️ A mochila está vazia. Nada para remover.\n");
        return;
    }
    
    char nomeBusca[MAX_NOME];
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: ");
    scanf("%29s", nomeBusca);

    int indice = buscarItemPorNome(mochila, nomeBusca);

    if (indice != -1) {
        // Desloca todos os itens seguintes uma posição para trás
        int i;
        for (i = indice; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        
        // A remoção pode manter ou quebrar a ordem, mas para simplificar, setamos como falso.
        estaOrdenadoPorNome = false; 
        
        printf("\n✅ Item '%s' removido com sucesso.\n", nomeBusca);
    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// Lista todos os itens da mochila em formato de tabela.
void listarItens(const Item mochila[]) {
    if (totalItens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }

    printf("\n--- Inventário Atual (Total: %d itens) ---\n", totalItens);
    printf("========================================================================\n");
    // Nível Mestre: Adiciona Prioridade na listagem
    printf("| %-28s | %-18s | %-10s | %-10s |\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("========================================================================\n");
    
    int i;
    for (i = 0; i < totalItens; i++) {
        printf("| %-28s | %-18s | %-10d | %-10d |\n", 
               mochila[i].nome, 
               mochila[i].tipo, 
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("========================================================================\n");
    
    // Nível Mestre: Exibe o estado da ordenação
    printf("Estado de Ordenação por Nome: %s\n", estaOrdenadoPorNome ? "ORDENADO" : "DESORDENADO");
}

// ----------------------------------------------------
// --- FUNCIONALIDADES NÍVEL AVENTUREIRO ---
// ----------------------------------------------------

// Busca sequencial (linear) por nome.
void buscarSequencial(const Item mochila[]) {
    if (totalItens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }
    
    char nomeBusca[MAX_NOME];
    printf("\n--- Buscar Item (Sequencial) ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    scanf("%29s", nomeBusca);

    int indice = buscarItemPorNome(mochila, nomeBusca);
    
    // Conceito Adicionado: Controle com flag (aqui, o índice != -1 serve como flag)
    if (indice != -1) {
        printf("\n✅ Item '%s' ENCONTRADO!\n", nomeBusca);
        // Exibição detalhada do item encontrado
        printf("Detalhes:\n");
        printf("  Nome: %s\n", mochila[indice].nome);
        printf("  Tipo: %s\n", mochila[indice].tipo);
        printf("  Quantidade: %d\n", mochila[indice].quantidade);
        printf("  Prioridade: %d\n", mochila[indice].prioridade);
    } else {
        // Saída: Mensagem de erro amigável
        printf("\n❌ Item '%s' NÃO encontrado na mochila.\n", nomeBusca);
    }
}

// ----------------------------------------------------
// --- FUNCIONALIDADES NÍVEL MESTRE ---
// ----------------------------------------------------

// Ordena a mochila usando Insertion Sort pelo critério escolhido.
void ordenarMochila(Item mochila[], int* comparacoes) {
    if (totalItens < 2) {
        printf("\n⚠️ É necessário no mínimo 2 itens para ordenar.\n");
        return;
    }

    int i, j, criterio;
    Item chave;

    printf("\n--- Menu de Ordenação ---\n");
    printf("1. Ordenar por Nome\n");
    printf("2. Ordenar por Tipo\n");
    printf("3. Ordenar por Prioridade (Decrescente)\n");
    printf("Escolha o critério: ");

    if (scanf("%d", &criterio) != 1 || criterio < 1 || criterio > 3) {
        printf("❌ Critério inválido.\n");
        return;
    }

    *comparacoes = 0; // Zera o contador de comparações
    
    // Implementação da Ordenação com Insertion Sort
    for (i = 1; i < totalItens; i++) {
        chave = mochila[i];
        j = i - 1;

        while (j >= 0) {
            (*comparacoes)++; // Contador de comparações

            bool condicaoTroca = false;
            // Verifica a condição de troca baseada no critério
            if (criterio == POR_NOME) {
                // Ordenação Alfabética por Nome (Crescente)
                if (strcmp(mochila[j].nome, chave.nome) > 0) {
                    condicaoTroca = true;
                }
            } else if (criterio == POR_TIPO) {
                // Ordenação Alfabética por Tipo (Crescente)
                if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                    condicaoTroca = true;
                }
            } else if (criterio == POR_PRIORIDADE) {
                // Ordenação Numérica por Prioridade (Decrescente)
                if (mochila[j].prioridade < chave.prioridade) {
                    condicaoTroca = true;
                }
            }

            if (condicaoTroca) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }

    // Atualiza a flag: só está ordenado por nome se a opção 1 foi escolhida
    estaOrdenadoPorNome = (criterio == POR_NOME);

    printf("\n✅ Mochila ordenada com sucesso por critério %d.\n", criterio);
    // Saída: Contador de comparações
    printf("Análise de Desempenho: %d comparações realizadas.\n", *comparacoes);
}

// Busca binária por nome (requer que a lista esteja ordenada por nome).
void buscarBinaria(const Item mochila[]) {
    if (totalItens == 0) {
        printf("\n⚠️ A mochila está vazia.\n");
        return;
    }

    // Validação de pré-requisito (Busca Binária só funciona em lista ordenada)
    if (!estaOrdenadoPorNome) {
        printf("\n❌ ERRO: A busca binária requer que a mochila esteja ORDENADA POR NOME primeiro (Opção 5).\n");
        return;
    }
    
    char nomeBusca[MAX_NOME];
    printf("\n--- Buscar Item (Binária) ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    scanf("%29s", nomeBusca);
    
    // Implementação da Busca Binária (binary search)
    int esq = 0;
    int dir = totalItens - 1;
    int meio;
    int comparacoes = 0;
    int indice = -1;

    while (esq <= dir) {
        comparacoes++;
        meio = esq + (dir - esq) / 2; // Evita overflow para arrays muito grandes
        
        int resultado = strcmp(mochila[meio].nome, nomeBusca);
        
        if (resultado == 0) {
            indice = meio; // Encontrado
            break;
        } else if (resultado < 0) {
            esq = meio + 1; // Item está na metade direita
        } else {
            dir = meio - 1; // Item está na metade esquerda
        }
    }

    if (indice != -1) {
        printf("\n✅ Item '%s' ENCONTRADO (Posição %d).\n", nomeBusca, indice + 1);
        printf("  Tipo: %s\n", mochila[indice].tipo);
        printf("  Prioridade: %d\n", mochila[indice].prioridade);
        printf("  Comparações na Busca Binária: %d\n", comparacoes);
    } else {
        printf("\n❌ Item '%s' NÃO encontrado após %d comparações.\n", nomeBusca, comparacoes);
    }
}

// ----------------------------------------------------
// --- FUNÇÃO PRINCIPAL (MAIN) ---
// ----------------------------------------------------

int main() {
    Item mochila[MAX_ITENS]; // Vetor estático de 10 itens
    int opcao;
    int comparacoesOrdenacao = 0; // Variável para o contador de comparações

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\n❌ Entrada inválida. Tente novamente.\n");
            opcao = -1; // Garante que o loop continue
            while (getchar() != '\n');
            continue;
        }
        
        switch (opcao) {
            case 1:
                adicionarItem(mochila);
                break;
            case 2:
                removerItem(mochila);
                break;
            case 3:
                listarItens(mochila);
                break;
            case 4:
                // Funcionalidade do Nível Aventureiro
                buscarSequencial(mochila);
                break;
            case 5:
                // Funcionalidade do Nível Mestre
                ordenarMochila(mochila, &comparacoesOrdenacao);
                break;
            case 6:
                // Funcionalidade do Nível Mestre
                buscarBinaria(mochila);
                break;
            case 0:
                printf("\n👋 Parabéns, você escapou da ilha! Fim do programa.\n");
                break;
            default:
                printf("\n❌ Opção inválida. Digite um número de 0 a 6.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}