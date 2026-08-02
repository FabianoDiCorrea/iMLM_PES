#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/SistemaExpectativas.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DO SISTEMA DE EXPECTATIVAS (DIRETORIA) " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar Clube de Grande Porte
    Clube palmeiras(101, L"Palmeiras", "PAL", 1);
    palmeiras.AlterarReputacao(88);
    palmeiras.DefinirTradicaoInstitucional(90);
    palmeiras.DefinirOrcamentoTransferencias(40000000.0);

    // 2. Elenco com titulares fortes (Média ~83)
    std::vector<Jogador> elenco = {
        Jogador(1, L"Craque 1", "C1", 26, 1, PosicaoCampo::Centroavante, 85, 88),
        Jogador(2, L"Craque 2", "C2", 28, 1, PosicaoCampo::MeioCampoOfensivo, 84, 85),
        Jogador(3, L"Craque 3", "C3", 27, 1, PosicaoCampo::Zagueiro, 82, 84)
    };
    for (auto& j : elenco) j.AlterarClube(101, 10);

    // 3. Instanciar SistemaExpectativas
    SistemaExpectativas sistema;
    MetaTemporada meta = sistema.GerarExpectativaTemporada(palmeiras, elenco);

    std::wcout << L"[DIRETORIA] Mensagem: " << meta.mensagemDiretoria << std::endl;
    std::cout << "[DIRETORIA] Posicao Minima Exigida: " << meta.posicaoMinimaExigida << L"º lugar" << std::endl;
    std::cout << "[DIRETORIA] Pressao Inicial: " << (int)meta.pressaoDiretoriaInicial << "%" << std::endl;
    std::cout << "[DIRETORIA] Orcamento Liberado: R$ " << meta.orcamentoDisponibilizado << std::endl << std::endl;

    // 4. Avaliar Caso 1: Campeão (1º lugar)
    uint8_t novaPressao = 0;
    bool cumpriu = sistema.AvaliarCumprimentoMeta(meta, 1, novaPressao);
    std::cout << "[AVALIACAO FINAL] Ficou em 1º lugar: " << (cumpriu ? "META CUMPRIDA! (Sucesso)" : "FALHOU") 
              << " | Nova Pressao: " << (int)novaPressao << "%" << std::endl;
    assert(cumpriu);

    // 5. Avaliar Caso 2: Ficou em 7º lugar (Fracasso)
    cumpriu = sistema.AvaliarCumprimentoMeta(meta, 7, novaPressao);
    std::cout << "[AVALIACAO FINAL] Ficou em 7º lugar: " << (cumpriu ? "META CUMPRIDA" : "FALHOU! (Risco de Demissao)") 
              << " | Nova Pressao: " << (int)novaPressao << "%" << std::endl;
    assert(!cumpriu);

    std::cout << std::endl << "[SUCESSO] Teste do Sistema de Expectativas concluído com êxito!" << std::endl;
    return 0;
}
