#ifndef MLM_FINANCEIRO_TIPOS_H
#define MLM_FINANCEIRO_TIPOS_H

#include <string>
#include <cstdint>

namespace MLM {

enum class TipoReceita : uint8_t {
    PremiacaoCompeticao = 0,
    VendaJogador = 1,
    MultaTecnicoRecebida = 2,
    Patrocinio = 3,
    DireitosTelevisao = 4,
    BilheteriaSimulada = 5,
    OutraReceita = 6
};

enum class TipoDespesa : uint8_t {
    CompraJogador = 0,
    ContratacaoTecnico = 1,
    SalarioJogador = 2,
    SalarioComissaoTecnica = 3,
    MultaRescisoriaPaga = 4,
    OutraDespesaFutebol = 5
};

struct TransacaoFinanceira {
    uint32_t anoTemporada;
    bool ehReceita; // true para receita, false para despesa
    uint8_t categoria; // Cast do enum TipoReceita ou TipoDespesa
    double valor;
    std::wstring descricao;

    TransacaoFinanceira(uint32_t ano = 0, bool receita = true, uint8_t cat = 0, double val = 0.0, const std::wstring& desc = L"")
        : anoTemporada(ano), ehReceita(receita), categoria(cat), valor(val), descricao(desc) {}
};

struct ResumoFinanceiroTemporada {
    uint32_t anoTemporada;
    double totalReceitas;
    double totalDespesas;
    double saldoFinalTemporada;

    ResumoFinanceiroTemporada(uint32_t ano = 0, double rec = 0.0, double desp = 0.0, double saldo = 0.0)
        : anoTemporada(ano), totalReceitas(rec), totalDespesas(desp), saldoFinalTemporada(saldo) {}
};

} // namespace MLM

#endif // MLM_FINANCEIRO_TIPOS_H
