#ifndef MLM_NEGOCIACAO_TIPOS_H
#define MLM_NEGOCIACAO_TIPOS_H

#include <cstdint>
#include <string>

namespace MLM {

enum class StatusProposta : uint8_t {
    Pendente = 0,
    AceitaClubeEJogador = 1,
    RecusadaPeloClube = 2,
    RecusadaPeloJogador = 3,
    SaldoInsuficienteComprador = 4
};

struct PropostaTransferencia {
    uint32_t jogadorId;
    uint32_t clubeVendedorId;
    uint32_t clubeCompradorId;
    double valorOferecido;     // Valor em Euros / Moeda do MLM
    double salarioOferecido;   // Salário mensal / anual oferecido ao atleta
    uint8_t anosContratoOferecidos;
};

struct RespostaTransferencia {
    StatusProposta status;
    bool clubeAceitou;
    bool jogadorAceitou;
    std::wstring motivoClube;
    std::wstring motivoJogador;
    double valorMinimoExigido;
    double salarioMinimoExigido;
};

} // namespace MLM

#endif // MLM_NEGOCIACAO_TIPOS_H
