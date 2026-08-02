#include <iostream>
#include <cassert>
#include "../Domain/Contrato.h"
#include "../Domain/Jogador.h"
#include "../Domain/Tecnico.h"
#include "../Persistence/ContratoRepositoryMemoria.h"
#include "../Persistence/ContratoSerializer.h"

using namespace MLM;

void TestarCriacaoEAtributosContrato() {
    std::cout << "[TESTE] Criacao e Atributos de Contrato... ";
    BonusContratual b{1000.0, 500.0, 50000.0};
    Contrato c(1, TipoContrato::Jogador, 101, 10, 2026, 36, 15000.0, 5000000.0, b);

    assert(c.ObterId() == 1);
    assert(c.ObterTipo() == TipoContrato::Jogador);
    assert(c.ObterEntidadeId() == 101);
    assert(c.ObterClubeId() == 10);
    assert(c.ObterAnoInicio() == 2026);
    assert(c.ObterAnoTermino() == 2029);
    assert(c.ObterDuracaoMeses() == 36);
    assert(c.ObterSalarioMensal() == 15000.0);
    assert(c.ObterMultaRescisoria() == 5000000.0);
    assert(c.ObterStatus() == StatusContrato::Ativo);
    assert(c.ObterBonus().bonusPorGol == 1000.0);
    std::cout << "OK!\n";
}

void TestarRenovacaoEEncerrarRescindir() {
    std::cout << "[TESTE] Renovacao, Encerramento e Rescisao... ";
    Contrato c(2, TipoContrato::Tecnico, 201, 10, 2026, 24, 40000.0, 1000000.0);

    // Renovação
    bool renovou = c.Renovar(12, 50000.0, 1500000.0, 2027);
    assert(renovou == true);
    assert(c.ObterDuracaoMeses() == 36);
    assert(c.ObterSalarioMensal() == 50000.0);
    assert(c.ObterHistoricoRenovacoes().size() == 1);
    assert(c.ObterHistoricoRenovacoes()[0].novoSalario == 50000.0);

    // Rescisão
    bool rescindiu = c.Rescindir();
    assert(rescindiu == true);
    assert(c.ObterStatus() == StatusContrato::Rescindido);

    // Não pode renovar nem rescindir contrato finalizado
    bool renovouNovamente = c.Renovar(12, 60000.0, 2000000.0, 2028);
    assert(renovouNovamente == false);

    std::cout << "OK!\n";
}

void TestarEmprestimoERetorno() {
    std::cout << "[TESTE] Emprestimo e Retorno de Jogador... ";
    Contrato c(3, TipoContrato::Jogador, 102, 10, 2026, 24, 8000.0, 2000000.0);

    // Empréstimo do clube 10 para o clube 20
    bool emprestado = c.ProcessarEmprestimo(20);
    assert(emprestado == true);
    assert(c.ObterStatus() == StatusContrato::Emprestado);
    assert(c.ObterClubeId() == 20);
    assert(c.ObterClubeOrigemEmprestimoId() == 10);

    // Retorno ao clube de origem
    bool retornou = c.RetornarDeEmprestimo();
    assert(retornou == true);
    assert(c.ObterStatus() == StatusContrato::Ativo);
    assert(c.ObterClubeId() == 10);
    assert(c.ObterClubeOrigemEmprestimoId() == 0);

    std::cout << "OK!\n";
}

void TestarPersistenciaESerializacaoContrato() {
    std::cout << "[TESTE] Persistencia, Serializacao e Incompatibilidade... ";
    ContratoRepositoryMemoria repo;

    Contrato c1(4, TipoContrato::Jogador, 103, 10, 2026, 12, 5000.0, 100000.0);
    Contrato c2Incompativel(5, TipoContrato::Jogador, 103, 15, 2026, 24, 7000.0, 200000.0);

    // Salvar c1
    OperationResult res1 = repo.Salvar(c1);
    assert(res1.success == true);

    // Tentar salvar c2 com a mesma entidade (Jogador 103) simultaneamente ativo -> deve falhar
    OperationResult res2 = repo.Salvar(c2Incompativel);
    assert(res2.success == false); // Validação de consistência contratual impediu duplicidade!

    // Obter do Repositório por Id e por Entidade
    auto resObter = repo.ObterContratoAtivoEntidade(TipoContrato::Jogador, 103);
    assert(resObter.success == true);
    assert(resObter.value.ObterId() == 4);

    // Serializar e Deserializar
    std::string serializado = ContratoSerializer::Serializar(c1);
    assert(!serializado.empty());

    auto resDes = ContratoSerializer::Deserializar(serializado);
    assert(resDes.success == true);
    assert(resDes.value.ObterEntidadeId() == 103);
    assert(resDes.value.ObterSalarioMensal() == 5000.0);

    std::cout << "OK!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << " INICIANDO SUÍTE DE TESTES: CONTRATOS   \n";
    std::cout << "========================================\n";

    TestarCriacaoEAtributosContrato();
    TestarRenovacaoEEncerrarRescindir();
    TestarEmprestimoERetorno();
    TestarPersistenciaESerializacaoContrato();

    std::cout << "========================================\n";
    std::cout << " TODOS OS TESTES PASSARAM COM SUCESSO!  \n";
    std::cout << "========================================\n";

    return 0;
}
