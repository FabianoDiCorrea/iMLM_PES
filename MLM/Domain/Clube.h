#ifndef MLM_CLUBE_H
#define MLM_CLUBE_H

#include <string>
#include <vector>
#include <cstdint>
#include "Jogador.h"
#include "HistoricoClube.h"

namespace MLM {

class Clube {
public:
    static constexpr size_t LIMITE_MAXIMO_ELENCO = 40;

    Clube() = default;

    Clube(uint32_t id,
          const std::wstring& nome,
          const std::string& nomeAbreviado,
          uint32_t paisId,
          const std::wstring& cidade = L"",
          uint32_t estadioId = 0,
          uint32_t ligaId = 0);

    ~Clube() = default;

    // Getters de Leitura de Atributos Institucionais
    uint32_t ObterId() const { return m_id; }
    std::wstring ObterNome() const { return m_nome; }
    std::string ObterNomeAbreviado() const { return m_nomeAbreviado; }
    uint32_t ObterPaisId() const { return m_paisId; }
    std::wstring ObterCidade() const { return m_cidade; }
    uint32_t ObterEstadioId() const { return m_estadioId; }
    uint32_t ObterLigaId() const { return m_ligaId; }
    uint32_t ObterTecnicoId() const { return m_tecnicoId; }
    double ObterSaldo() const { return m_saldo; }
    double ObterOrcamentoTransferencias() const { return m_orcamentoTransferencias; }
    double ObterFolhaSalarialMensal() const { return m_folhaSalarialMensal; }
    uint8_t ObterReputacao() const { return m_reputacao; }
    uint8_t ObterTradicaoInstitucional() const { return m_tradicaoInstitucional; }
    bool EstaAtivo() const { return m_ativo; }

    const std::vector<uint32_t>& ObterRivalidades() const { return m_clubesRivaisIds; }
    const std::vector<RegistroTituloClube>& ObterTitulos() const { return m_titulos; }
    const std::vector<RegistroCampanhaClube>& ObterCampanhas() const { return m_campanhas; }

    // Métodos de Negócio do Domínio
    void AlterarLiga(uint32_t novaLigaId) { m_ligaId = novaLigaId; }
    void AlterarTecnico(uint32_t novoTecnicoId);
    void RemoverTecnico();
    void AlterarEstadio(uint32_t novoEstadioId);
    bool ReceberValor(double valor);
    bool PagarValor(double valor);
    void DefinirOrcamentoTransferencias(double orcamento) { m_orcamentoTransferencias = orcamento; }
    void DefinirFolhaSalarialMensal(double folha) { m_folhaSalarialMensal = folha; }
    void AlterarReputacao(uint8_t novaReputacao);
    void DefinirTradicaoInstitucional(uint8_t tradicao);
    void Ativar();
    void Desativar();

    // Gestão de Rivalidades
    void AdicionarRival(uint32_t clubeRivalId);
    bool ERival(uint32_t clubeId) const;

    // Histórico e Memória Institucional
    void RegistrarTitulo(uint32_t anoTemporada, uint32_t competicaoId, const std::wstring& nomeCompeticao);
    void RegistrarCampanha(uint32_t anoTemporada, uint32_t competicaoId, const std::wstring& nomeCompeticao,
                           uint16_t posicaoFinal, const std::wstring& faseAlcancada,
                           uint16_t jogos, uint16_t vitorias, uint16_t empates, uint16_t derrotas);

    // Métodos de Gestão do Elenco
    bool AdicionarJogador(Jogador* jogador, uint16_t numeroCamisa);
    bool RemoverJogador(uint32_t jogadorId);
    bool PossuiJogador(uint32_t jogadorId) const;
    bool ValidarConsistenciaElenco() const;
    size_t QuantidadeJogadores() const { return m_elenco.size(); }
    const std::vector<Jogador*>& ObterJogadores() const { return m_elenco; }

private:
    uint32_t m_id;
    std::wstring m_nome;
    std::string m_nomeAbreviado;
    uint32_t m_paisId;
    std::wstring m_cidade;
    uint32_t m_ligaId;
    
    // Identificadores de Relacionamento
    uint32_t m_estadioId;
    uint32_t m_tecnicoId;

    // Estado Financeiro e Indicadores Institucionais (Fontes para IA)
    double m_saldo;
    double m_orcamentoTransferencias;
    double m_folhaSalarialMensal;
    uint8_t m_reputacao;             // 1 a 100
    uint8_t m_tradicaoInstitucional; // 1 a 100 (História de longo prazo)
    bool m_ativo;

    // Listas e Memória Institucional
    std::vector<uint32_t> m_clubesRivaisIds;
    std::vector<RegistroTituloClube> m_titulos;
    std::vector<RegistroCampanhaClube> m_campanhas;

    // Coleção Privada do Elenco do Clube
    std::vector<Jogador*> m_elenco;
};

} // namespace MLM

#endif // MLM_CLUBE_H
