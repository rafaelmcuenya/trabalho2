#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "forma.h"
#include <math.h>

struct PoligonoStruct {
    Lista* vertices; 
    int numVertices;
};

Poligono criaPoligonoVazio(void) {
    struct PoligonoStruct* p = malloc(sizeof(struct PoligonoStruct));
    if (!p) return NULL;
    
    p->vertices = iniciaLista();
    p->numVertices = 0;
    return (Poligono)p;
}

Poligono criaPoligonoDeLista(Lista* vertices) {
    if (!vertices || getTamLista(vertices) < 3) return NULL;
    
    struct PoligonoStruct* p = malloc(sizeof(struct PoligonoStruct));
    if (!p) return NULL;
    
    p->vertices = iniciaLista();
    p->numVertices = 0;
    
    No* atual = getHeadNo(vertices);
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt) {
            insereTail(p->vertices, clonaPonto(pt));
            p->numVertices++;
        }
        atual = vaiNoDepois(atual);
    }
    
    return (Poligono)p;
}

void liberaPoligono(Poligono p) {
    if (!p) return;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    No* atual = getHeadNo(poly->vertices);
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt) liberaPonto(pt);
        atual = vaiNoDepois(atual);
    }
    
    freeLista(poly->vertices, NULL);
    free(poly);
}

void adicionaVerticePoligono(Poligono p, Ponto vertice) {
    if (!p || !vertice) return;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    insereTail(poly->vertices, clonaPonto(vertice));
    poly->numVertices++;
}

void removeVerticePoligono(Poligono p, int indice) {
    if (!p || indice < 0) return;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (indice >= poly->numVertices) return;
    
    No* atual = getHeadNo(poly->vertices);
    int i = 0;
    
    while (atual && i < indice) {
        atual = vaiNoDepois(atual);
        i++;
    }
    
    if (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt) liberaPonto(pt);
        
        if (atual->prev) {
            atual->prev->next = atual->next;
        } else {
            poly->vertices->head = atual->next;
        }
        
        if (atual->next) {
            atual->next->prev = atual->prev;
        } else {
            poly->vertices->tail = atual->prev;
        }
        
        free(atual);
        poly->vertices->tam--;
        poly->numVertices--;
    }
}

int getNumVertices(Poligono p) {
    if (!p) return 0;
    return ((struct PoligonoStruct*)p)->numVertices;
}

int getNumSegmentos(Poligono p) {
    if (!p) return 0;
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    return (poly->numVertices > 0) ? poly->numVertices : 0;
}

Ponto getVertice(Poligono p, int indice) {
    if (!p || indice < 0) return NULL;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (indice >= poly->numVertices) return NULL;
    
    No* atual = getHeadNo(poly->vertices);
    int i = 0;
    
    while (atual && i < indice) {
        atual = vaiNoDepois(atual);
        i++;
    }
    
    if (atual) {
        Ponto original = (Ponto)getNoInfo(atual);
        if (original) {
            return clonaPonto(original);
        }
    }
    
    return NULL;
}

void* getSegmento(Poligono p, int indice) {
    return getVertice(p, indice);
}

Lista* getVertices(Poligono p) {
    if (!p) return NULL;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    Lista* copia = iniciaLista();
    
    if (!copia) return NULL;
    
    No* atual = getHeadNo(poly->vertices);
    while (atual) {
        Ponto original = (Ponto)getNoInfo(atual);
        if (original) {
            insereTail(copia, clonaPonto(original));
        }
        atual = vaiNoDepois(atual);
    }
    
    return copia;
}
/
Lista* getSegmentos(Poligono p) {
    return getVertices(p);
}

double calculaAreaPoligono(Poligono p) {
    if (!p) return 0.0;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return 0.0;
    
    double area = 0.0;
    No* atual = getHeadNo(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNoInfo(atual);
        No* prox = vaiNoDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNoInfo(prox);
        } else {
            p2 = (Ponto)getNoInfo(getHeadNo(poly->vertices));
        }
        
        if (p1 && p2) {
            double x1 = getXPonto(p1);
            double y1 = getYPonto(p1);
            double x2 = getXPonto(p2);
            double y2 = getYPonto(p2);
            
            area += (x1 * y2 - x2 * y1);
        }
        
        atual = prox;
    }
    
    return fabs(area) / 2.0;
}

double calculaPerimetro(Poligono p) {
    if (!p) return 0.0;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (poly->numVertices < 2) return 0.0;
    
    double perimetro = 0.0;
    No* atual = getHeadNo(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNoInfo(atual);
        No* prox = vaiNoDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNoInfo(prox);
        } else {
            p2 = (Ponto)getNoInfo(getHeadNo(poly->vertices));
        }
        
        if (p1 && p2) {
            double dx = getXPonto(p2) - getXPonto(p1);
            double dy = getYPonto(p2) - getYPonto(p1);
            perimetro += sqrt(dx*dx + dy*dy);
        }
        
        atual = prox;
    }
    
    return perimetro;
}

bool pontoDentroPoligono(Poligono p, Ponto ponto) {
    if (!p || !ponto) return false;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return false;
    
    double px = getXPonto(ponto);
    double py = getYPonto(ponto);
    
    bool dentro = false;
    No* atual = getHeadNo(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNoInfo(atual);
        No* prox = vaiNoDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNoInfo(prox);
        } else {
            p2 = (Ponto)getNoInfo(getHeadNo(poly->vertices));
        }
        
        if (p1 && p2) {
            double x1 = getXPonto(p1);
            double y1 = getYPonto(p1);
            double x2 = getXPonto(p2);
            double y2 = getYPonto(p2);
            
            if ((px - x1) * (y2 - y1) == (py - y1) * (x2 - x1) &&
                px >= fmin(x1, x2) && px <= fmax(x1, x2) &&
                py >= fmin(y1, y2) && py <= fmax(y1, y2)) {
                return true;
            }
            
            if (((y1 > py) != (y2 > py)) &&
                (px < (x2 - x1) * (py - y1) / (y2 - y1) + x1)) {
                dentro = !dentro;
            }
        }
        
        atual = prox;
    }
    
    return dentro;
}

bool poligonosSeInterceptam(Poligono p1, Poligono p2) {
    if (!p1 || !p2) return false;
    
    struct PoligonoStruct* poly1 = (struct PoligonoStruct*)p1;
    No* atual = getHeadNo(poly1->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt && pontoDentroPoligono(p2, pt)) {
            return true;
        }
        atual = vaiNoDepois(atual);
    }
    
    struct PoligonoStruct* poly2 = (struct PoligonoStruct*)p2;
    atual = getHeadNo(poly2->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt && pontoDentroPoligono(p1, pt)) {
            return true;
        }
        atual = vaiNoDepois(atual);
    }
    
    return false;
}

bool poligonoContemPoligono(Poligono externo, Poligono interno) {
    if (!externo || !interno) return false;
    
    struct PoligonoStruct* polyInt = (struct PoligonoStruct*)interno;
    No* atual = getHeadNo(polyInt->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt && !pontoDentroPoligono(externo, pt)) {
            return false;
        }
        atual = vaiNoDepois(atual);
    }
    
    return true;
}

TipoPoligono classificaPoligono(Poligono p) {
    if (!p) return POLIGONO_SIMPLES;
    return POLIGONO_SIMPLES;
}

bool ehPoligonoSimples(Poligono p) {
    return (p != NULL);
}

bool ehPoligonoConvexo(Poligono p) {
    if (!p) return false;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return false;
    
    int sinal = 0;
    No* atual = getHeadNo(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNoInfo(atual);
        No* prox = vaiNoDepois(atual);
        Ponto p2, p3;
        
        if (prox) {
            p2 = (Ponto)getNoInfo(prox);
            No* prox2 = vaiNoDepois(prox);
            if (prox2) {
                p3 = (Ponto)getNoInfo(prox2);
            } else {
                p3 = (Ponto)getNoInfo(getHeadNo(poly->vertices));
            }
        } else {
            p2 = (Ponto)getNoInfo(getHeadNo(poly->vertices));
            No* segundo = vaiNoDepois(getHeadNo(poly->vertices));
            p3 = (Ponto)getNoInfo(segundo);
        }
        
        if (p1 && p2 && p3) {
            double cross = (getXPonto(p2) - getXPonto(p1)) * (getYPonto(p3) - getYPonto(p2)) -
                          (getYPonto(p2) - getYPonto(p1)) * (getXPonto(p3) - getXPonto(p2));
            
            if (cross != 0) {
                if (sinal == 0) {
                    sinal = (cross > 0) ? 1 : -1;
                } else if ((cross > 0 && sinal < 0) || (cross < 0 && sinal > 0)) {
                    return false;
                }
            }
        }
        
        atual = prox;
    }
    
    return true;
}

bool ehPoligonoEstrela(Poligono p) {
    return ehPoligonoConvexo(p);
}

Poligono criaPoligonoConvexo(Lista* pontos) {
    return criaPoligonoDeLista(pontos);
}

void desenhaPoligonoSVG(Poligono p, FILE* svgFile, char* corPreench, char* corBorda) {
    if (!p || !svgFile) return;
    
    struct PoligonoStruct* poly = (struct PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return;
    
    fprintf(svgFile, "<polygon points=\"");
    
    No* atual = getHeadNo(poly->vertices);
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt) {
            fprintf(svgFile, "%.2f,%.2f ", getXPonto(pt), getYPonto(pt));
        }
        atual = vaiNoDepois(atual);
    }
    
    Ponto primeiro = (Ponto)getNoInfo(getHeadNo(poly->vertices));
    if (primeiro) {
        fprintf(svgFile, "%.2f,%.2f", getXPonto(primeiro), getYPonto(primeiro));
    }
    
    fprintf(svgFile, "\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\"/>\n",
            corPreench ? corPreench : "none",
            corBorda ? corBorda : "black");
}

Poligono transformaAnteparoEmPoligono(Anteparo a) {
    if (!a) return NULL;
    
    Poligono p = criaPoligonoVazio();
    if (!p) return NULL;
    
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    
    Ponto p1 = criaPonto(x1, y1);
    Ponto p2 = criaPonto(x2, y2);
    
    if (p1) adicionaVerticePoligono(p, p1);
    if (p2) adicionaVerticePoligono(p, p2);
    
    if (p1) liberaPonto(p1);
    if (p2) liberaPonto(p2);
    
    return p;
}

Lista* poligonosDeSegmentos(Lista* segmentos) {
    if (!segmentos) return NULL;
    
    Lista* poligonos = iniciaLista();
    if (!poligonos) return NULL;
    
    No* atual = getHeadNo(segmentos);
    while (atual) {
        Anteparo seg = (Anteparo)getNoInfo(atual);
        if (seg) {
            Poligono p = transformaAnteparoEmPoligono(seg);
            if (p) {
                insereTail(poligonos, p);
            }
        }
        atual = vaiNoDepois(atual);
    }
    
    return poligonos;
}


Poligono interseccaoPoligonos(Poligono p1, Poligono p2) {
    if (poligonoContemPoligono(p1, p2)) return criaPoligonoDeLista(getVertices(p2));
    if (poligonoContemPoligono(p2, p1)) return criaPoligonoDeLista(getVertices(p1));
    return NULL;
}

Poligono uniaoPoligonos(Poligono p1, Poligono p2) {
    Lista* todos = getVertices(p1);
    Lista* verts2 = getVertices(p2);
    
    No* atual = getHeadNo(verts2);
    while (atual) {
        Ponto pt = (Ponto)getNoInfo(atual);
        if (pt) {
            insereTail(todos, clonaPonto(pt));
        }
        atual = vaiNoDepois(atual);
    }
    
    Poligono uniao = criaPoligonoDeLista(todos);
    
    freeLista(todos, (void (*)(void*))liberaPonto);
    freeLista(verts2, (void (*)(void*))liberaPonto);
    
    return uniao;
}

Poligono diferencaPoligonos(Poligono p1, Poligono p2) {
    if (!poligonosSeInterceptam(p1, p2)) {
        return criaPoligonoDeLista(getVertices(p1));
    }
    return NULL;
}

Poligono criaPoligonoDeForma(int tipoForma, void* forma) {
    if (!forma) return criaPoligonoVazio();
    
    Forma f = (Forma)forma; 
    TipoForma tipo = getTipoForma(f);
    
    switch (tipo) {
        case Tc: { 
            Circulo c = getCirculoFromForma(f);
            if (!c) return criaPoligonoVazio();
            
            double x = getXCirculo(c);
            double y = getYCirculo(c);
            double r = getRaioCirculo(c);
            
            Poligono p = criaPoligonoVazio();
            if (!p) return NULL;
            
            for (int i = 0; i < 32; i++) {
                double angle = 2.0 * M_PI * i / 32.0;
                double px = x + r * cos(angle);
                double py = y + r * sin(angle);
                Ponto pt = criaPonto(px, py);
                if (pt) {
                    adicionaVerticePoligono(p, pt);
                    liberaPonto(pt);
                }
            }
            return p;
        }
        
        case Tr: { 
            Retangulo r = getRetanguloFromForma(f);
            if (!r) return criaPoligonoVazio();
            
            double x = getXRetangulo(r);
            double y = getYRetangulo(r);
            double w = getLarguraRetangulo(r);
            double h = getAlturaRetangulo(r);
            
            Poligono p = criaPoligonoVazio();
            if (!p) return NULL;
            
            Ponto pts[4];
            pts[0] = criaPonto(x, y);
            pts[1] = criaPonto(x + w, y);
            pts[2] = criaPonto(x + w, y + h);
            pts[3] = criaPonto(x, y + h);
            
            for (int i = 0; i < 4; i++) {
                if (pts[i]) {
                    adicionaVerticePoligono(p, pts[i]);
                    liberaPonto(pts[i]);
                }
            }
            return p;
        }
        
        case Tl: { 
            Linha l = getLinhaFromForma(f);
            if (!l) return criaPoligonoVazio();
            
            double x1 = getX1Linha(l);
            double y1 = getY1Linha(l);
            double x2 = getX2Linha(l);
            double y2 = getY2Linha(l);
            
            Poligono p = criaPoligonoVazio();
            if (!p) return NULL;
            
            Ponto p1 = criaPonto(x1, y1);
            Ponto p2 = criaPonto(x2, y2);
            
            if (p1) adicionaVerticePoligono(p, p1);
            if (p2) adicionaVerticePoligono(p, p2);
            
            if (p1) liberaPonto(p1);
            if (p2) liberaPonto(p2);
            
            return p;
        }
        
        case Tt: { 
            Texto t = getTextoFromForma(f);
            if (!t) return criaPoligonoVazio();
            
            double x = getXTexto(t);
            double y = getYTexto(t);
            char ancora = getAncoraTexto(t);
            int numChars = getNumCaracteresTexto(t);
            
            if (numChars <= 0) return criaPoligonoVazio();
            
            double comp = 10.0 * numChars; 
            double x1, x2;
            
            switch (ancora) {
                case 'i': 
                    x1 = x;
                    x2 = x + comp;
                    break;
                case 'f': 
                    x1 = x - comp;
                    x2 = x;
                    break;
                case 'm': 
                    x1 = x - comp / 2.0;
                    x2 = x + comp / 2.0;
                    break;
                default:
                    return criaPoligonoVazio();
            }
            
            Poligono p = criaPoligonoVazio();
            if (!p) return NULL;
            
            Ponto p1 = criaPonto(x1, y);
            Ponto p2 = criaPonto(x2, y);
            
            if (p1) adicionaVerticePoligono(p, p1);
            if (p2) adicionaVerticePoligono(p, p2);
            if (p1) liberaPonto(p1);
            if (p2) liberaPonto(p2);
            
            return p;
        }
        
        case Ta: { 
            Anteparo a = getAnteparoFromForma(f);
            if (!a) return criaPoligonoVazio();
            return transformaAnteparoEmPoligono(a);
        }
        
        default:
            fprintf(stderr, "Erro: tipo de forma desconhecido em criaPoligonoDeForma: %d\n", tipo);
            return criaPoligonoVazio();
    }
}
