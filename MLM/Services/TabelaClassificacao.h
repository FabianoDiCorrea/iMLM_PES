#ifndef MLM_TABELA_CLASSIFICACAO_H
#define MLM_TABELA_CLASSIFICACAO_H

#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include "../Domain/Partida.h"
#include "MotorSimulacao.h"

namespace MLM {

struct ItemTabela {
    uint32_t clubeId;
    std::wstring nomeClube;
    uint16_t posicao = 0;
    uint16_t pontos = 0;
    uint16_t jogos = 0;
    uint16_t vitorias = 0;
    uint16_t empates = 0;
    uint16_t derrotas = 0;
    uint16_t golsPro = 0;
    uint16_t golsContra = 0;
    int16_t saldoGols = 0;
    float aproveitamentoPct = 0.0f;
};

class TabelaClassificacao {
public:
    TabelaClassificacao() = default;
    ~TabelaClassificacao() = default;

    void AdicionarClube(uint32_t clubeId, const std::wstring& nomeClube) {
        ItemTabela item;
        item.clubeId = clubeId;
        item.nomeClube = nomeClube;
        m_itens.push_back(item);
    }

    void ProcessarResultadoPartida(uint32_t mandanteId, uint32_t visitanteId, uint16_t golsMandante, uint16_t golsVisitante) {
        ItemTabela* mandante = nullptr;
        ItemTabela* visitante = nullptr;

        for (auto& item : m_itens) {
            if (item.clubeId == mandanteId) mandante = &item;
            if (item.clubeId == visitanteId) visitante = &item;
        }

        if (!mandante || !visitante) return;

        mandante->jogos++;
        visitante->jogos++;
        mandante->golsPro += golsMandante;
        mandante->golsContra += golsVisitante;
        visitante->golsPro += golsVisitante;
        visitante->golsContra += golsMandante;

        mandante->saldoGols = mandante->golsPro - mandante->golsContra;
        visitante->saldoGols = visitante->golsPro - visitante->golsContra;

        if (golsMandante > golsVisitante) {
            mandante->pontos += 3;
            mandante->vitorias++;
            visitante->derrotas++;
        } else if (golsVisitante > golsMandante) {
            visitante->pontos += 3;
            visitante->vitorias++;
            mandante->derrotas++;
        } else {
            mandante->pontos += 1;
            visitante->pontos += 1;
            mandante->empates++;
            visitante->empates++;
        }
    }

    void AtualizarClassificacao() {
        for (auto& item : m_itens) {
            if (item.jogos > 0) {
                item.aproveitamentoPct = (static_cast<float>(item.pontos) / static_cast<float>(item.jogos * 3)) * 100.0f;
            } else {
                item.aproveitamentoPct = 0.0f;
            }
        }

        // Ordena por: Pontos > Vitórias > Saldo de Gols > Gols Pró
        std::sort(m_itens.begin(), m_itens.end(), [](const ItemTabela& a, const ItemTabela& b) {
            if (a.pontos != b.pontos) return a.pontos > b.pontos;
            if (a.vitorias != b.vitorias) return a.vitorias > b.vitorias;
            if (a.saldoGols != b.saldoGols) return a.saldoGols > b.saldoGols;
            return a.golsPro > b.golsPro;
        });

        for (uint16_t i = 0; i < m_itens.size(); ++i) {
            m_itens[i].posicao = i + 1;
        }
    }

    const std::vector<ItemTabela>& ObterItens() const { return m_itens; }

    uint32_t ObterCampeaoId() const {
        return m_itens.empty() ? 0 : m_itens.front().clubeId;
    }

    std::vector<uint32_t> ObterRebaixadosIds(uint16_t qtdRebaixados) const {
        std::vector<uint32_t> rebaixados;
        if (m_itens.size() < qtdRebaixados) return rebaixados;

        for (size_t i = m_itens.size() - qtdRebaixados; i < m_itens.size(); ++i) {
            rebaixados.push_back(m_itens[i].clubeId);
        }
        return rebaixados;
    }

    std::vector<uint32_t> ObterPromovidosIds(uint16_t qtdPromovidos) const {
        std::vector<uint32_t> promovidos;
        size_t limite = std::min(static_cast<size_t>(qtdPromovidos), m_itens.size());

        for (size_t i = 0; i < limite; ++i) {
            promovidos.push_back(m_itens[i].clubeId);
        }
        return promovidos;
    }

private:
    std::vector<ItemTabela> m_itens;
};

} // namespace MLM

#endif // MLM_TABELA_CLASSIFICACAO_H
