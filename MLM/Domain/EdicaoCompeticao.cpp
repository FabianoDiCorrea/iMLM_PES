#include "EdicaoCompeticao.h"
#include <algorithm>

namespace MLM {

EdicaoCompeticao::EdicaoCompeticao(uint32_t id,
                                   uint32_t competicaoId,
                                   uint16_t anoTemporada,
                                   const std::wstring& nomeEdicao)
    : m_id(id),
      m_competicaoId(competicaoId),
      m_anoTemporada(anoTemporada),
      m_nomeEdicao(nomeEdicao),
      m_campeaoId(0),
      m_viceId(0),
      m_encerrada(false)
{
}

bool EdicaoCompeticao::RegistrarParticipante(uint32_t clubeId)
{
    if (m_encerrada || clubeId == 0) {
        return false;
    }
    if (PossuiParticipante(clubeId)) {
        return false; // Evita duplicados
    }

    ParticipanteEdicao part;
    part.clubeId = clubeId;
    m_participantes.push_back(part);
    return true;
}

bool EdicaoCompeticao::DefinirResultadoFinal(uint32_t campeaoId, uint32_t viceId)
{
    if (m_encerrada || campeaoId == 0 || viceId == 0 || campeaoId == viceId) {
        return false;
    }
    if (!PossuiParticipante(campeaoId) || !PossuiParticipante(viceId)) {
        return false;
    }

    m_campeaoId = campeaoId;
    m_viceId = viceId;
    m_encerrada = true;
    return true;
}

bool EdicaoCompeticao::PossuiParticipante(uint32_t clubeId) const
{
    for (const auto& p : m_participantes) {
        if (p.clubeId == clubeId) return true;
    }
    return false;
}

} // namespace MLM
