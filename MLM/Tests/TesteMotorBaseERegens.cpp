#include <iostream>
#include <cassert>
#include <vector>
#include "../Services/MotorBaseERegens.h"
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

using namespace MLM;

int main() {
    std::cout << "===========================================" << std::endl;
    std::cout << "   TESTE DA ACADEMIA DE BASE E PROMOCAO    " << std::endl;
    std::cout << "===========================================" << std::endl << std::endl;

    // 1. Criar Clube
    Clube santos(101, L"Santos FC", "SAN", 1);
    
    // Preenche o clube com 39 jogadores fictícios para testar a trava do limite de 40 no PES
    std::vector<Jogador> elencoProfissional;
    elencoProfissional.reserve(40);

    for (uint32_t i = 1; i <= 39; ++i) {
        elencoProfissional.emplace_back(i, L"Jogador " + std::to_wstring(i), "JOG", 25, 1, PosicaoCampo::Zagueiro, 70, 75);
        elencoProfissional.back().AlterarClube(101, static_cast<uint16_t>(i));
        santos.AdicionarJogador(&elencoProfissional.back(), static_cast<uint16_t>(i));
    }

    std::cout << "[CLUBE] Jogadores atuais no elenco oficial do PES: " << santos.QuantidadeJogadores() << " / 40" << std::endl;
    assert(santos.QuantidadeJogadores() == 39);

    // 2. Gerar Safra da Base Virtual no iMLM
    MotorBaseERegens motorBase(12345);
    auto safra = motorBase.GerarSafraBase(101, 2);

    std::cout << std::endl << "[BASE VIRTUAL] Promessas geradas na Academia de Base:" << std::endl;
    for (const auto& garoto : safra) {
        std::wcout << L" -> Garoto: " << garoto.nome << L" | Idade: " << garoto.idade 
                   << L" | Overall: " << garoto.overallAtual << L" | Potencial: " << garoto.potencial << std::endl;
    }

    // 3. Promover a 1ª Promessa (Deve ter SUCESSO: o time vai para 40/40)
    Jogador promovido1(0, L"", "", 0, 0, PosicaoCampo::Desconhecida, 0, 0);
    std::wstring msg;
    bool ok1 = motorBase.PromoverParaProfissional(safra[0], santos, 5001, 40, promovido1, msg);

    std::wcout << std::endl << L"[PROMOTING 1] " << msg << std::endl;
    std::cout << "[CLUBE] Novo total no elenco do PES: " << santos.QuantidadeJogadores() << " / 40" << std::endl;
    assert(ok1);
    assert(santos.QuantidadeJogadores() == 40);

    // 4. Tentar Promover a 2ª Promessa (Deve FALHAR: trava ativada no 40º jogador)
    Jogador promovido2(0, L"", "", 0, 0, PosicaoCampo::Desconhecida, 0, 0);
    bool ok2 = motorBase.PromoverParaProfissional(safra[1], santos, 5002, 41, promovido2, msg);

    std::wcout << std::endl << L"[PROMOTING 2 - TRAVA DE SEGURANCA] " << msg << std::endl;
    assert(!ok2); // Bloqueado com sucesso!
    assert(santos.QuantidadeJogadores() == 40);

    std::cout << std::endl << "[SUCESSO] Teste da Academia de Base e Trava PES 40 atletas concluído com êxito!" << std::endl;
    return 0;
}
