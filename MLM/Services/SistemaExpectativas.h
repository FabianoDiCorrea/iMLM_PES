#ifndef MLM_SISTEMA_EXPECTATIVAS_H
#define MLM_SISTEMA_EXPECTATIVAS_H

#include <cstdint>
#include <string>
#include "../Domain/Clube.h"
#include "../Domain/Jogador.h"

namespace MLM {

enum class ObjetivoDiretoria : uint8_t {
    SerCampeao = 0,
    BrigarPorVagaContinental = 1,
    FicarNoTopHalf = 2,           // Metade de cima da tabela
    EvitarRebaixamento = 3,
    ReestruturacaoFinanceira = 4
};

struct MetaTemporada {
    ObjetivoDiretoria objetivoLiga;
    uint16_t posicaoMinimaExigida;
    double orcamentoDisponibilizado;
    uint8_t pressaoDiretoriaInicial; // 0 a 100
    std::wstring mensagemDiretoria;
};

class SistemaExpectativas {
public:
    SistemaExpectativas() = default;
    ~SistemaExpectativas() = default;

    // Calcula a expectativa anual com base na reputação do clube, tradição e força do elenco
    MetaTemporada GerarExpectativaTemporada(const Clube& clube, const std::vector<Jogador>& elenco) const {
        MetaTemporada meta;

        // 1. Calcula Overall Médio dos 11 melhores titulares
        float somaOverall = 0.0f;
        size_t qtd = 0;
        for (const auto& jog : elenco) {
            if (jog.ObterClubeId() == clube.ObterId()) {
                somaOverall += jog.ObterOverall();
                qtd++;
            }
        }
        float mediaElenco = (qtd > 0) ? (somaOverall / static_cast<float>(qtd)) : 65.0f;

        // 2. Pontuação composta (Reputação + Tradição + Força do Elenco)
        float score = (clube.ObterReputacao() * 0.4f) + (clube.ObterTradicaoInstitucional() * 0.2f) + (mediaElenco * 0.4f);

        if (score >= 82.0f) {
            meta.objetivoLiga = ObjetivoDiretoria::SerCampeao;
            meta.posicaoMinimaExigida = 1;
            meta.pressaoDiretoriaInicial = 70;
            meta.mensagemDiretoria = L"Nossa historia e elenco exigem o titulo nacional nesta temporada.";
        } else if (score >= 74.0f) {
            meta.objetivoLiga = ObjetivoDiretoria::BrigarPorVagaContinental;
            meta.posicaoMinimaExigida = 4;
            meta.pressaoDiretoriaInicial = 50;
            meta.mensagemDiretoria = L"O objetivo minimo e a classificacao para o torneio continental.";
        } else if (score >= 65.0f) {
            meta.objetivoLiga = ObjetivoDiretoria::FicarNoTopHalf;
            meta.posicaoMinimaExigida = 10;
            meta.pressaoDiretoriaInicial = 30;
            meta.mensagemDiretoria = L"Queremos uma campanha solida na metade superior da tabela.";
        } else {
            meta.objetivoLiga = ObjetivoDiretoria::EvitarRebaixamento;
            meta.posicaoMinimaExigida = 16;
            meta.pressaoDiretoriaInicial = 20;
            meta.mensagemDiretoria = L"A prioridade absoluta deste ano e evitar o rebaixamento.";
        }

        // Libera orçamento proporcional à saúde financeira e cobrança
        meta.orcamentoDisponibilizado = clube.ObterOrcamentoTransferencias() * (1.0 + (score / 200.0));

        return meta;
    }

    // Avalia se o técnico cumpriu a meta no final do campeonato
    bool AvaliarCumprimentoMeta(const MetaTemporada& meta, uint16_t posicaoFinalObtida, uint8_t& outNovaPressao) const {
        if (posicaoFinalObtida <= meta.posicaoMinimaExigida) {
            outNovaPressao = (meta.pressaoDiretoriaInicial >= 20) ? meta.pressaoDiretoriaInicial - 20 : 0;
            return true; // Meta cumprida
        } else {
            uint16_t diferenca = posicaoFinalObtida - meta.posicaoMinimaExigida;
            outNovaPressao = meta.pressaoDiretoriaInicial + (diferenca * 10);
            if (outNovaPressao > 100 || posicaoFinalObtida >= 17) outNovaPressao = 100; // Rebaixamento ou vexame é demissão imediata
            return false; // Meta frustrada (Risco de demissão)
        }
    }
};

} // namespace MLM

#endif // MLM_SISTEMA_EXPECTATIVAS_H
