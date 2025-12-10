#include "leitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trataNomeArquivo.h"

int main(int argc, char *argv[]) {
    char *geoFile = NULL;
    char *qryFile = NULL;
    char *outputDir = NULL;
    char *inputDir = NULL;
    
    char tipoOrdenacao = 'q';  // 'q' para qsort, 'm' para mergesort
    int threshold = 10;        

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
            tipoOrdenacao = argv[++i][0];
        } else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            threshold = atoi(argv[++i]);
        }
    }

    if (!geoFile || !outputDir) {
        fprintf(stderr, "Uso: %s -f <arquivo.geo> -o <diretorio> [-q <arquivo.qry>] [-e <diretorio>] [-to q|m] [-i <threshold>]\n", argv[0]);
        return 1;
    }

    char nomeBase[FILE_NAME_LEN];
    extrairNomeBase(geoFile, nomeBase);

    inicializarSistema();

    processarArquivo(geoFile, inputDir, 0, nomeBase, outputDir);

    if (qryFile) {
        processarArquivo(qryFile, inputDir, 1, nomeBase, outputDir);
    }

    finalizarSistema();

    return 0;
}
