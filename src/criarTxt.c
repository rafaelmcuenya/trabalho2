#include "criarTxt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "forma.h"
#include "strdupi.h"
#include "lista.h"

static FILE* txtFile = NULL;

void iniciarTxt(const char* caminhoCompleto) {
    if (txtFile) {
        fclose(txtFile);
    }
    
    txtFile = fopen(caminhoCompleto, "w");
    if (!txtFile) {
        fprintf(stderr, "Erro: Não foi possível criar arquivo TXT: %s\n", caminhoCompleto);
    }
}

void fecharTxt(void) {
    if (txtFile) {
        fclose(txtFile);
        txtFile = NULL;
    }
}

static char* getTipoFormaStr(Forma f) {
    if (!f) return "DESCONHECIDO";
    
    TipoForma tipo = getTipoForma(f);
    switch (tipo) {
        case Tc: return "círculo";
        case Tr: return "retângulo";
        case Tl: return "linha";
        case Tt: return "texto";
        case Ta: return "anteparo";
        default: return "DESCONHECIDO";
    }
}

static void escreveInfoForma(Forma f) {
    if (!txtFile || !f) return;
    
    int id = getIdForma(f);
    char* tipo = getTipoFormaStr(f);
    double x = getXForma(f);
    double y = getYForma(f);
    char* corB = getCorBForma(f);
    char* corP = getCorPForma(f);
    
    fprintf(txtFile, "  ID: %d, Tipo: %s, Posição: (%.2f, %.2f), Borda: %s, Preenchimento: %s\n",
            id, tipo, x, y, corB ? corB : "N/A", corP ? corP : "N/A");
    
    if (corB) free(corB);
    if (corP) free(corP);
}

static void escreveInfoAnteparo(Anteparo a) {
    if (!txtFile || !a) return;
    
    int id = idAnteparo(a);
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    char* cor = getCorAnteparo(a);
    
    fprintf(txtFile, "  ID: %d, Segmento: (%.2f, %.2f) -> (%.2f, %.2f), Cor: %s\n",
            id, x1, y1, x2, y2, cor ? cor : "N/A");
    
    if (cor) free(cor);
}

void txtA(int i, int j, char direcao, Forma formaOriginal, Anteparo anteparoGerado) {
    if (!txtFile) return;
    
    fprintf(txtFile, "[*] a %d %d %c\n", i, j, direcao);
    
    if (formaOriginal) {
        fprintf(txtFile, "Forma original transformada:\n");
        escreveInfoForma(formaOriginal);
    }
    
    if (anteparoGerado) {
        fprintf(txtFile, "Anteparo gerado:\n");
        escreveInfoAnteparo(anteparoGerado);
    }
    
    fprintf(txtFile, "\n");
}

void txtD(double x, double y, const char* sfx, Lista* formasDestruidas, Poligono regiaoVisivel) {
    if (!txtFile) return;
    
    fprintf(txtFile, "[*] d %.2f %.2f %s\n", x, y, sfx);
    
    if (formasDestruidas) {
        int count = getTamLista(formasDestruidas);
        fprintf(txtFile, "Formas destruídas: %d\n", count);
        
        if (count > 0) {
            No* atual = getHeadNo(formasDestruidas);
            while (atual) {
                Forma f = (Forma)getNoInfo(atual);
                if (f) {
                    escreveInfoForma(f);
                }
                atual = vaiNoDepois(atual);
            }
        }
    }
    
    if (regiaoVisivel) {
        double area = calculaAreaPoligono(regiaoVisivel);
        double perimetro = calculaPerimetro(regiaoVisivel);
        fprintf(txtFile, "Região de visibilidade:\n");
        fprintf(txtFile, "  Área: %.2f, Perímetro: %.2f\n", area, perimetro);
        fprintf(txtFile, "  Número de vértices: %d\n", getNumVertices(regiaoVisivel));
    }
    
    fprintf(txtFile, "\n");
}

void txtP(double x, double y, const char* cor, const char* sfx, Lista* formasPintadas, Poligono regiaoVisivel) {
    if (!txtFile) return;
    
    fprintf(txtFile, "[*] p %.2f %.2f %s %s\n", x, y, cor, sfx);
    
    if (formasPintadas) {
        int count = getTamLista(formasPintadas);
        fprintf(txtFile, "Formas pintadas com a cor %s: %d\n", cor, count);
        
        if (count > 0) {
            Node* atual = getHeadNode(formasPintadas);
            while (atual) {
                Forma f = (Forma)getNodeInfo(atual);
                if (f) {
                    escreveInfoForma(f);
                }
                atual = vaiNodeDepois(atual);
            }
        }
    }
    
    if (regiaoVisivel) {
        double area = calculaAreaPoligono(regiaoVisivel);
        fprintf(txtFile, "Região de visibilidade (pintura):\n");
        fprintf(txtFile, "  Área: %.2f\n", area);
    }
    
    fprintf(txtFile, "\n");
}

void txtCln(double x, double y, double dx, double dy, const char* sfx, Lista* formasOriginais, Lista* clones) {
    if (!txtFile) return;
    
    fprintf(txtFile, "[*] cln %.2f %.2f %.2f %.2f %s\n", x, y, dx, dy, sfx);
    
    if (formasOriginais && clones) {
        int count = getTamLista(formasOriginais);
        fprintf(txtFile, "Formas clonadas (deslocamento: %.2f, %.2f): %d\n", dx, dy, count);
        
        if (count > 0) {
            No* atualOrig = getHeadNode(formasOriginais);
            No* atualClone = getHeadNode(clones);
            
            while (atualOrig && atualClone) {
                Forma original = (Forma)getNodeInfo(atualOrig);
                Forma clone = (Forma)getNodeInfo(atualClone);
                
                if (original && clone) {
                    fprintf(txtFile, "Original (ID %d) -> Clone (ID %d):\n", 
                            getIdForma(original), getIdForma(clone));
                    
                    double xOrig = getXForma(original);
                    double yOrig = getYForma(original);
                    double xClone = getXForma(clone);
                    double yClone = getYForma(clone);
                    
                    fprintf(txtFile, "  Posição original: (%.2f, %.2f)\n", xOrig, yOrig);
                    fprintf(txtFile, "  Posição clone: (%.2f, %.2f)\n", xClone, yClone);
                }
                
                atualOrig = vaiNodeDepois(atualOrig);
                atualClone = vaiNodeDepois(atualClone);
            }
        }
    }
    
    fprintf(txtFile, "\n");
}

void txtComandoQry(const char* linhaComando) {
    if (!txtFile || !linhaComando) return;
    fprintf(txtFile, "[*] %s\n", linhaComando);
}

void txtResposta(const char* resposta) {
    if (!txtFile || !resposta) return;
    fprintf(txtFile, "%s\n", resposta);
}

void txtSeparador(void) {
    if (!txtFile) return;
    fprintf(txtFile, "\n");
}

void txtFinalRelatorio(int totalFormas, int formasDestruidas, int formasPintadas, int formasClonadas, 
                       int anteparosCriados, double areaTotalVisivel) {
    if (!txtFile) return;
    
    fprintf(txtFile, "========================================\n");
    fprintf(txtFile, "RELATÓRIO FINAL DO PROCESSAMENTO\n");
    fprintf(txtFile, "========================================\n\n");
    
    fprintf(txtFile, "ESTATÍSTICAS GERAIS:\n");
    fprintf(txtFile, "  Total de formas processadas: %d\n", totalFormas);
    fprintf(txtFile, "  Formas transformadas em anteparos: %d\n", anteparosCriados);
    fprintf(txtFile, "  Formas destruídas por bombas: %d\n", formasDestruidas);
    fprintf(txtFile, "  Formas pintadas por bombas: %d\n", formasPintadas);
    fprintf(txtFile, "  Formas clonadas por bombas: %d\n", formasClonadas);
    
    if (areaTotalVisivel > 0) {
        fprintf(txtFile, "  Área total de regiões de visibilidade: %.2f\n", areaTotalVisivel);
    }
    
    fprintf(txtFile, "\n========================================\n");
    fprintf(txtFile, "FIM DO RELATÓRIO\n");
    fprintf(txtFile, "========================================\n");
}


static char* formataInfoCirculo(Circulo c) {
    char* info = malloc(256);
    if (!info) return NULL;
    
    double x = getXCirculo(c);
    double y = getYCirculo(c);
    double r = getRaioCirculo(c);
    char* corB = getCorBCirculo(c);
    char* corP = getCorPCirculo(c);
    
    snprintf(info, 256, "círculo\n  centro: (%.2f, %.2f)\n  raio: %.2f\n  borda: %s\n  preenchimento: %s", x, y, r, corB, corP);
    
    free(corB);
    free(corP);
    return info;
}

static char* formataInfoRetangulo(Retangulo r) {
    char* info = malloc(256);
    if (!info) return NULL;
    
    double x = getXRetangulo(r);
    double y = getYRetangulo(r);
    double w = getLarguraRetangulo(r);
    double h = getAlturaRetangulo(r);
    char* corB = getCorBRetangulo(r);
    char* corP = getCorPRetangulo(r);
    
    snprintf(info, 256, "retângulo\n  âncora: (%.2f, %.2f)\n  largura: %.2f\n  altura: %.2f\n  borda: %s\n  preenchimento: %s",
             x, y, w, h, corB, corP);
    
    free(corB);
    free(corP);
    return info;
}

static char* formataInfoLinha(Linha l) {
    char* info = malloc(256);
    if (!info) return NULL;
    
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);
    char* cor = getCorLinha(l);
    
    snprintf(info, 256, "linha\n  ponto1: (%.2f, %.2f)\n  ponto2: (%.2f, %.2f)\n  cor: %s", x1, y1, x2, y2, cor);
    
    free(cor);
    return info;
}

static char* formataInfoTexto(Texto t) {
    char* info = malloc(512);
    if (!info) return NULL;
    
    double x = getXTexto(t);
    double y = getYTexto(t);
    char ancora = getAncoraTexto(t);
    char* texto = getTexto(t);
    char* corB = getCorBTexto(t);
    char* corP = getCorPTexto(t);
    char* fontFamily = getFontFamilyTexto(t);
    char* fontWeight = getFontWeightTexto(t);
    double fontSize = getFontSizeTexto(t);
    
    snprintf(info, 512, "texto\n  posição: (%.2f, %.2f)\n  âncora: %c\n  conteúdo: %s\n  borda: %s\n  preenchimento: %s\n  fonte: %s %s %.1fpx",
             x, y, ancora, texto, corB, corP, fontFamily, fontWeight, fontSize);
    
    free(texto);
    free(corB);
    free(corP);
    free(fontFamily);
    free(fontWeight);
    return info;
}
