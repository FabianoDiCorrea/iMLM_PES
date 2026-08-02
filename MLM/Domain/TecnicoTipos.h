#ifndef MLM_TECNICO_TIPOS_H
#define MLM_TECNICO_TIPOS_H

#include <cstdint>

namespace MLM {

enum class EstiloPreferencial : uint8_t {
    Equilibrado = 0,
    PosseDeBola = 1,
    ContraAtaque = 2,
    PressaoAlta = 3,
    JogoDireto = 4,
    RetrancaDefensiva = 5
};

enum class PersonalidadeTecnico : uint8_t {
    Disciplinador = 0,
    Motivador = 1,
    Tatico = 2,
    DesenvolvedorJovens = 3,
    Conservador = 4,
    Ambicioso = 5
};

enum class SituacaoTecnico : uint8_t {
    Empregado = 0,
    Desempregado = 1,
    Aposentado = 2
};

} // namespace MLM

#endif // MLM_TECNICO_TIPOS_H
