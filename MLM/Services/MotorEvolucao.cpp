#include "MotorEvolucao.h"
#include <algorithm>
#include <cmath>

namespace MLM {

FaseCarreira MotorEvolucao::IdentificarFaseCarreira(uint8_t idade, PosicaoCampo posicao) const
{
    bool ehGoleiro = (posicao == PosicaoCampo::Goleiro);

    if (ehGoleiro) {
        // Curva Especial para Goleiros (Auge mais longo)
        if (idade <= 21) return FaseCarreira::CrescimentoJovem;
        if (idade <= 26) return FaseCarreira::Desenvolvimento;
        if (idade <= 32) return FaseCarreira::AugeEstabilidade;
        if (idade <= 35) return FaseCarreira::InicioDeclinio;
        return FaseCarreira::RegressaoNatural;
    } else {
        // Curva Normal para Jogadores de Linha
        if (idade <= 20) return FaseCarreira::CrescimentoJovem;
        if (idade <= 25) return FaseCarreira::Desenvolvimento;
        if (idade <= 29) return FaseCarreira::AugeEstabilidade;
        if (idade <= 32) return FaseCarreira::InicioDeclinio;
        return FaseCarreira::RegressaoNatural;
    }
}

float MotorEvolucao::ObterFatorFase(FaseCarreira fase, bool ehGoleiro) const
{
    switch (fase) {
        case FaseCarreira::CrescimentoJovem: return 1.0f;  // Potencial máximo de ganho
        case FaseCarreira::Desenvolvimento:  return 0.6f;  // Ganho bom
        case FaseCarreira::AugeEstabilidade: return 0.1f;  // Manutenção
        case FaseCarreira::InicioDeclinio:   return -0.4f; // Pequena regressão
        case FaseCarreira::RegressaoNatural: return -0.8f; // Regressão mais acentuada
        default: return 0.0f;
    }
}

float MotorEvolucao::ObterFatorDesempenho(const DesempenhoTemporada& desempenho) const
{
    // 1. Fator de Partidas (Base de 30 partidas para 100% do bônus de minutagem)
    float fatorMinutos = (std::min)(1.0f, (float)desempenho.partidasDisputadas / 30.0f);

    // 2. Fator de Nota Média (6.0 é neutro; 7.5+ acelera; <5.5 reduz)
    float fatorNota = (desempenho.notaMediaAtuacao - 6.0f) * 0.2f;

    // 3. Penalidade por Lesão (mais de 60 dias reduz evolução)
    float penalidadeLesao = (desempenho.diasLesionado > 60) ? -0.3f : 0.0f;

    return fatorMinutos + fatorNota + penalidadeLesao;
}

ResultadoEvolucao MotorEvolucao::CalcularEvolucaoAnual(const Jogador& jogador, const DesempenhoTemporada& desempenho) const
{
    ResultadoEvolucao resultado;
    resultado.overallAnterior = jogador.ObterOverall();

    bool ehGoleiro = (jogador.ObterPosicaoPrincipal() == PosicaoCampo::Goleiro);
    FaseCarreira fase = IdentificarFaseCarreira(jogador.ObterIdade(), jogador.ObterPosicaoPrincipal());

    float fatorFase = ObterFatorFase(fase, ehGoleiro);
    float fatorDesempenho = ObterFatorDesempenho(desempenho);
    float fatorMoral = (jogador.ObterMoral() - 50.0f) / 100.0f; // -0.5 a +0.5

    // Distância até o potencial máximo teórico (exclusivo do MLM)
    int margemPotencial = (int)jogador.ObterPotencial() - (int)jogador.ObterOverall();

    float deltaCalculado = 0.0f;

    if (fatorFase >= 0.0f) {
        // Fase de Crescimento ou Auge
        if (margemPotencial > 0) {
            deltaCalculado = (fatorFase * 3.0f) + (fatorDesempenho * 1.5f) + (fatorMoral * 0.5f);
            // Garante que não ultrapassa a margem do potencial
            deltaCalculado = (std::min)(deltaCalculado, (float)margemPotencial);
        } else {
            // Teto atingido: mantém estabilidade com pequenas variações por desempenho
            deltaCalculado = (fatorDesempenho > 0.5f) ? 0.0f : -1.0f;
        }
    } else {
        // Fase de Declínio / Regressão Natural
        deltaCalculado = (fatorFase * 3.0f) - (fatorDesempenho * 0.5f); // Bom desempenho suaviza a queda
    }

    // Aplicação das travas de limites anuais
    int8_t deltaFinal = (int8_t)std::round(deltaCalculado);
    deltaFinal = (std::max)((int8_t)-LIMITE_MAXIMO_PERDA_ANUAL, (std::min)((int8_t)LIMITE_MAXIMO_GANHO_ANUAL, deltaFinal));

    // Garante que o novo overall respeita o limite do potencial máximo
    int novoOverallCalc = (int)jogador.ObterOverall() + deltaFinal;
    if (novoOverallCalc > (int)jogador.ObterPotencial()) {
        novoOverallCalc = (int)jogador.ObterPotencial();
    }
    novoOverallCalc = (std::max)(40, (std::min)(99, novoOverallCalc));

    resultado.overallNovo = (uint8_t)novoOverallCalc;
    resultado.deltaOverall = (int8_t)(resultado.overallNovo - resultado.overallAnterior);

    // Sugestão de divisão de atributos (Físico cai primeiro na velhice, Técnico mantém)
    if (resultado.deltaOverall < 0) {
        resultado.deltaAtributosFisicos = resultado.deltaOverall;
        resultado.deltaAtributosTecnicos = (resultado.deltaOverall / 2);
    } else {
        resultado.deltaAtributosTecnicos = resultado.deltaOverall;
        resultado.deltaAtributosFisicos = (resultado.deltaOverall > 2) ? 1 : 0;
    }

    return resultado;
}

} // namespace MLM
