#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matriz_esparsa.c"

//ajustar a main antiga e ver se ainda há erros na matriz.c
int main() {
    // Inicialização do Grafo
    Matriz_esparsa meu_grafo;

    if (!inicializar_matriz(&meu_grafo, 2, -1)) {
        printf("Erro fatal ao inicializar o sistema.\n");
        return 1;
    }

    // Pré-cadastro dos aeroportos 
    inserir_aeroporto(&meu_grafo, "Brasilia", "BSB");      
    inserir_aeroporto(&meu_grafo, "Belo Horizonte", "CNF"); 

    int opcao = 0;
    char sigla_ori[5], sigla_dest[5], cidade_busca[50];
    int distancia;

    // Menu de Opções
    do {
        printf("    SISTEMA DE GERENCIAMENTO DE VOOS\n");
        printf("1 - Cadastrar Novo Voo\n");
        printf("2 - Consultar Cronograma de um Aeroporto\n");
        printf("3 - Sair do Sistema\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("\n--- CADASTRAR NOVO VOO ---\n");
                
                // Verifica o id origem
                printf("Digite a sigla do aeroporto de ORIGEM (Ex: BSB): ");
                scanf("%s", sigla_ori);
                int id_ori = buscar_id_por_sigla(&meu_grafo, sigla_ori);
                
                if (id_ori == -1) {
                    printf("Erro: Aeroporto '%s' nao cadastrado!\n", sigla_ori);
                    break; // Sai do switch e volta para o menu
                }

                // Verifica o Destino
                printf("Digite a sigla do aeroporto de DESTINO (Ex: SSA): ");
                scanf("%s", sigla_dest);
                int id_dest = buscar_id_por_sigla(&meu_grafo, sigla_dest);
                
                if (id_dest == -1) {
                    printf("Erro: Aeroporto '%s' nao cadastrado!\n", sigla_dest);
                    break;
                }

                // Chega a distância
                printf("Digite a distancia do voo (em km): ");
                scanf("%d", &distancia);

                // inserção na lista/matriz esparsa
                if (guardar_voo(&meu_grafo, id_ori, id_dest, distancia)) {
                    printf("Sucesso! Voo %s -> %s (%d km) processado.\n", sigla_ori, sigla_dest, distancia);
                }else {
                    printf("Erro de memoria ao inserir voo.\n");
                }
                break;

            case 2:
                printf("\n--- CONSULTAR CRONOGRAMA DE VOOS ---\n");
                printf("Digite o nome da cidade do aeroporto de origem: ");
                
                // Limpa o buffer do teclado antes da próxima leitura
                getchar(); 
                // Armazena o nome da cidade digitado pelo usuário
                fgets(cidade_busca, 50, stdin);
                
                // Remove os espaços 
                cidade_busca[strcspn(cidade_busca, "\n")] = '\0';

                // função de impressão do cronograma 
                cronograma_de_voos(&meu_grafo, cidade_busca);
                break;

            case 3:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while(opcao != 3);

    // Liberação/Limpeza da Memória
    liberar_matriz(&meu_grafo);

    return 0;
}