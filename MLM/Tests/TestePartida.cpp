#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Partida.h"

using namespace MLM;

void TestarCriacaoPartida() {
    Partida p1(1, 125, 1264, 2026, 1, 5); // Roma (125) vs Inter Miami (1264), Rodada 5
    assert(p1.ObterId() == 1);
    assert(p1.ObterMandanteId() == 125);
    assert(p1.ObterVisitanteId() == 1264);
    assert(p1.ObterAnoTemporada() == 2026);
    assert(p1.ObterCompeticaoId() == 1);
    assert(p1.ObterRodada() == 5);
    assert(p1.ObterGolsMandante() == 0);
    assert(p1.ObterGolsVisitante() == 0);
    assert(p1.EstaAgendada() == true);
    std::cout << "[TESTE OK] Criacao da Partida" << std::endl;
}

void TestarImpedimentoClubesIguais() {
    Partida pInvalida(2, 125, 125, 2026, 1, 1); // Roma vs Roma
    assert(pInvalida.EstaCancelada() == true);
    assert(pInvalida.Iniciar() == false);
    std::cout << "[TESTE OK] Impedimento de Clubes Iguais" << std::endl;
}

void TestarFluxoPartidaEGols() {
    Partida p(3, 125, 1264, 2026, 1, 10);
    
    // Tenta registrar gol antes de iniciar
    bool golSemInicio = p.RegistrarGolMandante();
    assert(golSemInicio == false);

    // Inicia a partida
    bool inicioOk = p.Iniciar();
    assert(inicioOk == true);
    assert(p.EstaEmAndamento() == true);

    // Registra gols
    p.RegistrarGolMandante(); // 1 x 0
    p.RegistrarGolMandante(); // 2 x 0
    p.RegistrarGolVisitante(); // 2 x 1

    assert(p.ObterGolsMandante() == 2);
    assert(p.ObterGolsVisitante() == 1);
    assert(p.Empatada() == false);
    assert(p.ObterVencedorId() == 125); // Roma vence

    std::cout << "[TESTE OK] Fluxo de Partida, Andamento e Registro de Gols" << std::endl;
}

void TestarDefinicaoResultadoEEmpate() {
    Partida p(4, 125, 1264, 2026, 1, 12);
    p.Iniciar();

    // Placar direto (ex: retornado por simulação ou leitura futura por OCR)
    p.DefinirPlacarDirecto(3, 3);
    assert(p.Empatada() == true);
    assert(p.ObterVencedorId() == 0); // Nenhum vencedor
    assert(p.ObterResultado() == ResultadoPartida::Empate);

    // Finalização da partida
    bool fimOk = p.Finalizar();
    assert(fimOk == true);
    assert(p.EstaFinalizada() == true);

    std::cout << "[TESTE OK] Definicao de Resultado e Empate" << std::endl;
}

void TestarImpedimentoAlteracaoAposFinalizada() {
    Partida p(5, 125, 1264, 2026, 1, 15);
    p.Iniciar();
    p.RegistrarGolMandante();
    p.Finalizar();

    assert(p.EstaFinalizada() == true);

    // Tentativas de alterações após finalizada
    bool golPosFim = p.RegistrarGolVisitante();
    assert(golPosFim == false);

    bool placarPosFim = p.DefinirPlacarDirecto(5, 5);
    assert(placarPosFim == false);

    bool cancelarPosFim = p.Cancelar();
    assert(cancelarPosFim == false);

    assert(p.ObterGolsMandante() == 1);
    assert(p.ObterGolsVisitante() == 0); // Permanece inalterado

    std::cout << "[TESTE OK] Impedimento de Alteracoes Apos Finalizada" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "    MLM - TESTES UNITARIOS DA ENTIDADE PARTIDA   " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCriacaoPartida();
    TestarImpedimentoClubesIguais();
    TestarFluxoPartidaEGols();
    TestarDefinicaoResultadoEEmpate();
    TestarImpedimentoAlteracaoAposFinalizada();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES UNITARIOS PASSARAM COM SUCESSO!  " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
