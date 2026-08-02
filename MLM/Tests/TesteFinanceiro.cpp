#include <iostream>
#include <cassert>
#include "../Domain/FinancasClube.h"
#include "../Persistence/FinanceiroRepositoryMemoria.h"
#include "../Persistence/FinanceiroSerializer.h"

using namespace MLM;

void TestarReceitasEDespesas() {
    std::cout << "[TESTE] Receitas e Despesas Esportivas... ";
    FinancasClube f(10, 10000000.0, 5000000.0, 500000.0);

    // Receita de premiação
    bool r1 = f.RegistrarReceita(TipoReceita::PremiacaoCompeticao, 2000000.0, L"Título da Copa", 2026);
    assert(r1 == true);
    assert(f.ObterSaldoAtual() == 12000000.0);
    assert(f.ObterOrcamentoTransferencias() == 5000000.0 + (2000000.0 * 0.70)); // +1.4M

    // Despesa de compra de jogador
    bool d1 = f.RegistrarDespesa(TipoDespesa::CompraJogador, 3000000.0, L"Compra de Atacante", 2026);
    assert(d1 == true);
    assert(f.ObterSaldoAtual() == 9000000.0);
    assert(f.ObterHistoricoTransacoes().size() == 2);

    // Despesa maior que saldo deve falhar
    bool dInvalida = f.RegistrarDespesa(TipoDespesa::CompraJogador, 50000000.0, L"Compra Impossível", 2026);
    assert(dInvalida == false);

    std::cout << "OK!\n";
}

void TestarCapacidadeInvestimentoEConsistencia() {
    std::cout << "[TESTE] Capacidade de Investimento e Viabilidade da IA... ";
    FinancasClube f(20, 8000000.0, 4000000.0, 300000.0);

    // Pode arcar com transferência prudente
    bool viavel = f.PodeArcarComTransferencia(3000000.0, 50000.0, 24); // 3M transf + 1.2M salários = 4.2M < 8M
    assert(viavel == true);

    // Não pode arcar se estourar o orçamento de transferências
    bool inviavelTransf = f.PodeArcarComTransferencia(5000000.0, 50000.0, 24); // 5M > 4M orcamento
    assert(inviavelTransf == false);

    // Fechamento de temporada
    f.EncerrarTemporada(2026);
    assert(f.ObterHistoricoTemporadas().size() == 1);
    assert(f.ObterHistoricoTemporadas()[0].saldoFinalTemporada == 8000000.0);

    std::cout << "OK!\n";
}

void TestarPersistenciaESerializacaoFinanceira() {
    std::cout << "[TESTE] Persistencia e Serializacao Financeira... ";
    FinanceiroRepositoryMemoria repo;

    FinancasClube f(30, 25000000.0, 15000000.0, 1000000.0);

    // Salvar no repositório
    OperationResult res1 = repo.Salvar(f);
    assert(res1.success == true);

    // Obter por Clube ID
    auto resObter = repo.ObterPorClubeId(30);
    assert(resObter.success == true);
    assert(resObter.value.ObterSaldoAtual() == 25000000.0);

    // Serializar e Deserializar
    std::string serializado = FinanceiroSerializer::Serializar(f);
    assert(!serializado.empty());

    auto resDes = FinanceiroSerializer::Deserializar(serializado);
    assert(resDes.success == true);
    assert(resDes.value.ObterClubeId() == 30);
    assert(resDes.value.ObterOrcamentoTransferencias() == 15000000.0);

    std::cout << "OK!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << " INICIANDO SUÍTE DE TESTES: FINANCEIRO  \n";
    std::cout << "========================================\n";

    TestarReceitasEDespesas();
    TestarCapacidadeInvestimentoEConsistencia();
    TestarPersistenciaESerializacaoFinanceira();

    std::cout << "========================================\n";
    std::cout << " TODOS OS TESTES PASSARAM COM SUCESSO!  \n";
    std::cout << "========================================\n";

    return 0;
}
