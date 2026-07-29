#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Jogador.h"
#include "../Services/MotorEvolucao.h"

using namespace MLM;

void TestarEvolucaoJovemPromessa() {
    // Jovem de 18 anos, overall 72, potencial 90
    Jogador jovem(101, L"Jovem Talento", "JOVEM", 18, 10, PosicaoCampo::PontaEsquerda, 72, 90);
    MotorEvolucao motor;

    DesempenhoTemporada bomDesempenho;
    bomDesempenho.partidasDisputadas = 35;
    bomDesempenho.minutosJogados = 2800;
    bomDesempenho.notaMediaAtuacao = 7.8f;
    bomDesempenho.diasLesionado = 0;

    ResultadoEvolucao res = motor.CalcularEvolucaoAnual(jovem, bomDesempenho);

    assert(res.overallAnterior == 72);
    assert(res.deltaOverall > 0); // Deve evoluir
    assert(res.overallNovo <= 90); // Nunca ultrapassa potencial
    assert(res.deltaOverall <= 5); // Respeita limite máximo de +5

    std::cout << "[TESTE OK] Evolucao de Jovem Promessa: 72 -> " << (int)res.overallNovo 
              << " (+ " << (int)res.deltaOverall << ")" << std::endl;
}

void TestarRespeitoAoTetoDePotencial() {
    // Jogador perto do teto de potencial (89 / 90)
    Jogador promessaQuasePronta(102, L"Promessa Quase Pronta", "PROMESSA", 20, 10, PosicaoCampo::Centroavante, 89, 90);
    MotorEvolucao motor;

    DesempenhoTemporada excelenteDesempenho;
    excelenteDesempenho.partidasDisputadas = 40;
    excelenteDesempenho.notaMediaAtuacao = 8.5f;

    ResultadoEvolucao res = motor.CalcularEvolucaoAnual(promessaQuasePronta, excelenteDesempenho);

    assert(res.overallNovo == 90); // Trava exatamente no potencial de 90
    assert(res.deltaOverall == 1);

    std::cout << "[TESTE OK] Respeito ao Teto de Potencial Exclusivo MLM: Trava em 90" << std::endl;
}

void TestarCurvaEspecialDeGoleiro() {
    // Goleiro de 31 anos (em plena maturidade pela curva de GK)
    Jogador goleiro(103, L"Goleiro Experiente", "GOLEIRO", 31, 10, PosicaoCampo::Goleiro, 84, 88);
    MotorEvolucao motor;

    DesempenhoTemporada bomDesempenho;
    bomDesempenho.partidasDisputadas = 38;
    bomDesempenho.notaMediaAtuacao = 7.2f;

    ResultadoEvolucao res = motor.CalcularEvolucaoAnual(goleiro, bomDesempenho);

    // Goleiros de 31 anos ainda mantêm estabilidade/crescimento suave
    assert(res.deltaOverall >= 0);
    std::cout << "[TESTE OK] Curva Especial de Goleiro (31 anos mantem estabilidade): " 
              << (int)res.overallAnterior << " -> " << (int)res.overallNovo << std::endl;
}

void TestarDeclinioVeterano() {
    // Atleta de linha com 35 anos (regressão natural)
    Jogador veterano(104, L"Veterano Linha", "VETERANO", 35, 10, PosicaoCampo::MeioCampoDefensivo, 82, 85);
    MotorEvolucao motor;

    DesempenhoTemporada desempenhoRegular;
    desempenhoRegular.partidasDisputadas = 20;
    desempenhoRegular.notaMediaAtuacao = 6.2f;

    ResultadoEvolucao res = motor.CalcularEvolucaoAnual(veterano, desempenhoRegular);

    assert(res.deltaOverall < 0); // Regressão natural
    assert(res.overallNovo < 82);

    std::cout << "[TESTE OK] Declinio Natural de Veterano de Linha (35 anos): 82 -> " 
              << (int)res.overallNovo << " (" << (int)res.deltaOverall << ")" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "  MLM - TESTES DO MOTOR DE EVOLUCAO DOS JOGADORES " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarEvolucaoJovemPromessa();
    TestarRespeitoAoTetoDePotencial();
    TestarCurvaEspecialDeGoleiro();
    TestarDeclinioVeterano();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES DO MOTOR PASSARAM COM SUCESSO!   " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
