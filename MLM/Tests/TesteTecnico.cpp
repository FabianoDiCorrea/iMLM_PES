#include <iostream>
#include <cassert>
#include "../Domain/Tecnico.h"
#include "../Domain/Clube.h"
#include "../Persistence/TecnicoRepositoryMemoria.h"
#include "../Persistence/TecnicoSerializer.h"

using namespace MLM;

void TestarCriacaoEAtributos() {
    std::cout << "[TESTE] Criacao e Atributos do Tecnico... ";
    Tecnico t(1, L"Abel Ferreira", 10, 45, 85, 90, EstiloPreferencial::PressaoAlta, PersonalidadeTecnico::Motivador);

    assert(t.ObterId() == 1);
    assert(t.ObterNome() == L"Abel Ferreira");
    assert(t.ObterPaisId() == 10);
    assert(t.ObterIdade() == 45);
    assert(t.ObterReputacao() == 85);
    assert(t.ObterPotencial() == 90);
    assert(t.ObterEstiloPreferencial() == EstiloPreferencial::PressaoAlta);
    assert(t.ObterPersonalidade() == PersonalidadeTecnico::Motivador);
    assert(t.ObterSituacao() == SituacaoTecnico::Desempregado);
    assert(t.ObterClubeAtualId() == 0);
    assert(t.ObterSelecaoAtualId() == 0);
    std::cout << "OK!\n";
}

void TestarCarreiraEHistorico() {
    std::cout << "[TESTE] Carreira e Historico do Tecnico... ";
    Tecnico t(2, L"Renato Gaúcho", 10, 58, 75, 80, EstiloPreferencial::PosseDeBola, PersonalidadeTecnico::Motivador);
    Clube c1(100, L"Grêmio", "GRE", 10);

    // Assumir clube
    bool assumiu = t.AssumirClube(c1.ObterId(), 50000.0, 24, 2026);
    assert(assumiu == true);
    assert(t.ObterClubeAtualId() == 100);
    assert(t.ObterSituacao() == SituacaoTecnico::Empregado);
    assert(t.ObterSalario() == 50000.0);
    assert(t.ObterHistorico().size() == 1);

    // Sincronizar no clube
    c1.AlterarTecnico(t.ObterId());
    assert(c1.ObterTecnicoId() == 2);

    // Registrar partida e títulos
    t.RegistrarPartida(3, 1); // Vitória
    t.RegistrarTitulo(500, L"Copa do Brasil", 2026);
    assert(t.ObterEstatisticas().vitorias == 1);
    assert(t.ObterEstatisticas().titulosConquistados == 1);
    assert(t.ObterReputacao() > 75); // Aumentou a reputação pelo título

    // Demissão
    t.DeixarClube(2027, true, false);
    assert(t.ObterClubeAtualId() == 0);
    assert(t.ObterSituacao() == SituacaoTecnico::Desempregado);
    c1.RemoverTecnico();
    assert(c1.ObterTecnicoId() == 0);

    // Assumir Seleção
    bool assumiuSelecao = t.AssumirSelecao(300, 2027); // Seleção Brasileira ID 300
    assert(assumiuSelecao == true);
    assert(t.ObterSelecaoAtualId() == 300);
    assert(t.ObterSituacao() == SituacaoTecnico::Empregado);

    // Aposentadoria
    t.Aposentar(2030);
    assert(t.ObterSituacao() == SituacaoTecnico::Aposentado);
    assert(t.ObterSelecaoAtualId() == 0);
    assert(t.ObterClubeAtualId() == 0);

    std::cout << "OK!\n";
}

void TestarPersistenciaESerializacao() {
    std::cout << "[TESTE] Persistencia e Serializacao do Tecnico... ";
    TecnicoRepositoryMemoria repo;

    Tecnico t1(3, L"Carlo Ancelotti", 15, 64, 95, 95, EstiloPreferencial::Equilibrado, PersonalidadeTecnico::Tatico);
    t1.AssumirClube(200, 120000.0, 36, 2026);

    // Salvar
    OperationResult resSalvar = repo.Salvar(t1);
    assert(resSalvar.success == true);

    // Obter por ID
    auto resObter = repo.ObterPorId(3);
    assert(resObter.success == true);
    assert(resObter.value.ObterNome() == L"Carlo Ancelotti");
    assert(resObter.value.ObterClubeAtualId() == 200);

    // Serializar
    std::string dados = TecnicoSerializer::Serializar(t1);
    assert(!dados.empty());

    // Deserializar
    auto resDes = TecnicoSerializer::Deserializar(3, L"Carlo Ancelotti", dados);
    assert(resDes.success == true);
    assert(resDes.value.ObterReputacao() == 95);
    assert(resDes.value.ObterClubeAtualId() == 200);

    std::cout << "OK!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << " INICIANDO SUÍTE DE TESTES: TECNICOS   \n";
    std::cout << "========================================\n";

    TestarCriacaoEAtributos();
    TestarCarreiraEHistorico();
    TestarPersistenciaESerializacao();

    std::cout << "========================================\n";
    std::cout << " TODOS OS TESTES PASSARAM COM SUCESSO!  \n";
    std::cout << "========================================\n";

    return 0;
}
