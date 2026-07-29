#include <iostream>
#include <cassert>
#include <clocale>
#include <vector>
#include "../Domain/Temporada.h"
#include "../Services/MotorTemporada.h"

using namespace MLM;

void TestarMaquinaDeEstadosETransicao() {
    Temporada temp2026(2026, 1);
    MotorTemporada motor(temp2026);

    assert(motor.ObterFaseAtual() == FaseTemporada::PreTemporada);
    assert(motor.ObterAnoTemporada() == 2026);

    // 1. Transição para Janela de Transferências
    bool ok1 = motor.AvancarFase();
    assert(ok1 == true);
    assert(motor.ObterFaseAtual() == FaseTemporada::JanelaTransferencias);

    // 2. Transição para EmCompetição
    bool ok2 = motor.AvancarFase();
    assert(ok2 == true);
    assert(motor.ObterFaseAtual() == FaseTemporada::EmCompeticao);

    // 3. Transição para Pós-Temporada
    bool ok3 = motor.AvancarFase();
    assert(ok3 == true);
    assert(motor.ObterFaseAtual() == FaseTemporada::PosTemporada);

    // 4. Encerrar
    bool ok4 = motor.AvancarFase();
    assert(ok4 == true);
    assert(motor.ObterFaseAtual() == FaseTemporada::Encerrada);

    // Tentativa de avançar após encerrada
    bool okInv = motor.AvancarFase();
    assert(okInv == false);

    std::cout << "[TESTE OK] Maquina de Estados e Transicao da Temporada" << std::endl;
}

void TestarExecucaoPreTemporadaEEvolucao() {
    Temporada temp2026(2026, 1);
    MotorTemporada motor(temp2026);

    std::vector<Jogador> jogadores;
    jogadores.emplace_back(1, L"Jovem Atleta", "JOVEM", 18, 10, PosicaoCampo::PontaEsquerda, 70, 88);

    std::vector<DesempenhoTemporada> desempenhos;
    DesempenhoTemporada d;
    d.partidasDisputadas = 35; // Aumentado para garantir pontuação de ganho
    d.minutosJogados = 2800;
    d.notaMediaAtuacao = 7.8f;
    desempenhos.push_back(d);

    // Executa Pré-Temporada
    bool okPre = motor.ExecutarPreTemporada(jogadores, desempenhos);
    assert(okPre == true);

    // Valida envelhecimento (+1 ano) e evolução de overall
    assert(jogadores[0].ObterIdade() == 19);
    assert(jogadores[0].ObterOverall() >= 70);

    std::cout << "[TESTE OK] Execucao de Pre-Temporada, Aniversario e Evolucao" << std::endl;
}

void TestarBloqueioDeFasesInvalidas() {
    Temporada temp2026(2026, 1);
    MotorTemporada motor(temp2026);

    std::vector<Competicao> competicoes;
    
    // Tenta executar competições enquanto está na Pré-Temporada (Inválido)
    bool okCompInvalida = motor.ExecutarCompeticoes(competicoes, ModoResolucaoCompeticao::ModoHumano);
    assert(okCompInvalida == false);

    // Avança para EmCompeticao
    motor.AvancarFase(); // JanelaTransferencias
    motor.AvancarFase(); // EmCompeticao

    // Agora sim pode executar competições
    bool okCompValida = motor.ExecutarCompeticoes(competicoes, ModoResolucaoCompeticao::ModoHumano);
    assert(okCompValida == true);

    std::cout << "[TESTE OK] Bloqueio de Fases Invalidas na Maquina de Estados" << std::endl;
}

void TestarViradaDeAno() {
    Temporada temp2026(2026, 1);
    MotorTemporada motor(temp2026);

    motor.AvancarFase(); // Janela
    motor.AvancarFase(); // EmCompeticao
    motor.AvancarFase(); // PosTemporada
    motor.AvancarFase(); // Encerrada

    Temporada temp2027 = motor.ConcluirEVirarTemporada();

    assert(temp2027.ObterAno() == 2027);
    assert(temp2027.ObterNumeroSequencial() == 2);
    assert(temp2027.EstaPlanejada() == true);

    std::cout << "[TESTE OK] Conclusao de Ciclo Anual e Criacao da Temporada 2027" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << "  MLM - TESTES DO MOTOR DE FLUXO DA TEMPORADA     " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarMaquinaDeEstadosETransicao();
    TestarExecucaoPreTemporadaEEvolucao();
    TestarBloqueioDeFasesInvalidas();
    TestarViradaDeAno();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES DO MOTOR PASSARAM COM SUCESSO!   " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
