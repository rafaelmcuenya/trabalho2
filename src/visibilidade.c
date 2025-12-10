#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include "visibilidade.h"
#include "geometria.h"
#include "sort.h"
#include "lista.h"
#include "strdupi.h"

#define EPSILON 1e-12
#define PI 3.14159265358979323846
#define INF 1e30

typedef enum {
    TIPO_INICIO = 0,
    TIPO_FIM = 1,
    TIPO_RETANGULO = 2
} TipoEvento;

typedef struct {
    Ponto ponto;         
    double angulo;       
    double distancia;    
    TipoEvento tipo;      
    Anteparo anteparo;     
    int id;              
} Evento;

typedef struct {
    Anteparo anteparo;   
    Ponto inicio;          
    Ponto fim;            
    double anguloInicio;   
    double anguloFim;      
    int ativo;             
    int id;                
} SegmentoAtivo;

typedef struct {
    Poligono poligono;     
    Ponto origem;          
    Lista* segmentos;    
    double raioMaximo;     
} RegiaoVisivelStruct;

static Ponto fOrigemComp = NULL;


static double normalAngulo(double angulo) {
    while (angulo < 0) angulo += 2 * PI;
    while (angulo >= 2 * PI) angulo -= 2 * PI;
    return angulo;
}

static double calcularAngulo(Ponto origem, Ponto p) {
    if (!origem || !p) return 0.0;
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    double px = getXPonto(p);
    double py = getYPonto(p);
    
    double dx = px - ox;
    double dy = py - oy;
    
    if (fabs(dx) < EPSILON && fabs(dy) < EPSILON) return 0.0;
    
    double angulo = atan2(dy, dx);
    if (angulo < 0) angulo += 2 * PI;
    
    return angulo;
}

static double calcularDistancia(Ponto a, Ponto b) {
    if (!a || !b) return 0.0;
    
    double dx = getXPonto(b) - getXPonto(a);
    double dy = getYPonto(b) - getYPonto(a);
    return sqrt(dx*dx + dy*dy);
}

static int comparaPontoAngulo(const void* a, const void* b) {
    const Ponto* pa = (const Ponto*)a;
    const Ponto* pb = (const Ponto*)b;
    
    if (!fOrigemComp || !*pa || !*pb) return 0;
    
    double ang_a = calcularAngulo(fOrigemComp, *pa);
    double ang_b = calcularAngulo(fOrigemComp, *pb);
    
    if (ang_a < ang_b - EPSILON) return -1;
    if (ang_a > ang_b + EPSILON) return 1;
    
    double dist_a = calcularDistancia(fOrigemComp, *pa);
    double dist_b = calcularDistancia(fOrigemComp, *pb);
    
    if (dist_a < dist_b - EPSILON) return -1;
    if (dist_a > dist_b + EPSILON) return 1;
    
    return 0;
}

static int comparaSegAtivos(void* a, void* b) {
    SegmentoAtivo* sa = (SegmentoAtivo*)a;
    SegmentoAtivo* sb = (SegmentoAtivo*)b;
    
    if (!sa || !sb) return 0;
    
    if (sa->anguloInicio < sb->anguloInicio - EPSILON) return -1;
    if (sa->anguloInicio > sb->anguloInicio + EPSILON) return 1;
    
    if (sa->anguloFim < sb->anguloFim - EPSILON) return -1;
    if (sa->anguloFim > sb->anguloFim + EPSILON) return 1;
    
    return (sa->id < sb->id) ? -1 : (sa->id > sb->id) ? 1 : 0;
}

static int atrasSegmento(Ponto origem, Ponto vertice, SegmentoAtivo* seg) {
    if (!origem || !vertice || !seg) return 0;
    
    Ponto p1 = seg->inicio;
    Ponto p2 = seg->fim;
    
    if (!p1 || !p2) return 0;
    
    int orient1 = orientacaoPontos(origem, p1, vertice);
    int orient2 = orientacaoPontos(origem, p2, vertice);
    
    return (orient1 == orient2 && orient1 != 0);
}

static SegmentoAtivo* findSegPerto(Arvore* segAtivos, Ponto origem, double angulo) {
    if (!segAtivos || vaziaAVL(segAtivos)) return NULL;
    
    NoArv* raiz = getRaiz(segAtivos);
    if (!raiz) return NULL;
    
    SegmentoAtivo* mais_proximo = NULL;
    double menor_distancia = INF;
    
    void percorrerArvore(NoArv* no) {
        if (!no) return;
        
        SegmentoAtivo* seg = (SegmentoAtivo*)getNoInfo(no);
        if (seg && seg->ativo) {
            double ox = getXPonto(origem);
            double oy = getYPonto(origem);
            double dx = cos(angulo);
            double dy = sin(angulo);
            
            double x1 = getXPonto(seg->inicio);
            double y1 = getYPonto(seg->inicio);
            double x2 = getXPonto(seg->fim);
            double y2 = getYPonto(seg->fim);
            
            double xi, yi;
            if (interseccaoSegmentosCoordenadas(ox, oy, ox + dx * INF, oy + dy * INF,
                                               x1, y1, x2, y2, &xi, &yi)) {
                double dist = sqrt(pow(xi - ox, 2) + pow(yi - oy, 2));
                if (dist < menor_distancia) {
                    menor_distancia = dist;
                    mais_proximo = seg;
                }
            }
        }
        
        percorrerArvore(getNoEsquerda(no));
        percorrerArvore(getNoDireita(no));
    }
    
    percorrerArvore(raiz);
    return mais_proximo;
}

static void addBigBB(Lista* anteparos, double xmin, double ymin, double xmax, double ymax) {
    char* cor = "#000000";
    
    /* Lado inferior */
    Anteparo a1 = criaAnteparo(-1, xmin, ymin, xmax, ymin, cor);
    /* Lado direito */
    Anteparo a2 = criaAnteparo(-2, xmax, ymin, xmax, ymax, cor);
    /* Lado superior */
    Anteparo a3 = criaAnteparo(-3, xmax, ymax, xmin, ymax, cor);
    /* Lado esquerdo */
    Anteparo a4 = criaAnteparo(-4, xmin, ymax, xmin, ymin, cor);
    
    if (a1) insereTail(anteparos, a1);
    if (a2) insereTail(anteparos, a2);
    if (a3) insereTail(anteparos, a3);
    if (a4) insereTail(anteparos, a4);
}

RegiaoVisivel criaRegiaoVisivel(void) {
    RegiaoVisivelStruct* regiao = (RegiaoVisivelStruct*)malloc(sizeof(RegiaoVisivelStruct));
    if (!regiao) {
        fprintf(stderr, "Erro: falha na alocação da região visível\n");
        return NULL;
    }
    
    regiao->poligono = NULL;
    regiao->origem = NULL;
    regiao->segmentos = iniciaLista();
    regiao->raioMaximo = 0.0;
    
    return (RegiaoVisivel)regiao;
}

void liberaRegiaoVisivel(RegiaoVisivel regiao) {
    if (!regiao) return;
    
    RegiaoVisivelStruct* r = (RegiaoVisivelStruct*)regiao;
    
    if (r->poligono) {
        liberaPoligono(r->poligono);
    }
    
    if (r->origem) {
        liberaPonto(r->origem);
    }
    
    if (r->segmentos) {
        freeLista(r->segmentos, NULL);
    }
    
    free(r);
}

void* prepararEventosVisibilidade(Ponto origem, Lista* anteparos, int* numEventos) {
    if (!origem || !anteparos) {
        *numEventos = 0;
        return NULL;
    }
    
    int numAnteparos = getTamLista(anteparos);
    Evento* eventos = (Evento*)malloc(numAnteparos * 2 * sizeof(Evento));
    if (!eventos) {
        fprintf(stderr, "Erro: falha na alocação de eventos\n");
        *numEventos = 0;
        return NULL;
    }
    
    int count = 0;
    No* atual = getHeadNo(anteparos);
    int id_counter = 0;
    
    while (atual) {
        Anteparo a = (Anteparo)getNoInfo(atual);
        if (a) {
            double x1 = getX1Anteparo(a);
            double y1 = getY1Anteparo(a);
            double x2 = getX2Anteparo(a);
            double y2 = getY2Anteparo(a);
            
            Ponto p1 = criaPonto(x1, y1);
            Ponto p2 = criaPonto(x2, y2);
            
            if (p1 && p2) {
                double ang1 = calcularAngulo(origem, p1);
                double ang2 = calcularAngulo(origem, p2);
                
                if (ang1 <= ang2) {
                    eventos[count].ponto = p1;
                    eventos[count].angulo = ang1;
                    eventos[count].distancia = calcularDistancia(origem, p1);
                    eventos[count].tipo = TIPO_INICIO;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                    
                    eventos[count].ponto = p2;
                    eventos[count].angulo = ang2;
                    eventos[count].distancia = calcularDistancia(origem, p2);
                    eventos[count].tipo = TIPO_FIM;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                } else {
                    eventos[count].ponto = p2;
                    eventos[count].angulo = ang2;
                    eventos[count].distancia = calcularDistancia(origem, p2);
                    eventos[count].tipo = TIPO_INICIO;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                    
                    eventos[count].ponto = p1;
                    eventos[count].angulo = ang1;
                    eventos[count].distancia = calcularDistancia(origem, p1);
                    eventos[count].tipo = TIPO_FIM;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                }
            } else {
                if (p1) liberaPonto(p1);
                if (p2) liberaPonto(p2);
            }
        }
        atual = vaiNoDepois(atual);
    }
    
    *numEventos = count;
    return eventos;
}

int comparaEventosVisibilidade(const void* a, const void* b) {
    const Evento* ea = (const Evento*)a;
    const Evento* eb = (const Evento*)b;
    
    if (ea->angulo < eb->angulo - EPSILON) return -1;
    if (ea->angulo > eb->angulo + EPSILON) return 1;
    
    if (ea->distancia < eb->distancia - EPSILON) return -1;
    if (ea->distancia > eb->distancia + EPSILON) return 1;
    
    if (ea->tipo == TIPO_INICIO && eb->tipo == TIPO_FIM) return -1;
    if (ea->tipo == TIPO_FIM && eb->tipo == TIPO_INICIO) return 1;
    
    return (ea->id < eb->id) ? -1 : (ea->id > eb->id) ? 1 : 0;
}

Ponto raioAteAnteparo(Arvore* segAtivos, Ponto origem, double angulo, double raioMax) {
    if (!origem) {
        return NULL;
    }
    
    SegmentoAtivo* mais_proximo = findSegPerto(segAtivos, origem, angulo);
    
    if (!mais_proximo) {
        double x = getXPonto(origem) + raioMax * cos(angulo);
        double y = getYPonto(origem) + raioMax * sin(angulo);
        return criaPonto(x, y);
    }
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    double dx = cos(angulo);
    double dy = sin(angulo);
    
    double x1 = getXPonto(mais_proximo->inicio);
    double y1 = getYPonto(mais_proximo->inicio);
    double x2 = getXPonto(mais_proximo->fim);
    double y2 = getYPonto(mais_proximo->fim);
    
    double xi, yi;
    if (interseccaoSegmentosCoordenadas(ox, oy, ox + dx * INF, oy + dy * INF,
                                       x1, y1, x2, y2, &xi, &yi)) {
        return criaPonto(xi, yi);
    }
    
    double x = getXPonto(origem) + raioMax * cos(angulo);
    double y = getYPonto(origem) + raioMax * sin(angulo);
    return criaPonto(x, y);
}

Poligono calculaRegiaoVisivel(Ponto origem, Lista* anteparos, char tipoOrdenacao, double raioMaximo, int threshold) {
    if (!origem || !anteparos) {
        fprintf(stderr, "Erro: parâmetros inválidos em calculaRegiaoVisivel\n");
        return NULL;
    }
    
    Lista* todos_anteparos = iniciaLista();
    No* atual = getHeadNo(anteparos);
    while (atual) {
        Anteparo a = (Anteparo)getNoInfo(atual);
        if (a) {
            Anteparo copia = clonaAnteparo(a, idAnteparo(a));
            if (copia) {
                insereTail(todos_anteparos, copia);
            }
        }
        atual = vaiNoDepois(atual);
    }
    
    double xmin = INF, ymin = INF, xmax = -INF, ymax = -INF;
    atual = getHeadNo(todos_anteparos);
    while (atual) {
        Anteparo a = (Anteparo)getNoInfo(atual);
        if (a) {
            xmin = fmin(xmin, fmin(getX1Anteparo(a), getX2Anteparo(a)));
            ymin = fmin(ymin, fmin(getY1Anteparo(a), getY2Anteparo(a)));
            xmax = fmax(xmax, fmax(getX1Anteparo(a), getX2Anteparo(a)));
            ymax = fmax(ymax, fmax(getY1Anteparo(a), getY2Anteparo(a)));
        }
        atual = vaiNoDepois(atual);
    }
    
    double expand = raioMaximo * 2;
    xmin -= expand; ymin -= expand;
    xmax += expand; ymax += expand;
    
    addBigBB(todos_anteparos, xmin, ymin, xmax, ymax);
    
    int numEventos = 0;
    Evento* eventos = (Evento*)prepararEventosVisibilidade(origem, todos_anteparos, &numEventos);
    
    if (!eventos || numEventos == 0) {
        fprintf(stderr, "Erro: não foi possível preparar eventos\n");
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    if (tipoOrdenacao == 'q') {
        qsort(eventos, numEventos, sizeof(Evento), comparaEventosVisibilidade);
    } else {
        mergeSort(eventos, numEventos, sizeof(Evento), 
                  (ComparaFunc)comparaEventosVisibilidade, NULL, threshold);
    }
    
    Arvore* segAtivos = iniciarArvore(comparaSegAtivos);
    if (!segAtivos) {
        fprintf(stderr, "Erro: falha ao criar árvore AVL\n");
        free(eventos);
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    Poligono poligono = criaPoligonoVazio();
    if (!poligono) {
        fprintf(stderr, "Erro: falha ao criar polígono\n");
        freeArvore(segAtivos, NULL);
        free(eventos);
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    SegmentoAtivo* biombo_atual = NULL;
    Ponto ponto_biombo = NULL;
    
    for (int i = 0; i < numEventos; i++) {
        Evento* ev = &eventos[i];
        
        if (ev->tipo == TIPO_INICIO) {
            SegmentoAtivo* seg = (SegmentoAtivo*)malloc(sizeof(SegmentoAtivo));
            if (seg) {
                seg->anteparo = ev->anteparo;
                seg->inicio = clonaPonto(ev->ponto);
                
                double x1 = getX1Anteparo(ev->anteparo);
                double y1 = getY1Anteparo(ev->anteparo);
                double x2 = getX2Anteparo(ev->anteparo);
                double y2 = getY2Anteparo(ev->anteparo);
                
                Ponto p1 = criaPonto(x1, y1);
                Ponto p2 = criaPonto(x2, y2);
                
                if (calcularDistancia(origem, p1) < calcularDistancia(origem, p2) + EPSILON) {
                    seg->fim = clonaPonto(p2);
                } else {
                    seg->fim = clonaPonto(p1);
                }
                
                seg->anguloInicio = ev->angulo;
                seg->anguloFim = calcularAngulo(origem, seg->fim);
                seg->ativo = 1;
                seg->id = ev->id;
                
                insereNo(segAtivos, seg);
                
                liberaPonto(p1);
                liberaPonto(p2);
                
                if (!atrasSegmento(origem, ev->ponto, biombo_atual)) {
                    if (ponto_biombo) {
                        Ponto interseccao = raioAteAnteparo(segAtivos, origem, ev->angulo, raioMaximo);
                        if (interseccao) {
                            adicionaVerticePoligono(poligono, ponto_biombo);
                            adicionaVerticePoligono(poligono, interseccao);
                            liberaPonto(interseccao);
                        }
                        
                        adicionaVerticePoligono(poligono, ev->ponto);
                        
                        liberaPonto(ponto_biombo);
                        ponto_biombo = clonaPonto(ev->ponto);
                        biombo_atual = seg;
                    }
                }
            }
            
        } else if (ev->tipo == TIPO_FIM) {
            SegmentoAtivo chave;
            chave.id = ev->id;
            
            NoArv* no = buscaBinaria(segAtivos, &chave);
            if (no) {
                SegmentoAtivo* seg = (SegmentoAtivo*)getNoInfo(no);
                
                if (seg == biombo_atual) {
                    if (ponto_biombo) {
                        adicionaVerticePoligono(poligono, ponto_biombo);
                        adicionaVerticePoligono(poligono, ev->ponto);
                        
                        liberaPonto(ponto_biombo);
                        ponto_biombo = raioAteAnteparo(segAtivos, origem, ev->angulo, raioMaximo);
                        
                        biombo_atual = findSegPerto(segAtivos, origem, ev->angulo);
                    }
                }
                
                removeNo(segAtivos, seg);
                
                if (seg->inicio) liberaPonto(seg->inicio);
                if (seg->fim) liberaPonto(seg->fim);
                free(seg);
            }
        }
    }
    
    int num_vertices = getNumVertices(poligono);
    if (num_vertices > 0) {
        Ponto primeiro = getVertice(poligono, 0);
        if (primeiro) {
            adicionaVerticePoligono(poligono, primeiro);
            liberaPonto(primeiro);
        }
    }
    
    for (int i = 0; i < numEventos; i++) {
        if (eventos[i].ponto) {
            liberaPonto(eventos[i].ponto);
        }
    }
    free(eventos);
    
    if (ponto_biombo) {
        liberaPonto(ponto_biombo);
    }
    
    freeArvore(segAtivos, NULL);
    
    freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
    
    return poligono;
}

bool pontoDentroRegiaoVisivel(RegiaoVisivel regiao, Ponto p) {
    if (!regiao || !p) return false;
    
    RegiaoVisivelStruct* r = (RegiaoVisivelStruct*)regiao;
    if (!r->poligono) return false;
    
    return pontoDentroPoligono(r->poligono, p);
}

Poligono getPoligonoRegiaoVisivel(RegiaoVisivel regiao) {
    if (!regiao) return NULL;
    RegiaoVisivelStruct* r = (RegiaoVisivelStruct*)regiao;
    return r->poligono;
}

Ponto getOrigemRegiaoVisivel(RegiaoVisivel regiao) {
    if (!regiao) return NULL;
    RegiaoVisivelStruct* r = (RegiaoVisivelStruct*)regiao;
    if (!r->origem) return NULL;
    return clonaPonto(r->origem);
}

void desenhaRegiaoVisivelSVG(RegiaoVisivel regiao, FILE* svgFile, const char* corPreench, const char* corBorda) {
    if (!regiao || !svgFile) return;
    
    RegiaoVisivelStruct* r = (RegiaoVisivelStruct*)regiao;
    if (!r->poligono) return;
    
    desenhaPoligonoSVG(r->poligono, svgFile, (char*)corPreench, (char*)corBorda);
}
