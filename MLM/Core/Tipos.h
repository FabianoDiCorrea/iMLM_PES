#ifndef MLM_TIPOS_H
#define MLM_TIPOS_H

#include <string>
#include <cstdint>

namespace MLM {

enum class PosicaoCampo : uint8_t {
    Goleiro = 0,
    Zagueiro = 1,
    LateralEsquerdo = 2,
    LateralDireito = 3,
    Volante = 4,
    MeioCampoDefensivo = 5,
    MeioCampoEsquerdo = 6,
    MeioCampoDireito = 7,
    MeioCampoOfensivo = 8,
    PontaEsquerda = 9,
    PontaDireita = 10,
    SegundoAtacante = 11,
    Centroavante = 12,
    Desconhecida = 255
};

enum class PeDominante : uint8_t {
    Direito = 0,
    Esquerdo = 1
};

} // namespace MLM

#endif // MLM_TIPOS_H
