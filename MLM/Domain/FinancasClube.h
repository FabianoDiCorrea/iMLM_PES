#ifndef MLM_FINANCAS_CLUBE_H
#define MLM_FINANCAS_CLUBE_H

#include <string>
#include <vector>
#include <cstdint>
#include "FinanceiroTipos.h"

namespace MLM {

class FinancasClube {
public:
    FinancasClube() = default;

    FinancasClube(uint32_t clubeId, double saldoInicial = 0.0, double orcamentoTransferencias = 0.0, double folhaSalarialMensal = 0.0);

    ~FinancasClube() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterClubeId() const { return m_clubeId; }
    double ObterSaldoAtual() const { return m_saldoAtual; }
    double ObterOrcamentoTransferencias() const { return m_orcamentoTransferencias; }
    double ObterFolhaSalarialMensal() const { return m_folhaSalarialMensal; }
    double ObterReceitasAcumuladasTotal() const { return m_receitasAcumuladasTotal; }
    double ObterDespesasAcumuladasTotal() const { return m_despesasAcumuladasTotal; }

    const std::vector<TransacaoFinanceira>& ObterHistoricoTransacoes() const { return m_historicoTransacoes; }
    const std::vector<ResumoFinanceiroTemporada>& ObterHistoricoTemporadas() const { return m_historicoTemporadas; }

    // Métodos de Gestão de Receitas e Despesas Esportivas
    bool RegistrarReceita(TipoReceita tipo, double valor, const std::wstring& descricao, uint32_t anoTemporada);
    bool RegistrarDespesa(TipoDespesa tipo, double valor, const std::wstring& descricao, uint32_t anoTemporada);

    // Ajuste de Orçamento e Gestão Financeira
    void DefinirOrcamentoTransferencias(double novoOrcamento) { m_orcamentoTransferencias = novoOrcamento; }
    void DefinirFolhaSalarialMensal(double novaFolha) { m_folhaSalarialMensal = novaFolha; }

    // Fechamento de Temporada
    void EncerrarTemporada(uint32_t anoTemporada);

    // Análise de Saúde Financeira e Capacidade de Investimento da IA
    double CalcularCapacidadeInvestimento() const;
    bool PodeArcarComTransferencia(double valorTransferencia, double salarioMensalAdicional, uint16_t mesesContrato) const;

private:
    uint32_t m_clubeId;
    double m_saldoAtual;
    double m_orcamentoTransferencias;
    double m_folhaSalarialMensal;

    double m_receitasAcumuladasTotal;
    double m_despesasAcumuladasTotal;

    std::vector<TransacaoFinanceira> m_historicoTransacoes;
    std::vector<ResumoFinanceiroTemporada> m_historicoTemporadas;
};

} // namespace MLM

#endif // MLM_FINANCAS_CLUBE_H
