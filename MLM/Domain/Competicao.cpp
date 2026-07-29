#include "Competicao.h"
#include <algorithm>

namespace MLM {

Competicao::Competicao(uint32_t id,
                       const std::wstring& nome,
                       const std::string& nomeAbreviado,
                       uint16_t anoTemporada,
                       TipoCompeticao tipo,
                       size_t limiteMaximoClubes)
    : m_id(id),
      m_nome(nome),
      m_nomeAbreviado(nomeAbreviado),
      m_anoTemporada(anoTemporada),
      m_tipo(tipo),
      m_situacao(SituacaoCompeticao::Planejada),
      m_limiteMaximoClubes(limiteMaximoClubes)
{
}

bool Competicao::Iniciar()
{
    if (m_situacao != SituacaoCompeticao::Planejada) {
        return false; // Só inicia se estiver em Planejada
    }
    if (m_clubesIds.empty()) {
        return false; // Não inicia competição sem clubes
    }

    m_situacao = SituacaoCompeticao::EmAndamento;
    return true;
}

bool Competicao::Encerrar()
{
    if (m_situacao != SituacaoCompeticao::EmAndamento) {
        return false; // Só encerra se estiver Em Andamento
    }

    m_situacao = SituacaoCompeticao::Encerrada;
    return true;
}

bool Competicao::AdicionarClube(uint32_t clubeId)
{
    if (m_situacao != SituacaoCompeticao::Planejada) {
        return false; // Bloqueado: Não altera participantes após início
    }
    if (clubeId == 0) {
        return false;
    }
    if (m_clubesIds.size() >= m_limiteMaximoClubes) {
        return false; // Bloqueado: Limite atingido
    }
    if (PossuiClube(clubeId)) {
        return false; // Bloqueado: Duplicidade
    }

    m_clubesIds.push_back(clubeId);
    return true;
}

bool Competicao::RemoverClube(uint32_t clubeId)
{
    if (m_situacao != SituacaoCompeticao::Planejada) {
        return false; // Bloqueado: Não altera participantes após início
    }

    auto it = std::find(m_clubesIds.begin(), m_clubesIds.end(), clubeId);
    if (it == m_clubesIds.end()) {
        return false; // Clube não encontrado
    }

    m_clubesIds.erase(it);
    return true;
}

bool Competicao::PossuiClube(uint32_t clubeId) const
{
    return std::find(m_clubesIds.begin(), m_clubesIds.end(), clubeId) != m_clubesIds.end();
}

bool Competicao::AdicionarPartida(const Partida& partida)
{
    if (PossuiPartida(partida.ObterId())) {
        return false; // Evita partidas duplicadas na mesma competição
    }

    m_partidas.push_back(partida);
    return true;
}

bool Competicao::PossuiPartida(uint32_t partidaId) const
{
    for (const auto& p : m_partidas) {
        if (p.ObterId() == partidaId) {
            return true;
        }
    }
    return false;
}

} // namespace MLM
