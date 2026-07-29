#include "Partida.h"

namespace MLM {

Partida::Partida(uint32_t id,
                 uint32_t mandanteId,
                 uint32_t visitanteId,
                 uint16_t anoTemporada,
                 uint32_t competicaoId,
                 uint16_t rodada)
    : m_id(id),
      m_mandanteId(mandanteId),
      m_visitanteId(visitanteId),
      m_anoTemporada(anoTemporada),
      m_competicaoId(competicaoId),
      m_rodada(rodada),
      m_golsMandante(0),
      m_golsVisitante(0),
      m_situacao(SituacaoPartida::Agendada)
{
    // Validação: Clube Mandante não pode ser igual ao Clube Visitante
    if (m_mandanteId == m_visitanteId && m_mandanteId != 0) {
        m_situacao = SituacaoPartida::Cancelada; // Cancela por inconsistência
    }
}

bool Partida::Iniciar()
{
    if (m_situacao != SituacaoPartida::Agendada) {
        return false; // Só pode iniciar se estiver Agendada
    }
    if (m_mandanteId == m_visitanteId || m_mandanteId == 0 || m_visitanteId == 0) {
        return false; // Não inicia partidas com clubes inválidos
    }

    m_situacao = SituacaoPartida::EmAndamento;
    return true;
}

bool Partida::RegistrarGolMandante()
{
    if (m_situacao != SituacaoPartida::EmAndamento) {
        return false; // Não permite registrar gol fora do andamento
    }
    m_golsMandante++;
    return true;
}

bool Partida::RegistrarGolVisitante()
{
    if (m_situacao != SituacaoPartida::EmAndamento) {
        return false;
    }
    m_golsVisitante++;
    return true;
}

bool Partida::DefinirPlacarDirecto(uint16_t golsMandante, uint16_t golsVisitante)
{
    if (m_situacao == SituacaoPartida::Finalizada || m_situacao == SituacaoPartida::Cancelada) {
        return false; // Não altera placar após finalizada ou cancelada
    }

    m_golsMandante = golsMandante;
    m_golsVisitante = golsVisitante;
    return true;
}

bool Partida::Finalizar()
{
    if (m_situacao != SituacaoPartida::EmAndamento && m_situacao != SituacaoPartida::Agendada) {
        return false; // Não finaliza se já estiver finalizada ou cancelada
    }

    m_situacao = SituacaoPartida::Finalizada;
    return true;
}

bool Partida::Cancelar()
{
    if (m_situacao == SituacaoPartida::Finalizada) {
        return false; // Não cancela partida já finalizada
    }

    m_situacao = SituacaoPartida::Cancelada;
    return true;
}

bool Partida::Empatada() const
{
    if (m_situacao != SituacaoPartida::Finalizada && m_situacao != SituacaoPartida::EmAndamento) {
        return false;
    }
    return m_golsMandante == m_golsVisitante;
}

ResultadoPartida Partida::ObterResultado() const
{
    if (m_situacao != SituacaoPartida::Finalizada && m_situacao != SituacaoPartida::EmAndamento) {
        return ResultadoPartida::Indefinido;
    }
    if (m_golsMandante > m_golsVisitante) {
        return ResultadoPartida::VitoriaMandante;
    }
    if (m_golsVisitante > m_golsMandante) {
        return ResultadoPartida::VitoriaVisitante;
    }
    return ResultadoPartida::Empate;
}

uint32_t Partida::ObterVencedorId() const
{
    ResultadoPartida res = ObterResultado();
    if (res == ResultadoPartida::VitoriaMandante) {
        return m_mandanteId;
    }
    if (res == ResultadoPartida::VitoriaVisitante) {
        return m_visitanteId;
    }
    return 0; // Empate ou Indefinido
}

} // namespace MLM
