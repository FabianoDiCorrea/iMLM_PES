#include "Temporada.h"

namespace MLM {

Temporada::Temporada(uint16_t ano, uint16_t numeroSequencial)
    : m_ano(ano),
      m_numeroSequencial(numeroSequencial),
      m_situacao(SituacaoTemporada::Planejada)
{
}

bool Temporada::Iniciar()
{
    if (m_situacao != SituacaoTemporada::Planejada) {
        return false; // Só pode iniciar se estiver em estado de Planejada
    }

    m_situacao = SituacaoTemporada::EmAndamento;
    return true;
}

bool Temporada::Encerrar()
{
    if (m_situacao != SituacaoTemporada::EmAndamento) {
        return false; // Só pode encerrar se estiver Em Andamento
    }

    m_situacao = SituacaoTemporada::Encerrada;
    return true;
}

Temporada Temporada::CriarProximaTemporada() const
{
    // A nova temporada é gerada para o ano seguinte com a sequência incrementada
    return Temporada(m_ano + 1, m_numeroSequencial + 1);
}

} // namespace MLM
