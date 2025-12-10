#include "trataNomeArquivo.h"
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

void criarDiretorioRecursivo(const char* path){
    if (!path || strlen(path) == 0) return;
    
    char tmp[PATH_LEN];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    
    if (len > 0 && (tmp[len - 1] == '/' || tmp[len - 1] == '\\')){
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++){
        if (*p == '/' || *p == '\\'){
            *p = 0;
            #ifdef _WIN32
                _mkdir(tmp);
            #else
                mkdir(tmp, 0755);
            #endif
            *p = PATH_SEP;
        }
    }

    #ifdef _WIN32
        _mkdir(tmp);
    #else
        mkdir(tmp, 0755);
    #endif
}

void extrairNomeBase(const char* caminhoCompleto, char* nomeBase){
    if (!caminhoCompleto || !nomeBase) return;
    
    const char* ultimaBarra = strrchr(caminhoCompleto, '/');
    if (!ultimaBarra) ultimaBarra = strrchr(caminhoCompleto, '\\');
    
    const char* inicioNome = (ultimaBarra != NULL) ? ultimaBarra + 1 : caminhoCompleto;
    
    strcpy(nomeBase, inicioNome);
    
    char* ponto = strrchr(nomeBase, '.');
    if (ponto != NULL && (
        strcmp(ponto, ".geo") == 0 || 
        strcmp(ponto, ".qry") == 0 ||
        strcmp(ponto, ".svg") == 0 ||
        strcmp(ponto, ".txt") == 0)) {
        *ponto = '\0';
    }
}

void gerarNomeGeoSvg(const char* nomeBase, const char* outputDir, char* caminhoCompleto){
    if (!nomeBase || !caminhoCompleto) return;
    
    if (outputDir && strlen(outputDir) > 0){
        criarDiretorioRecursivo(outputDir);
        snprintf(caminhoCompleto, PATH_LEN, "%s%c%s.svg", outputDir, PATH_SEP, nomeBase);
    } else {
        snprintf(caminhoCompleto, PATH_LEN, "%s.svg", nomeBase);
    }
}

void gerarNomeQrySvg(const char* nomeBaseGeo, const char* nomeBaseQry, const char* outputDir, char* caminhoCompleto){
    if (!nomeBaseGeo || !nomeBaseQry || !caminhoCompleto) return;
    
    if (outputDir && strlen(outputDir) > 0){
        criarDiretorioRecursivo(outputDir);
        snprintf(caminhoCompleto, PATH_LEN, "%s%c%s-%s.svg", outputDir, PATH_SEP, nomeBaseGeo, nomeBaseQry);
    } else {
        snprintf(caminhoCompleto, PATH_LEN, "%s-%s.svg", nomeBaseGeo, nomeBaseQry);
    }
}

void gerarNomeQryTxt(const char* nomeBaseGeo, const char* nomeBaseQry, const char* outputDir, char* caminhoCompleto){
    if (!nomeBaseGeo || !nomeBaseQry || !caminhoCompleto) return;
    
    if (outputDir && strlen(outputDir) > 0){
        criarDiretorioRecursivo(outputDir);
        snprintf(caminhoCompleto, PATH_LEN, "%s%c%s-%s.txt", outputDir, PATH_SEP, nomeBaseGeo, nomeBaseQry);
    } else {
        snprintf(caminhoCompleto, PATH_LEN, "%s-%s.txt", nomeBaseGeo, nomeBaseQry);
    }
}

void gerarNomeComSufixo(const char* nomeBaseGeo, const char* nomeBaseQry, const char* sufixo, const char* outputDir, char* caminhoCompleto, int isSvg){
    if (!nomeBaseGeo || !sufixo || !caminhoCompleto) return;
    
    if (strcmp(sufixo, "-") == 0) {
        if (nomeBaseQry && strlen(nomeBaseQry) > 0) {
            if (isSvg) {
                gerarNomeQrySvg(nomeBaseGeo, nomeBaseQry, outputDir, caminhoCompleto);
            } else {
                gerarNomeQryTxt(nomeBaseGeo, nomeBaseQry, outputDir, caminhoCompleto);
            }
        } else {
            if (isSvg) {
                gerarNomeGeoSvg(nomeBaseGeo, outputDir, caminhoCompleto);
            }
        }
        return;
    }
    
    if (outputDir && strlen(outputDir) > 0){
        criarDiretorioRecursivo(outputDir);
        if (nomeBaseQry && strlen(nomeBaseQry) > 0) {
            snprintf(caminhoCompleto, PATH_LEN, "%s%c%s-%s-%s.%s", 
                    outputDir, PATH_SEP, nomeBaseGeo, nomeBaseQry, sufixo,
                    isSvg ? "svg" : "txt");
        } else {
            snprintf(caminhoCompleto, PATH_LEN, "%s%c%s-%s.%s", 
                    outputDir, PATH_SEP, nomeBaseGeo, sufixo,
                    isSvg ? "svg" : "txt");
        }
    } else {
        if (nomeBaseQry && strlen(nomeBaseQry) > 0) {
            snprintf(caminhoCompleto, PATH_LEN, "%s-%s-%s.%s", 
                    nomeBaseGeo, nomeBaseQry, sufixo,
                    isSvg ? "svg" : "txt");
        } else {
            snprintf(caminhoCompleto, PATH_LEN, "%s-%s.%s", 
                    nomeBaseGeo, sufixo,
                    isSvg ? "svg" : "txt");
        }
    }
}

int criarDiretorioSeNecessario(const char* caminho){
    if (!caminho) return 0;
    criarDiretorioRecursivo(caminho);
    return 1;
}
