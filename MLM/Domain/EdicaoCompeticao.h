#ifndef MLM_EDICAO_COMPETICAO_H
#define MLM_EDICAO_COMPETICAO_H

#include <string>
#include <vector>
#include <cstdint>
#include "CompeticaoTipos.h"
#include "Competicao.h"

namespace MLM {

struct ParticipanteEdicao {
    uint32_t clubeId;
    uint16_t posicaoFinal = 0;
    double premiacaoRecebida = 0.0;
};

class EdicaoCompeticao {
public:
    EdicaoCompeticao(uint32_t id,
                     uint32_t competicaoId,
                     uint16_t anoTemporada,
                     const std::wstring& nomeEdicao);

    ~EdicaoCompeticao() = default;

    // Getters de Leitura
    uint32_t ObterId() const { return m_id; }
    uint32_t ObterCompeticaoId() const { return m_competicaoId; }
    uint16_t ObterAnoTemporada() const { return m_anoTemporada; }
    std::wstring ObterNomeEdicao() const { return m_nomeEdicao; }
    uint32_t ObterCampeaoId() const { return m_campeaoId; }
    uint32_t ObterViceId() const { return m_viceId; }
    bool EstaEncerrada() const { return m_encerrada; }

    // Métodos de Domínio
    bool RegistrarParticipante(uint32_t clubeId);
    bool DefinirResultadoFinal(uint32_t campeaoId, uint32_t viceId);
    bool PossuiParticipante(uint32_t clubeId) const;
    size_t QuantidadeParticipantes() const { return m_participantes.size(); }
    const std::vector<ParticipanteEdicao>& ObterParticipantes() const { return m_participantes; }

private:
    uint32_t m_id;
    uint32_t m_competicaoId;
    uint16_t m_anoTemporada;
    std::wstring m_nomeEdicao;
    
    uint32_t m_campeaoId;
    uint32_t m_viceId;
    bool m_encerrada;

    std::vector<ParticipanteEdicao> m_participantes;
};

} // namespace MLM

#endif // MLM_EDICAO_COMPETICAO_H
