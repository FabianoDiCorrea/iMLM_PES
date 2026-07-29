#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Temporada.h"

using namespace MLM;

void TestarCriacaoTemporada() {
    Temporada temp2026(2026, 1);
    assert(temp2026.ObterAno() == 2026);
    assert(temp2026.ObterNumeroSequencial() == 1);
    assert(temp2026.ObterSituacao() == SituacaoTemporada::Planejada);
    assert(temp2026.EstaPlanejada() == true);
    assert(temp2026.EstaEmAndamento() == false);
    assert(temp2026.EstaEncerrada() == false);
    std::cout << "[TESTE OK] Criacao da Temporada (2026 - 1a Temporada)" << std::endl;
}

void TestarInicioEEncerramento() {
    Temporada temp(2026, 1);

    // Início
    bool inicioOk = temp.Iniciar();
    assert(inicioOk == true);
    assert(temp.EstaEmAndamento() == true);
    assert(temp.EstaPlanejada() == false);

    // Tentativa de reiniciar temporada já em andamento
    bool reinicioInvalido = temp.Iniciar();
    assert(reinicioInvalido == false);

    // Encerramento
    bool encerramentoOk = temp.Encerrar();
    assert(encerramentoOk == true);
    assert(temp.EstaEncerrada() == true);
    assert(temp.EstaEmAndamento() == false);

    // Tentativa de encerrar novamente
    bool reEncerramentoInvalido = temp.Encerrar();
    assert(reEncerramentoInvalido == false);

    std::cout << "[TESTE OK] Fluxo de Transicao de Estados (Planejada -> Em Andamento -> Encerrada)" << std::endl;
}

void TestarCriacaoProximaTemporada() {
    Temporada temp2026(2026, 1);
    temp2026.Iniciar();
    temp2026.Encerrar();

    Temporada temp2027 = temp2026.CriarProximaTemporada();
    assert(temp2027.ObterAno() == 2027);
    assert(temp2027.ObterNumeroSequencial() == 2);
    assert(temp2027.EstaPlanejada() == true); // A próxima temporada nasce planejada

    std::cout << "[TESTE OK] Criacao da Proxima Temporada (2027 - 2a Temporada)" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "  MLM - TESTES UNITARIOS DA ENTIDADE TEMPORADA   " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCriacaoTemporada();
    TestarInicioEEncerramento();
    TestarCriacaoProximaTemporada();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO!  " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
