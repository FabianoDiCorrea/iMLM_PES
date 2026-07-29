#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Jogador.h"

using namespace MLM;

void TestarCriacaoJogador() {
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);
    assert(messi.ObterId() == 7511);
    assert(messi.ObterNome() == L"Lionel Messi");
    assert(messi.ObterNomeCamisa() == "MESSI");
    assert(messi.ObterIdade() == 36);
    assert(messi.ObterPaisId() == 10);
    assert(messi.ObterPosicaoPrincipal() == PosicaoCampo::SegundoAtacante);
    assert(messi.ObterOverall() == 93);
    assert(messi.ObterPotencial() == 93);
    assert(messi.ObterClubeId() == 0); // Inicializa como Agente Livre
    assert(messi.ObterNumeroCamisa() == 0);
    std::cout << "[TESTE OK] Criacao do Jogador" << std::endl;
}

void TestarAlteracaoClube() {
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);
    messi.AlterarClube(125, 10); // Roma ID: 125, Camisa 10
    assert(messi.ObterClubeId() == 125);
    assert(messi.ObterNumeroCamisa() == 10);

    messi.DesvincularClube();
    assert(messi.ObterClubeId() == 0);
    assert(messi.ObterNumeroCamisa() == 0);
    std::cout << "[TESTE OK] Alteracao e Desvinculo de Clube" << std::endl;
}

void TestarAlteracaoNumeroCamisa() {
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);
    messi.AlterarClube(1264, 10); // Inter Miami ID: 1264
    messi.AlterarNumeroCamisa(30);
    assert(messi.ObterNumeroCamisa() == 30);
    std::cout << "[TESTE OK] Alteracao do Numero da Camisa" << std::endl;
}

void TestarAniversario() {
    Jogador haaland(133543, L"Erling Haaland", "HAALAND", 23, 40, PosicaoCampo::Centroavante, 88, 94);
    assert(haaland.ObterIdade() == 23);
    haaland.FazerAniversario();
    assert(haaland.ObterIdade() == 24);
    std::cout << "[TESTE OK] Aniversario (Incremento da Idade)" << std::endl;
}

void TestarAtualizacaoMoralEForma() {
    Jogador haaland(133543, L"Erling Haaland", "HAALAND", 23, 40, PosicaoCampo::Centroavante, 88, 94);
    
    // Teste de Moral
    float moralInicial = haaland.ObterMoral();
    haaland.AtualizarMoral(15.0f);
    assert(haaland.ObterMoral() == moralInicial + 15.0f);
    
    haaland.AtualizarMoral(-200.0f); // Teste de limite inferior (0.0)
    assert(haaland.ObterMoral() == 0.0f);

    haaland.AtualizarMoral(150.0f); // Teste de limite superior (100.0)
    assert(haaland.ObterMoral() == 100.0f);

    // Teste de Forma Física
    haaland.AtualizarForma(85.5f);
    assert(haaland.ObterFormaFisica() == 85.5f);

    std::cout << "[TESTE OK] Atualizacao de Moral e Forma Fisica (Limites Respeitados)" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "  MLM - TESTES UNITARIOS DA ENTIDADE JOGADOR      " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCriacaoJogador();
    TestarAlteracaoClube();
    TestarAlteracaoNumeroCamisa();
    TestarAniversario();
    TestarAtualizacaoMoralEForma();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO!  " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
