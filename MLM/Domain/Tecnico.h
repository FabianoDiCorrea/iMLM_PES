#ifndef MLM_TECNICO_H
#define MLM_TECNICO_H

#include <string>
#include <vector>
#include <cstdint>
#include "TecnicoTipos.h"
#include "HistoricoTecnico.h"

namespace MLM {

struct EstatisticasTecnico {
    uint32_t totalPartidas = 0;
    uint32_t vitorias = 0;
    uint32_t empates = 0;
    uint32_t derrotas = 0;
    uint32_t golsPro = 0;
    uint32_t golsContra = 0;
    uint32_t titulosConquistados = 0;

    void RegistrarResultado(uint32_t gp, uint32_t gc) {
        totalPartidas++;
        golsPro += gp;
        golsContra += gc;
        if (gp > gc) {
            vitorias++;
        } else if (gp == gc) {
            empates++;
        } else {
            derrotas++;
        }
    }
};

class Tecnico {
public:
    Tecnico() = default;

    Tecnico(uint32_t id,
            const std::wstring& nome,
            uint32_t paisId,
            uint8_t idade,
            uint8_t reputacao = 50,
            uint8_t potencial = 75,
            EstiloPreferencial estilo = EstiloPreferencial::Equilibrado,
            PersonalidadeTecnico personalidade = PersonalidadeTecnico::Tatico);

    ~Tecnico() = default;

    // Getters
    uint32_t ObterId() const { return m_id; }
    std::wstring ObterNome() const { return m_nome; }
    uint32_t ObterPaisId() const { return m_paisId; }
    uint8_t ObterIdade() const { return m_idade; }
    uint32_t ObterClubeAtualId() const { return m_clubeAtualId; }
    uint32_t ObterSelecaoAtualId() const { return m_selecaoAtualId; }
    uint8_t ObterReputacao() const { return m_reputacao; }
    uint8_t ObterPotencial() const { return m_potencial; }
    EstiloPreferencial ObterEstiloPreferencial() const { return m_estiloPreferencial; }
    PersonalidadeTecnico ObterPersonalidade() const { return m_personalidade; }
    double ObterSalario() const { return m_salario; }
    double ObterValorContratual() const { return m_valorContratual; }
    uint16_t ObterTempoContratoMeses() const { return m_tempoContratoMeses; }
    SituacaoTecnico ObterSituacao() const { return m_situacao; }
    bool EHumano() const { return m_isHumano; }
    uint32_t ObterPesEditId() const { return m_pesEditId; }

    const EstatisticasTecnico& ObterEstatisticas() const { return m_estatisticas; }
    const std::vector<RegistroHistoricoTecnico>& ObterHistorico() const { return m_historico; }

    // Setters e Regras de Domínio
    void DefinirHumano(bool ehHumano) { m_isHumano = ehHumano; }
    void DefinirPesEditId(uint32_t pesId) { m_pesEditId = pesId; }
    void FazerAniversario();
    void AlterarReputacao(int delta);
    void DefinirReputacao(uint8_t novaReputacao);

    // Contrato e Carreira
    bool AssumirClube(uint32_t clubeId, double salario, uint16_t tempoMeses, uint32_t anoTemporada);
    void DeixarClube(uint32_t anoTemporada, bool porDemissao = false, bool porPedido = false);
    
    bool AssumirSelecao(uint32_t selecaoId, uint32_t anoTemporada);
    void DeixarSelecao(uint32_t anoTemporada);

    void RenovarContrato(uint16_t mesesAdicionais, double novoSalario, uint32_t anoTemporada);
    void Aposentar(uint32_t anoTemporada);

    // Histórico e Estatísticas
    void AdicionarRegistroHistorico(uint32_t ano, TipoRegistroHistoricoTecnico tipo, uint32_t entidadeId, const std::wstring& desc);
    void RegistrarPartida(uint32_t golsPro, uint32_t golsContra);
    void RegistrarTitulo(uint32_t competicaoId, const std::wstring& nomeCompeticao, uint32_t anoTemporada);

private:
    uint32_t m_id;
    std::wstring m_nome;
    uint32_t m_paisId;
    uint8_t m_idade;

    // Estado da Carreira
    uint32_t m_clubeAtualId;   // 0 se desempregado de clube
    uint32_t m_selecaoAtualId; // 0 se não dirigir seleção
    uint8_t m_reputacao;       // 1 a 100
    uint8_t m_potencial;       // 1 a 100
    EstiloPreferencial m_estiloPreferencial;
    PersonalidadeTecnico m_personalidade;

    // Dados Financeiros e Contratuais
    double m_salario;
    double m_valorContratual;
    uint16_t m_tempoContratoMeses;
    SituacaoTecnico m_situacao;
    bool m_isHumano;

    // Ponto de extensão para PES EDIT00000000 sincronização futura
    uint32_t m_pesEditId;

    // Estatísticas e Histórico
    EstatisticasTecnico m_estatisticas;
    std::vector<RegistroHistoricoTecnico> m_historico;
};

} // namespace MLM

#endif // MLM_TECNICO_H
