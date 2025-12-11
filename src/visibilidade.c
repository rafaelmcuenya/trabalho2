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
    
    if (sa->id < sb->id) return -1;
    if (sa->id > sb->id) return 1;
    return 0;
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
    if (!segAtivos || vaziaAVL(segAtivos)) {
        printf("[DEBUG FINDSEG] Árvore vazia ou nula\n");
        return NULL;
    }
    
    NoArv* raiz = getRaiz(segAtivos);
    if (!raiz) {
        printf("[DEBUG FINDSEG] Raiz nula\n");
        return NULL;
    }
    
    SegmentoAtivo* mais_proximo = NULL;
    double menor_distancia = INF;
    
    void percorrerArvore(NoArv* no) {
        if (!no) return;
        
        SegmentoAtivo* seg = (SegmentoAtivo*)getNoInfo(no);
        if (seg && seg->ativo) {
            printf("[DEBUG FINDSEG] Analisando segmento %d\n", seg->id);
            
            double ox = getXPonto(origem);
            double oy = getYPonto(origem);
            double dx = cos(angulo);
            double dy = sin(angulo);
            
            double x1 = getXPonto(seg->inicio);
            double y1 = getYPonto(seg->inicio);
            double x2 = getXPonto(seg->fim);
            double y2 = getYPonto(seg->fim);
            
            double xi, yi;
            if (interseccaoSegmentosCoordenadas(ox, oy, ox + dx * INF, oy + dy * INF, x1, y1, x2, y2, &xi, &yi)) {
                double dist = sqrt(pow(xi - ox, 2) + pow(yi - oy, 2));
                printf("[DEBUG FINDSEG]  Interseção encontrada a distância %.2f\n", dist);
                
                if (dist < menor_distancia && dist > EPSILON) {
                    menor_distancia = dist;
                    mais_proximo = seg;
                    printf("[DEBUG FINDSEG]  Novo mais próximo: segmento %d a %.2f\n", seg->id, dist);
                }
            } else {
                printf("[DEBUG FINDSEG]  Sem interseção com este segmento\n");
            }
        }
        
        percorrerArvore(getNoEsquerda(no));
        percorrerArvore(getNoDireita(no));
    }
    
    percorrerArvore(raiz);
    
    if (mais_proximo) {
        printf("[DEBUG FINDSEG] Segmento mais próximo encontrado: id=%d a dist=%.2f\n", 
               mais_proximo->id, menor_distancia);
    } else {
        printf("[DEBUG FINDSEG] Nenhum segmento intercepta o raio\n");
    }

    return mais_proximo;
}



static void addBigBB(Lista* anteparos, double xmin, double ymin, double xmax, double ymax) {
    char* cor = "#000000";
    
    /* Lado inferior */
    Anteparo a1 = criaAnteparo(9999991, xmin, ymin, xmax, ymin, cor);
    /* Lado direito */
    Anteparo a2 = criaAnteparo(9999992, xmax, ymin, xmax, ymax, cor);
    /* Lado superior */
    Anteparo a3 = criaAnteparo(9999993, xmax, ymax, xmin, ymax, cor);
    /* Lado esquerdo */
    Anteparo a4 = criaAnteparo(9999994, xmin, ymax, xmin, ymin, cor);
    
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

static void* prepararEventosVisibilidade(Ponto origem, Lista* anteparos, int* numEventos) {
    if (!origem || !anteparos) {
        *numEventos = 0;
        return NULL;
    }
    
    int numAnteparos = getTamLista(anteparos);
    printf("[DEBUG VISIBILIDADE] Preparando eventos para %d anteparos\n", numAnteparos);
    
    Evento* eventos = (Evento*)malloc(numAnteparos * 2 * sizeof(Evento));
    if (!eventos) {
        fprintf(stderr, "Erro: falha na alocação de eventos\n");
        *numEventos = 0;
        return NULL;
    }
    
    int count = 0;
    Node* atual = getHeadNode(anteparos);
    int id_counter = 0;
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    
    while (atual) {
        Anteparo a = (Anteparo)getNodeInfo(atual);
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
                double dist1 = calcularDistancia(origem, p1);
                double dist2 = calcularDistancia(origem, p2);
                
                printf("[DEBUG EVENTOS] Anteparo %d: p1(%.2f,%.2f) ang1=%.4f, p2(%.2f,%.2f) ang2=%.4f\n",
                       idAnteparo(a), x1, y1, ang1, x2, y2, ang2);
                
                if (ang1 < ang2 || (fabs(ang1 - ang2) < EPSILON && dist1 < dist2)) {
                
                    eventos[count].ponto = clonaPonto(p1);  
                    eventos[count].angulo = ang1;
                    eventos[count].distancia = dist1;
                    eventos[count].tipo = TIPO_INICIO;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                    
                    eventos[count].ponto = clonaPonto(p2);  
                    eventos[count].angulo = ang2;
                    eventos[count].distancia = dist2;
                    eventos[count].tipo = TIPO_FIM;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                } else {
                    eventos[count].ponto = clonaPonto(p2);  
                    eventos[count].angulo = ang2;
                    eventos[count].distancia = dist2;
                    eventos[count].tipo = TIPO_INICIO;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                    
                    eventos[count].ponto = clonaPonto(p1);  
                    eventos[count].angulo = ang1;
                    eventos[count].distancia = dist1;
                    eventos[count].tipo = TIPO_FIM;
                    eventos[count].anteparo = a;
                    eventos[count].id = id_counter++;
                    count++;
                }
                
                liberaPonto(p1);
                liberaPonto(p2);
            } else {
                if (p1) liberaPonto(p1);
                if (p2) liberaPonto(p2);
            }
        }
        atual = vaiNodeDepois(atual);
    }
    
    *numEventos = count;
    printf("[DEBUG VISIBILIDADE] Total de eventos criados: %d\n", count);
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
    printf("[DEBUG VISIBILIDADE] ===== ENTRANDO EM calculaRegiaoVisivel =====\n");
    
    if (!origem || !anteparos) {
        fprintf(stderr, "Erro: parâmetros inválidos em calculaRegiaoVisivel\n");
        return NULL;
    }
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    printf("[DEBUG VISIBILIDADE] Origem: (%.2f, %.2f)\n", ox, oy);
    printf("[DEBUG VISIBILIDADE] Número de anteparos: %d\n", getTamLista(anteparos));
    printf("[DEBUG VISIBILIDADE] Tipo ordenação: %c, Raio máximo: %.2f\n", tipoOrdenacao, raioMaximo);
    
    Node* atual = getHeadNode(anteparos);
    int anteparo_count = 0;
    while (atual && anteparo_count < 10) {  
        Anteparo a = (Anteparo)getNodeInfo(atual);
        if (a) {
            printf("[DEBUG ANTEPAROS] %d: (%.2f,%.2f)->(%.2f,%.2f) cor=%s\n",
                   idAnteparo(a), getX1Anteparo(a), getY1Anteparo(a),
                   getX2Anteparo(a), getY2Anteparo(a), getCorAnteparo(a));
            anteparo_count++;
        }
        atual = vaiNodeDepois(atual);
    }
    if (anteparo_count >= 10) {
        printf("[DEBUG ANTEPAROS] ... e mais %d anteparos\n", getTamLista(anteparos) - 10);
    }
    
    Lista* todos_anteparos = iniciaLista();
    if (!todos_anteparos) {
        fprintf(stderr, "Erro: falha ao criar lista de anteparos\n");
        return NULL;
    }
    
    atual = getHeadNode(anteparos);
    int clones_criados = 0;
    while (atual) {
        Anteparo a = (Anteparo)getNodeInfo(atual);
        if (a) {
            Anteparo copia = clonaAnteparo(a, idAnteparo(a));
            if (copia) {
                insereTail(todos_anteparos, copia);
                clones_criados++;
            }
        }
        atual = vaiNodeDepois(atual);
    }
    printf("[DEBUG VISIBILIDADE] Clones criados: %d\n", clones_criados);
    
    double MARGEM = 50.0;  
    double xmin = INF, ymin = INF, xmax = -INF, ymax = -INF;
    int formas_encontradas = 0;
    
    atual = getHeadNode(todos_anteparos);
    while (atual) {
        Anteparo a = (Anteparo)getNodeInfo(atual);
        if (a) {
            double x1 = getX1Anteparo(a);
            double y1 = getY1Anteparo(a);
            double x2 = getX2Anteparo(a);
            double y2 = getY2Anteparo(a);
            
            xmin = fmin(xmin, fmin(x1, x2));
            ymin = fmin(ymin, fmin(y1, y2));
            xmax = fmax(xmax, fmax(x1, x2));
            ymax = fmax(ymax, fmax(y1, y2));
            formas_encontradas++;
        }
        atual = vaiNodeDepois(atual);
    }
    
    printf("[DEBUG RETANGULO] Formas encontradas para bounding box: %d\n", formas_encontradas);
    
    if (xmin == INF || ymin == INF || xmax == -INF || ymax == -INF) {
        printf("[DEBUG RETANGULO] Usando raio ao redor da origem\n");
        xmin = ox - raioMaximo;
        ymin = oy - raioMaximo;
        xmax = ox + raioMaximo;
        ymax = oy + raioMaximo;
    }
    
    xmin -= MARGEM;
    ymin -= MARGEM;
    xmax += MARGEM;
    ymax += MARGEM;
    
    printf("[DEBUG RETANGULO] Bounding box: (%.2f,%.2f) -> (%.2f,%.2f) com margem %.1f\n",
           xmin, ymin, xmax, ymax, MARGEM);
    
    addBigBB(todos_anteparos, xmin, ymin, xmax, ymax);
    printf("[DEBUG VISIBILIDADE] Retângulo envolvente adicionado\n");
    
    int numEventos = 0;
    Evento* eventos = (Evento*)prepararEventosVisibilidade(origem, todos_anteparos, &numEventos);
    
    if (!eventos || numEventos == 0) {
        fprintf(stderr, "Erro: não foi possível preparar eventos\n");
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    printf("[DEBUG VISIBILIDADE] Eventos criados: %d\n", numEventos);
    
    for (int i = 0; i < numEventos && i < 5; i++) {
        printf("[DEBUG EVENTOS] %d: tipo=%d, angulo=%.4f, dist=%.2f\n",
               i, eventos[i].tipo, eventos[i].angulo, eventos[i].distancia);
    }
    if (numEventos > 10) {
        printf("[DEBUG EVENTOS] ...\n");
        for (int i = numEventos - 5; i < numEventos; i++) {
            printf("[DEBUG EVENTOS] %d: tipo=%d, angulo=%.4f, dist=%.2f\n",
                   i, eventos[i].tipo, eventos[i].angulo, eventos[i].distancia);
        }
    }
    
    if (tipoOrdenacao == 'q') {
        printf("[DEBUG ORDENACAO] Usando qsort\n");
        qsort(eventos, numEventos, sizeof(Evento), comparaEventosVisibilidade);
    } else {
        printf("[DEBUG ORDENACAO] Usando mergeSort\n");
        mergeSort(eventos, numEventos, sizeof(Evento), 
                  (ComparaFunc)comparaEventosVisibilidade, NULL, threshold);
    }
    
    printf("[DEBUG ORDENACAO] Eventos ordenados\n");
    
    Arvore* segAtivos = iniciarArvore(comparaSegAtivos);
    if (!segAtivos) {
        fprintf(stderr, "Erro: falha ao criar árvore AVL\n");
        free(eventos);
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    printf("[DEBUG VISIBILIDADE] Árvore AVL criada\n");
    
    Poligono poligono = criaPoligonoVazio();
    if (!poligono) {
        fprintf(stderr, "Erro: falha ao criar polígono\n");
        freeArvore(segAtivos, NULL);
        free(eventos);
        freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
        return NULL;
    }
    
    printf("[DEBUG VISIBILIDADE] Polígono vazio criado\n");
    
    SegmentoAtivo* biombo_atual = NULL;
    Ponto ponto_biombo = NULL;
    int eventos_processados = 0;
    
    for (int i = 0; i < numEventos; i++) {
        Evento* ev = &eventos[i];
        eventos_processados++;
        
        printf("[DEBUG PROCESSAMENTO] Evento %d/%d: tipo=%d, angulo=%.4f, dist=%.2f\n",
               i+1, numEventos, ev->tipo, ev->angulo, ev->distancia);
        
        if (ev->tipo == TIPO_INICIO) {
            printf("[DEBUG PROCESSAMENTO]  INÍCIO de segmento\n");
            
            SegmentoAtivo* seg = (SegmentoAtivo*)malloc(sizeof(SegmentoAtivo));
            if (seg) {
                seg->anteparo = ev->anteparo;
                seg->inicio = clonaPonto(ev->ponto);
                
                double x1 = getX1Anteparo(ev->anteparo);
                double y1 = getY1Anteparo(ev->anteparo);
                double x2 = getX2Anteparo(ev->anteparo);
                double y2 = getY2Anteparo(ev->anteparo);
                
                Ponto p_inicio = ev->ponto;
                double px = getXPonto(p_inicio);
                double py = getYPonto(p_inicio);
                
                if (fabs(px - x1) < EPSILON && fabs(py - y1) < EPSILON) {
                    seg->fim = criaPonto(x2, y2);
                } else {
                    seg->fim = criaPonto(x1, y1);
                }
                
                seg->anguloInicio = ev->angulo;
                seg->anguloFim = calcularAngulo(origem, seg->fim);
                seg->ativo = 1;
                seg->id = ev->id;
                
                printf("[DEBUG SEGMENTO] Novo segmento ativo: id=%d, ang=[%.4f,%.4f]\n",
                       seg->id, seg->anguloInicio, seg->anguloFim);
                
                NoArv* no_inserido = insereNo(segAtivos, seg);
                if (no_inserido) {
                    printf("[DEBUG SEGMENTO] Segmento inserido na árvore\n");
                } else {
                    printf("[DEBUG SEGMENTO] ERRO: Falha ao inserir segmento\n");
                }
                
                int atras = atrasSegmento(origem, ev->ponto, biombo_atual);
                printf("[DEBUG BIOMBO] Ponto está atrás do biombo? %d\n", atras);
                
                if (!atras) {
                    printf("[DEBUG BIOMBO] Ponto NÃO está atrás, processando...\n");
                    
                    if (ponto_biombo) {
                        Ponto interseccao = raioAteAnteparo(segAtivos, origem, ev->angulo, raioMaximo);
                        if (interseccao) {
                            printf("[DEBUG POLIGONO] Adicionando vértices: biombo->interseccao\n");
                            adicionaVerticePoligono(poligono, ponto_biombo);
                            adicionaVerticePoligono(poligono, interseccao);
                            liberaPonto(interseccao);
                        }
                        
                        printf("[DEBUG POLIGONO] Adicionando vértice: interseccao->evento\n");
                        adicionaVerticePoligono(poligono, ev->ponto);
                        
                        liberaPonto(ponto_biombo);
                        ponto_biombo = clonaPonto(ev->ponto);
                        biombo_atual = seg;
                        
                        printf("[DEBUG BIOMBO] Novo biombo definido no ponto do evento\n");
                    } else {
                        printf("[DEBUG BIOMBO] Primeiro biombo definido\n");
                        ponto_biombo = clonaPonto(ev->ponto);
                        biombo_atual = seg;
                    }
                } else {
                    printf("[DEBUG BIOMBO] Ponto está atrás, ignorando\n");
                }
            } else {
                printf("[DEBUG PROCESSAMENTO] ERRO: Falha ao alocar segmento ativo\n");
            }
            
        } else if (ev->tipo == TIPO_FIM) {
    printf("[DEBUG PROCESSAMENTO]  FIM de segmento (id=%d)\n", ev->id);
    
    SegmentoAtivo* chave_busca = (SegmentoAtivo*)malloc(sizeof(SegmentoAtivo));
    if (!chave_busca) {
        printf("[DEBUG SEGMENTO] ERRO: Falha ao alocar chave de busca\n");
        continue;
    }
    
    memset(chave_busca, 0, sizeof(SegmentoAtivo));
    chave_busca->id = ev->id;
    
    NoArv* no = buscaBinaria(segAtivos, chave_busca);
    free(chave_busca);  
    
    if (no) {
        SegmentoAtivo* seg = (SegmentoAtivo*)getNoInfo(no);
        printf("[DEBUG SEGMENTO] Segmento encontrado: id=%d\n", seg->id);
        
        if (seg == biombo_atual) {
            printf("[DEBUG BIOMBO] Removendo biombo atual\n");
            
            if (ponto_biombo) {
                printf("[DEBUG POLIGONO] Adicionando vértices: biombo->evento\n");
                adicionaVerticePoligono(poligono, ponto_biombo);
                adicionaVerticePoligono(poligono, ev->ponto);
                
                liberaPonto(ponto_biombo);
                ponto_biombo = raioAteAnteparo(segAtivos, origem, ev->angulo, raioMaximo);
                
                biombo_atual = findSegPerto(segAtivos, origem, ev->angulo);
                if (biombo_atual) {
                    printf("[DEBUG BIOMBO] Novo biombo encontrado: id=%d\n", biombo_atual->id);
                } else {
                    printf("[DEBUG BIOMBO] Nenhum biombo encontrado\n");
                }
            }
        }
        
        printf("[DEBUG SEGMENTO] Removendo segmento da árvore\n");
        removeNo(segAtivos, seg);
        printf("[DEBUG SEGMENTO] Segmento marcado para remoção\n");
    } else {
        printf("[DEBUG SEGMENTO] AVISO: Segmento não encontrado na árvore (id=%d)\n", ev->id);
    }
}
        
        int num_vertices = getNumVertices(poligono);
        printf("[DEBUG POLIGONO] Vértices após evento %d: %d\n", i+1, num_vertices);
    }
    
    printf("[DEBUG VISIBILIDADE] Eventos processados: %d/%d\n", eventos_processados, numEventos);
    
    int num_vertices = getNumVertices(poligono);
    if (num_vertices > 0) {
        printf("[DEBUG POLIGONO] Fechando polígono com %d vértices\n", num_vertices);
        Ponto primeiro = getVertice(poligono, 0);
        if (primeiro) {
            adicionaVerticePoligono(poligono, primeiro);
            liberaPonto(primeiro);
            printf("[DEBUG POLIGONO] Polígono fechado, agora com %d vértices\n", getNumVertices(poligono));
        }
    } else {
        printf("[DEBUG POLIGONO] Polígono ainda vazio após processar todos os eventos\n");
    }
    
    printf("[DEBUG VISIBILIDADE] Liberando eventos...\n");
    for (int i = 0; i < numEventos; i++) {
        if (eventos[i].ponto) {
            liberaPonto(eventos[i].ponto);
        }
    }
    free(eventos);
    printf("[DEBUG VISIBILIDADE] Eventos liberados\n");
    
    if (ponto_biombo) {
        liberaPonto(ponto_biombo);
        printf("[DEBUG VISIBILIDADE] Ponto do biombo liberado\n");
    }
    
    printf("[DEBUG VISIBILIDADE] Liberando árvore AVL...\n");
    freeArvore(segAtivos, NULL);
    
    printf("[DEBUG VISIBILIDADE] Liberando lista de anteparos...\n");
    freeLista(todos_anteparos, (void (*)(void*))liberaAnteparo);
    
    printf("[DEBUG VISIBILIDADE] ===== SAINDO DE calculaRegiaoVisivel =====\n");
    printf("[DEBUG VISIBILIDADE] Polígono final criado com %d vértices\n", getNumVertices(poligono));
    
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
