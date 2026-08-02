#include "FinancasClube.h"
#include <algorithm>

namespace MLM {

FinancasClube::FinancasClube(uint32_t clubeId, double saldoInicial, double orcamentoTransferencias, double folhaSalarialMensal)
    : m_clubeId(clubeId),
      m_saldoAtual(saldoInicial),
      m_orcamentoTransferencias(orcamentoTransferencias),
      m_folhaSalarialMensal(folhaSalarialMensal),
      m_receitasAcumuladasTotal(0.0),
      m_despesasAcumuladasTotal(0.0)
{
}

bool FinancasClube::RegistrarReceita(TipoReceita tipo, double valor, const std::wstring& descricao, uint32_t anoTemporada)
{
    if (valor <= 0.0) {
        return false;
    }

    m_saldoAtual += valor;
    m_receitasAcumuladasTotal += valor;

    // Se for venda de jogador ou prêmio, influi no orçamento de transferências proporcionalmente (ex: 70%)
    if (tipo == TipoReceita::VendaJogador || tipo == TipoReceita::PremiacaoCompeticao) {
        m_orcamentoTransferencias += (valor * 0.70);
    }

    m_historicoTransacoes.emplace_back(anoTemporada, true, static_cast<uint8_t>(tipo), valor, descricao);
    return true;
}

bool FinancasClube::RegistrarDespesa(TipoDespesa tipo, double valor, const std::wstring& descricao, uint32_t anoTemporada)
{
    if (valor <= 0.0) {
        return false;
    }

    // Permite despesa se houver saldo suficiente
    if (m_saldoAtual < valor) {
        return false;
    }

    m_saldoAtual -= valor;
    m_despesasAcumuladasTotal += valor;

    if (tipo == TipoDespesa::CompraJogador) {
        m_orcamentoTransferencias = (std::max)(0.0, m_orcamentoTransferencias - valor);
    }

    m_historicoTransacoes.emplace_back(anoTemporada, false, static_cast<uint8_t>(tipo), valor, descricao);
    return true;
}

void FinancasClube::EncerrarTemporada(uint32_t anoTemporada)
{
    double receitasTemporada = 0.0;
    double despesasTemporada = 0.0;

    for (const auto& t : m_historicoTransacoes) {
        if (t.anoTemporada == anoTemporada) {
            if (t.ehReceita) {
                receitasTemporada += t.valor;
            } else {
                despesasTemporada += t.valor;
            }
        }
    }

    m_historicoTemporadas.emplace_back(anoTemporada, receitasTemporada, despesasTemporada, m_saldoAtual);
}

double FinancasClube::CalcularCapacidadeInvestimento() const
{
    // Capacidade prudente de investimento: Orçamento de transferências limitado ao saldo disponível
    return (std::min)(m_saldoAtual, m_orcamentoTransferencias);
}

bool FinancasClube::PodeArcarComTransferencia(double valorTransferencia, double salarioMensalAdicional, uint16_t mesesContrato) const
{
    double custoTotalCompromisso = valorTransferencia + (salarioMensalAdicional * mesesContrato);
    if (custoTotalCompromisso > m_saldoAtual) {
        return false;
    }
    if (valorTransferencia > m_orcamentoTransferencias) {
        return false;
    }
    return true;
}

} // namespace MLM
