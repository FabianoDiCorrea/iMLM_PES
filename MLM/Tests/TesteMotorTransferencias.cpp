#include <iostream>
#include <cassert>
#include <clocale>
#include "../Domain/Jogador.h"
#include "../Domain/Clube.h"
#include "../Services/MotorTransferencias.h"

using namespace MLM;

void TestarCompraRecusadaPorValorBaixo() {
    Jogador jogador(1, L"Jogador Mediano", "MEDIANO", 26, 10, PosicaoCampo::MeioCampoOfensivo, 78, 80);
    Clube vendedor(10, L"Roma", "ROM", 105);
    vendedor.ReceberValor(50000000.0);
    
    Clube comprador(20, L"Fiorentina", "FIO", 105);
    comprador.ReceberValor(30000000.0);

    MotorTransferencias motor;
    double valMercado = motor.CalcularValorMercado(jogador);

    PropostaTransferencia proposta;
    proposta.jogadorId = 1;
    proposta.clubeVendedorId = 10;
    proposta.clubeCompradorId = 20;
    proposta.valorOferecido = valMercado * 0.5; // Oferece apenas 50% do valor de mercado
    proposta.salarioOferecido = 500000.0;
    proposta.anosContratoOferecidos = 3;

    RespostaTransferencia res = motor.AvaliarProposta(proposta, jogador, vendedor, comprador);

    assert(res.status == StatusProposta::RecusadaPeloClube);
    assert(res.clubeAceitou == false);
    std::cout << "[TESTE OK] Compra Recusada por Valor Insuficiente" << std::endl;
}

void TestarRecusaDeVendaDeIdolo() {
    Jogador idolo(2, L"Lionel Messi", "MESSI", 33, 10, PosicaoCampo::PontaDireita, 93, 93);
    Clube vendedor(10, L"Inter Miami", "MIA", 100);
    vendedor.ReceberValor(50000000.0);
    vendedor.AlterarReputacao(70);

    Clube comprador(20, L"Roma", "ROM", 105);
    comprador.ReceberValor(100000000.0);
    comprador.AlterarReputacao(80);

    MotorTransferencias motor;
    double valMercado = motor.CalcularValorMercado(idolo);

    PropostaTransferencia proposta;
    proposta.jogadorId = 2;
    proposta.clubeVendedorId = 10;
    proposta.clubeCompradorId = 20;
    proposta.valorOferecido = valMercado * 1.1; // Oferece valor normal, mas clube pede sobrepreço de ídolo (+35%)
    proposta.salarioOferecido = 2000000.0;
    proposta.anosContratoOferecidos = 2;

    RespostaTransferencia res = motor.AvaliarProposta(proposta, idolo, vendedor, comprador);

    assert(res.status == StatusProposta::RecusadaPeloClube);
    assert(res.clubeAceitou == false);
    std::cout << "[TESTE OK] Clube Recusa Vender Idolo sem Sobrepreco Alto" << std::endl;
}

void TestarJogadorRecusandoTransferenciaPorClubeMenor() {
    Jogador estrela(3, L"Estrela Mundial", "ESTRELA", 25, 10, PosicaoCampo::Centroavante, 88, 92);
    Clube vendedor(10, L"Real Madrid", "RMA", 100);
    vendedor.ReceberValor(200000000.0);
    vendedor.AlterarReputacao(95);

    Clube comprador(20, L"Clube Pequeno", "CPQ", 100);
    comprador.ReceberValor(500000000.0);
    comprador.AlterarReputacao(40); // Reputação 40 vs 95

    MotorTransferencias motor;
    double valMercado = motor.CalcularValorMercado(estrela);
    double salarioExigido = motor.CalcularSalarioExigido(estrela);

    PropostaTransferencia proposta;
    proposta.jogadorId = 3;
    proposta.clubeVendedorId = 10;
    proposta.clubeCompradorId = 20;
    proposta.valorOferecido = valMercado * 2.0; // Clube aceita valor alto
    proposta.salarioOferecido = salarioExigido * 0.9; // Salário insuficiente para clube menor (exige 1.5x)

    RespostaTransferencia res = motor.AvaliarProposta(proposta, estrela, vendedor, comprador);

    assert(res.clubeAceitou == true);
    assert(res.jogadorAceitou == false);
    assert(res.status == StatusProposta::RecusadaPeloJogador);
    std::cout << "[TESTE OK] Jogador Recusa Transferencia para Clube de Reputacao Inferior" << std::endl;
}

void TestarCompraAceitaComSucesso() {
    Jogador jovem(4, L"Jovem Promessa", "JOVEM", 19, 10, PosicaoCampo::Volante, 75, 88);
    Clube vendedor(10, L"Santos", "SAN", 105);
    vendedor.ReceberValor(20000000.0);
    vendedor.AlterarReputacao(60);

    Clube comprador(20, L"Roma", "ROM", 105);
    comprador.ReceberValor(100000000.0);
    comprador.AlterarReputacao(80);

    MotorTransferencias motor;
    double valMercado = motor.CalcularValorMercado(jovem);

    PropostaTransferencia proposta;
    proposta.jogadorId = 4;
    proposta.clubeVendedorId = 10;
    proposta.clubeCompradorId = 20;
    proposta.valorOferecido = valMercado * 1.5; // Paga o sobrepreço de promessa (+40%)
    proposta.salarioOferecido = motor.CalcularSalarioExigido(jovem) * 1.2;
    proposta.anosContratoOferecidos = 4;

    RespostaTransferencia res = motor.AvaliarProposta(proposta, jovem, vendedor, comprador);

    assert(res.clubeAceitou == true);
    assert(res.jogadorAceitou == true);
    assert(res.status == StatusProposta::AceitaClubeEJogador);
    std::cout << "[TESTE OK] Negociacao Concluida e Aceita com Sucesso!" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");

    std::cout << "==================================================" << std::endl;
    std::cout << " MLM - TESTES DO MOTOR DE MERCADO DE TRANSFERENCIAS " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    TestarCompraRecusadaPorValorBaixo();
    TestarRecusaDeVendaDeIdolo();
    TestarJogadorRecusandoTransferenciaPorClubeMenor();
    TestarCompraAceitaComSucesso();

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES DO MOTOR PASSARAM COM SUCESSO!   " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
