#ifndef MLM_MOTOR_SIMULACAO_H
#define MLM_MOTOR_SIMULACAO_H

#include <cstdint>
#include <vector>
#include <random>
#include "../Domain/Partida.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

namespace MLM {

struct ResultadoSimulacaoPartida {
    uint32_t partidaId;
    uint16_t golsMandante;
    uint16_t golsVisitante;
    uint32_t vencedorId;
    std::vector<uint32_t> marcadoresMandanteIds;
    std::vector<uint32_t> marcadoresVisitanteIds;
};

class MotorSimulacao {
public:
    MotorSimulacao();
    explicit MotorSimulacao(uint32_t seed);
    ~MotorSimulacao() = default;

    // Simula uma partida isolada considerando os elencos e atributos dos clubes
    ResultadoSimulacaoPartida SimularPartida(Partida& partida, 
                                            const Clube& mandante, 
                                            const Clube& visitante,
                                            const std::vector<Jogador>& jogadoresMandante,
                                            const std::vector<Jogador>& jogadoresVisitante);

    // Simula um lote de partidas (ex: uma rodada inteira)
    std::vector<ResultadoSimulacaoPartida> SimularLotePartidas(std::vector<Partida>& partidas,
                                                             const std::vector<Clube>& clubes,
                                                             const std::vector<Jogador>& todosJogadores);

private:
    std::mt19937 m_rng;

    // Métodos auxiliares de cálculo de força
    float CalcularForcaAtaque(const std::vector<Jogador>& jogadores) const;
    float CalcularForcaDefesa(const std::vector<Jogador>& jogadores) const;
    uint16_t GerarGolsPoisson(float lambda);
};

} // namespace MLM

#endif // MLM_MOTOR_SIMULACAO_H
