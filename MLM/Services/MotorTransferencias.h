#ifndef MLM_MOTOR_TRANSFERENCIAS_H
#define MLM_MOTOR_TRANSFERENCIAS_H

#include "../Domain/Jogador.h"
#include "../Domain/Clube.h"
#include "../Domain/NegociacaoTipos.h"

namespace MLM {

class MotorTransferencias {
public:
    MotorTransferencias() = default;
    ~MotorTransferencias() = default;

    // Cálculo do Valor de Mercado Estimado no Domínio MLM
    double CalcularValorMercado(const Jogador& jogador) const;
    double CalcularSalarioExigido(const Jogador& jogador) const;

    // Método Principal de Avaliação de Proposta de Transferência
    RespostaTransferencia AvaliarProposta(const PropostaTransferencia& proposta,
                                          const Jogador& jogador,
                                          const Clube& clubeVendedor,
                                          const Clube& clubeComprador) const;

private:
    // Avaliações de Veto
    bool AvaliarDecisaoClube(const PropostaTransferencia& proposta,
                             const Jogador& jogador,
                             const Clube& clubeVendedor,
                             double valorMercado,
                             std::wstring& outMotivo,
                             double& outValorExigido) const;

    bool AvaliarDecisaoJogador(const PropostaTransferencia& proposta,
                               const Jogador& jogador,
                               const Clube& clubeVendedor,
                               const Clube& clubeComprador,
                               double salarioBase,
                               std::wstring& outMotivo,
                               double& outSalarioExigido) const;
};

} // namespace MLM

#endif // MLM_MOTOR_TRANSFERENCIAS_H
