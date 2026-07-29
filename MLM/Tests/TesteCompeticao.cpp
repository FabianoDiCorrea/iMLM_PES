#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Competicao.h"

using namespace MLM;

void TestarCriacaoCompeticao() {
    Competicao serieA(1, L"Serie A TIM", "SEA", 2026, TipoCompeticao::Liga, 20);
    assert(serieA.ObterId() == 1);
    assert(serieA.ObterNome() == L"Serie A TIM");
    assert(serieA.ObterNomeAbreviado() == "SEA");
    assert(serieA.ObterAnoTemporada() == 2026);
    assert(serieA.ObterTipo() == TipoCompeticao::Liga);
    assert(serieA.ObterLimiteMaximoClubes() == 20);
    assert(serieA.EstaPlanejada() == true);
    assert(serieA.QuantidadeClubes() == 0);
    assert(serieA.QuantidadePartidas() == 0);
    std::cout << "[TESTE OK] Criacao da Competicao" << std::endl;
}

void TestarCadastroEValidacaoClubes() {
    Competicao serieA(1, L"Serie A TIM", "SEA", 2026, TipoCompeticao::Liga, 2); // Limite pequeno para teste (2 participantes)

    // Adiciona Clube 1
    bool add1 = serieA.AdicionarClube(125); // Roma ID: 125
    assert(add1 == true);
    assert(serieA.QuantidadeClubes() == 1);
    assert(serieA.PossuiClube(125) == true);

    // Tentativa de duplicidade
    bool addDup = serieA.AdicionarClube(125);
    assert(addDup == false);
    assert(serieA.QuantidadeClubes() == 1);

    // Adiciona Clube 2
    bool add2 = serieA.AdicionarClube(1264); // Inter Miami ID: 1264
    assert(add2 == true);
    assert(serieA.QuantidadeClubes() == 2);

    // Tentativa de ultrapassar limite máximo (2)
    bool addExcedente = serieA.AdicionarClube(300);
    assert(addExcedente == false);
    assert(serieA.QuantidadeClubes() == 2);

    std::cout << "[TESTE OK] Cadastro de Clubes, Impedimento de Duplicidade e Respeito ao Limite" << std::endl;
}

void TestarInicioEBloqueioDeAlteracoes() {
    Competicao serieA(1, L"Serie A TIM", "SEA", 2026, TipoCompeticao::Liga, 20);
    serieA.AdicionarClube(125);
    serieA.AdicionarClube(1264);

    // Inicia Competição
    bool inicioOk = serieA.Iniciar();
    assert(inicioOk == true);
    assert(serieA.EstaEmAndamento() == true);

    // Bloqueio de alteração de clubes após iniciada
    bool addPosInicio = serieA.AdicionarClube(500);
    assert(addPosInicio == false);

    bool remPosInicio = serieA.RemoverClube(125);
    assert(remPosInicio == false);

    assert(serieA.QuantidadeClubes() == 2);
    std::cout << "[TESTE OK] Inicio da Competicao e Bloqueio de Alteracao de Participantes" << std::endl;
}

void TestarCadastroEConsultaPartidas() {
    Competicao serieA(1, L"Serie A TIM", "SEA", 2026, TipoCompeticao::Liga, 20);
    serieA.AdicionarClube(125);
    serieA.AdicionarClube(1264);
    serieA.Iniciar();

    Partida p1(10, 125, 1264, 2026, 1, 1); // Partida Rodada 1
    bool addPartidaOk = serieA.AdicionarPartida(p1);
    assert(addPartidaOk == true);
    assert(serieA.QuantidadePartidas() == 1);
    assert(serieA.PossuiPartida(10) == true);

    // Tentativa de duplicidade de partida
    bool addPartidaDup = serieA.AdicionarPartida(p1);
    assert(addPartidaDup == false);
    assert(serieA.QuantidadePartidas() == 1);

    const auto& listaPartidas = serieA.ObterPartidas();
    assert(listaPartidas.size() == 1);
    assert(listaPartidas[0].ObterId() == 10);

    std::cout << "[TESTE OK] Cadastro e Consulta de Partidas" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "  MLM - TESTES UNITARIOS DA ENTIDADE COMPETICAO  " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCriacaoCompeticao();
    TestarCadastroEValidacaoClubes();
    TestarInicioEBloqueioDeAlteracoes();
    TestarCadastroEConsultaPartidas();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO!  " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
