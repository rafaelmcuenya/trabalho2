#include "retangulo.h"
#include "anteparo.h"
#include "circulo.h"
#include "texto.h"
#include "linha.h"


Anteparo* pintaRetangulo(Retangulo retangulo, int ids[4]) {
    if (!retangulo) {
        fprintf(stderr, "Erro: retângulo NULL em criarAnteparosDeRetangulo\n");
        return NULL;
    }
    
    double x = getXRetangulo(retangulo);
    double y = getYRetangulo(retangulo);
    double largura = getLarguraRetangulo(retangulo);
    double altura = getAlturaRetangulo(retangulo);
    char* corBorda = getCorBRetangulo(retangulo);
    
    if (!corBorda) {
        fprintf(stderr, "Erro: não foi possível obter a cor da borda do retângulo\n");
        return NULL;
    }
    
    Anteparo* anteparos = (Anteparo*)malloc(4 * sizeof(Anteparo));
    if (!anteparos) {
        fprintf(stderr, "Erro: falha na alocação dos anteparos\n");
        free(corBorda);
        return NULL;
    }
    
    anteparos[0] = criaAnteparo(ids[0], x, y, x + largura, y, corBorda);
    anteparos[1] = criaAnteparo(ids[1], x, y + altura, x + largura, y + altura, corBorda);
    anteparos[2] = criaAnteparo(ids[2], x, y, x, y + altura, corBorda);
    anteparos[3] = criaAnteparo(ids[3], x + largura, y, x + largura, y + altura, corBorda);
    
    free(corBorda);
    
    for (int i = 0; i < 4; i++) {
        if (!anteparos[i]) {
            fprintf(stderr, "Erro: falha ao criar anteparo %d do retângulo\n", ids[i]);
            for (int j = 0; j < i; j++) {
                liberaAnteparo(anteparos[j]);
            }
            free(anteparos);
            return NULL;
        }
    }
    
    return anteparos;
}

Anteparo pintaLinha(Linha linha, int novoId) {
    if (!linha) {
        fprintf(stderr, "Erro: linha NULL em usarLinhaComoAnteparo\n");
        return NULL;
    }
    
    double x1 = getX1Linha(linha);
    double y1 = getY1Linha(linha);
    double x2 = getX2Linha(linha);
    double y2 = getY2Linha(linha);
    char* cor = getCorLinha(linha);
    
    if (!cor) {
        fprintf(stderr, "Erro: não foi possível obter a cor da linha\n");
        return NULL;
    }
    
     Anteparo anteparo = criaAnteparo(novoId, x1, y1, x2, y2, cor);
    free(cor);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir da linha %d\n", idLinha(linha));
    }
    
    return anteparo;
}

Anteparo pintaTexto(Texto texto, int novoId) {
    if (!texto) {
        fprintf(stderr, "Erro: texto NULL em transformaTextoEmAnteparo\n");
        return NULL;
    }
    
    double x = getXTexto(texto);
    double y = getYTexto(texto);
    char ancora = getAncoraTexto(texto);
    double comprimento = getComprimentoTexto(texto);
    
    if (comprimento < 0) {
        fprintf(stderr, "Erro: comprimento inválido do texto\n");
        return NULL;
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
            return NULL;
    }
    
    char* corBorda = getCorBTexto(texto);
    
    if (!corBorda) {
        fprintf(stderr, "Erro: não foi possível obter a cor da borda do texto\n");
        return NULL;
    }
    
    Anteparo anteparo = criaAnteparo(novoId, x1, y, x2, y, corBorda);
    free(corBorda);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir do texto %d\n", idTexto(texto));
    }
    
    return anteparo;
}

Anteparo pintaCirculo(Circulo circulo, char direcao, int novo_id) {
    if (!circulo) {
        fprintf(stderr, "Erro: círculo NULL em transformarCirculoEmAnteparo\n");
        return NULL;
    }
    
    CirculoStruct* circle = (CirculoStruct*)circulo;
    
    if (novo_id <= 0) {
        fprintf(stderr, "Erro: ID inválido para anteparo criado a partir do círculo\n");
        return NULL;
    }
    
    if (direcao != 'h' && direcao != 'v') {
        fprintf(stderr, "Erro: direção inválida '%c' para transformação. Use 'h' (horizontal) ou 'v' (vertical)\n", direcao);
        return NULL;
    }
    
    double x1, y1, x2, y2;
    
    if (direcao == 'h') {
        x1 = circle->x - circle->r;
        y1 = circle->y;
        x2 = circle->x + circle->r;
        y2 = circle->y;
    } else { 
        x1 = circle->x;
        y1 = circle->y - circle->r;
        x2 = circle->x;
        y2 = circle->y + circle->r;
    }
    
    if (x1 != x1 || y1 != y1 || x2 != x2 || y2 != y2 ||
        isinf(x1) || isinf(y1) || isinf(x2) || isinf(y2)) {
        fprintf(stderr, "Erro: coordenadas inválidas geradas para anteparo do círculo %d\n", circle->id);
        return NULL;
    }
    
    Anteparo anteparo = criaAnteparo(novo_id, x1, y1, x2, y2, circle->corB);
    
    if (!anteparo) {
        fprintf(stderr, "Erro: falha ao criar anteparo a partir do círculo %d\n", circle->id);
    } else {
        if (!validaAnteparo(anteparo)) {
            fprintf(stderr, "Aviso: anteparo criado a partir do círculo %d pode ser inválido\n", circle->id);
        }
    }
    
    return anteparo;
}
