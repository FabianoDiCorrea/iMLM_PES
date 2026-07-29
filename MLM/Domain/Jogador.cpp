#include "Jogador.h"
#include <algorithm>

namespace MLM {

Jogador::Jogador(uint32_t id, 
                 const std::wstring& nome, 
                 const std::string& nomeCamisa, 
                 uint8_t idade, 
                 uint32_t paisId, 
                 PosicaoCampo posicaoPrincipal, 
                 uint8_t overallBase, 
                 uint8_t potencial)
    : m_id(id),
      m_nome(nome),
      m_nomeCamisa(nomeCamisa),
      m_idade(idade),
      m_paisId(paisId),
      m_posicaoPrincipal(posicaoPrincipal),
      m_overall(overallBase),
      m_potencial(potencial),
      m_clubeAtualId(0),
      m_numeroCamisa(0),
      m_moral(75.0f),       // Moral padrão inicial neutro/bom
      m_formaFisica(100.0f), // 100% fisicamente pronto
      m_valorMercado(0.0)
{
}

void Jogador::AlterarClube(uint32_t novoClubeId, uint16_t novoNumeroCamisa)
{
    m_clubeAtualId = novoClubeId;
    m_numeroCamisa = novoNumeroCamisa;
}

void Jogador::DesvincularClube()
{
    m_clubeAtualId = 0;
    m_numeroCamisa = 0;
}

void Jogador::AlterarNumeroCamisa(uint16_t novoNumeroCamisa)
{
    m_numeroCamisa = novoNumeroCamisa;
}

void Jogador::FazerAniversario()
{
    m_idade++;
}

void Jogador::AtualizarMoral(float deltaMoral)
{
    m_moral += deltaMoral;
    m_moral = (std::max)(0.0f, (std::min)(100.0f, m_moral));
}

void Jogador::AtualizarForma(float novaForma)
{
    m_formaFisica = (std::max)(0.0f, (std::min)(100.0f, novaForma));
}

void Jogador::AtualizarValorMercado(double novoValor)
{
    if (novoValor >= 0.0) {
        m_valorMercado = novoValor;
    }
}

} // namespace MLM
