#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/MotorCarreiraTecnico.h"
#include "../Domain/Tecnico.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DO MOTOR DE CARREIRA DO TECNICO  " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar Técnico Protagonista (Humano)
    Tecnico treinador(1, L"Fabiano Correa", 1, 40, 75, 90);
    treinador.DefinirHumano(true);

    // 2. Criar Clube e Contratar o Técnico
    Clube santos(101, L"Santos", "SAN", 1);
    santos.AlterarReputacao(80);
    santos.DefinirFolhaSalarialMensal(2000000.0);
    treinador.AssumirClube(101, 150000.0, 12, 2026);
    santos.AlterarTecnico(1);

    // 3. Simular Expectativa da Diretoria (Meta: 4º lugar)
    MetaTemporada meta;
    meta.objetivoLiga = ObjetivoDiretoria::BrigarPorVagaContinental;
    meta.posicaoMinimaExigida = 4;
    meta.pressaoDiretoriaInicial = 50;

    MotorCarreiraTecnico motorCarreira;
    std::wstring mensagem;

    // 4. Cenário A: Técnico fica em 2º lugar (Sucesso)
    bool manteve = motorCarreira.AvaliarDesempenhoETomadaDecisao(treinador, santos, meta, 2, 2026, mensagem);
    std::wcout << L"[CENARIO A - 2º LUGAR] " << mensagem << std::endl;
    std::cout << " -> Nova Reputacao do Tecnico: " << (int)treinador.ObterReputacao() << std::endl;
    assert(manteve);
    assert(treinador.ObterClubeAtualId() == 101);

    // 5. Cenário B: Técnico fica em 18º lugar (Vexame -> Demissão)
    manteve = motorCarreira.AvaliarDesempenhoETomadaDecisao(treinador, santos, meta, 18, 2026, mensagem);
    std::wcout << L"[CENARIO B - 18º LUGAR] " << mensagem << std::endl;
    assert(!manteve);
    assert(treinador.ObterClubeAtualId() == 0); // Técnico ficou desempregado
    assert(santos.ObterTecnicoId() == 0);       // Santos ficou sem técnico

    // 6. Testar Recebimento de Ofertas de Emprego enquanto desempregado
    Clube bahia(102, L"Bahia", "BAH", 1);
    bahia.AlterarReputacao(72);
    bahia.DefinirFolhaSalarialMensal(1500000.0);

    std::vector<Clube> clubesLivres = { bahia };
    std::vector<Jogador> jogadores; // Elenco vazio para teste de propostas

    auto ofertas = motorCarreira.GerarOfertasEmprego(treinador, clubesLivres, jogadores);
    std::cout << std::endl << "[MERCADO DE TECNICOS] Ofertas Recebidas enquanto Desempregado: " << ofertas.size() << std::endl;
    assert(!ofertas.empty());
    std::wcout << L" -> Oferta de Contrato Recebida do: " << ofertas[0].nomeClube 
               << L" | Salario: R$ " << ofertas[0].salarioOferecido << std::endl;

    std::cout << std::endl << "[SUCESSO] Teste do Motor de Carreira do Técnico concluído com êxito!" << std::endl;
    return 0;
}
