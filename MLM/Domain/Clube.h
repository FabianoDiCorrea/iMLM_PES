#ifndef MLM_CLUBE_H
#define MLM_CLUBE_H

#include <string>
#include <vector>
#include <cstdint>
#include "Jogador.h"

namespace MLM {

class Clube {
public:
    static constexpr size_t LIMITE_MAXIMO_ELENCO = 40;

    Clube(uint32_t id,
          const std::wstring& nome,
          const std::string& nomeAbreviado,
          uint32_t paisId,
          const std::wstring& cidade = L"",
          uint32_t estadioId = 0);

    ~Clube() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterId() const { return m_id; }
    std::wstring ObterNome() const { return m_nome; }
    std::string ObterNomeAbreviado() const { return m_nomeAbreviado; }
    uint32_t ObterPaisId() const { return m_paisId; }
    std::wstring ObterCidade() const { return m_cidade; }
    uint32_t ObterEstadioId() const { return m_estadioId; }
    uint32_t ObterTecnicoId() const { return m_tecnicoId; }
    double ObterSaldo() const { return m_saldo; }
    uint8_t ObterReputacao() const { return m_reputacao; }
    bool EstaAtivo() const { return m_ativo; }

    // Métodos de Negócio do Domínio
    void AlterarTecnico(uint32_t novoTecnicoId);
    void RemoverTecnico();
    void AlterarEstadio(uint32_t novoEstadioId);
    bool ReceberValor(double valor);
    bool PagarValor(double valor);
    void AlterarReputacao(uint8_t novaReputacao);
    void Ativar();
    void Desativar();

    // Métodos de Gestão do Elenco
    bool AdicionarJogador(Jogador* jogador, uint16_t numeroCamisa);
    bool RemoverJogador(uint32_t jogadorId);
    bool PossuiJogador(uint32_t jogadorId) const;
    size_t QuantidadeJogadores() const { return m_elenco.size(); }
    const std::vector<Jogador*>& ObterJogadores() const { return m_elenco; }

private:
    uint32_t m_id;
    std::wstring m_nome;
    std::string m_nomeAbreviado;
    uint32_t m_paisId;
    std::wstring m_cidade;
    
    // Identificadores de Relacionamento
    uint32_t m_estadioId;
    uint32_t m_tecnicoId;

    // Estado Financeiro e de Prestígio
    double m_saldo;
    uint8_t m_reputacao;
    bool m_ativo;

    // Coleção Privada do Elenco do Clube
    std::vector<Jogador*> m_elenco;
};

} // namespace MLM

#endif // MLM_CLUBE_H
