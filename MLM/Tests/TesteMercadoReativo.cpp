#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/MotorTransferencias.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DO MERCADO REATIVO DE TRANSFERENCIAS " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar Clubes (Campeão, Rebaixado e Neutro)
    Clube palmeiras(101, L"Palmeiras", "PAL", 1);
    palmeiras.AlterarReputacao(85);
    palmeiras.ReceberValor(50000000.0);

    Clube vasco(102, L"Vasco", "VAS", 1);
    vasco.AlterarReputacao(70);

    Clube flamengo(103, L"Flamengo", "FLA", 1);
    flamengo.AlterarReputacao(88);
    flamengo.ReceberValor(60000000.0);

    std::vector<Clube> clubes = { palmeiras, vasco, flamengo };

    // 2. Criar Jogadores
    // Payet: Estrela do Vasco (Overall 81, clube será rebaixado)
    Jogador payet(1, L"Dimitri Payet", "Payet", 37, 1, PosicaoCampo::MeioCampoOfensivo, 81, 81);
    payet.AlterarClube(102, 10);

    // Endrick: Jovem promessa do Santos (Overall 78, Potencial 90, 18 anos)
    Jogador promessa(2, L"Estevão", "Estevao", 17, 1, PosicaoCampo::PontaDireita, 78, 90);
    promessa.AlterarClube(103, 41);

    std::vector<Jogador> todosJogadores = { payet, promessa };

    // 3. IDs de campeões e rebaixados da temporada
    std::vector<uint32_t> rebaixados = { 102 }; // Vasco rebaixado
    std::vector<uint32_t> campeoes = { 101 };   // Palmeiras campeão

    // 4. Rodar o Mercado Reativo
    MotorTransferencias motor;
    auto propostas = motor.GerarMercadoReativo(clubes, todosJogadores, rebaixados, campeoes);

    std::cout << "[MERCADO] Propostas Reativas Geradas pela IA: " << propostas.size() << std::endl << std::endl;

    for (const auto& prop : propostas) {
        std::cout << " -> Proposta pelo Jogador ID: " << prop.jogadorId << std::endl;
        std::cout << "    Clube Vendedor: " << prop.clubeVendedorId << std::endl;
        std::cout << "    Clube Comprador: " << prop.clubeCompradorId << std::endl;
        std::cout << "    Valor Oferecido: R$ " << prop.valorOferecido << std::endl;
        std::cout << "    Salario Mensal: R$ " << prop.salarioOferecido << std::endl << std::endl;
    }

    assert(!propostas.empty());

    std::cout << "[SUCESSO] Teste do Mercado Reativo concluído com êxito!" << std::endl;
    return 0;
}
