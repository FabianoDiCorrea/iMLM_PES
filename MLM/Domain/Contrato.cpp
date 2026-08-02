#include "Contrato.h"

namespace MLM {

Contrato::Contrato(uint32_t id,
                   TipoContrato tipo,
                   uint32_t entidadeId,
                   uint32_t clubeId,
                   uint32_t anoInicio,
                   uint16_t duracaoMeses,
                   double salarioMensal,
                   double multaRescisoria,
                   BonusContratual bonus)
    : m_id(id),
      m_tipo(tipo),
      m_entidadeId(entidadeId),
      m_clubeId(clubeId),
      m_clubeOrigemEmprestimoId(0),
      m_anoInicio(anoInicio),
      m_duracaoMeses(duracaoMeses),
      m_salarioMensal(salarioMensal),
      m_multaRescisoria(multaRescisoria),
      m_bonus(bonus),
      m_status(StatusContrato::Ativo),
      m_renovavel(true)
{
}

bool Contrato::Renovar(uint16_t mesesAdicionais, double novoSalarioMensal, double novaMulta, uint32_t anoTemporada)
{
    if (m_status != StatusContrato::Ativo || !m_renovavel) {
        return false;
    }

    m_duracaoMeses += mesesAdicionais;
    m_salarioMensal = novoSalarioMensal;
    m_multaRescisoria = novaMulta;

    m_historicoRenovacoes.emplace_back(anoTemporada, mesesAdicionais, novoSalarioMensal, novaMulta);
    return true;
}

bool Contrato::Rescindir()
{
    if (m_status == StatusContrato::Encerrado || m_status == StatusContrato::Rescindido) {
        return false;
    }

    m_status = StatusContrato::Rescindido;
    return true;
}

bool Contrato::Encerrar()
{
    if (m_status == StatusContrato::Encerrado || m_status == StatusContrato::Rescindido) {
        return false;
    }

    m_status = StatusContrato::Encerrado;
    return true;
}

bool Contrato::ProcessarEmprestimo(uint32_t clubeDestinoId)
{
    if (m_status != StatusContrato::Ativo || m_tipo != TipoContrato::Jogador || clubeDestinoId == 0) {
        return false;
    }

    m_clubeOrigemEmprestimoId = m_clubeId;
    m_clubeId = clubeDestinoId;
    m_status = StatusContrato::Emprestado;
    return true;
}

bool Contrato::RetornarDeEmprestimo()
{
    if (m_status != StatusContrato::Emprestado || m_clubeOrigemEmprestimoId == 0) {
        return false;
    }

    m_clubeId = m_clubeOrigemEmprestimoId;
    m_clubeOrigemEmprestimoId = 0;
    m_status = StatusContrato::Ativo;
    return true;
}

bool Contrato::ECompativelCom(const Contrato& outro) const
{
    // Nunca permitir dois contratos ativos simultâneos para a mesma entidade do mesmo tipo
    if (m_entidadeId == outro.m_entidadeId && m_tipo == outro.m_tipo) {
        if (m_status == StatusContrato::Ativo && outro.m_status == StatusContrato::Ativo) {
            return false; // Conflito de contratos ativos
        }
    }
    return true;
}

} // namespace MLM
