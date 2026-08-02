#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/TabelaClassificacao.h"
#include "../Services/MotorSimulacao.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"
#include "../Domain/Partida.h"

#include <iomanip>

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DE SIMULACAO DE LIGA E TABELA    " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar 4 clubes de teste
    Clube c1(1, L"Flamengo", "FLA", 1);
    Clube c2(2, L"Palmeiras", "PAL", 1);
    Clube c3(3, L"Santos", "SAN", 1);
    Clube c4(4, L"Vasco", "VAS", 1);

    std::vector<Clube> clubes = { c1, c2, c3, c4 };

    // 2. Montar Tabela
    TabelaClassificacao tabela;
    for (const auto& c : clubes) {
        tabela.AdicionarClube(c.ObterId(), c.ObterNome());
    }

    // 3. Gerar partidas de Turno e Returno (6 rodadas, 12 partidas)
    std::vector<Partida> partidas;
    uint32_t idPartida = 1000;

    for (const auto& mandante : clubes) {
        for (const auto& visitante : clubes) {
            if (mandante.ObterId() != visitante.ObterId()) {
                partidas.emplace_back(idPartida++, mandante.ObterId(), visitante.ObterId(), 2026, 1, 1);
            }
        }
    }

    // 4. Jogadores fictícios para a simulação
    std::vector<Jogador> todosJogadores = {
        Jogador(10, L"Atacante 1", "A1", 25, 1, PosicaoCampo::Centroavante, 85, 88),
        Jogador(11, L"Atacante 2", "A2", 25, 1, PosicaoCampo::Centroavante, 80, 82),
        Jogador(12, L"Atacante 3", "A3", 25, 1, PosicaoCampo::Centroavante, 75, 78),
        Jogador(13, L"Atacante 4", "A4", 25, 1, PosicaoCampo::Centroavante, 70, 72)
    };
    todosJogadores[0].AlterarClube(1, 9);
    todosJogadores[1].AlterarClube(2, 9);
    todosJogadores[2].AlterarClube(3, 9);
    todosJogadores[3].AlterarClube(4, 9);

    // 5. Simular todas as partidas
    MotorSimulacao motor(42);
    auto resultados = motor.SimularLotePartidas(partidas, clubes, todosJogadores);

    for (const auto& res : resultados) {
        for (const auto& p : partidas) {
            if (p.ObterId() == res.partidaId) {
                tabela.ProcessarResultadoPartida(p.ObterMandanteId(), p.ObterVisitanteId(), res.golsMandante, res.golsVisitante);
            }
        }
    }

    tabela.AtualizarClassificacao();

    // 6. Exibir Tabela Final com AP%
    std::cout << "POS | CLUBE      | PTS | J  | V  | E  | D  | GP | GC | SG  | AP%" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    for (const auto& item : tabela.ObterItens()) {
        std::wcout << item.posicao << L"   | ";
        std::wcout.width(10);
        std::wcout << std::left << item.nomeClube << L" | "
                   << item.pontos << L"   | "
                   << item.jogos << L"  | "
                   << item.vitorias << L"  | "
                   << item.empates << L"  | "
                   << item.derrotas << L"  | "
                   << item.golsPro << L"  | "
                   << item.golsContra << L"  | "
                   << item.saldoGols << L"  | ";
        std::cout << std::fixed << std::setprecision(1) << item.aproveitamentoPct << "%" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "[CAMPEAO] ID: " << tabela.ObterCampeaoId() << std::endl;

    auto rebaixados = tabela.ObterRebaixadosIds(1);
    if (!rebaixados.empty()) {
        std::cout << "[REBAIXADO] ID: " << rebaixados[0] << std::endl;
    }

    std::cout << std::endl << "[SUCESSO] Teste de Simulação de Liga e Tabela concluído!" << std::endl;
    return 0;
}
