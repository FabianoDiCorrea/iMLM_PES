#ifndef MLM_MOTOR_BASE_E_REGENS_H
#define MLM_MOTOR_BASE_E_REGENS_H

#include <cstdint>
#include <vector>
#include <string>
#include <random>
#include "../Domain/Jogador.h"
#include "../Domain/Clube.h"

namespace MLM {

struct PromessaBase {
    uint32_t idVirtual;
    std::wstring nome;
    std::string nomeCamisa;
    uint8_t idade;
    PosicaoCampo posicao;
    uint8_t overallAtual;
    uint8_t potencial;
    uint32_t clubeId;
};

class MotorBaseERegens {
public:
    MotorBaseERegens() {
        std::random_device rd;
        m_rng.seed(rd());
    }

    explicit MotorBaseERegens(uint32_t seed) {
        m_rng.seed(seed);
    }

    ~MotorBaseERegens() = default;

    // Gera novos jovens talentos para a Academia Virtual de Base do Clube
    std::vector<PromessaBase> GerarSafraBase(uint32_t clubeId, uint8_t quantidade = 3) {
        std::vector<PromessaBase> safra;
        safra.reserve(quantidade);

        static const std::vector<std::wstring> nomes = { L"Gabriel", L"Lucas", L"Matheus", L"Enzo", L"Thiago", L"Felipe", L"Rodrigo", L"Leo" };
        static const std::vector<std::wstring> sobrenomes = { L"Silva", L"Santos", L"Oliveira", L"Souza", L"Lima", L"Ferreira", L"Costa", L"Pereira" };

        std::uniform_int_distribution<size_t> distN(0, nomes.size() - 1);
        std::uniform_int_distribution<size_t> distS(0, sobrenomes.size() - 1);
        std::uniform_int_distribution<int> distPos(0, 12);
        std::uniform_int_distribution<int> distOv(58, 68);
        std::uniform_int_distribution<int> distPot(78, 90);

        static uint32_t proximoIdVirtual = 900000;

        for (uint8_t i = 0; i < quantidade; ++i) {
            PromessaBase p;
            p.idVirtual = proximoIdVirtual++;
            p.nome = nomes[distN(m_rng)] + L" " + sobrenomes[distS(m_rng)];
            
            // Converte wstring para string ascii simples para camisa
            p.nomeCamisa = "JOVEM " + std::to_string(i + 1);
            p.idade = 16;
            p.posicao = static_cast<PosicaoCampo>(distPos(m_rng));
            p.overallAtual = static_cast<uint8_t>(distOv(m_rng));
            p.potencial = static_cast<uint8_t>(distPot(m_rng));
            p.clubeId = clubeId;

            safra.push_back(p);
        }

        return safra;
    }

    // Promove um garoto da base para o Elenco Profissional (Respeitando o limite de 40 jogadores do PES)
    bool PromoverParaProfissional(const PromessaBase& promessa, 
                                  Clube& clube, 
                                  uint32_t novoIdPesEdit,
                                  uint16_t numeroCamisa,
                                  Jogador& outJogadorPromovido,
                                  std::wstring& outMensagem) {
        if (clube.QuantidadeJogadores() >= Clube::LIMITE_MAXIMO_ELENCO) {
            outMensagem = L"Promocao cancelada: O clube ja atingiu o limite maximo de 40 jogadores no elenco do PES.";
            return false;
        }

        outJogadorPromovido = Jogador(novoIdPesEdit, 
                                      promessa.nome, 
                                      promessa.nomeCamisa, 
                                      promessa.idade, 
                                      clube.ObterPaisId(), 
                                      promessa.posicao, 
                                      promessa.overallAtual, 
                                      promessa.potencial);

        outJogadorPromovido.AlterarClube(clube.ObterId(), numeroCamisa);
        clube.AdicionarJogador(&outJogadorPromovido, numeroCamisa);

        outMensagem = L"Promocao realizada com sucesso! O jogador foi integrado ao elenco oficial do PES.";
        return true;
    }

private:
    std::mt19937 m_rng;
};

} // namespace MLM

#endif // MLM_MOTOR_BASE_E_REGENS_H
