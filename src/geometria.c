#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include "geometria.h"
#include "ponto.h"
#include "anteparo.h"

#define EPSILON 1e-12
#define PI 3.14159265358979323846

typedef struct {
    double xmin, ymin, xmax, ymax;
} BoundingBoxStruct;

typedef struct {
    double x, y;
} Vetor2DStruct;

static inline double max_d(double a, double b) { return (a > b) ? a : b; }
static inline double min_d(double a, double b) { return (a < b) ? a : b; }

static double produto_vetorial_2d(double ax, double ay, double bx, double by, double cx, double cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

static bool entre(double valor, double limite1, double limite2, double epsilon) {
    double min_val = min_d(limite1, limite2) - epsilon;
    double max_val = max_d(limite1, limite2) + epsilon;
    return (valor >= min_val && valor <= max_val);
}

BoundingBox criaBoundingBox(double xmin, double ymin, double xmax, double ymax) {
    BoundingBoxStruct* bb = (BoundingBoxStruct*)malloc(sizeof(BoundingBoxStruct));
    if (!bb) {
        fprintf(stderr, "Erro: falha na alocação da bounding box\n");
        return NULL;
    }
    
    bb->xmin = min_d(xmin, xmax);
    bb->ymin = min_d(ymin, ymax);
    bb->xmax = max_d(xmin, xmax);
    bb->ymax = max_d(ymin, ymax);
    
    return (BoundingBox)bb;
}

void liberaBoundingBox(BoundingBox bb) {
    if (bb) {
        free(bb);
    }
}

double distanciaEntrePontos(Ponto p1, Ponto p2) {
    if (!p1 || !p2) {
        fprintf(stderr, "Erro: pontos NULL em distanciaEntrePontos\n");
        return -1.0;
    }
    
    double x1 = getXPonto(p1);
    double y1 = getYPonto(p1);
    double x2 = getXPonto(p2);
    double y2 = getYPonto(p2);
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    return sqrt(dx*dx + dy*dy);
}

double anguloPontoRelativo(Ponto origem, Ponto p) {
    if (!origem || !p) {
        fprintf(stderr, "Erro: pontos NULL em anguloPontoRelativo\n");
        return -1.0;
    }
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    double px = getXPonto(p);
    double py = getYPonto(p);
    
    double dx = px - ox;
    double dy = py - oy;
    double angulo = atan2(dy, dx);

    if (angulo < 0) {
        angulo += 2 * PI;
    }
    
    return angulo;
}

double distanciaPontoAnteparo(Ponto p, Anteparo a) {
    if (!p || !a) {
        fprintf(stderr, "Erro: parâmetros NULL em distanciaPontoAnteparo\n");
        return -1.0;
    }
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    if (fabs(dx) < EPSILON && fabs(dy) < EPSILON) {
        double ptx = px - x1;
        double pty = py - y1;
        return sqrt(ptx*ptx + pty*pty);
    }
    
    double t = ((px - x1) * dx + (py - y1) * dy) / (dx*dx + dy*dy);
    
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    
    double proj_x = x1 + t * dx;
    double proj_y = y1 + t * dy;
    
    double dist_x = px - proj_x;
    double dist_y = py - proj_y;
    
    return sqrt(dist_x*dist_x + dist_y*dist_y);
}

int orientacaoPontos(Ponto a, Ponto b, Ponto c) {
    if (!a || !b || !c) {
        fprintf(stderr, "Erro: pontos NULL em orientacaoPontos\n");
        return -2;
    }
    
    double ax = getXPonto(a);
    double ay = getYPonto(a);
    double bx = getXPonto(b);
    double by = getYPonto(b);
    double cx = getXPonto(c);
    double cy = getYPonto(c);
    
    double cross = produto_vetorial_2d(ax, ay, bx, by, cx, cy);
    
    if (fabs(cross) < EPSILON) {
        return 0;  // Colinear
    } else if (cross > 0) {
        return 1;  // Esquerda 
    } else {
        return -1; // Direita 
    }
}

bool pontoNoSegmento(Ponto p, Anteparo a) {
    if (!p || !a) {
        return false;
    }
    
    double px = getXPonto(p);
    double py = getYPonto(p);
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    
    double cross = produto_vetorial_2d(x1, y1, x2, y2, px, py);
    if (fabs(cross) > EPSILON) {
        return false;
    }
    return (entre(px, x1, x2, EPSILON) && entre(py, y1, y2, EPSILON));
}

bool segmentosSeInterceptam(Anteparo a1, Anteparo a2, Ponto* interseccao) {
    if (!a1 || !a2) {
        return false;
    }
    
    double x1 = getX1Anteparo(a1);
    double y1 = getY1Anteparo(a1);
    double x2 = getX2Anteparo(a1);
    double y2 = getY2Anteparo(a1);
    double x3 = getX1Anteparo(a2);
    double y3 = getY1Anteparo(a2);
    double x4 = getX2Anteparo(a2);
    double y4 = getY2Anteparo(a2);
    
    double xi, yi;
    bool interceptam = interseccaoSegmentosCoordenadas(x1, y1, x2, y2, x3, y3, x4, y4, &xi, &yi);
    
    if (interceptam && interseccao) {
        *interseccao = criaPonto(xi, yi);
    }
    
    return interceptam;
}

bool pontoDentroBoundingBox(Ponto p, BoundingBox bb) {
    if (!p || !bb) {
        return false;
    }
    
    BoundingBoxStruct* bbox = (BoundingBoxStruct*)bb;
    double px = getXPonto(p);
    double py = getYPonto(p);
    
    return (px >= bbox->xmin - EPSILON && px <= bbox->xmax + EPSILON &&
            py >= bbox->ymin - EPSILON && py <= bbox->ymax + EPSILON);
}

bool boundingBoxesSobrepoem(BoundingBox bb1, BoundingBox bb2) {
    if (!bb1 || !bb2) {
        return false;
    }
    
    BoundingBoxStruct* bbox1 = (BoundingBoxStruct*)bb1;
    BoundingBoxStruct* bbox2 = (BoundingBoxStruct*)bb2;
    
    if (bbox1->xmax < bbox2->xmin - EPSILON || bbox2->xmax < bbox1->xmin - EPSILON) {
        return false;
    }
    
    if (bbox1->ymax < bbox2->ymin - EPSILON || bbox2->ymax < bbox1->ymin - EPSILON) {
        return false;
    }
    
    return true;
}

BoundingBox boundingBoxDeAnteparo(Anteparo a) {
    if (!a) {
        return NULL;
    }
    
    double x1 = getX1Anteparo(a);
    double y1 = getY1Anteparo(a);
    double x2 = getX2Anteparo(a);
    double y2 = getY2Anteparo(a);
    
    return criaBoundingBox(min_d(x1, x2), min_d(y1, y2), max_d(x1, x2), max_d(y1, y2));
}

BoundingBox uniaoBoundingBoxes(BoundingBox bb1, BoundingBox bb2) {
    if (!bb1 && !bb2) return NULL;
    if (!bb1) return criaBoundingBox(((BoundingBoxStruct*)bb2)->xmin, ((BoundingBoxStruct*)bb2)->ymin, ((BoundingBoxStruct*)bb2)->xmax, ((BoundingBoxStruct*)bb2)->ymax);
    if (!bb2) return criaBoundingBox(((BoundingBoxStruct*)bb1)->xmin, ((BoundingBoxStruct*)bb1)->ymin, ((BoundingBoxStruct*)bb1)->xmax, ((BoundingBoxStruct*)bb1)->ymax);
    
    BoundingBoxStruct* bbox1 = (BoundingBoxStruct*)bb1;
    BoundingBoxStruct* bbox2 = (BoundingBoxStruct*)bb2;
    
    double xmin = min_d(bbox1->xmin, bbox2->xmin);
    double ymin = min_d(bbox1->ymin, bbox2->ymin);
    double xmax = max_d(bbox1->xmax, bbox2->xmax);
    double ymax = max_d(bbox1->ymax, bbox2->ymax);
    
    return criaBoundingBox(xmin, ymin, xmax, ymax);
}

int orientacaoCoordenadas(double ax, double ay, double bx, double by, double cx, double cy) {
    double cross = produto_vetorial_2d(ax, ay, bx, by, cx, cy);
    
    if (fabs(cross) < EPSILON) {
        return 0;  // Colinear
    } else if (cross > 0) {
        return 1;  // Esquerda
    } else {
        return -1; // Direita
    }
}

bool interseccaoSegmentosCoordenadas(double x1, double y1, double x2, double y2,double x3, double y3,
double x4, double y4, double* xi, double* yi) {
    int o1 = orientacaoCoordenadas(x1, y1, x2, y2, x3, y3);
    int o2 = orientacaoCoordenadas(x1, y1, x2, y2, x4, y4);
    int o3 = orientacaoCoordenadas(x3, y3, x4, y4, x1, y1);
    int o4 = orientacaoCoordenadas(x3, y3, x4, y4, x2, y2);
    
    if (o1 != o2 && o3 != o4) {
        if (xi && yi) {
            double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            
            if (fabs(denom) < EPSILON) {
                return false;
            }
            
            double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
            *xi = x1 + t * (x2 - x1);
            *yi = y1 + t * (y2 - y1);
        }
        return true;
    }
    
    if (o1 == 0 && entre(x3, x1, x2, EPSILON) && entre(y3, y1, y2, EPSILON)) {
        if (xi && yi) { *xi = x3; *yi = y3; }
        return true;
    }
    if (o2 == 0 && entre(x4, x1, x2, EPSILON) && entre(y4, y1, y2, EPSILON)) {
        if (xi && yi) { *xi = x4; *yi = y4; }
        return true;
    }
    if (o3 == 0 && entre(x1, x3, x4, EPSILON) && entre(y1, y3, y4, EPSILON)) {
        if (xi && yi) { *xi = x1; *yi = y1; }
        return true;
    }
    if (o4 == 0 && entre(x2, x3, x4, EPSILON) && entre(y2, y3, y4, EPSILON)) {
        if (xi && yi) { *xi = x2; *yi = y2; }
        return true;
    }
    
    return false;
}

double produtoVetorial2D(double ax, double ay, double bx, double by, double cx, double cy) {
    return produto_vetorial_2d(ax, ay, bx, by, cx, cy);
}

bool pontoVisivelDeOrigem(Ponto origem, Ponto p, Anteparo obstaculo) {
    if (!origem || !p || !obstaculo) {
        return false;
    }
    
    double ox = getXPonto(origem);
    double oy = getYPonto(origem);
    double px = getXPonto(p);
    double py = getYPonto(p);
    double x1 = getX1Anteparo(obstaculo);
    double y1 = getY1Anteparo(obstaculo);
    double x2 = getX2Anteparo(obstaculo);
    double y2 = getY2Anteparo(obstaculo);
    
    double xi, yi;
    if (interseccaoSegmentosCoordenadas(ox, oy, px, py, x1, y1, x2, y2, &xi, &yi)) {
        double dist_origem_int = sqrt(pow(xi - ox, 2) + pow(yi - oy, 2));
        double dist_origem_p = sqrt(pow(px - ox, 2) + pow(py - oy, 2));
      
        if (dist_origem_int < dist_origem_p - EPSILON) {
            return false;
        }
    }
    return true;
}

double distanciaRelativa(Ponto origem, Ponto p1, Ponto p2) {
    if (!origem || !p1 || !p2) {
        return 0.0;
    }
    
    double dist1 = distanciaEntrePontos(origem, p1);
    double dist2 = distanciaEntrePontos(origem, p2);
    
    return dist1 - dist2;
}
