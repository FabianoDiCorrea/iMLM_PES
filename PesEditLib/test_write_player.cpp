#include <iostream>
#include <cassert>
#include "EditDatabase.h"
#include "PlayerManager.h"

int main()
{
    std::cout << "==================================================" << std::endl;
    std::cout << " PesEditLib - TESTES AUTOMATIZADOS DE ESCRITA    " << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;

    const std::string original_edit = "../PesTransferTest/EDIT00000000";
    const std::string test_edit = "../EDIT_TEST_WRITE00000000";

    EditDatabase db;
    if (!db.Load(original_edit)) {
        std::cerr << "[ERRO] Falha ao carregar o arquivo " << original_edit << std::endl;
        return 1;
    }

    uint32_t messi_id = 7511; // Lionel Messi
    Player* messi = db.Players().Get(messi_id);
    if (!messi) {
        std::cerr << "[ERRO] Messi nao encontrado no banco." << std::endl;
        return 1;
    }

    std::wcout << L"[ORIGINAL] Nome: " << messi->name
               << L" | Idade: " << (int)messi->age
               << L" | Altura: " << (int)messi->height << L" cm"
               << L" | Peso: " << (int)messi->weight << L" kg"
               << L" | Velocidade: " << (int)messi->speed
               << L" | Overall: " << (int)messi->overall << std::endl;

    // 1. Modifica Atributos
    uint8_t nova_idade = 38;
    uint8_t nova_altura = 172;
    uint8_t novo_peso = 74;
    uint8_t nova_velocidade = 92;

    db.Players().UpdatePlayerAge(messi_id, nova_idade);
    db.Players().UpdatePlayerHeight(messi_id, nova_altura);
    db.Players().UpdatePlayerWeight(messi_id, novo_peso);

    messi->speed = nova_velocidade;
    db.Players().UpdatePlayerSkills(messi_id, *messi);

    // 2. Salva em novo arquivo de teste
    if (!db.Save(test_edit)) {
        std::cerr << "[ERRO] Falha ao salvar o novo binario em " << test_edit << std::endl;
        return 1;
    }
    std::cout << "[OK] Binario alterado e salvo em: " << test_edit << std::endl;

    // 3. Reabre o arquivo salvo para confirmação de persistência
    EditDatabase db_reopen;
    if (!db_reopen.Load(test_edit)) {
        std::cerr << "[ERRO] Falha ao reabrir o arquivo de teste " << test_edit << std::endl;
        return 1;
    }

    Player* messi_relido = db_reopen.Players().Get(messi_id);
    assert(messi_relido != nullptr);

    std::wcout << L"[RELIDO]   Nome: " << messi_relido->name
               << L" | Idade: " << (int)messi_relido->age
               << L" | Altura: " << (int)messi_relido->height << L" cm"
               << L" | Peso: " << (int)messi_relido->weight << L" kg"
               << L" | Velocidade: " << (int)messi_relido->speed
               << L" | Overall: " << (int)messi_relido->overall << std::endl;

    // Validações dos Atributos Alterados
    assert(messi_relido->age == nova_idade);
    assert(messi_relido->height == nova_altura);
    assert(messi_relido->weight == novo_peso);
    assert(messi_relido->speed == nova_velocidade);

    std::cout << std::endl << "==================================================" << std::endl;
    std::cout << "  TODOS OS TESTES DE ESCRITA PASSARAM COM SUCESSO! " << std::endl;
    std::cout << "==================================================" << std::endl;

    return 0;
}
