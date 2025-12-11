#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pintura.h"
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "linha.h"
#include "anteparo.h"
#include "lista.h"
#include "strdupi.h"

static int adicionaAnteparoNaLista(Anteparo anteparo, Lista* lista) {
    insereTail(lista, anteparo); 
    return 1; 
}

static void liberaAnteparoSeNecessario(Anteparo anteparo) {
    if (anteparo) {
        liberaAnteparo(anteparo);
    }
}

int pintaRetangulo(Retangulo retangulo, int ids[4], Lista* lista) {
    printf("[DEBUG PINTURA] Entrando em pintaRetangulo\n");
    
    if (!retangulo || !lista) {
        fprintf(stderr, "Erro: parâmetros inválidos em pintaRetangulo\n");
        return 0;
    }
    
    for (int i = 0; i < 4; i++) {
        printf("[DEBUG PINTURA] ID[%d] = %d\n", i, ids[i]);
        if (ids[i] <= 0) {
            fprintf(stderr, "Erro: ID %d inválido para anteparo do retângulo\n", ids[i]);
            return 0;
        }
    }
    
    double x = getXRetangulo(retangulo);
    double y = getYRetangulo(retangulo);
    double largura = getLarguraRetangulo(retangulo);
    double altura = getAlturaRetangulo(retangulo);
    
    printf("[DEBUG PINTURA] Retângulo: pos=(%.2f,%.2f) larg=%.2f alt=%.2f\n", 
           x, y, largura, altura);
    
    char* corBorda = getCorBRetangulo(retangulo);
    printf("[DEBUG PINTURA] Cor da borda: %s\n", corBorda ? corBorda : "NULL");
    
    if (!corBorda) {
        fprintf(stderr, "Erro: não foi possível obter a cor da borda do retângulo\n");
        return 0;
    }
    
    Anteparo anteparos[4];
    int sucesso = 1;
    
    // Embaixo
    anteparos[0] = criaAnteparo(ids[0], x, y, x + largura, y, corBorda);
    // Encima
    anteparos[1] = criaAnteparo(ids[1], x, y + altura, x + largura, y + altura, corBorda);
    // Esquerda
    anteparos[2] = criaAnteparo(ids[2], x, y, x, y + altura, corBorda);
    // Direita
    anteparos[3] = criaAnteparo(ids[3], x + largura, y, x + largura, y + altura, corBorda);
    
    free(corBorda);
    
    for (int i = 0; i < 4; i++) {
        if (!anteparos[i]) {
            fprintf(stderr, "Erro: falha ao criar anteparo %d do retângulo\n", ids[i]);
            sucesso = 0;
            for (int j = 0; j < i; j++) {
                liberaAnteparoSeNecessario(anteparos[j]);
            }
            break;
        }
        
        if (!adicionaAnteparoNaLista(anteparos[i], lista)) {
            fprintf(stderr, "Erro: falha ao adicionar anteparo %d na lista\n", ids[i]);
            liberaAnteparoSeNecessario(anteparos[i]);
            sucesso = 0;
            for (int j = i + 1; j < 4; j++) {
                if (anteparos[j]) liberaAnteparoSeNecessario(anteparos[j]);
            }
            break;
        }
    }
    
    return sucesso;
}

int pintaLinha(Linha linha, int novoId, Lista* lista) {
    if (!linha || !lista) {
        fprintf(stderr, "Erro: parâmetros inválidos em pintaLinha\n");
        return 0;
    }
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: ID %d inválido para anteparo da linha\n", novoId);
        return 0;
    }
    
    double x1 = getX1Linha(linha);
    double y1 = getY1Linha(linha);
    double x2 = getX2Linha(linha);
    double y2 = getY2Linha(linha);
    char* cor = getCorLinha(linha);
    
    if (!cor) {
        fprintf(stderr, "Erro: não foi possível obter a cor da linha\n");
        return 0;
    }
    
    Anteparo anteparo = criaAnteparo(novoId, x1, y1, x2, y2, cor);
    free(cor);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir da linha\n");
        return 0;
    }
    
    if (!adicionaAnteparoNaLista(anteparo, lista)) {
        fprintf(stderr, "Erro: falha ao adicionar anteparo da linha na lista\n");
        liberaAnteparoSeNecessario(anteparo);
        return 0;
    }
    
    return 1;
}

int pintaTexto(Texto texto, int novoId, Lista* lista) {
    if (!texto || !lista) {
        fprintf(stderr, "Erro: parâmetros inválidos em pintaTexto\n");
        return 0;
    }
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: ID %d inválido para anteparo do texto\n", novoId);
        return 0;
    }
    
    double x = getXTexto(texto);
    double y = getYTexto(texto);
    char ancora = getAncoraTexto(texto);
    double comprimento = getComprimentoTexto(texto);
    
    if (comprimento <= 0) {
        fprintf(stderr, "Erro: comprimento inválido do texto: %f\n", comprimento);
        return 0;
    }
    
    double x1, x2;
    
    switch (ancora) {
        case 'i': 
            x1 = x;
            x2 = x + comprimento;
            break;
            
        case 'f': 
            x1 = x - comprimento;
            x2 = x;
            break;
            
        case 'm':
            x1 = x - comprimento / 2.0;
            x2 = x + comprimento / 2.0;
            break;
            
        default:
            fprintf(stderr, "Erro: âncora inválida '%c' em texto\n", ancora);
            return 0;
    }
    
    char* corBorda = getCorBTexto(texto);
    
    if (!corBorda) {
        fprintf(stderr, "Erro: não foi possível obter a cor da borda do texto\n");
        return 0;
    }
    
    Anteparo anteparo = criaAnteparo(novoId, x1, y, x2, y, corBorda);
    free(corBorda);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir do texto\n");
        return 0;
    }
    
    if (!adicionaAnteparoNaLista(anteparo, lista)) {
        fprintf(stderr, "Erro: falha ao adicionar anteparo do texto na lista\n");
        liberaAnteparoSeNecessario(anteparo);
        return 0;
    }
    
    return 1;
}

int pintaCirculo(Circulo circulo, char direcao, int novoId, Lista* lista) {
    if (!circulo || !lista) {
        fprintf(stderr, "Erro: parâmetros inválidos em pintaCirculo\n");
        return 0;
    }
    
    if (novoId <= 0) {
        fprintf(stderr, "Erro: ID %d inválido para anteparo do círculo\n", novoId);
        return 0;
    }
    
    if (direcao != 'h' && direcao != 'v') {
        fprintf(stderr, "Erro: direção inválida '%c' para transformação. Use 'h' (horizontal) ou 'v' (vertical)\n", direcao);
        return 0;
    }
    
    double x = getXCirculo(circulo);
    double y = getYCirculo(circulo);
    double r = getRaioCirculo(circulo);
    char* corB = getCorBCirculo(circulo);
    
    if (!corB) {
        fprintf(stderr, "Erro: não foi possível obter a cor da borda do círculo\n");
        return 0;
    }
    
    double x1, y1, x2, y2;
    
    if (direcao == 'h') {
        x1 = x - r;
        y1 = y;
        x2 = x + r;
        y2 = y;
    } else {
        x1 = x;
        y1 = y - r;
        x2 = x;
        y2 = y + r;
    }
    
    if (x1 != x1 || y1 != y1 || x2 != x2 || y2 != y2 ||
        isinf(x1) || isinf(y1) || isinf(x2) || isinf(y2)) {
        fprintf(stderr, "Erro: coordenadas inválidas geradas para anteparo do círculo\n");
        free(corB);
        return 0;
    }
    
    Anteparo anteparo = criaAnteparo(novoId, x1, y1, x2, y2, corB);
    free(corB);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir do círculo\n");
        return 0;
    }
    
    if (!validaAnteparo(anteparo)) {
        fprintf(stderr, "Aviso: anteparo criado a partir do círculo pode ser inválido\n");
    }
    
    if (!adicionaAnteparoNaLista(anteparo, lista)) {
        fprintf(stderr, "Erro: falha ao adicionar anteparo do círculo na lista\n");
        liberaAnteparoSeNecessario(anteparo);
        return 0;
    }
    
    return 1;
}
