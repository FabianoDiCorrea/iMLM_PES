#ifndef MLM_MOTOR_CARREIRA_TECNICO_H
#define MLM_MOTOR_CARREIRA_TECNICO_H

#include <cstdint>
#include <vector>
#include <string>
#include "../Domain/Tecnico.h"
#include "../Domain/Clube.h"
#include "SistemaExpectativas.h"

namespace MLM {

struct OfertaEmpregoTecnico {
    uint32_t clubeId;
    std::wstring nomeClube;
    double salarioOferecido;
    uint16_t tempoContratoMeses;
    MetaTemporada metaProposta;
};

class MotorCarreiraTecnico {
public:
    MotorCarreiraTecnico() = default;
    ~MotorCarreiraTecnico() = default;

    // Avalia demissão ou manutenção do técnico após término da liga
    bool AvaliarDesempenhoETomadaDecisao(Tecnico& tecnico, 
                                          Clube& clube, 
                                          const MetaTemporada& meta, 
                                          uint16_t posicaoFinal,
                                          uint32_t anoTemporada,
                                          std::wstring& outMensagem) const {
        uint8_t novaPressao = 0;
        SistemaExpectativas expectativas;
        bool cumpriuMeta = expectativas.AvaliarCumprimentoMeta(meta, posicaoFinal, novaPressao);

        if (cumpriuMeta) {
            tecnico.AlterarReputacao(5); // +5 de reputação por sucesso
            tecnico.RegistrarPartida(0, 0); // Atualiza métricas
            if (posicaoFinal == 1) {
                tecnico.AlterarReputacao(10); // +10 por título
            }
            outMensagem = L"A diretoria esta extremamente satisfeita e confirma a permanencia do treinador.";
            return true;
        } else {
            tecnico.AlterarReputacao(-8); // Perde reputação por insucesso
            if (novaPressao >= 90) {
                // Demissão por mau desempenho
                tecnico.DeixarClube(anoTemporada, true, false);
                clube.RemoverTecnico();
                outMensagem = L"Devido ao nao cumprimento das metas e alta pressao, o treinador foi DEMITIDO.";
                return false;
            } else {
                outMensagem = L"A diretoria concede uma ultima chance, mas exige resultados imediatos.";
                return true;
            }
        }
    }

    // Gera ofertas de emprego de outros clubes para o técnico (Humano ou IA) com base na sua reputação
    std::vector<OfertaEmpregoTecnico> GerarOfertasEmprego(const Tecnico& tecnico,
                                                          const std::vector<Clube>& clubesLivres,
                                                          const std::vector<Jogador>& todosJogadores) const {
        std::vector<OfertaEmpregoTecnico> ofertas;
        SistemaExpectativas expectativas;

        for (const auto& clube : clubesLivres) {
            // O clube contrata apenas se a reputação do técnico for compatível (margem de até -15 pontos)
            if (clube.ObterTecnicoId() == 0 && tecnico.ObterReputacao() >= (clube.ObterReputacao() - 15)) {
                OfertaEmpregoTecnico oferta;
                oferta.clubeId = clube.ObterId();
                oferta.nomeClube = clube.ObterNome();
                oferta.salarioOferecido = (clube.ObterFolhaSalarialMensal() * 0.05) + (tecnico.ObterReputacao() * 1000.0);
                oferta.tempoContratoMeses = 12;
                oferta.metaProposta = expectativas.GerarExpectativaTemporada(clube, todosJogadores);

                ofertas.push_back(oferta);
            }
        }

        return ofertas;
    }
};

} // namespace MLM

#endif // MLM_MOTOR_CARREIRA_TECNICO_H
