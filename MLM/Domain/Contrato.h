#ifndef MLM_CONTRATO_H
#define MLM_CONTRATO_H

#include <string>
#include <vector>
#include <cstdint>
#include "ContratoTipos.h"

namespace MLM {

class Contrato {
public:
    Contrato() = default;

    Contrato(uint32_t id,
             TipoContrato tipo,
             uint32_t entidadeId,
             uint32_t clubeId,
             uint32_t anoInicio,
             uint16_t duracaoMeses,
             double salarioMensal,
             double multaRescisoria = 0.0,
             BonusContratual bonus = BonusContratual());

    ~Contrato() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterId() const { return m_id; }
    TipoContrato ObterTipo() const { return m_tipo; }
    uint32_t ObterEntidadeId() const { return m_entidadeId; }
    uint32_t ObterClubeId() const { return m_clubeId; }
    uint32_t ObterClubeOrigemEmprestimoId() const { return m_clubeOrigemEmprestimoId; }
    uint32_t ObterAnoInicio() const { return m_anoInicio; }
    uint32_t ObterAnoTermino() const { return m_anoInicio + (m_duracaoMeses / 12); }
    uint16_t ObterDuracaoMeses() const { return m_duracaoMeses; }
    double ObterSalarioMensal() const { return m_salarioMensal; }
    double ObterMultaRescisoria() const { return m_multaRescisoria; }
    const BonusContratual& ObterBonus() const { return m_bonus; }
    StatusContrato ObterStatus() const { return m_status; }
    bool ERenovavel() const { return m_renovavel; }
    const std::vector<RegistroRenovacaoContrato>& ObterHistoricoRenovacoes() const { return m_historicoRenovacoes; }

    // Regras de Negócio e Validações de Domínio
    bool Renovar(uint16_t mesesAdicionais, double novoSalarioMensal, double novaMulta, uint32_t anoTemporada);
    bool Rescindir();
    bool Encerrar();
    bool ProcessarEmprestimo(uint32_t clubeDestinoId);
    bool RetornarDeEmprestimo();
    void DefinirRenovavel(bool renovavel) { m_renovavel = renovavel; }
    void DefinirMultaRescisoria(double novaMulta) { m_multaRescisoria = novaMulta; }

    // Validação de Incompatibilidade Contratual
    bool ECompativelCom(const Contrato& outro) const;

private:
    uint32_t m_id;
    TipoContrato m_tipo;
    uint32_t m_entidadeId; // ID do Jogador ou do Técnico
    uint32_t m_clubeId;    // ID do Clube empregador atual
    uint32_t m_clubeOrigemEmprestimoId; // 0 se não for empréstimo

    uint32_t m_anoInicio;
    uint16_t m_duracaoMeses;
    double m_salarioMensal;
    double m_multaRescisoria;
    BonusContratual m_bonus;

    StatusContrato m_status;
    bool m_renovavel;
    std::vector<RegistroRenovacaoContrato> m_historicoRenovacoes;
};

} // namespace MLM

#endif // MLM_CONTRATO_H
