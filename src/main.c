#include "leitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trataNomeArquivo.h"
#include "sort.h"
#include "visibilidade.h"

static char tipoOrdenacao = 'q'; 
static int threshold = 10;     

int main(int argc, char *argv[]) {
    char *geoFile = NULL;
    char *qryFile = NULL;
    char *outputDir = NULL;
    char *inputDir = NULL;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
            geoFile = argv[++i];
        } else if (strcmp(argv[i], "-q") == 0 && i+1 < argc) {
            qryFile = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) {
            outputDir = argv[++i];
        } else if (strcmp(argv[i], "-e") == 0 && i+1 < argc) {
            inputDir = argv[++i];
        } else if (strcmp(argv[i], "-to") == 0 && i+1 < argc) {
            char tipo = argv[++i][0];
            if (tipo == 'q' || tipo == 'm') {
                tipoOrdenacao = tipo;
            } else {
                fprintf(stderr, "AVISO: Tipo de ordenação inválido '%c'. Usando padrão 'q'.\n", tipo);
            }
        } else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            threshold = atoi(argv[++i]);
            if (threshold <= 0) {
                fprintf(stderr, "AVISO: Threshold inválido %d. Usando padrão 10.\n", threshold);
                threshold = 10;
            }
        } else {
            fprintf(stderr, "AVISO: Argumento desconhecido '%s'\n", argv[i]);
        }
    }

    if (!geoFile) {
        fprintf(stderr, "ERRO: Arquivo .geo não especificado\n");
        fprintf(stderr, "Uso: %s -f <arquivo.geo> [-q <arquivo.qry>] -o <diretorio> [-e <diretorio>] [-to q|m] [-i <threshold>]\n", argv[0]);
        fprintf(stderr, "  -f <arquivo.geo>   : Arquivo de entrada com formas geométricas (obrigatório)\n");
        fprintf(stderr, "  -q <arquivo.qry>   : Arquivo de consultas (opcional)\n");
        fprintf(stderr, "  -o <diretorio>     : Diretório de saída (obrigatório)\n");
        fprintf(stderr, "  -e <diretorio>     : Diretório de entrada (opcional, padrão: diretório atual)\n");
        fprintf(stderr, "  -to q|m           : Tipo de ordenação: q (qsort) ou m (mergesort) (opcional, padrão: q)\n");
        fprintf(stderr, "  -i <threshold>    : Threshold para insertion sort no mergesort (opcional, padrão: 10)\n");
        return 1;
    }

    if (!outputDir) {
        fprintf(stderr, "ERRO: Diretório de saída não especificado\n");
        fprintf(stderr, "Uso: %s -f <arquivo.geo> [-q <arquivo.qry>] -o <diretorio> [-e <diretorio>] [-to q|m] [-i <threshold>]\n", argv[0]);
        return 1;
    }

    printf("=== CONFIGURAÇÃO DO SISTEMA ===\n");
    printf("Arquivo GEO: %s\n", geoFile);
    printf("Arquivo QRY: %s\n", qryFile ? qryFile : "(nenhum)");
    printf("Diretório de entrada: %s\n", inputDir ? inputDir : "(diretório atual)");
    printf("Diretório de saída: %s\n", outputDir);
    printf("Tipo de ordenação: %c\n", tipoOrdenacao);
    printf("Threshold: %d\n", threshold);
    printf("==============================\n\n");

    criarDiretorioSeNecessario(outputDir);

    char nomeBase[FILE_NAME_LEN];
    extrairNomeBase(geoFile, nomeBase);
    printf("Nome base: %s\n", nomeBase);

    printf("Inicializando sistema...\n");
    inicializarSistema();
    
    printf("\n=== PROCESSANDO ARQUIVO GEO ===\n");
    processarArquivo(geoFile, inputDir, 0, nomeBase, outputDir);
    
    if (qryFile) {
        printf("\n=== PROCESSANDO ARQUIVO QRY ===\n");
        processarArquivo(qryFile, inputDir, 1, nomeBase, outputDir);
    } else {
        printf("\n=== NENHUM ARQUIVO QRY FORNECIDO ===\n");
    }

    printf("\n=== FINALIZANDO SISTEMA ===\n");
    finalizarSistema();

    printf("\n=== EXECUÇÃO CONCLUÍDA ===\n");
    return 0;
}
