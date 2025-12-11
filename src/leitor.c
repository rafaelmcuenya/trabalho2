#include "leitor.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "anteparo.h"
#include "pintura.h"
#include "strdupi.h"
#include "criarTxt.h"
#include "criarSvg.h"
#include "trataNomeArquivo.h"
#include "lista.h"
#include "visibilidade.h"
#include "poligono.h"
#include "ponto.h"
#include "arvore.h"
#include "sort.h"

#define PATH_LEN 256

static Lista* formas = NULL;        
static Lista* anteparos = NULL;    

static int totalInstrucoes = 0;
static int totalFormasCriadas = 0;
static int totalFormasDestruidas = 0;
static int totalFormasPintadas = 0;
static int totalFormasClonadas = 0;
static int totalAnteparosCriados = 0;

static char currentFontFamily[32] = "sans";
static char currentFontWeight[8] = "n";
static double currentFontSize = 12.0;

static void construirCaminhoCompleto(const char* baseDir, const char* arquivo, char* caminhoCompleto) {
    if (!baseDir || strlen(baseDir) == 0) {
        strcpy(caminhoCompleto, arquivo);
        return;
    }
    
    if (baseDir[strlen(baseDir)-1] == '/') {
        snprintf(caminhoCompleto, PATH_LEN, "%s%s", baseDir, arquivo);
    } else {
        snprintf(caminhoCompleto, PATH_LEN, "%s/%s", baseDir, arquivo);
    }
}

static int comparaFormasPorId(void* a, void* b) {
    Forma f1 = (Forma)a;
    Forma f2 = (Forma)b;
    int id1 = getIdForma(f1);
    int id2 = getIdForma(f2);
    return id1 - id2;
}

static Forma buscaFormaPorId(Lista* lista, int id) {
    if (!lista || estaVazia(lista)) return NULL;
    
    Node* atual = getHeadNode(lista);
    while (atual) {
        Forma f = (Forma)getNodeInfo(atual);
        if (f && getIdForma(f) == id) {
            return f;
        }
        atual = vaiNodeDepois(atual);
    }
    return NULL;
}

static void removeFormaPorId(Lista* lista, int id, void (*libera)(void*)) {
    if (!lista || estaVazia(lista)) return;
    
    Node* atual = getHeadNode(lista);
    Node* anterior = NULL;
    
    while (atual) {
        Forma f = (Forma)getNodeInfo(atual);
        if (f && getIdForma(f) == id) {
            Node* proximo = vaiNodeDepois(atual);
            
            if (anterior == NULL) {
                removeHead(lista);
            } else {
                Lista* temp = iniciaLista();
                Node* corrente = getHeadNode(lista);
                
                while (corrente) {
                    if (corrente != atual) {
                        insereTail(temp, getNodeInfo(corrente));
                    }
                    corrente = vaiNodeDepois(corrente);
                }
                
                limpaLista(lista, NULL);
                corrente = getHeadNode(temp);
                while (corrente) {
                    insereTail(lista, getNodeInfo(corrente));
                    corrente = vaiNodeDepois(corrente);
                }
                
                freeLista(temp, NULL);
            }
            
            if (libera) {
                libera(f);
            }
            
            break;
        }
        
        anterior = atual;
        atual = vaiNodeDepois(atual);
    }
}

static int removeFormasPorPredicado(Lista* lista, int (*predicado)(void* data), void (*libera)(void*)) {
    if (!lista || estaVazia(lista)) return 0;
    
    int removidas = 0;
    
    Lista* temp = iniciaLista();
    Node* atual = getHeadNode(lista);
    
    while (atual) {
        void* data = getNodeInfo(atual);
        if (data && (!predicado || !predicado(data))) {
            insereTail(temp, data);
        } else {
            removidas++;
            if (libera) {
                libera(data);
            }
        }
        atual = vaiNodeDepois(atual);
    }
    
    limpaLista(lista, NULL);
    atual = getHeadNode(temp);
    while (atual) {
        insereTail(lista, getNodeInfo(atual));
        atual = vaiNodeDepois(atual);
    }
    
    freeLista(temp, NULL);
    return removidas;
}

static int formaTemId(void* data, int idAlvo) {
    Forma f = (Forma)data;
    return (f && getIdForma(f) == idAlvo) ? 1 : 0;
}

static void cmdCriaCirculo(int id, double x, double y, double r, char corb[], char corp[]) {
    totalInstrucoes++;
    Circulo circulo = criaCirculo(id, x, y, r, corb, corp);
    if (circulo && formas) {
        Forma forma = criaForma(Tc, circulo);
        insereTail(formas, forma);
        totalFormasCriadas++;
        printf("[GEO] Círculo %d criado em (%.2f, %.2f) raio %.2f\n", id, x, y, r);
    }
}

static void cmdCriaRetangulo(int id, double x, double y, double w, double h, char corb[], char corp[]) {
    totalInstrucoes++;
    Retangulo retangulo = criaRetangulo(id, x, y, w, h, corb, corp);
    if (retangulo && formas) {
        Forma forma = criaForma(Tr, retangulo);
        insereTail(formas, forma);
        totalFormasCriadas++;
        printf("[GEO] Retângulo %d criado em (%.2f, %.2f) dim %.2fx%.2f\n", id, x, y, w, h);
    }
}

static void cmdCriaLinha(int id, double x1, double y1, double x2, double y2, char cor[]) {
    totalInstrucoes++;
    Linha linha = criaLinha(id, x1, y1, x2, y2, cor);
    if (linha && formas) {
        Forma forma = criaForma(Tl, linha);
        insereTail(formas, forma);
        totalFormasCriadas++;
        printf("[GEO] Linha %d criada de (%.2f,%.2f) até (%.2f,%.2f)\n", id, x1, y1, x2, y2);
    }
}

static void cmdCriaTexto(int id, double x, double y, char corb[], char corp[], char anchor, char texto[]) {
    totalInstrucoes++;
    Texto textoObj = criaTexto(id, x, y, corb, corp, anchor, texto);
    if (textoObj && formas) {
        aplicaStyleTexto(textoObj, currentFontFamily, currentFontWeight, currentFontSize);
        Forma forma = criaForma(Tt, textoObj);
        insereTail(formas, forma);
        totalFormasCriadas++;
        printf("[GEO] Texto %d: '%s' em (%.2f,%.2f) anchor=%c\n", id, texto, x, y, anchor);
    }
}

static void cmdTextoStyle(char family[], char weight[], double size) {
    totalInstrucoes++;
    if (family) strcpy(currentFontFamily, family);
    if (weight) strcpy(currentFontWeight, weight);
    currentFontSize = size;
    printf("[GEO] Estilo texto: family=%s weight=%s size=%.1f\n", 
           currentFontFamily, currentFontWeight, currentFontSize);
}

static void cmdTransformaAnteparo(int i, int j, char direcao) {
    totalInstrucoes++;
    
    if (!formas || !anteparos) {
        printf("[ERRO] Sistema não inicializado\n");
        return;
    }
    
    printf("[QRY] Transformando formas %d a %d em anteparos (direção: %c)\n", i, j, direcao);
    
    for (int id = i; id <= j; id++) {
        Forma forma = buscaFormaPorId(formas, id);
        
        if (forma) {
            TipoForma tipo = getTipoForma(forma);
            Lista* listaAnteparosLocais = iniciaLista();
            int sucesso = 0;
            
            switch (tipo) {
                case Tc: {
                    Circulo c = getCirculoFromForma(forma);
                    sucesso = pintaCirculo(c, direcao, id * 1000, listaAnteparosLocais);
                    break;
                }
                case Tr: {
                    Retangulo r = getRetanguloFromForma(forma);
                    int ids[4] = {id*1000+1, id*1000+2, id*1000+3, id*1000+4};
                    sucesso = pintaRetangulo(r, ids, listaAnteparosLocais);
                    break;
                }
                case Tl: {
                    Linha l = getLinhaFromForma(forma);
                    sucesso = pintaLinha(l, id*1000, listaAnteparosLocais);
                    break;
                }
                case Tt: {
                    Texto t = getTextoFromForma(forma);
                    sucesso = pintaTexto(t, id*1000, listaAnteparosLocais);
                    break;
                }
                default:
                    printf("[AVISO] Tipo de forma não suportado: %d\n", tipo);
                    break;
            }
            
            if (sucesso && getTamLista(listaAnteparosLocais) > 0) {
    
              Node* atual = getHeadNode(listaAnteparosLocais);
              while (atual) {
                  Anteparo a = (Anteparo)getNodeInfo(atual);
                  if (a) {
                    insereTail(anteparos, a);
                    totalAnteparosCriados++;
            
                    txtA(id, id, direcao, forma, a);  
                  }
                  atual = vaiNodeDepois(atual);
               }
    
               removeFormaPorId(formas, id, (void (*)(void*))freeForma);
               totalFormasDestruidas++;
              } else {
                printf("[AVISO] Falha ao transformar forma %d em anteparo\n", id);
            }
            
            freeLista(listaAnteparosLocais, NULL);
        } else {
            printf("[AVISO] Forma com ID %d não encontrada\n", id);
        }
    }
}

typedef struct {
    Poligono regiao;
    Poligono regiao_clone;  
} ContextoPredicado;

static int predicadoFormaDentroPoligono(void* data, void* contexto) {
    Forma f = (Forma)data;
    ContextoPredicado* ctx = (ContextoPredicado*)contexto;
    
    if (!f || !ctx) return 0;
    
    Poligono regiao_usar = ctx->regiao_clone ? ctx->regiao_clone : ctx->regiao;
    
    if (!regiao_usar) return 0;
    
    Ponto centro = criaPonto(getXForma(f), getYForma(f));
    int resultado = pontoDentroPoligono(regiao_usar, centro);
    liberaPonto(centro);
    
    return resultado;
}


static void cmdBombaDestruicao(double x, double y, const char* sfx) {
    totalInstrucoes++;
    
    if (!formas || !anteparos) {
        printf("[ERRO] Sistema não inicializado\n");
        return;
    }
    
    printf("[QRY] Bomba de destruição em (%.2f, %.2f) sufixo: %s\n", x, y, sfx);
    
    Ponto origem = criaPonto(x, y);
    printf("[DEBUG] Ponto origem criado: (%.2f, %.2f)\n", getXPonto(origem), getYPonto(origem));
    
    Poligono regiaoVisivel = calculaRegiaoVisivel(origem, anteparos, 'q', 1000.0, 10);
    
    if (!regiaoVisivel) {
        printf("[ERRO] Não foi possível calcular região visível\n");
        liberaPonto(origem);
        return;
    }
    
    printf("[DEBUG BOMBA DESTRUICAO] Região visível criada com %d vértices\n", 
       getNumVertices(regiaoVisivel));

if (regiaoVisivel) {
    int num_verts = getNumVertices(regiaoVisivel);
    printf("[DEBUG BOMBA DESTRUICAO] Acessando vértices do polígono...\n");
    
    for (int i = 0; i < num_verts && i < 3; i++) {  
        printf("[DEBUG BOMBA DESTRUICAO] Tentando acessar vértice %d...\n", i);
        Ponto v = getVertice(regiaoVisivel, i);
        if (v) {
            printf("[DEBUG BOMBA DESTRUICAO] Vértice %d: (%.2f, %.2f)\n", 
                   i, getXPonto(v), getYPonto(v));
            liberaPonto(v);
        } else {
            printf("[DEBUG BOMBA DESTRUICAO] ERRO: Vértice %d é NULL!\n", i);
        }
    }
}
    
    Lista* formasDestruidas = iniciaLista();
    
    Node* atual = getHeadNode(formas);
    while (atual) {
        Forma forma = (Forma)getNodeInfo(atual);
        Node* proximo = vaiNodeDepois(atual); 
        
        if (forma) {
            Ponto centro = criaPonto(getXForma(forma), getYForma(forma));
            
            if (pontoDentroPoligono(regiaoVisivel, centro)) {
                insereTail(formasDestruidas, forma);
            }
            
            liberaPonto(centro);
        }
        atual = proximo;
    }
    
    ContextoPredicado ctx;
ctx.regiao = regiaoVisivel;
ctx.regiao_clone = criaPoligonoDeLista(getVertices(regiaoVisivel));  // CLONE

totalFormasDestruidas += removeFormasPorPredicado(formas, 
    (int (*)(void*))predicadoFormaDentroPoligono, 
    (void (*)(void*))freeForma, &ctx);  
    
if (ctx.regiao_clone) {
    liberaPoligono(ctx.regiao_clone);
}
    
    txtD(x, y, sfx, formasDestruidas, regiaoVisivel);
    
    if (strcmp(sfx, "-") != 0) {
        printf("[SVG] Gerando arquivo com sufixo %s \n", sfx);
    }
    
    liberaPonto(origem);
    liberaPoligono(regiaoVisivel);
    freeLista(formasDestruidas, NULL);
}

static void cmdBombaPintura(double x, double y, const char* cor, const char* sfx) {
    totalInstrucoes++;
    
    if (!formas || !anteparos) {
        printf("[ERRO] Sistema não inicializado\n");
        return;
    }
    
    printf("[QRY] Bomba de pintura em (%.2f, %.2f) cor: %s sufixo: %s\n", x, y, cor, sfx);
    
    Ponto origem = criaPonto(x, y);
    Poligono regiaoVisivel = calculaRegiaoVisivel(origem, anteparos, 'q', 1000.0, 10);
    
    if (!regiaoVisivel) {
        printf("[ERRO] Não foi possível calcular região visível\n");
        liberaPonto(origem);
        return;
    }
    
    Lista* formasPintadas = iniciaLista();
    
    Node* atual = getHeadNode(formas);
    while (atual) {
        Forma forma = (Forma)getNodeInfo(atual);
        
        if (forma) {
            Ponto centro = criaPonto(getXForma(forma), getYForma(forma));
            
            if (pontoDentroPoligono(regiaoVisivel, centro)) {
                insereTail(formasPintadas, forma);
                totalFormasPintadas++;
                
                TipoForma tipo = getTipoForma(forma);
                char* corComHash = (char*)malloc(strlen(cor) + 2);
                if (corComHash) {
                    if (cor[0] == '#') {
                        strcpy(corComHash, cor);
                    } else {
                        sprintf(corComHash, "#%s", cor);
                    }
                    
                    switch (tipo) {
                        case Tc: {
                            Circulo c = getCirculoFromForma(forma);
                            setCorBCirculo(c, corComHash);
                            setCorPCirculo(c, corComHash);
                            break;
                        }
                        case Tr: {
                            Retangulo r = getRetanguloFromForma(forma);
                            setCorBRetangulo(r, corComHash);
                            setCorPRetangulo(r, corComHash);
                            break;
                        }
                        case Tl: {
                            Linha l = getLinhaFromForma(forma);
                            setCorLinha(l, corComHash);
                            break;
                        }
                        case Tt: {
                            Texto t = getTextoFromForma(forma);
                            setCorBTexto(t, corComHash);
                            setCorPTexto(t, corComHash);
                            break;
                        }
                        default:
                            break;
                    }
                    
                    free(corComHash);
                }
            }
            
            liberaPonto(centro);
        }
        atual = vaiNodeDepois(atual);
    }
    
    txtP(x, y, cor, sfx, formasPintadas, regiaoVisivel);
    
    if (strcmp(sfx, "-") != 0) {
        printf("[SVG] Gerando arquivo de pintura com sufixo %s (implementação pendente)\n", sfx);
    }
    
    liberaPonto(origem);
    liberaPoligono(regiaoVisivel);
    freeLista(formasPintadas, NULL);
}

static void cmdBombaClonagem(double x, double y, double dx, double dy, const char* sfx) {
    totalInstrucoes++;
    
    if (!formas || !anteparos) {
        printf("[ERRO] Sistema não inicializado\n");
        return;
    }
    
    printf("[QRY] Bomba de clonagem em (%.2f, %.2f) deslocamento: (%.2f, %.2f) sufixo: %s\n", 
           x, y, dx, dy, sfx);
    
    Ponto origem = criaPonto(x, y);
    Poligono regiaoVisivel = calculaRegiaoVisivel(origem, anteparos, 'q', 1000.0, 10);
    
    if (!regiaoVisivel) {
        printf("[ERRO] Não foi possível calcular região visível\n");
        liberaPonto(origem);
        return;
    }
    
    Lista* formasOriginais = iniciaLista();
    Lista* clones = iniciaLista();
    
    Node* atual = getHeadNode(formas);
    while (atual) {
        Forma forma = (Forma)getNodeInfo(atual);
        
        if (forma) {
            Ponto centro = criaPonto(getXForma(forma), getYForma(forma));
            
            if (pontoDentroPoligono(regiaoVisivel, centro)) {
                insereTail(formasOriginais, forma);
            }
            
            liberaPonto(centro);
        }
        atual = vaiNodeDepois(atual);
    }
    
    atual = getHeadNode(formasOriginais);
    while (atual) {
        Forma forma = (Forma)getNodeInfo(atual);
        
        if (forma) {
            int idOriginal = getIdForma(forma);
            int idClone = idOriginal * 10000 + totalFormasClonadas + 1;
            TipoForma tipo = getTipoForma(forma);
            Forma clone = NULL;
            
            switch (tipo) {
                case Tc: {
                    Circulo c = getCirculoFromForma(forma);
                    Circulo cClone = clonaCirculo(c);
                    if (cClone) {
                        moveCirculo(cClone, dx, dy);
                        clone = criaForma(Tc, cClone);
                    }
                    break;
                }
                case Tr: {
                    Retangulo r = getRetanguloFromForma(forma);
                    Retangulo rClone = clonaRetangulo(r, idClone);
                    if (rClone) {
                        moveRetangulo(rClone, dx, dy);
                        clone = criaForma(Tr, rClone);
                    }
                    break;
                }
                case Tl: {
                    Linha l = getLinhaFromForma(forma);
                    Linha lClone = clonaLinha(l, idClone);
                    if (lClone) {
                        moveLinha(lClone, dx, dy);
                        clone = criaForma(Tl, lClone);
                    }
                    break;
                }
                case Tt: {
                    Texto t = getTextoFromForma(forma);
                    Texto tClone = clonaTexto(t, idClone);
                    if (tClone) {
                        moveTexto(tClone, dx, dy);
                        clone = criaForma(Tt, tClone);
                    }
                    break;
                }
                default:
                    break;
            }
            
            if (clone) {
                insereTail(clones, clone);
                insereTail(formas, clone); 
                totalFormasClonadas++;
                totalFormasCriadas++;
            }
        }
        atual = vaiNodeDepois(atual);
    }
    
    txtCln(x, y, dx, dy, sfx, formasOriginais, clones);

    if (strcmp(sfx, "-") != 0) {
        printf("[SVG] Gerando arquivo de clonagem com sufixo %s (implementação pendente)\n", sfx);
    }
    
    liberaPonto(origem);
    liberaPoligono(regiaoVisivel);
    freeLista(formasOriginais, NULL);
    freeLista(clones, NULL);
}

void inicializarSistema(void) {
    formas = iniciaLista();
    anteparos = iniciaLista();
    
    printf("[SISTEMA] Sistema inicializado\n");
}

void finalizarSistema(void) {
    if (formas) {
        freeLista(formas, (void (*)(void*))freeForma);
    }
    
    if (anteparos) {
        freeLista(anteparos, (void (*)(void*))liberaAnteparo);
    }
    
    printf("[SISTEMA] Sistema finalizado\n");
    printf("[ESTATÍSTICAS] Instruções: %d, Formas criadas: %d, Destruídas: %d, Pintadas: %d, Clonadas: %d, Anteparos: %d\n",
           totalInstrucoes, totalFormasCriadas, totalFormasDestruidas, totalFormasPintadas, totalFormasClonadas, totalAnteparosCriados);
}

void abrirArquivo(FILE **f, const char *caminho) {
    *f = fopen(caminho, "r");
    if (!(*f)) {
        fprintf(stderr, "Erro: Não foi possível abrir %s\n", caminho);
        exit(1);
    }
}

void processarComando(const char* linha, int ehQry, const char* nomeBase, const char* outputDir) {
    if (linha[0] == '\n' || linha[0] == '#' || linha[0] == '\0') return;
    
    char comando[10];
    sscanf(linha, "%s", comando);
    
    if (!ehQry) {
        if (strcmp(comando, "c") == 0) {
            int id; double x, y, r; char corb[32], corp[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %31s %31s", &id, &x, &y, &r, corb, corp) == 6) {
                cmdCriaCirculo(id, x, y, r, corb, corp);
            }
        }
        else if (strcmp(comando, "r") == 0) {
            int id; double x, y, w, h; char corb[32], corp[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %lf %31s %31s", &id, &x, &y, &w, &h, corb, corp) == 7) {
                cmdCriaRetangulo(id, x, y, w, h, corb, corp);
            }
        }
        else if (strcmp(comando, "l") == 0) {
            int id; double x1, y1, x2, y2; char cor[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %lf %31s", &id, &x1, &y1, &x2, &y2, cor) == 6) {
                cmdCriaLinha(id, x1, y1, x2, y2, cor);
            }
        }
        else if (strcmp(comando, "t") == 0) {
            int id; double x, y; char corb[32], corp[32], anchor, texto[256];
            if (sscanf(linha, "%*s %d %lf %lf %31s %31s %c %255[^\n]", &id, &x, &y, corb, corp, &anchor, texto) == 7) {
                cmdCriaTexto(id, x, y, corb, corp, anchor, texto);
            }
        }
        else if (strcmp(comando, "ts") == 0) {
            char family[32], weight[8]; double size;
            if (sscanf(linha, "%*s %31s %7s %lf", family, weight, &size) == 3) {
                cmdTextoStyle(family, weight, size);
            }
        }
    } else {
        txtComandoQry(linha);
        
        if (strcmp(comando, "a") == 0) {
            int i, j; char direcao;
            if (sscanf(linha, "%*s %d %d %c", &i, &j, &direcao) == 3) {
                cmdTransformaAnteparo(i, j, direcao);
            }
        }
        else if (strcmp(comando, "d") == 0) {
            double x, y; char sfx[32];
            if (sscanf(linha, "%*s %lf %lf %31s", &x, &y, sfx) == 3) {
                cmdBombaDestruicao(x, y, sfx);
            }
        }
        else if (strcmp(comando, "p") == 0) {
            double x, y; char cor[32], sfx[32];
            if (sscanf(linha, "%*s %lf %lf %31s %31s", &x, &y, cor, sfx) == 4) {
                cmdBombaPintura(x, y, cor, sfx);
            }
        }
        else if (strcmp(comando, "cln") == 0) {
            double x, y, dx, dy; char sfx[32];
            if (sscanf(linha, "%*s %lf %lf %lf %lf %31s", &x, &y, &dx, &dy, sfx) == 5) {
                cmdBombaClonagem(x, y, dx, dy, sfx);
            }
        }
        
        txtSeparador(); 
    }
}

void processarArquivo(const char* caminho, const char* inputDir, int ehQry, const char* nomeBase, const char* outputDir) {
    char caminhoCompleto[PATH_LEN];
    construirCaminhoCompleto(inputDir, caminho, caminhoCompleto);
    
    FILE *f;
    abrirArquivo(&f, caminhoCompleto);
    
    if (ehQry) {
        char nomeBaseQry[FILE_NAME_LEN];
        extrairNomeBase(caminho, nomeBaseQry);
        
        char caminhoTxt[PATH_LEN];
        gerarNomeQryTxt(nomeBase, nomeBaseQry, outputDir, caminhoTxt);
        iniciarTxt(caminhoTxt);
        
        txtComandoQry("=== Início do processamento do arquivo QRY ===");
        txtSeparador();
    }
    
    char linha[1024];
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = 0;
        processarComando(linha, ehQry, nomeBase, outputDir);
    }
    
    if (ehQry) {
        int totalFormasAtuais = getTamLista(formas);
        txtFinalRelatorio(totalFormasAtuais, totalFormasDestruidas, totalFormasPintadas, totalFormasClonadas, totalAnteparosCriados, 0.0);
        fecharTxt();
        
        char nomeBaseQry[FILE_NAME_LEN];
        extrairNomeBase(caminho, nomeBaseQry);
        
        char caminhoSvgFinal[PATH_LEN];
        gerarNomeQrySvg(nomeBase, nomeBaseQry, outputDir, caminhoSvgFinal);
        
 
        printf("[SVG] Arquivo final com consulta gerado: %s\n", caminhoSvgFinal);
    } else {
        char caminhoSvg[PATH_LEN];
        gerarNomeGeoSvg(nomeBase, outputDir, caminhoSvg);
        printf("[SVG] Arquivo base gerado: %s\n", caminhoSvg);
    }
    
    fclose(f);
}
