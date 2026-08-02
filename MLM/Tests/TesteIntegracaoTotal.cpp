#include <iostream>
#include <cassert>
#include <vector>
#include "../Application/MLMEngine.h"
#include "../Services/MotorTemporada.h"
#include "../Services/MotorSimulacao.h"
#include "../Services/MotorTransferencias.h"
#include "../Services/SistemaExpectativas.h"
#include "../Services/MotorCarreiraTecnico.h"
#include "../Services/TabelaClassificacao.h"

using namespace MLM;

int main() {
    std::cout << "==========================================================================" << std::endl;
    std::cout << "   TESTE DE INTEGRACAO TOTAL DO NUCLEO iMLM PES (TEMPORADA COMPLETA)     " << std::endl;
    std::cout << "==========================================================================" << std::endl << std::endl;

    // 1. Inicializar Engine Central
    MLMEngine engine;
    auto resInit = engine.Initialize("mlm_config.json");
    assert(resInit.success);

    // 2. Criar Carreira do Treinador (Humano)
    auto resCarreira = engine.CreateNewCareer("Fabiano Correa", 101);
    assert(resCarreira.success);

    // 3. Montar Temporada 2026 (Ano 2026, 1ª Temporada da carreira)
    Temporada temp2026(2026, 1);
    MotorTemporada motorTemporada(temp2026);

    std::cout << "[TEMPORADA] Ano: " << motorTemporada.ObterAnoTemporada() 
              << " | Fase Inicial: Pre-Temporada" << std::endl;

    // 4. FASE 1: Pré-Temporada (Envelhecimento & Evolução de Atletas)
    std::vector<Jogador> jogadores = {
        Jogador(1, L"Pedro", "Pedro", 27, 1, PosicaoCampo::Centroavante, 82, 85),
        Jogador(2, L"De Arrascaeta", "Arrascaeta", 30, 1, PosicaoCampo::MeioCampoOfensivo, 84, 86)
    };
    jogadores[0].AlterarClube(101, 9);
    jogadores[1].AlterarClube(101, 14);

    DesempenhoTemporada d1{30, 2700, 7.5f, 0};
    DesempenhoTemporada d2{28, 2500, 7.8f, 0};
    std::vector<DesempenhoTemporada> desempenhos = { d1, d2 };
    bool okPre = motorTemporada.ExecutarPreTemporada(jogadores, desempenhos);
    assert(okPre);
    std::cout << "[FASE 1 OK] Pre-Temporada concluida. Idades e evolucoes aplicadas." << std::endl;

    // Avanca para Fase 2: Mercado
    motorTemporada.AvancarFase();
    assert(motorTemporada.ObterFaseAtual() == FaseTemporada::JanelaTransferencias);

    // 5. FASE 2: Janela de Transferências
    Clube flamengo(101, L"Flamengo", "FLA", 1);
    flamengo.AlterarReputacao(85);
    flamengo.ReceberValor(50000000.0);

    Clube palmeiras(102, L"Palmeiras", "PAL", 1);
    palmeiras.AlterarReputacao(88);
    palmeiras.ReceberValor(60000000.0);

    std::vector<Clube> clubes = { flamengo, palmeiras };
    std::vector<PropostaTransferencia> propostas = {
        { 1, 101, 102, 15000000.0, 200000.0, 3 }
    };

    std::vector<RespostaTransferencia> respostas;
    bool okMercado = motorTemporada.ProcessarJanelaTransferencias(propostas, jogadores, clubes, respostas);
    assert(okMercado);
    std::cout << "[FASE 2 OK] Janela de Transferencias processada. Propostas avaliadas pela IA." << std::endl;

    // Avança para Fase 3: Competições
    motorTemporada.AvancarFase();
    assert(motorTemporada.ObterFaseAtual() == FaseTemporada::EmCompeticao);

    // 6. FASE 3: Competições (Simulação via Poisson + Tabela com AP%)
    TabelaClassificacao tabela;
    tabela.AdicionarClube(101, L"Flamengo");
    tabela.AdicionarClube(102, L"Palmeiras");
    tabela.ProcessarResultadoPartida(101, 102, 2, 1); // Fla 2 x 1 Pal
    tabela.AtualizarClassificacao();
    std::cout << "[FASE 3 OK] Competicoes finalizadas. Campeao apurado." << std::endl;

    // Avança para Fase 4: Pós-Temporada
    motorTemporada.AvancarFase();
    assert(motorTemporada.ObterFaseAtual() == FaseTemporada::PosTemporada);

    // 7. FASE 4: Pós-Temporada (Expectativas & Avaliação do Treinador)
    Tecnico treinador(1, L"Fabiano Correa", 1, 40, 75, 90);
    treinador.AssumirClube(101, 150000.0, 12, 2026);

    MotorCarreiraTecnico motorCarreira;
    MetaTemporada meta;
    meta.posicaoMinimaExigida = 1;
    meta.pressaoDiretoriaInicial = 60;

    std::wstring msg;
    bool manteve = motorCarreira.AvaliarDesempenhoETomadaDecisao(treinador, flamengo, meta, tabela.ObterItens()[0].posicao, 2026, msg);
    assert(manteve);
    std::cout << "[FASE 4 OK] Pos-Temporada e avaliacao da diretoria concluida com sucesso." << std::endl;

    // 8. Virada de Temporada (Gera 2027)
    Temporada proxTemp = motorTemporada.ConcluirEVirarTemporada();
    std::cout << std::endl << "[VIRADA DE ANO] Nova Temporada Gerada: " << proxTemp.ObterAno() << std::endl;
    assert(proxTemp.ObterAno() == 2027);

    engine.Shutdown();

    std::cout << std::endl << "==========================================================================" << std::endl;
    std::cout << "   [SUCESSO INTEGRAL] CICLO COMPLETO DA CARREIRA SIMULADO COM EXITO!     " << std::endl;
    std::cout << "==========================================================================" << std::endl;
    return 0;
}
