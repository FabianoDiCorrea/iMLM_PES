#ifndef MLM_MOTOR_EVOLUCAO_H
#define MLM_MOTOR_EVOLUCAO_H

#include <cstdint>
#include "../Domain/Jogador.h"

namespace MLM {

struct DesempenhoTemporada {
    uint16_t partidasDisputadas = 0;
    uint32_t minutosJogados = 0;
    float notaMediaAtuacao = 6.0f; // Escala de 0.0 a 10.0 (padrão 6.0)
    uint16_t diasLesionado = 0;
};

struct ResultadoEvolucao {
    uint8_t overallAnterior = 0;
    uint8_t overallNovo = 0;
    int8_t deltaOverall = 0;
    
    // Sugestão de variação técnica por grupo de atributos
    int8_t deltaAtributosTecnicos = 0;
    int8_t deltaAtributosFisicos = 0;
};

enum class FaseCarreira : uint8_t {
    CrescimentoJovem = 0,   // 16 a 20 anos
    Desenvolvimento = 1,     // 21 a 25 anos
    AugeEstabilidade = 2,    // 26 a 29 anos
    InicioDeclinio = 3,      // 30 a 32 anos
    RegressaoNatural = 4     // 33+ anos
};

class MotorEvolucao {
public:
    MotorEvolucao() = default;
    ~MotorEvolucao() = default;

    // Método Principal de Cálculo de Fim de Temporada
    ResultadoEvolucao CalcularEvolucaoAnual(const Jogador& jogador, const DesempenhoTemporada& desempenho) const;

    // Métodos Auxiliares de Curva
    FaseCarreira IdentificarFaseCarreira(uint8_t idade, PosicaoCampo posicao) const;
    float ObterFatorFase(FaseCarreira fase, bool ehGoleiro) const;
    float ObterFatorDesempenho(const DesempenhoTemporada& desempenho) const;

private:
    static constexpr uint8_t LIMITE_MAXIMO_GANHO_ANUAL = 5;  // Máximo +5 overall em 1 temporada
    static constexpr uint8_t LIMITE_MAXIMO_PERDA_ANUAL = 4;  // Máximo -4 overall em 1 temporada
};

} // namespace MLM

#endif // MLM_MOTOR_EVOLUCAO_H
