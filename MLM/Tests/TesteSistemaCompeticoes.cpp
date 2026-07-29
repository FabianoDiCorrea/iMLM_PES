#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Competicao.h"
#include "../Domain/EdicaoCompeticao.h"

using namespace MLM;

void TestarCriacaoEAssociacaoEdicaoCompeticao() {
    // 1. Cria Competição Base (Série A TIM)
    Competicao serieA(1, L"Serie A TIM", "SEA", 2026, TipoCompeticao::Liga, 20);
    assert(serieA.ObterId() == 1);
    assert(serieA.ObterTipo() == TipoCompeticao::Liga);

    // 2. Cria Edição Histórica de 2026
    EdicaoCompeticao edicao2026(101, 1, 2026, L"Serie A TIM 2026");
    assert(edicao2026.ObterId() == 101);
    assert(edicao2026.ObterCompeticaoId() == 1);
    assert(edicao2026.ObterAnoTemporada() == 2026);
    assert(edicao2026.EstaEncerrada() == false);

    std::cout << "[TESTE OK] Criacao de Competicao e Edicao Anual Associada" << std::endl;
}

void TestarRegistroParticipantesEResultadoFinal() {
    EdicaoCompeticao edicao(102, 1, 2026, L"Copa do Brasil 2026");
    
    // Cadastra Participantes
    bool r1 = edicao.RegistrarParticipante(125);  // Roma ID: 125
    bool r2 = edicao.RegistrarParticipante(1264); // Inter Miami ID: 1264
    assert(r1 == true);
    assert(r2 == true);
    assert(edicao.QuantidadeParticipantes() == 2);

    // Tentativa de participante duplicado
    bool rDup = edicao.RegistrarParticipante(125);
    assert(rDup == false);
    assert(edicao.QuantidadeParticipantes() == 2);

    // Encerra com Campeão e Vice
    bool resFinal = edicao.DefinirResultadoFinal(125, 1264); // Roma Campeão, Miami Vice
    assert(resFinal == true);
    assert(edicao.ObterCampeaoId() == 125);
    assert(edicao.ObterViceId() == 1264);
    assert(edicao.EstaEncerrada() == true);

    // Tenta registrar novo participante após encerrada
    bool rPosFim = edicao.RegistrarParticipante(500);
    assert(rPosFim == false);

    std::cout << "[TESTE OK] Registro de Participantes e Definicao de Campeao/Vice" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "   MLM - TESTES DO SISTEMA DE COMPETICOES        " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCriacaoEAssociacaoEdicaoCompeticao();
    TestarRegistroParticipantesEResultadoFinal();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES DO DOMINIO PASSARAM COM SUCESSO! " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
