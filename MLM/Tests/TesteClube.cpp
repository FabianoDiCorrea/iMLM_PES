#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

using namespace MLM;

void TestarAdicaoJogador() {
    Clube roma(125, L"Roma", "ROM", 9, L"Roma", 12);
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);

    assert(messi.ObterClubeId() == 0); // Inicialmente Agente Livre

    bool adicionado = roma.AdicionarJogador(&messi, 10);
    assert(adicionado == true);
    assert(roma.QuantidadeJogadores() == 1);
    assert(roma.PossuiJogador(7511) == true);
    assert(messi.ObterClubeId() == 125); // Jogador agora reflete o ID da Roma
    assert(messi.ObterNumeroCamisa() == 10);

    std::cout << "[TESTE OK] Adicao de Jogador ao Elenco e Sincronizacao de Estado" << std::endl;
}

void TestarRemocaoJogador() {
    Clube roma(125, L"Roma", "ROM", 9, L"Roma", 12);
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);

    roma.AdicionarJogador(&messi, 10);
    assert(roma.QuantidadeJogadores() == 1);

    bool removido = roma.RemoverJogador(7511);
    assert(removido == true);
    assert(roma.QuantidadeJogadores() == 0);
    assert(roma.PossuiJogador(7511) == false);
    assert(messi.ObterClubeId() == 0); // Jogador voltou a ser Agente Livre
    assert(messi.ObterNumeroCamisa() == 0);

    // Tentativa de remover jogador inexistente
    bool remInexistente = roma.RemoverJogador(99999);
    assert(remInexistente == false);

    std::cout << "[TESTE OK] Remocao de Jogador do Elenco e Desvinculo" << std::endl;
}

void TestarImpedimentoDuplicidade() {
    Clube roma(125, L"Roma", "ROM", 9, L"Roma", 12);
    Jogador messi(7511, L"Lionel Messi", "MESSI", 36, 10, PosicaoCampo::SegundoAtacante, 93, 93);

    bool add1 = roma.AdicionarJogador(&messi, 10);
    assert(add1 == true);

    // Tentativa de adicionar o mesmo jogador novamente
    bool add2 = roma.AdicionarJogador(&messi, 10);
    assert(add2 == false); // Deve ser recusado
    assert(roma.QuantidadeJogadores() == 1);

    std::cout << "[TESTE OK] Impedimento de Duplicidade de Jogador no Elenco" << std::endl;
}

void TestarLimiteMaximoElenco() {
    Clube roma(125, L"Roma", "ROM", 9, L"Roma", 12);
    std::vector<Jogador> elencoFicticio;
    elencoFicticio.reserve(41);

    // Preenche o clube até o limite máximo de 40 jogadores
    for (uint32_t i = 1; i <= 40; ++i) {
        elencoFicticio.emplace_back(i, L"Jogador Teste", "TESTE", 20, 1, PosicaoCampo::Centroavante, 70, 75);
    }

    for (size_t i = 0; i < 40; ++i) {
        bool ok = roma.AdicionarJogador(&elencoFicticio[i], (uint16_t)(i + 1));
        assert(ok == true);
    }

    assert(roma.QuantidadeJogadores() == 40);

    // Tentativa de adicionar o 41º jogador
    Jogador jogadorExcedente(41, L"Excedente", "EXCED", 20, 1, PosicaoCampo::Centroavante, 70, 75);
    bool addExcedente = roma.AdicionarJogador(&jogadorExcedente, 99);
    assert(addExcedente == false); // Recusado devido ao limite
    assert(roma.QuantidadeJogadores() == 40);

    std::cout << "[TESTE OK] Validacao do Limite Maximo do Elenco (40 Jogadores)" << std::endl;
}

void TestarConsultaElenco() {
    Clube roma(125, L"Roma", "ROM", 9, L"Roma", 12);
    Jogador j1(101, L"Jogador 1", "J1", 20, 1, PosicaoCampo::Goleiro, 75, 80);
    Jogador j2(102, L"Jogador 2", "J2", 22, 1, PosicaoCampo::Zagueiro, 78, 82);

    roma.AdicionarJogador(&j1, 1);
    roma.AdicionarJogador(&j2, 4);

    const auto& listaElenco = roma.ObterJogadores();
    assert(listaElenco.size() == 2);
    assert(listaElenco[0]->ObterId() == 101);
    assert(listaElenco[1]->ObterId() == 102);

    std::cout << "[TESTE OK] Consulta dos Jogadores do Elenco" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << " MLM - TESTES UNITARIOS DE GESTAO DE ELENCO      " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarAdicaoJogador();
    TestarRemocaoJogador();
    TestarImpedimentoDuplicidade();
    TestarLimiteMaximoElenco();
    TestarConsultaElenco();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO!  " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
