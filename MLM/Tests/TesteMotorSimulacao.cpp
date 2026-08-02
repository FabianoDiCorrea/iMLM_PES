#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/MotorSimulacao.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"
#include "../Domain/Partida.h"

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DO MOTOR DE SIMULACAO DE JOGOS  " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar dois clubes
    Clube flamengo(101, L"Flamengo", "FLA", 1);
    Clube palmeiras(102, L"Palmeiras", "PAL", 1);

    // 2. Criar elencos simples para os dois clubes
    std::vector<Jogador> elencoFlamengo = {
        Jogador(1, L"Pedro", "Pedro", 27, 1, PosicaoCampo::Centroavante, 82, 85),
        Jogador(2, L"De Arrascaeta", "Arrascaeta", 30, 1, PosicaoCampo::MeioCampoOfensivo, 84, 86),
        Jogador(3, L"Gerson", "Gerson", 27, 1, PosicaoCampo::MeioCampoOfensivo, 81, 83),
        Jogador(4, L"Fabrício Bruno", "F. Bruno", 28, 1, PosicaoCampo::Zagueiro, 78, 80),
        Jogador(5, L"Rossi", "Rossi", 29, 1, PosicaoCampo::Goleiro, 79, 81)
    };

    for (auto& j : elencoFlamengo) {
        j.AlterarClube(101, 10);
    }

    std::vector<Jogador> elencoPalmeiras = {
        Jogador(6, L"Flaco López", "Flaco", 23, 1, PosicaoCampo::Centroavante, 78, 83),
        Jogador(7, L"Veiga", "R. Veiga", 29, 1, PosicaoCampo::MeioCampoOfensivo, 83, 84),
        Jogador(8, L"Zé Rafael", "Zé Rafael", 31, 1, PosicaoCampo::MeioCampoOfensivo, 79, 80),
        Jogador(9, L"Gomez", "Gomez", 31, 1, PosicaoCampo::Zagueiro, 82, 83),
        Jogador(10, L"Weverton", "Weverton", 36, 1, PosicaoCampo::Goleiro, 81, 82)
    };

    for (auto& j : elencoPalmeiras) {
        j.AlterarClube(102, 10);
    }

    // 3. Criar partida agendada
    Partida jogo(5001, 101, 102, 2026, 1, 1);
    assert(jogo.EstaAgendada());

    // 4. Instanciar Motor de Simulação
    MotorSimulacao motor(12345); // Seed fixa para reprodutibilidade

    std::cout << "[SIMULANDO] Flamengo vs Palmeiras (Rodada 1)..." << std::endl;
    ResultadoSimulacaoPartida res = motor.SimularPartida(jogo, flamengo, palmeiras, elencoFlamengo, elencoPalmeiras);

    std::cout << "[RESULTADO] Placar Final: Flamengo " << res.golsMandante 
              << " x " << res.golsVisitante << " Palmeiras" << std::endl;
    
    assert(jogo.EstaFinalizada());

    if (res.vencedorId == 101) {
        std::cout << "[RESULTADO] Vencedor: Flamengo!" << std::endl;
    } else if (res.vencedorId == 102) {
        std::cout << "[RESULTADO] Vencedor: Palmeiras!" << std::endl;
    } else {
        std::cout << "[RESULTADO] Empate!" << std::endl;
    }

    std::cout << std::endl << "--- Gols do Flamengo ---" << std::endl;
    for (auto id : res.marcadoresMandanteIds) {
        for (const auto& j : elencoFlamengo) {
            if (j.ObterId() == id) {
                std::wcout << L" -> Gol de: " << j.ObterNome() << std::endl;
            }
        }
    }

    std::cout << std::endl << "--- Gols do Palmeiras ---" << std::endl;
    for (auto id : res.marcadoresVisitanteIds) {
        for (const auto& j : elencoPalmeiras) {
            if (j.ObterId() == id) {
                std::wcout << L" -> Gol de: " << j.ObterNome() << std::endl;
            }
        }
    }

    std::cout << std::endl << "[SUCESSO] Teste do Motor de Simulação concluído com exito!" << std::endl;
    return 0;
}
