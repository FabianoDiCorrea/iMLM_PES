#include "MotorSimulacao.h"
#include <cmath>
#include <algorithm>

namespace MLM {

MotorSimulacao::MotorSimulacao() {
    std::random_device rd;
    m_rng.seed(rd());
}

MotorSimulacao::MotorSimulacao(uint32_t seed) {
    m_rng.seed(seed);
}

float MotorSimulacao::CalcularForcaAtaque(const std::vector<Jogador>& jogadores) const {
    if (jogadores.empty()) return 60.0f;

    float somaOverall = 0.0f;
    size_t qtd = 0;
    for (const auto& jog : jogadores) {
        somaOverall += jog.ObterOverall();
        qtd++;
    }
    float media = somaOverall / static_cast<float>(qtd);
    return media;
}

float MotorSimulacao::CalcularForcaDefesa(const std::vector<Jogador>& jogadores) const {
    if (jogadores.empty()) return 60.0f;

    float somaOverall = 0.0f;
    size_t qtd = 0;
    for (const auto& jog : jogadores) {
        somaOverall += jog.ObterOverall();
        qtd++;
    }
    float media = somaOverall / static_cast<float>(qtd);
    return media;
}

uint16_t MotorSimulacao::GerarGolsPoisson(float lambda) {
    if (lambda < 0.1f) lambda = 0.1f;
    std::poisson_distribution<int> d(lambda);
    int gols = d(m_rng);
    return static_cast<uint16_t>(std::max(0, gols));
}

ResultadoSimulacaoPartida MotorSimulacao::SimularPartida(Partida& partida, 
                                                        const Clube& mandante, 
                                                        const Clube& visitante,
                                                        const std::vector<Jogador>& jogadoresMandante,
                                                        const std::vector<Jogador>& jogadoresVisitante) {
    ResultadoSimulacaoPartida res;
    res.partidaId = partida.ObterId();

    // 1. Calcula força base dos elencos
    float forcaAtaqueMandante = CalcularForcaAtaque(jogadoresMandante);
    float forcaDefesaMandante = CalcularForcaDefesa(jogadoresMandante);

    float forcaAtaqueVisitante = CalcularForcaAtaque(jogadoresVisitante);
    float forcaDefesaVisitante = CalcularForcaDefesa(jogadoresVisitante);

    // 2. Vantagem de mandar o jogo (+5% de força de ataque)
    forcaAtaqueMandante *= 1.05f;

    // 3. Fator de expectativa de gols (lambda para distribuição de Poisson)
    // Média de gols típica de partida ~ 1.3 a 1.6 por time
    float lambdaMandante = (forcaAtaqueMandante / std::max(forcaDefesaVisitante, 1.0f)) * 1.35f;
    float lambdaVisitante = (forcaAtaqueVisitante / std::max(forcaDefesaMandante, 1.0f)) * 1.15f;

    // 4. Gerar gols de forma estocástica (Poisson)
    res.golsMandante = GerarGolsPoisson(lambdaMandante);
    res.golsVisitante = GerarGolsPoisson(lambdaVisitante);

    // 5. Atualizar estado da partida no domínio
    partida.Iniciar();
    partida.DefinirPlacarDirecto(res.golsMandante, res.golsVisitante);
    partida.Finalizar();

    res.vencedorId = partida.ObterVencedorId();

    // 6. Sortear marcadores dos gols (quem fez os gols)
    if (res.golsMandante > 0 && !jogadoresMandante.empty()) {
        std::uniform_int_distribution<size_t> dist(0, jogadoresMandante.size() - 1);
        for (uint16_t i = 0; i < res.golsMandante; ++i) {
            res.marcadoresMandanteIds.push_back(jogadoresMandante[dist(m_rng)].ObterId());
        }
    }

    if (res.golsVisitante > 0 && !jogadoresVisitante.empty()) {
        std::uniform_int_distribution<size_t> dist(0, jogadoresVisitante.size() - 1);
        for (uint16_t i = 0; i < res.golsVisitante; ++i) {
            res.marcadoresVisitanteIds.push_back(jogadoresVisitante[dist(m_rng)].ObterId());
        }
    }

    return res;
}

std::vector<ResultadoSimulacaoPartida> MotorSimulacao::SimularLotePartidas(std::vector<Partida>& partidas,
                                                                         const std::vector<Clube>& clubes,
                                                                         const std::vector<Jogador>& todosJogadores) {
    std::vector<ResultadoSimulacaoPartida> resultados;
    resultados.reserve(partidas.size());

    for (auto& partida : partidas) {
        if (partida.EstaFinalizada()) continue;

        // Procura clubes
        const Clube* mandante = nullptr;
        const Clube* visitante = nullptr;

        for (const auto& c : clubes) {
            if (c.ObterId() == partida.ObterMandanteId()) mandante = &c;
            if (c.ObterId() == partida.ObterVisitanteId()) visitante = &c;
        }

        // Filtra jogadores dos clubes
        std::vector<Jogador> jogMandante;
        std::vector<Jogador> jogVisitante;

        for (const auto& j : todosJogadores) {
            if (j.ObterClubeId() == partida.ObterMandanteId()) jogMandante.push_back(j);
            if (j.ObterClubeId() == partida.ObterVisitanteId()) jogVisitante.push_back(j);
        }

        Clube clubePadraoMandante(partida.ObterMandanteId(), L"Mandante", "MND", 1);
        Clube clubePadraoVisitante(partida.ObterVisitanteId(), L"Visitante", "VIS", 1);

        resultados.push_back(SimularPartida(partida, 
                                            mandante ? *mandante : clubePadraoMandante,
                                            visitante ? *visitante : clubePadraoVisitante,
                                            jogMandante,
                                            jogVisitante));
    }

    return resultados;
}

} // namespace MLM
