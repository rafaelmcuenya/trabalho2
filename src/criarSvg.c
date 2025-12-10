#include "criarSvg.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "strdupi.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "lista.h"

void svgCabecalho(FILE* svgFile, double viewBoxX, double viewBoxY, double viewBoxW, double viewBoxH) {
    if (!svgFile) return;
    
    fprintf(svgFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(svgFile, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ");
    fprintf(svgFile, "viewBox=\"%.2f %.2f %.2f %.2f\">\n", viewBoxX, viewBoxY, viewBoxW, viewBoxH);
    
    fprintf(svgFile, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" fill=\"white\" stroke=\"none\"/>\n",
            viewBoxX, viewBoxY, viewBoxW, viewBoxH);
}

void svgRodape(FILE* svgFile) {
    if (!svgFile) return;
    fprintf(svgFile, "</svg>\n");
}

void svgForma(FILE* svgFile, Forma f) {
    if (!svgFile || !f) return;
    
    TipoForma tipo = getTipoForma(f);
    
    switch(tipo) {
        case Tr: { 
            Retangulo r = getRetanguloFromForma(f);
            double x = getXRetangulo(r);
            double y = getYRetangulo(r);
            double w = getLarguraRetangulo(r);
            double h = getAlturaRetangulo(r);
            char* corP = getCorPRetangulo(r);
            char* corB = getCorBRetangulo(r);
            
            fprintf(svgFile, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" ", x, y, w, h);
            fprintf(svgFile, "fill=\"#%s\" stroke=\"#%s\" stroke-width=\"1\" ", corP, corB);
            fprintf(svgFile, "fill-opacity=\"0.5\" stroke-opacity=\"0.8\"/>\n");
            
            fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"black\">%d</text>\n",
                    x + w/2, y + h/2, getIdForma(f));
            
            free(corP);
            free(corB);
            break;
        }
        
        case Tc: { 
            Circulo c = getCirculoFromForma(f);
            double x = getXCirculo(c);
            double y = getYCirculo(c);
            double r = getRaioCirculo(c);
            char* corP = getCorPCirculo(c);
            char* corB = getCorBCirculo(c);
            
            fprintf(svgFile, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" ", x, y, r);
            fprintf(svgFile, "fill=\"#%s\" stroke=\"#%s\" stroke-width=\"1\" ", corP, corB);
            fprintf(svgFile, "fill-opacity=\"0.5\" stroke-opacity=\"0.8\"/>\n");
            
            
            fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"black\">%d</text>\n", x, y, getIdForma(f));
            
            free(corP);
            free(corB);
            break;
        }
        
        case Tl: { 
            Linha l = getLinhaFromForma(f);
            double x1 = getX1Linha(l);
            double y1 = getY1Linha(l);
            double x2 = getX2Linha(l);
            double y2 = getY2Linha(l);
            char* cor = getCorLinha(l);
            
            fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", x1, y1, x2, y2);
            fprintf(svgFile, "stroke=\"#%s\" stroke-width=\"2\" stroke-opacity=\"0.8\"/>\n", cor);
            
            fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"black\">%d</text>\n",
                    (x1 + x2)/2, (y1 + y2)/2, getIdForma(f));
            
            free(cor);
            break;
        }
        
        case Tt: { 
            Texto t = getTextoFromForma(f);
            double x = getXTexto(t);
            double y = getYTexto(t);
            char* texto = getTexto(t);
            char* corP = getCorPTexto(t);
            char* corB = getCorBTexto(t);
            
            fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" ", x, y);
            fprintf(svgFile, "fill=\"#%s\" stroke=\"#%s\" stroke-width=\"0.5\" ", corP, corB);
            fprintf(svgFile, "fill-opacity=\"0.8\" stroke-opacity=\"0.8\">%s</text>\n", texto);
            
            free(texto);
            free(corP);
            free(corB);
            break;
        }
        
        case Ta: { 
            break;
        }
    }
}

void svgAnteparos(FILE* svgFile, Lista* anteparos) {
    if (!svgFile || !anteparos || estaVazia(anteparos)) return;
    
    fprintf(svgFile, "<!-- Anteparos -->\n");
    
    No* atual = getHeadNo(anteparos);
    while (atual) {
        Anteparo a = (Anteparo)getNoInfo(atual);
        if (a) {
            double x1 = getX1Anteparo(a);
            double y1 = getY1Anteparo(a);
            double x2 = getX2Anteparo(a);
            double y2 = getY2Anteparo(a);
            char* cor = getCorAnteparo(a);
            
            fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", x1, y1, x2, y2);
            fprintf(svgFile, "stroke=\"#%s\" stroke-width=\"3\" stroke-dasharray=\"5,5\" ", cor ? cor : "000000");
            fprintf(svgFile, "stroke-opacity=\"0.7\"/>\n");
            
            fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"4\" fill=\"darkblue\">A%d</text>\n",
                    (x1 + x2)/2, (y1 + y2)/2 - 3, idAnteparo(a));
            
            if (cor) free(cor);
        }
        atual = vaiNoDepois(atual);
    }
}

void svgRegiaoVisivel(FILE* svgFile, Poligono regiaoVisivel, const char* corPreench, const char* corBorda) {
    if (!svgFile || !regiaoVisivel) return;
    
    int numVertices = getNumVertices(regiaoVisivel);
    if (numVertices < 3) return;
    
    fprintf(svgFile, "<!-- Região de Visibilidade -->\n");
    fprintf(svgFile, "<polygon points=\"");
    
    for (int i = 0; i < numVertices; i++) {
        Ponto vertice = getVertice(regiaoVisivel, i);
        if (vertice) {
            fprintf(svgFile, "%.2f,%.2f ", getXPonto(vertice), getYPonto(vertice));
            liberaPonto(vertice);
        }
    }
    
    Ponto primeiro = getVertice(regiaoVisivel, 0);
    if (primeiro) {
        fprintf(svgFile, "%.2f,%.2f", getXPonto(primeiro), getYPonto(primeiro));
        liberaPonto(primeiro);
    }
    
    fprintf(svgFile, "\" fill=\"%s\" stroke=\"%s\" ", corPreench ? corPreench : "lightblue", corBorda ? corBorda : "blue");
    fprintf(svgFile, "stroke-width=\"1\" fill-opacity=\"0.3\" stroke-opacity=\"0.7\"/>\n");
}

void svgBombaDestruicao(FILE* svgFile, double x, double y, const char* cor) {
    if (!svgFile) return;
    
    fprintf(svgFile, "<!-- Bomba de Destruição -->\n");
    
    fprintf(svgFile, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"8\" ", x, y);
    fprintf(svgFile, "fill=\"%s\" stroke=\"darkred\" stroke-width=\"2\" ", cor ? cor : "red");
    fprintf(svgFile, "fill-opacity=\"0.7\" stroke-opacity=\"1\"/>\n");
    
    fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", x - 5, y - 5, x + 5, y + 5);
    fprintf(svgFile, "stroke=\"white\" stroke-width=\"2\"/>\n");
    
    fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ", x + 5, y - 5, x - 5, y + 5);
    fprintf(svgFile, "stroke=\"white\" stroke-width=\"2\"/>\n");
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"darkred\">Bomba D</text>\n", x + 10, y);
}

void svgBombaPintura(FILE* svgFile, double x, double y, const char* cor) {
    if (!svgFile) return;
    
    fprintf(svgFile, "<!-- Bomba de Pintura -->\n");
    
    fprintf(svgFile, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"8\" ", x, y);
    fprintf(svgFile, "fill=\"%s\" stroke=\"orange\" stroke-width=\"2\" ", cor ? cor : "yellow");
    fprintf(svgFile, "fill-opacity=\"0.7\" stroke-opacity=\"1\"/>\n");
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"7\" font-weight=\"bold\" fill=\"black\">P</text>\n",
            x - 2.5, y + 2.5);
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"orange\">Bomba P</text>\n", x + 10, y);
}

void svgBombaClonagem(FILE* svgFile, double x, double y, double dx, double dy) {
    if (!svgFile) return;
    
    fprintf(svgFile, "<!-- Bomba de Clonagem -->\n");
    
    fprintf(svgFile, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"8\" ", x, y);
    fprintf(svgFile, "fill=\"lightgreen\" stroke=\"green\" stroke-width=\"2\" ");
    fprintf(svgFile, "fill-opacity=\"0.7\" stroke-opacity=\"1\"/>\n");
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"7\" font-weight=\"bold\" fill=\"darkgreen\">C</text>\n",
            x - 2.5, y + 2.5);
    
    if (fabs(dx) > 0.001 || fabs(dy) > 0.001) {
        double x2 = x + dx;
        double y2 = y + dy;
        
        fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ",  x, y, x2, y2);
        fprintf(svgFile, "stroke=\"green\" stroke-width=\"2\" stroke-dasharray=\"3,3\"/>\n");
        
        double angle = atan2(dy, dx);
        double arrowLength = 10.0;
        
        double x3 = x2 - arrowLength * cos(angle - M_PI/6);
        double y3 = y2 - arrowLength * sin(angle - M_PI/6);
        double x4 = x2 - arrowLength * cos(angle + M_PI/6);
        double y4 = y2 - arrowLength * sin(angle + M_PI/6);
        
        fprintf(svgFile, "<polygon points=\"%.2f,%.2f %.2f,%.2f %.2f,%.2f\" ", x2, y2, x3, y3, x4, y4);
        fprintf(svgFile, "fill=\"green\"/>\n");
        
        fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"4\" fill=\"darkgreen\">(%.1f,%.1f)</text>\n",
                (x + x2)/2, (y + y2)/2 - 5, dx, dy);
    }
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"5\" fill=\"green\">Bomba C</text>\n", x + 10, y);
}

void svgTextoRelatorio(FILE* svgFile, double x, double y, const char* texto, const char* cor) {
    if (!svgFile || !texto) return;
    
    fprintf(svgFile, "<text x=\"%.2f\" y=\"%.2f\" font-size=\"4\" fill=\"%s\">%s</text>\n", x, y, cor ? cor : "black", texto);
}

void svgGeo(const char* caminhoCompleto, Lista* formas) {
    if (!caminhoCompleto || !formas) return;
    
    FILE* svgFile = fopen(caminhoCompleto, "w");
    if (!svgFile) {
        fprintf(stderr, "Erro: Não foi possível criar arquivo SVG: %s\n", caminhoCompleto);
        return;
    }
    
    double minX = 1e30, minY = 1e30, maxX = -1e30, maxY = -1e30;
    No* atual = getHeadNo(formas);
    while (atual) {
        Forma f = (Forma)getNoInfo(atual);
        if (f) {
            double x = getXForma(f);
            double y = getYForma(f);
            minX = (x < minX) ? x : minX;
            minY = (y < minY) ? y : minY;
            maxX = (x > maxX) ? x : maxX;
            maxY = (y > maxY) ? y : maxY;
            
            TipoForma tipo = getTipoForma(f);
            if (tipo == Tr) {
                Retangulo r = getRetanguloFromForma(f);
                maxX = fmax(maxX, x + getLarguraRetangulo(r));
                maxY = fmax(maxY, y + getAlturaRetangulo(r));
            } else if (tipo == Tc) {
                Circulo c = getCirculoFromForma(f);
                double r = getRaioCirculo(c);
                minX = fmin(minX, x - r);
                minY = fmin(minY, y - r);
                maxX = fmax(maxX, x + r);
                maxY = fmax(maxY, y + r);
            }
        }
        atual = vaiNoDepois(atual);
    }
    
    double margem = 50.0;
    minX -= margem; minY -= margem;
    maxX += margem; maxY += margem;
    double largura = maxX - minX;
    double altura = maxY - minY;
    
    svgCabecalho(svgFile, minX, minY, largura, altura);
    
    fprintf(svgFile, "<!-- Formas Geométricas do arquivo .geo -->\n");
    atual = getHeadNo(formas);
    while (atual) {
        Forma f = (Forma)getNoInfo(atual);
        if (f) {
            svgForma(svgFile, f);
        }
        atual = vaiNoDepois(atual);
    }
    
    svgRodape(svgFile);
    fclose(svgFile);
    
    printf("SVG gerado: %s\n", caminhoCompleto);
}

void svgQry(const char* caminhoCompleto, Lista* formas, Lista* anteparos) {
    if (!caminhoCompleto) return;
    
    FILE* svgFile = fopen(caminhoCompleto, "w");
    if (!svgFile) {
        fprintf(stderr, "Erro: Não foi possível criar arquivo SVG: %s\n", caminhoCompleto);
        return;
    }
    
    double minX = 1e30, minY = 1e30, maxX = -1e30, maxY = -1e30;
    
    if (formas) {
        No* atual = getHeadNo(formas);
        while (atual) {
            Forma f = (Forma)getNoInfo(atual);
            if (f) {
                double x = getXForma(f);
                double y = getYForma(f);
                minX = fmin(minX, x);
                minY = fmin(minY, y);
                maxX = fmax(maxX, x);
                maxY = fmax(maxY, y);
            }
            atual = vaiNoDepois(atual);
        }
    }
    
    if (anteparos) {
        No* atual = getHeadNo(anteparos);
        while (atual) {
            Anteparo a = (Anteparo)getNoInfo(atual);
            if (a) {
                double x1 = getX1Anteparo(a);
                double y1 = getY1Anteparo(a);
                double x2 = getX2Anteparo(a);
                double y2 = getY2Anteparo(a);
                minX = fmin(minX, fmin(x1, x2));
                minY = fmin(minY, fmin(y1, y2));
                maxX = fmax(maxX, fmax(x1, x2));
                maxY = fmax(maxY, fmax(y1, y2));
            }
            atual = vaiNoDepois(atual);
        }
    }
    
    double margem = 50.0;
    minX -= margem; minY -= margem;
    maxX += margem; maxY += margem;
    double largura = maxX - minX;
    double altura = maxY - minY;
    
    svgCabecalho(svgFile, minX, minY, largura, altura);
    
    if (formas && !estaVazia(formas)) {
        fprintf(svgFile, "<!-- Formas Remanescentes -->\n");
        No* atual = getHeadNo(formas);
        while (atual) {
            Forma f = (Forma)getNoInfo(atual);
            if (f) {
                svgForma(svgFile, f);
            }
            atual = vaiNoDepois(atual);
        }
    }
    
    if (anteparos && !estaVazia(anteparos)) {
        svgAnteparos(svgFile, anteparos);
    }
    
    svgRodape(svgFile);
    fclose(svgFile);
    
    printf("SVG pós-consulta gerado: %s\n", caminhoCompleto);
}
