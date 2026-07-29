#ifndef MLM_COMPETICAO_TIPOS_H
#define MLM_COMPETICAO_TIPOS_H

#include <cstdint>

namespace MLM {

enum class CategoriaCompeticao : uint8_t {
    Nacional = 0,
    Continental = 1,
    Mundial = 2,
    InternacionalSelecoes = 3
};

enum class FormatoCompeticao : uint8_t {
    PontosCorridos = 0,
    MataMata = 1,
    GruposEMataMata = 2,
    LigaComPlayoff = 3
};

enum class FrequenciaCompeticao : uint8_t {
    Anual = 1,
    Bienal = 2,
    Quadrienal = 4
};

struct RegrasCompeticao {
    uint8_t vagasContinentais = 4;
    uint8_t vagasRebaixamento = 4;
    double premiacaoCampeao = 10000000.0;
    double premiacaoVice = 5000000.0;
    uint8_t prestigio = 80; // 0 a 100
};

} // namespace MLM

#endif // MLM_COMPETICAO_TIPOS_H
