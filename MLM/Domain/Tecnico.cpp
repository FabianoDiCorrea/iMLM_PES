#include "Tecnico.h"
#include <algorithm>

namespace MLM {

Tecnico::Tecnico(uint32_t id,
                 const std::wstring& nome,
                 uint32_t paisId,
                 uint8_t idade,
                 uint8_t reputacao,
                 uint8_t potencial,
                 EstiloPreferencial estilo,
                 PersonalidadeTecnico personalidade)
    : m_id(id),
      m_nome(nome),
      m_paisId(paisId),
      m_idade(idade),
      m_clubeAtualId(0),
      m_selecaoAtualId(0),
      m_reputacao((std::max)((uint8_t)1, (std::min)((uint8_t)100, reputacao))),
      m_potencial((std::max)((uint8_t)1, (std::min)((uint8_t)100, potencial))),
      m_estiloPreferencial(estilo),
      m_personalidade(personalidade),
      m_salario(0.0),
      m_valorContratual(0.0),
      m_tempoContratoMeses(0),
      m_situacao(SituacaoTecnico::Desempregado),
      m_isHumano(false),
      m_pesEditId(0)
{
}

void Tecnico::FazerAniversario()
{
    if (m_situacao != SituacaoTecnico::Aposentado) {
        m_idade++;
    }
}

void Tecnico::AlterarReputacao(int delta)
{
    int novaRep = static_cast<int>(m_reputacao) + delta;
    m_reputacao = static_cast<uint8_t>((std::max)(1, (std::min)(100, novaRep)));
}

void Tecnico::DefinirReputacao(uint8_t novaReputacao)
{
    m_reputacao = (std::max)((uint8_t)1, (std::min)((uint8_t)100, novaReputacao));
}

bool Tecnico::AssumirClube(uint32_t clubeId, double salario, uint16_t tempoMeses, uint32_t anoTemporada)
{
    if (m_situacao == SituacaoTecnico::Aposentado || clubeId == 0) {
        return false;
    }

    m_clubeAtualId = clubeId;
    m_salario = salario;
    m_tempoContratoMeses = tempoMeses;
    m_valorContratual = salario * (tempoMeses / 12.0);
    m_situacao = SituacaoTecnico::Empregado;

    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::AssumiuClube, clubeId, L"Assumiu o comando do clube.");
    return true;
}

void Tecnico::DeixarClube(uint32_t anoTemporada, bool porDemissao, bool porPedido)
{
    if (m_clubeAtualId == 0) {
        return;
    }

    uint32_t clubeAntigo = m_clubeAtualId;
    m_clubeAtualId = 0;
    m_salario = 0.0;
    m_tempoContratoMeses = 0;
    m_valorContratual = 0.0;

    if (m_selecaoAtualId == 0) {
        m_situacao = SituacaoTecnico::Desempregado;
    }

    TipoRegistroHistoricoTecnico tipo = TipoRegistroHistoricoTecnico::DeixouClube;
    std::wstring msg = L"Deixou o comando do clube.";

    if (porDemissao) {
        tipo = TipoRegistroHistoricoTecnico::Demissao;
        msg = L"Foi demitido do clube.";
        AlterarReputacao(-5);
    } else if (porPedido) {
        tipo = TipoRegistroHistoricoTecnico::PedidoDemissao;
        msg = L"Pediu demissão do clube.";
    }

    AdicionarRegistroHistorico(anoTemporada, tipo, clubeAntigo, msg);
}

bool Tecnico::AssumirSelecao(uint32_t selecaoId, uint32_t anoTemporada)
{
    if (m_situacao == SituacaoTecnico::Aposentado || selecaoId == 0) {
        return false;
    }

    m_selecaoAtualId = selecaoId;
    if (m_situacao == SituacaoTecnico::Desempregado) {
        m_situacao = SituacaoTecnico::Empregado;
    }

    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::AssumiuSelecao, selecaoId, L"Assumiu o comando da seleção nacional.");
    return true;
}

void Tecnico::DeixarSelecao(uint32_t anoTemporada)
{
    if (m_selecaoAtualId == 0) {
        return;
    }

    uint32_t selecaoAntiga = m_selecaoAtualId;
    m_selecaoAtualId = 0;

    if (m_clubeAtualId == 0) {
        m_situacao = SituacaoTecnico::Desempregado;
    }

    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::DeixouSelecao, selecaoAntiga, L"Deixou o comando da seleção nacional.");
}

void Tecnico::RenovarContrato(uint16_t mesesAdicionais, double novoSalario, uint32_t anoTemporada)
{
    if (m_clubeAtualId == 0) {
        return;
    }

    m_tempoContratoMeses += mesesAdicionais;
    m_salario = novoSalario;
    m_valorContratual = m_salario * (m_tempoContratoMeses / 12.0);

    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::RenovacaoContrato, m_clubeAtualId, L"Renovou contrato com o clube.");
}

void Tecnico::Aposentar(uint32_t anoTemporada)
{
    if (m_situacao == SituacaoTecnico::Aposentado) {
        return;
    }

    if (m_clubeAtualId != 0) {
        DeixarClube(anoTemporada);
    }
    if (m_selecaoAtualId != 0) {
        DeixarSelecao(anoTemporada);
    }

    m_situacao = SituacaoTecnico::Aposentado;
    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::Aposentadoria, 0, L"Aposentou-se do futebol.");
}

void Tecnico::AdicionarRegistroHistorico(uint32_t ano, TipoRegistroHistoricoTecnico tipo, uint32_t entidadeId, const std::wstring& desc)
{
    m_historico.emplace_back(ano, tipo, entidadeId, desc);
}

void Tecnico::RegistrarPartida(uint32_t golsPro, uint32_t golsContra)
{
    m_estatisticas.RegistrarResultado(golsPro, golsContra);
}

void Tecnico::RegistrarTitulo(uint32_t competicaoId, const std::wstring& nomeCompeticao, uint32_t anoTemporada)
{
    m_estatisticas.titulosConquistados++;
    AlterarReputacao(8);
    AdicionarRegistroHistorico(anoTemporada, TipoRegistroHistoricoTecnico::TituloConquistado, competicaoId, L"Campeão: " + nomeCompeticao);
}

} // namespace MLM
