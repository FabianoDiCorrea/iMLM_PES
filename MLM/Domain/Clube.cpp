#include "Clube.h"
#include <algorithm>

namespace MLM {

Clube::Clube(uint32_t id,
             const std::wstring& nome,
             const std::string& nomeAbreviado,
             uint32_t paisId,
             const std::wstring& cidade,
             uint32_t estadioId)
    : m_id(id),
      m_nome(nome),
      m_nomeAbreviado(nomeAbreviado),
      m_paisId(paisId),
      m_cidade(cidade),
      m_estadioId(estadioId),
      m_tecnicoId(0),
      m_saldo(0.0),
      m_reputacao(3),
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
    m_reputacao = (std::max)((uint8_t)1, (std::min)((uint8_t)5, novaReputacao));
}

void Clube::Ativar()
{
    m_ativo = true;
}

void Clube::Desativar()
{
    m_ativo = false;
}

// Implementação da Gestão de Elenco

bool Clube::AdicionarJogador(Jogador* jogador, uint16_t numeroCamisa)
{
    if (!jogador) {
        return false;
    }

    // 1. Validação de Limite Máximo do Elenco (40 jogadores)
    if (m_elenco.size() >= LIMITE_MAXIMO_ELENCO) {
        return false;
    }

    // 2. Validação de Duplicidade (Não permite o mesmo jogador duas vezes)
    if (PossuiJogador(jogador->ObterId())) {
        return false;
    }

    // 3. Adiciona à coleção do clube
    m_elenco.push_back(jogador);

    // 4. Sincroniza a associação no objeto Jogador
    jogador->AlterarClube(m_id, numeroCamisa);

    return true;
}

bool Clube::RemoverJogador(uint32_t jogadorId)
{
    auto it = std::find_if(m_elenco.begin(), m_elenco.end(), [jogadorId](const Jogador* j) {
        return j && j->ObterId() == jogadorId;
    });

    if (it == m_elenco.end()) {
        return false; // Jogador não encontrado no elenco
    }

    Jogador* jogador = *it;

    // 1. Remove da coleção do clube
    m_elenco.erase(it);

    // 2. Atualiza a relação no objeto Jogador (torna Agente Livre)
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

} // namespace MLM
