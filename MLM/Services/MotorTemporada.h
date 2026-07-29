#ifndef MLM_MOTOR_TEMPORADA_H
#define MLM_MOTOR_TEMPORADA_H

#include <cstdint>
#include <vector>
#include <string>
#include "../Domain/Temporada.h"
#include "../Domain/Competicao.h"
#include "MotorEvolucao.h"
#include "MotorTransferencias.h"

namespace MLM {

enum class FaseTemporada : uint8_t {
    PreTemporada = 0,
    JanelaTransferencias = 1,
    EmCompeticao = 2,
    PosTemporada = 3,
    Encerrada = 4
};

enum class ModoResolucaoCompeticao : uint8_t {
    ModoHumano = 0,    // Partidas jogadas no PES 2021 / Resultados importados
    ModoSimulado = 1   // Partidas simuladas pelo motor MLM
};

class MotorTemporada {
public:
    MotorTemporada(Temporada& temporadaAtual);
    ~MotorTemporada() = default;

    // Getters de Estado
    FaseTemporada ObterFaseAtual() const { return m_faseAtual; }
    uint16_t ObterAnoTemporada() const { return m_temporada.ObterAno(); }

    // Máquina de Estados da Temporada
    bool AvancarFase();

    // 1. Fase de Pré-Temporada
    bool ExecutarPreTemporada(std::vector<Jogador>& jogadores, const std::vector<DesempenhoTemporada>& desempenhos);

    // 2. Fase de Janela de Transferências
    bool ProcessarJanelaTransferencias(const std::vector<PropostaTransferencia>& propostas,
                                       const std::vector<Jogador>& jogadores,
                                       const std::vector<Clube>& clubes,
                                       std::vector<RespostaTransferencia>& outRespostas);

    // 3. Fase de Resolução de Competições
    bool ExecutarCompeticoes(std::vector<Competicao>& competicoes, ModoResolucaoCompeticao modo);

    // 4. Fase de Pós-Temporada (Premiações & Rankings)
    bool ExecutarPosTemporada();

    // 5. Virada de Ano (Gera Nova Temporada)
    Temporada ConcluirEVirarTemporada();

private:
    Temporada& m_temporada;
    FaseTemporada m_faseAtual;
    MotorEvolucao m_motorEvolucao;
    MotorTransferencias m_motorTransferencias;
};

} // namespace MLM

#endif // MLM_MOTOR_TEMPORADA_H
