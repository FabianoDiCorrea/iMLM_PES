#include <iostream>
#include <cassert>
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"
#include "../Domain/Tecnico.h"
#include "../Persistence/ClubeRepositoryMemoria.h"
#include "../Persistence/ClubeSerializer.h"

using namespace MLM;

void TestarCriacaoEAtributosExpansaoClube() {
    std::cout << "[TESTE] Criacao e Atributos Institucionais do Clube... ";
    Clube c(1, L"Flamengo", "FLA", 10, L"Rio de Janeiro", 50, 1);

    assert(c.ObterId() == 1);
    assert(c.ObterNome() == L"Flamengo");
    assert(c.ObterNomeAbreviado() == "FLA");
    assert(c.ObterPaisId() == 10);
    assert(c.ObterCidade() == L"Rio de Janeiro");
    assert(c.ObterEstadioId() == 50);
    assert(c.ObterLigaId() == 1);
    assert(c.ObterReputacao() == 50);

    c.DefinirTradicaoInstitucional(90);
    assert(c.ObterTradicaoInstitucional() == 90);

    c.AdicionarRival(2); // Vasco
    c.AdicionarRival(3); // Fluminense
    assert(c.ERival(2) == true);
    assert(c.ERival(3) == true);
    assert(c.ERival(4) == false);

    std::cout << "OK!\n";
}

void TestarGestaoDeElencoETecnico() {
    std::cout << "[TESTE] Gestao de Elenco e Integracao com Tecnico... ";
    Clube c(10, L"Palmeiras", "PAL", 10, L"São Paulo", 51, 1);
    Tecnico t(100, L"Abel Ferreira", 10, 45, 85);

    // Vínculo com técnico
    c.AlterarTecnico(t.ObterId());
    assert(c.ObterTecnicoId() == 100);

    // Vínculo com jogadores
    Jogador j1(1001, L"Dudu", "DUDU", 31, 10, PosicaoCampo::PontaEsquerda, 82, 85);
    Jogador j2(1002, L"Veiga", "VEIGA", 28, 10, PosicaoCampo::MeioCampoOfensivo, 84, 86);

    assert(c.AdicionarJogador(&j1, 7) == true);
    assert(c.AdicionarJogador(&j2, 23) == true);
    assert(c.QuantidadeJogadores() == 2);
    assert(c.PossuiJogador(1001) == true);
    assert(c.ValidarConsistenciaElenco() == true);

    // Tentativa de duplicar jogador
    assert(c.AdicionarJogador(&j1, 77) == false);

    // Remover jogador
    assert(c.RemoverJogador(1001) == true);
    assert(c.QuantidadeJogadores() == 1);
    assert(c.PossuiJogador(1001) == false);

    std::cout << "OK!\n";
}

void TestarMemoriaInstitucionalEHistorico() {
    std::cout << "[TESTE] Memoria Institucional (Titulos e Campanhas)... ";
    Clube c(20, L"Santos", "SAN", 10, L"Santos", 52, 1);

    c.RegistrarTitulo(2026, 101, L"Copa do Brasil");
    assert(c.ObterTitulos().size() == 1);
    assert(c.ObterTitulos()[0].nomeCompeticao == L"Copa do Brasil");

    c.RegistrarCampanha(2026, 1, L"Brasileirão Série A", 2, L"Vice-Campeão", 38, 22, 10, 6);
    assert(c.ObterCampanhas().size() == 1);
    assert(c.ObterCampanhas()[0].posicaoFinal == 2);

    std::cout << "OK!\n";
}

void TestarPersistenciaESerializacaoClube() {
    std::cout << "[TESTE] Persistencia e Serializacao do Clube... ";
    ClubeRepositoryMemoria repo;

    Clube c1(30, L"Real Madrid", "RMA", 15, L"Madrid", 60, 5);
    c1.ReceberValor(50000000.0);
    c1.DefinirOrcamentoTransferencias(30000000.0);
    c1.AlterarReputacao(95);

    // Salvar no repositório
    OperationResult res1 = repo.Salvar(c1);
    assert(res1.success == true);

    // Buscar por ID e por Liga
    auto resObter = repo.ObterPorId(30);
    assert(resObter.success == true);
    assert(resObter.value.ObterNome() == L"Real Madrid");

    auto listaLiga = repo.ObterPorLiga(5);
    assert(listaLiga.size() == 1);

    // Serialização e Deserialização
    std::string serializado = ClubeSerializer::Serializar(c1);
    assert(!serializado.empty());

    auto resDes = ClubeSerializer::Deserializar(30, L"Real Madrid", "RMA", serializado);
    assert(resDes.success == true);
    assert(resDes.value.ObterSaldo() == 50000000.0);
    assert(resDes.value.ObterReputacao() == 95);

    std::cout << "OK!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << " INICIANDO SUÍTE DE TESTES: CLUBES     \n";
    std::cout << "========================================\n";

    TestarCriacaoEAtributosExpansaoClube();
    TestarGestaoDeElencoETecnico();
    TestarMemoriaInstitucionalEHistorico();
    TestarPersistenciaESerializacaoClube();

    std::cout << "========================================\n";
    std::cout << " TODOS OS TESTES PASSARAM COM SUCESSO!  \n";
    std::cout << "========================================\n";

    return 0;
}
