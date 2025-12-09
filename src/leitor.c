#include "leitor.h"
#include <string.h>
#include <ctype.h>
#include "forma.h"
#include "chao.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "strdupi.h"
#include "criarTxt.h"
#include "criarSvg.h"
#include "trataNomeArquivo.h"

static void construirCaminhoCompleto(const char* baseDir, const char* arquivo, char* caminhoCompleto){
    if(!baseDir || strlen(baseDir) == 0){
        strcpy(caminhoCompleto, arquivo);
        return;
    }
    
    if(baseDir[strlen(baseDir)-1] == '/'){
        snprintf(caminhoCompleto, PATH_LEN, "%s%s", baseDir, arquivo);
    }else{
        snprintf(caminhoCompleto, PATH_LEN, "%s/%s", baseDir, arquivo);
    }
}

static Chao chao = NULL;
static Arena arena = NULL;
static Disparador disparadores[100] ={0};
static Carregador carregadores[100] ={0};

static double pontuacaoFinal = 0.0;
static int totalInstrucoes = 0;
static int totalDisparos = 0;
static int totalEsmagadas = 0;
static int totalClonadas = 0;

static char currentFontFamily[32] = "sans";
static char currentFontWeight[8] = "n";
static double currentFontSize = 12.0;

static void cmdCriaCirculo(int id, double x, double y, double r, char corb[], char corp[]){
    totalInstrucoes++;
    Circulo circulo = criaCirculo(id, x, y, r, corb, corp);
    if (circulo && chao){
        Forma forma = criaForma(Tc, circulo);
        inFormaChao(chao, forma);
        printf("[GEO] Círculo %d criado em (%.2f, %.2f) raio %.2f\n", id, x, y, r);
    }
}

static void cmdCriaRetangulo(int id, double x, double y, double w, double h, char corb[], char corp[]){
    totalInstrucoes++;
    Retangulo retangulo = criaRetangulo(id, x, y, w, h, corb, corp);
    if (retangulo && chao){
        Forma forma = criaForma(Tr, retangulo);
        inFormaChao(chao, forma);
        printf("[GEO] Retângulo %d criado em (%.2f, %.2f) dim %.2fx%.2f\n", id, x, y, w, h);
    }
}

static void cmdCriaLinha(int id, double x1, double y1, double x2, double y2, char cor[]){
    totalInstrucoes++;
    Linha linha = criaLinha(id, x1, y1, x2, y2, cor);
    if (linha && chao){
        Forma forma = criaForma(Tl, linha);
        inFormaChao(chao, forma);
        printf("[GEO] Linha %d criada de (%.2f,%.2f) até (%.2f,%.2f)\n", id, x1, y1, x2, y2);
    }
}

static void cmdCriaTexto(int id, double x, double y, char corb[], char corp[], char anchor, char texto[]){
    totalInstrucoes++;
    Texto textoObj = criaTexto(id, x, y, corb, corp, anchor, texto);
    if (textoObj && chao){
        aplicaStyleTexto(textoObj, currentFontFamily, currentFontWeight, currentFontSize);
        Forma forma = criaForma(Tt, textoObj);
        inFormaChao(chao, forma);
        printf("[GEO] Texto %d: '%s' em (%.2f,%.2f) anchor=%c\n", id, texto, x, y, anchor);
    }
}

static void cmdTextoStyle(char family[], char weight[], double size){
    totalInstrucoes++;
    strcpy(currentFontFamily, family);
    strcpy(currentFontWeight, weight);
    currentFontSize = size;
    printf("[GEO] Estilo texto: family=%s weight=%s size=%.1f\n", family, weight, size);
}

static void cmdPD(int idDisp, double x, double y){
    totalInstrucoes++;
    if (idDisp >= 0 && idDisp < 100){
        disparadores[idDisp] = criaDisparador(idDisp, x, y);
        printf("[QRY] Disparador %d posicionado em (%.2f, %.2f)\n", idDisp, x, y);
    }
}

static void cmdATCH(int idDisp, int idCesq, int idCdir){
    totalInstrucoes++;
    
    if (idDisp < 0 || idDisp >= 100 || !disparadores[idDisp]){
        printf("[ERRO] Disparador %d não encontrado\n", idDisp);
        return;
    }

    if (idCesq < 0 || idCesq >= 100){
        printf("[ERRO] ID de carregador esquerdo inválido: %d\n", idCesq);
        return;
    }
    if (!carregadores[idCesq]){
        carregadores[idCesq] = criaCarregador(idCesq);
        printf("[QRY] Carregador %d (esq) criado vazio\n", idCesq);
    }

    if (idCdir < 0 || idCdir >= 100){
        printf("[ERRO] ID de carregador direito inválido: %d\n", idCdir);
        return;
    }
    if (!carregadores[idCdir]){
        carregadores[idCdir] = criaCarregador(idCdir);
        printf("[QRY] Carregador %d (dir) criado vazio\n", idCdir);
    }
    
    encaixaCarregadores(disparadores[idDisp], carregadores[idCesq], carregadores[idCdir]);
    printf("[QRY] Disparador %d: carregadores %d(esq) e %d(dir) encaixados\n", 
           idDisp, idCesq, idCdir);
}

static void cmdLC(int idCarregador, int n){
    totalInstrucoes++;
    if (idCarregador >= 0 && idCarregador < 100){
        if (carregadores[idCarregador]){
            printf("[ERRO] Carregador %d já existe\n", idCarregador);
            return;
        }
        
        carregadores[idCarregador] = criaCarregador(idCarregador);
        if (chao && carregadores[idCarregador]){
            int formasCarregadas = 0;
            for (int i = 0; i < n && !voidChao(chao); i++){
                Forma forma = outFormaChao(chao);
                if (forma){
                    empilhaCarregador(carregadores[idCarregador], forma);
                    formasCarregadas++;
                }
            }
            printf("[QRY] Carregador %d: %d formas carregadas do chão\n", idCarregador, formasCarregadas);
            
            if (formasCarregadas < n){
                printf("[AVISO] Chão tinha apenas %d formas (solicitadas: %d)\n", formasCarregadas, n);
            }
        }
    } else{
        printf("[ERRO] ID de carregador inválido: %d\n", idCarregador);
    }
}

static void cmdSHFT(int idDisp, char botao, int n){
    totalInstrucoes++;
    if (disparadores[idDisp]){
        shftDisparador(disparadores[idDisp], botao, n);
        Forma forma = getPosDisparo(disparadores[idDisp]);
        if (forma){
            printf("[QRY] Shift %c: forma %d em posição de disparo\n", 
                   botao, getIdForma(forma));
        }
    }
}

static void cmdDSP(int idDisp, double dx, double dy, bool svgFlag){
    totalInstrucoes++;
    totalDisparos++;
    
    if (disparadores[idDisp] && arena){
        Forma forma = disparaForma(disparadores[idDisp], dx, dy);
        if (forma){
            insereFormaArena(arena, forma);
            printf("[QRY] Disparo: forma %d em arena com offset (%.2f,%.2f)\n", 
                   getIdForma(forma), dx, dy);
            
            if (svgFlag){
                printf("[SVG] Anotar dimensões do disparo\n");
            }
        }
    }
}

static void cmdRJD(int idDisp, char lado, double dx, double dy, double ix, double iy){
    totalInstrucoes++;
    
    if (idDisp < 0 || idDisp >= 100 || !disparadores[idDisp]){
        printf("[ERRO] Disparador %d não encontrado\n", idDisp);
        return;
    }
    
    if (lado != 'd' && lado != 'e'){
        printf("[ERRO] Lado inválido para rajada: %c\n", lado);
        return;
    }
    
    if (!arena){
        printf("[ERRO] Arena não inicializada\n");
        return;
    }

    printf("[QRY] Iniciando rajada no disparador %d, lado %c\n", idDisp, lado);
    int disparos = 0;
    int maxDisparos = 100;

    if (!possuiCarregadorEsq(disparadores[idDisp]) || !possuiCarregadorDir(disparadores[idDisp])){
        printf("[ERRO] Disparador %d não tem carregadores encaixados\n", idDisp);
        return;
    }

    for (int i = 0; i < maxDisparos; i++){
        shftDisparador(disparadores[idDisp], lado, 1);
        Forma forma = getPosDisparo(disparadores[idDisp]);
    
        if (!forma){
            break;
        }

        double currentDx = dx + i * ix;
        double currentDy = dy + i * iy;
        Forma formaDisparada = disparaForma(disparadores[idDisp], currentDx, currentDy);
        
        if (formaDisparada){
            insereFormaArena(arena, formaDisparada);
            printf("[QRY] Rajada %d: forma %d disparada\n", i, getIdForma(formaDisparada));
            disparos++;
            totalDisparos++;
        } else{
            break;
        }
    }
    printf("[QRY] Rajada concluída: %d disparos realizados\n", disparos);
}

static void cmdCALC(const char* nomeBase, const char* outputDir){
    totalInstrucoes++;
    if (arena){
        printf("[QRY] Calculando sobreposições na arena...\n");
        
        double pontuacaoRound = 0.0;
        int esmagadasRound = 0;
        int clonadasRound = 0;
        processaArena(arena, chao, &pontuacaoRound, &esmagadasRound, &clonadasRound, nomeBase, outputDir);
        pontuacaoFinal += pontuacaoRound;
        totalEsmagadas += esmagadasRound;
        totalClonadas += clonadasRound;
        
        printf("[CALC] Área esmagada neste round: %.2f\n", pontuacaoRound);
        printf("[CALC] Formas esmagadas: %d\n", esmagadasRound);
        printf("[CALC] Formas clonadas: %d\n", clonadasRound);
    }
}

void inicializarSistema(void){
    chao = criaChao();
    arena = criaArena();
    printf("[SISTEMA] Sistema inicializado\n");
}

void finalizarSistema(void){
    if (chao) freeChao(chao);
    if (arena) liberaArena(arena);
    
    for (int i = 0; i < 100; i++){
        if (disparadores[i]) freeDisparador(disparadores[i]);
        if (carregadores[i]) freeCarregador(carregadores[i]);
    }
    printf("[SISTEMA] Sistema finalizado\n");
}

void abrirArquivo(FILE **f, const char *caminho){
    *f = fopen(caminho, "r");
    if (!(*f)){
        fprintf(stderr, "Erro: Não foi possível abrir %s\n", caminho);
        exit(1);
    }
}

void processarComando(const char* linha, int ehQry, const char* nomeBase, const char* outputDir){
    if (linha[0] == '\n' || linha[0] == '#' || linha[0] == '\0') return;
    
    char comando[10];
    sscanf(linha, "%s", comando);
    
    if (!ehQry){
        if (strcmp(comando, "c") == 0){
            int id; double x, y, r; char corb[32], corp[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %31s %31s", &id, &x, &y, &r, corb, corp) == 6){
                cmdCriaCirculo(id, x, y, r, corb, corp);
            }
        }
        else if (strcmp(comando, "r") == 0){
            int id; double x, y, w, h; char corb[32], corp[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %lf %31s %31s", &id, &x, &y, &w, &h, corb, corp) == 7){
                cmdCriaRetangulo(id, x, y, w, h, corb, corp);
            }
        }
        else if (strcmp(comando, "l") == 0){
            int id; double x1, y1, x2, y2; char cor[32];
            if (sscanf(linha, "%*s %d %lf %lf %lf %lf %31s", &id, &x1, &y1, &x2, &y2, cor) == 6){
                cmdCriaLinha(id, x1, y1, x2, y2, cor);
            }
        }
        else if (strcmp(comando, "t") == 0){
            int id; double x, y; char corb[32], corp[32], anchor, texto[256];
            if (sscanf(linha, "%*s %d %lf %lf %31s %31s %c %255[^\n]", &id, &x, &y, corb, corp, &anchor, texto) == 7){
                cmdCriaTexto(id, x, y, corb, corp, anchor, texto);
            }
        }
        else if (strcmp(comando, "ts") == 0){
            char family[32], weight[8]; double size;
            if (sscanf(linha, "%*s %31s %7s %lf", family, weight, &size) == 3){
                cmdTextoStyle(family, weight, size);
            }
        }
    } else{
        if (strcmp(comando, "pd") == 0){
            int id; double x, y;
            if (sscanf(linha, "%*s %d %lf %lf", &id, &x, &y) == 3){
                cmdPD(id, x, y);
            }
        }
        else if (strcmp(comando, "lc") == 0){
            int id, n;
            if (sscanf(linha, "%*s %d %d", &id, &n) == 2){
                cmdLC(id, n);
            }
        }
        else if (strcmp(comando, "atch") == 0){
            int disp, esq, dir;
            if (sscanf(linha, "%*s %d %d %d", &disp, &esq, &dir) == 3){
                cmdATCH(disp, esq, dir);
            }
        }
        else if (strcmp(comando, "shft") == 0){
            int disp, n; char lado;
            if (sscanf(linha, "%*s %d %c %d", &disp, &lado, &n) == 3){
                cmdSHFT(disp, lado, n);
            }
        }
        else if (strcmp(comando, "dsp") == 0){
            int disp; double dx, dy; char flag[2];
            int res = sscanf(linha, "%*s %d %lf %lf %1s", &disp, &dx, &dy, flag);
            cmdDSP(disp, dx, dy, (res == 4 && strcmp(flag, "v") == 0));
        }
        else if (strcmp(comando, "rjd") == 0){
            int disp; char lado; double dx, dy, ix, iy;
            if (sscanf(linha, "%*s %d %c %lf %lf %lf %lf", &disp, &lado, &dx, &dy, &ix, &iy) == 6){
                cmdRJD(disp, lado, dx, dy, ix, iy);
            } else{
                printf("[ERRO] Formato inválido para rjd: %s\n", linha);
            }
        }
        else if (strcmp(comando, "calc") == 0){
            cmdCALC(nomeBase, outputDir);
        }
    }
}

void processarArquivo(const char* caminho, const char* inputDir, int ehQry, const char* nomeBase, const char* outputDir){
    char caminhoCompleto[PATH_LEN];
    construirCaminhoCompleto(inputDir, caminho, caminhoCompleto);
    
    FILE *f;
    abrirArquivo(&f, caminhoCompleto);
    
    if (ehQry){
        char nomeBaseQry[FILE_NAME_LEN];
        extrairNomeBase(caminho, nomeBaseQry);
        
        char caminhoTxt[PATH_LEN];
        gerarNomeQryTxt(nomeBase, nomeBaseQry, outputDir, caminhoTxt);
        iniciarTxt(caminhoTxt);
    }
    
    char linha[1024];
    while (fgets(linha, sizeof(linha), f)){
        processarComando(linha, ehQry, nomeBase, outputDir);
    }
    
    if (ehQry){
        txtFinal(getPontuacaoFinal(), getTotalInstrucoes(), getTotalDisparos(), getTotalEsmagadas(), getTotalClonadas());
        fecharTxt();
    
        char nomeBaseQry[FILE_NAME_LEN];
        extrairNomeBase(caminho, nomeBaseQry);
        
        char caminhoSvgFinal[PATH_LEN];
        gerarNomeQrySvg(nomeBase, nomeBaseQry, outputDir, caminhoSvgFinal);
        
        svgQry(caminhoSvgFinal, chao);
        printf("[SVG] Arquivo final com consulta gerado: %s\n", caminhoSvgFinal);
    } else{
        char caminhoSvg[PATH_LEN];
        gerarNomeGeoSvg(nomeBase, outputDir, caminhoSvg);
        svgGeo(caminhoSvg, chao);
    }
    fclose(f);
}
