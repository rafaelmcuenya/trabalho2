#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "forma.h"
#include <math.h>
#include "poligono.h" 
#include <stdlib.h>    
#include <math.h>
#include <float.h>
#include "strdupi.h"

#define EPSILON 1e-12
#define M_PI 3.14159265358979323846


typedef struct {
    Lista* vertices; 
    int numVertices;
} PoligonoStruct;

Poligono criaPoligonoVazio(void) {
    PoligonoStruct* p = malloc(sizeof(PoligonoStruct));
    if (!p) return NULL;
    
    p->vertices = iniciaLista();
    p->numVertices = 0;
    return (Poligono)p;
}

Poligono criaPoligonoDeLista(Lista* vertices) {
    if (!vertices || getTamLista(vertices) < 2) return NULL;
    
    PoligonoStruct* p = malloc(sizeof(PoligonoStruct));
    if (!p) return NULL;
    
    p->vertices = iniciaLista();
    p->numVertices = 0;
    
    Node* atual = getHeadNode(vertices);
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt) {
            insereTail(p->vertices, clonaPonto(pt));
            p->numVertices++;
        }
        atual = vaiNodeDepois(atual);
    }
    
    if (p->numVertices < 2) {
        freeLista(p->vertices, NULL);
        free(p);
        return NULL;
    }
    
    return (Poligono)p;
}

void liberaPoligono(Poligono p) {
    if (!p) return;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    Node* atual = getHeadNode(poly->vertices);
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt) liberaPonto(pt);
        atual = vaiNodeDepois(atual);
    }
    
    freeLista(poly->vertices, NULL);
    free(poly);
}

void adicionaVerticePoligono(Poligono p, Ponto vertice) {
    if (!p || !vertice) return;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    insereTail(poly->vertices, clonaPonto(vertice));
    poly->numVertices++;
}

void removeVerticePoligono(Poligono p, int indice) {
    if (!p || indice < 0) return;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (indice >= poly->numVertices) return;
    
    Lista* novaLista = iniciaLista();
    if (!novaLista) return;
    
    Node* atual = getHeadNode(poly->vertices);
    int i = 0;
    
    while (atual) {
        if (i != indice) {
            Ponto ponto = (Ponto)getNodeInfo(atual);
            if (ponto) {
                insereTail(novaLista, clonaPonto(ponto));
            }
        } else {
            Ponto ponto = (Ponto)getNodeInfo(atual);
            if (ponto) {
                liberaPonto(ponto);
            }
        }
        
        atual = vaiNodeDepois(atual);
        i++;
    }
    
    freeLista(poly->vertices, NULL);
    poly->vertices = novaLista;
    poly->numVertices--;
}

int getNumVertices(Poligono p) {
    if (!p) return 0;
    return ((PoligonoStruct*)p)->numVertices;
}

int getNumSegmentos(Poligono p) {
    if (!p) return 0;
    PoligonoStruct* poly = (PoligonoStruct*)p;
    return (poly->numVertices > 0) ? poly->numVertices : 0;
}

Ponto getVertice(Poligono p, int indice) {
    if (!p || indice < 0) return NULL;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (indice >= poly->numVertices) {
        printf("[ERRO POLIGONO] Índice %d fora dos limites (%d vértices)\n", 
               indice, poly->numVertices);
        return NULL;
    }
    
    Node* atual = getHeadNode(poly->vertices);
    int i = 0;
    
    while (atual && i < indice) {
        atual = vaiNodeDepois(atual);
        i++;
    }
    
    if (atual) {
        Ponto original = (Ponto)getNodeInfo(atual);
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
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    Lista* copia = iniciaLista();
    
    if (!copia) return NULL;
    
    Node* atual = getHeadNode(poly->vertices);
    while (atual) {
        Ponto original = (Ponto)getNodeInfo(atual);
        if (original) {
            insereTail(copia, clonaPonto(original));
        }
        atual = vaiNodeDepois(atual);
    }
    
    return copia;
}

Lista* getSegmentos(Poligono p) {
    return getVertices(p);
}

double calculaAreaPoligono(Poligono p) {
    if (!p) return 0.0;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    Node* atual = getHeadNode(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNodeInfo(atual);
        Node* prox = vaiNodeDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNodeInfo(prox);
        } else {
            p2 = (Ponto)getNodeInfo(getHeadNode(poly->vertices));
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
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 2) return 0.0;
    
    double perimetro = 0.0;
    Node* atual = getHeadNode(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNodeInfo(atual);
        Node* prox = vaiNodeDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNodeInfo(prox);
        } else {
            if (poly->numVertices >= 3) {
                p2 = (Ponto)getNodeInfo(getHeadNode(poly->vertices));
            } else {
                break;
            }
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
    printf("[DEBUG POLIGONO] pontoDentroPoligono chamado\n");
    
    if (!p || !ponto) {
        printf("[DEBUG POLIGONO] ERRO: Parâmetros NULL\n");
        return false;
    }
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    printf("[DEBUG POLIGONO] Polígono tem %d vértices\n", poly->numVertices);
    
    if (poly->numVertices < 2) {
        printf("[DEBUG POLIGONO] Polígono com menos de 2 vértices\n");
        return false;
    }
    
    if (poly->numVertices == 2) {
        Ponto p1 = getVertice(p, 0);
        Ponto p2 = getVertice(p, 1);
        
        if (!p1 || !p2) {
            if (p1) liberaPonto(p1);
            if (p2) liberaPonto(p2);
            return false;
        }
        
        double px = getXPonto(ponto);
        double py = getYPonto(ponto);
        double x1 = getXPonto(p1);
        double y1 = getYPonto(p1);
        double x2 = getXPonto(p2);
        double y2 = getYPonto(p2);
        
        liberaPonto(p1);
        liberaPonto(p2);
        
        double cross = (px - x1) * (y2 - y1) - (py - y1) * (x2 - x1);
        if (fabs(cross) > EPSILON) return false;
        
        return (px >= fmin(x1, x2) - EPSILON && px <= fmax(x1, x2) + EPSILON &&
                py >= fmin(y1, y2) - EPSILON && py <= fmax(y1, y2) + EPSILON);
    }
    
    double px = getXPonto(ponto);
    double py = getYPonto(ponto);
    
    bool dentro = false;
    Node* atual = getHeadNode(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNodeInfo(atual);
        Node* prox = vaiNodeDepois(atual);
        Ponto p2;
        
        if (prox) {
            p2 = (Ponto)getNodeInfo(prox);
        } else {
            p2 = (Ponto)getNodeInfo(getHeadNode(poly->vertices));
        }
        
        if (p1 && p2) {
            double x1 = getXPonto(p1);
            double y1 = getYPonto(p1);
            double x2 = getXPonto(p2);
            double y2 = getYPonto(p2);
            
            if ((px - x1) * (y2 - y1) == (py - y1) * (x2 - x1) &&
                px >= fmin(x1, x2) - EPSILON && px <= fmax(x1, x2) + EPSILON &&
                py >= fmin(y1, y2) - EPSILON && py <= fmax(y1, y2) + EPSILON) {
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
    
    PoligonoStruct* poly1 = (PoligonoStruct*)p1;
    Node* atual = getHeadNode(poly1->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt && pontoDentroPoligono(p2, pt)) {
            return true;
        }
        atual = vaiNodeDepois(atual);
    }
    
    PoligonoStruct* poly2 = (PoligonoStruct*)p2;
    atual = getHeadNode(poly2->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt && pontoDentroPoligono(p1, pt)) {
            return true;
        }
        atual = vaiNodeDepois(atual);
    }
    
    return false;
}

bool poligonoContemPoligono(Poligono externo, Poligono interno) {
    if (!externo || !interno) return false;
    
    PoligonoStruct* polyInt = (PoligonoStruct*)interno;
    Node* atual = getHeadNode(polyInt->vertices);
    
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt && !pontoDentroPoligono(externo, pt)) {
            return false;
        }
        atual = vaiNodeDepois(atual);
    }
    
    return true;
}

TipoPoligono classificaPoligono(Poligono p) {
    if (!p) return POLIGONO_SIMPLES;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 3) {
        return POLIGONO_SIMPLES;
    }
    
    if (ehPoligonoConvexo(p)) {
        return POLIGONO_CONVEXO;
    } else if (ehPoligonoEstrela(p)) {
        return POLIGONO_ESTRELA;
    }
    
    return POLIGONO_SIMPLES;
}

bool ehPoligonoSimples(Poligono p) {
    if (!p) return false;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return true;
    
    return true;
}

bool ehPoligonoConvexo(Poligono p) {
    if (!p) return false;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return false;
    
    int sinal = 0;
    Node* atual = getHeadNode(poly->vertices);
    
    while (atual) {
        Ponto p1 = (Ponto)getNodeInfo(atual);
        Node* prox = vaiNodeDepois(atual);
        Ponto p2, p3;
        
        if (prox) {
            p2 = (Ponto)getNodeInfo(prox);
            Node* prox2 = vaiNodeDepois(prox);
            if (prox2) {
                p3 = (Ponto)getNodeInfo(prox2);
            } else {
                p3 = (Ponto)getNodeInfo(getHeadNode(poly->vertices));
            }
        } else {
            p2 = (Ponto)getNodeInfo(getHeadNode(poly->vertices));
            Node* segundo = vaiNodeDepois(getHeadNode(poly->vertices));
            p3 = (Ponto)getNodeInfo(segundo);
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
    if (!p) return false;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 3) return false;
    
    return ehPoligonoConvexo(p);
}

Poligono criaPoligonoConvexo(Lista* pontos) {
    return criaPoligonoDeLista(pontos);
}

void desenhaPoligonoSVG(Poligono p, FILE* svgFile, char* corPreench, char* corBorda) {
    if (!p || !svgFile) return;
    
    PoligonoStruct* poly = (PoligonoStruct*)p;
    
    if (poly->numVertices < 2) return;
    
    if (poly->numVertices == 2) {
        Ponto p1 = getVertice(p, 0);
        Ponto p2 = getVertice(p, 1);
        
        if (p1 && p2) {
            fprintf(svgFile, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\"/>\n",
            getXPonto(p1), getYPonto(p1), getXPonto(p2), getYPonto(p2), corBorda ? corBorda : "black");
        }
        
        if (p1) liberaPonto(p1);
        if (p2) liberaPonto(p2);
        return;
    }
    
    fprintf(svgFile, "<polygon points=\"");
    
    Node* atual = getHeadNode(poly->vertices);
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt) {
            fprintf(svgFile, "%.2f,%.2f ", getXPonto(pt), getYPonto(pt));
        }
        atual = vaiNodeDepois(atual);
    }
    
    Ponto primeiro = getVertice(p, 0);
    if (primeiro) {
        fprintf(svgFile, "%.2f,%.2f", getXPonto(primeiro), getYPonto(primeiro));
        liberaPonto(primeiro);
    }
    
    fprintf(svgFile, "\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\"/>\n", corPreench ? corPreench : "none", corBorda ? corBorda : "black");
}

Poligono transformaAnteparoEmPoligono(Anteparo a) {
    if (!a) return NULL;
    
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    Poligono p = criaPoligonoVazio();
    if (!p) return NULL;
    
    Ponto p1 = criaPonto(x1, y1);
    Ponto p2 = criaPonto(x2, y2);
    double mx = (x1 + x2) / 2.0;
    double my = (y1 + y2) / 2.0;
    double dx = y2 - y1;
    double dy = x1 - x2;
    double length = sqrt(dx*dx + dy*dy);
    if (length > 0) {
        dx = dx / length * 0.1; 
        dy = dy / length * 0.1;
    } else {
        dx = 0.1;
        dy = 0.1;
    }
    
    Ponto p3 = criaPonto(mx + dx, my + dy);
    
    if (p1) {
        adicionaVerticePoligono(p, p1);
        liberaPonto(p1);
    }
    
    if (p2) {
        adicionaVerticePoligono(p, p2);
        liberaPonto(p2);
    }
    
    if (p3) {
        adicionaVerticePoligono(p, p3);
        liberaPonto(p3);
    }
    
    return p;
}

Lista* poligonosDeSegmentos(Lista* segmentos) {
    if (!segmentos) return NULL;
    
    Lista* poligonos = iniciaLista();
    if (!poligonos) return NULL;
    
    Node* atual = getHeadNode(segmentos);
    while (atual) {
        Anteparo seg = (Anteparo)getNodeInfo(atual);
        if (seg) {
            Poligono p = transformaAnteparoEmPoligono(seg);
            if (p) {
                insereTail(poligonos, p);
            }
        }
        atual = vaiNodeDepois(atual);
    }
    
    return poligonos;
}

Poligono interseccaoPoligonos(Poligono p1, Poligono p2) {
    if (!p1 || !p2) return NULL;
    
    if (poligonoContemPoligono(p1, p2)) {
        return criaPoligonoDeLista(getVertices(p2));
    }
    
    if (poligonoContemPoligono(p2, p1)) {
        return criaPoligonoDeLista(getVertices(p1));
    }
    
    return NULL;
}

Poligono uniaoPoligonos(Poligono p1, Poligono p2) {
    if (!p1 && !p2) return NULL;
    if (!p1) return criaPoligonoDeLista(getVertices(p2));
    if (!p2) return criaPoligonoDeLista(getVertices(p1));
    
    Lista* todos = getVertices(p1);
    Lista* verts2 = getVertices(p2);
    
    Node* atual = getHeadNode(verts2);
    while (atual) {
        Ponto pt = (Ponto)getNodeInfo(atual);
        if (pt) {
            insereTail(todos, clonaPonto(pt));
        }
        atual = vaiNodeDepois(atual);
    }
    
    Poligono uniao = criaPoligonoDeLista(todos);
    
    freeLista(todos, (void (*)(void*))liberaPonto);
    freeLista(verts2, (void (*)(void*))liberaPonto);
    
    return uniao;
}

Poligono diferencaPoligonos(Poligono p1, Poligono p2) {
    if (!p1) return NULL;
    if (!p2) return criaPoligonoDeLista(getVertices(p1));
    
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
            
            double mx = (x1 + x2) / 2.0;
            double my = (y1 + y2) / 2.0;
            Ponto p3 = criaPonto(mx + 0.1, my + 0.1);
            if (p3) {
                adicionaVerticePoligono(p, p3);
                liberaPonto(p3);
            }
            
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
            
            double mx = (x1 + x2) / 2.0;
            Ponto p3 = criaPonto(mx, y + 5.0);
            if (p3) {
                adicionaVerticePoligono(p, p3);
                liberaPonto(p3);
            }
            
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
