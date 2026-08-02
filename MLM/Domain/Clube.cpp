#include "Clube.h"
#include <algorithm>

namespace MLM {

Clube::Clube(uint32_t id,
             const std::wstring& nome,
             const std::string& nomeAbreviado,
             uint32_t paisId,
             const std::wstring& cidade,
             uint32_t estadioId,
             uint32_t ligaId)
    : m_id(id),
      m_nome(nome),
      m_nomeAbreviado(nomeAbreviado),
      m_paisId(paisId),
      m_cidade(cidade),
      m_ligaId(ligaId),
      m_estadioId(estadioId),
      m_tecnicoId(0),
      m_saldo(0.0),
      m_orcamentoTransferencias(0.0),
      m_folhaSalarialMensal(0.0),
      m_reputacao(50),
      m_tradicaoInstitucional(50),
      m_ativo(true)
{
}

void Clube::AlterarTecnico(uint32_t novoTecnicoId)
{
    m_tecnicoId = novoTecnicoId;
}

void Clube::RemoverTecnico()
{
    m_tecnicoId = 0;
}

void Clube::AlterarEstadio(uint32_t novoEstadioId)
{
    m_estadioId = novoEstadioId;
}

bool Clube::ReceberValor(double valor)
{
    if (valor <= 0.0) {
        return false;
    }
    m_saldo += valor;
    return true;
}

bool Clube::PagarValor(double valor)
{
    if (valor <= 0.0) {
        return false;
    }
    if (m_saldo < valor) {
        return false;
    }
    m_saldo -= valor;
    return true;
}

void Clube::AlterarReputacao(uint8_t novaReputacao)
{
    m_reputacao = (std::max)((uint8_t)1, (std::min)((uint8_t)100, novaReputacao));
}

void Clube::DefinirTradicaoInstitucional(uint8_t tradicao)
{
    m_tradicaoInstitucional = (std::max)((uint8_t)1, (std::min)((uint8_t)100, tradicao));
}

void Clube::Ativar()
{
    m_ativo = true;
}

void Clube::Desativar()
{
    m_ativo = false;
}

void Clube::AdicionarRival(uint32_t clubeRivalId)
{
    if (clubeRivalId != 0 && clubeRivalId != m_id && !ERival(clubeRivalId)) {
        m_clubesRivaisIds.push_back(clubeRivalId);
    }
}

bool Clube::ERival(uint32_t clubeId) const
{
    return std::find(m_clubesRivaisIds.begin(), m_clubesRivaisIds.end(), clubeId) != m_clubesRivaisIds.end();
}

void Clube::RegistrarTitulo(uint32_t anoTemporada, uint32_t competicaoId, const std::wstring& nomeCompeticao)
{
    m_titulos.emplace_back(anoTemporada, competicaoId, nomeCompeticao);
}

void Clube::RegistrarCampanha(uint32_t anoTemporada, uint32_t competicaoId, const std::wstring& nomeCompeticao,
                               uint16_t posicaoFinal, const std::wstring& faseAlcancada,
                               uint16_t jogos, uint16_t vitorias, uint16_t empates, uint16_t derrotas)
{
    m_campanhas.emplace_back(anoTemporada, competicaoId, nomeCompeticao, posicaoFinal, faseAlcancada, jogos, vitorias, empates, derrotas);
}

// Implementação da Gestão de Elenco

bool Clube::AdicionarJogador(Jogador* jogador, uint16_t numeroCamisa)
{
    if (!jogador) {
        return false;
    }

    if (m_elenco.size() >= LIMITE_MAXIMO_ELENCO) {
        return false;
    }

    if (PossuiJogador(jogador->ObterId())) {
        return false;
    }

    m_elenco.push_back(jogador);
    jogador->AlterarClube(m_id, numeroCamisa);
    return true;
}

bool Clube::RemoverJogador(uint32_t jogadorId)
{
    auto it = std::find_if(m_elenco.begin(), m_elenco.end(), [jogadorId](const Jogador* j) {
        return j && j->ObterId() == jogadorId;
    });

    if (it == m_elenco.end()) {
        return false;
    }

    Jogador* jogador = *it;
    m_elenco.erase(it);

    if (jogador) {
        jogador->DesvincularClube();
    }

    return true;
}

bool Clube::PossuiJogador(uint32_t jogadorId) const
{
    for (const auto* j : m_elenco) {
        if (j && j->ObterId() == jogadorId) {
            return true;
        }
    }
    return false;
}

bool Clube::ValidarConsistenciaElenco() const
{
    if (m_elenco.size() > LIMITE_MAXIMO_ELENCO) {
        return false;
    }
    for (size_t i = 0; i < m_elenco.size(); ++i) {
        if (!m_elenco[i]) return false;
        for (size_t j = i + 1; j < m_elenco.size(); ++j) {
            if (m_elenco[i]->ObterId() == m_elenco[j]->ObterId()) {
                return false; // Jogador duplicado no elenco
            }
        }
    }
    return true;
}

} // namespace MLM
