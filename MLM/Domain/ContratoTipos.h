#ifndef MLM_CONTRATO_TIPOS_H
#define MLM_CONTRATO_TIPOS_H

#include <cstdint>

namespace MLM {

enum class TipoContrato : uint8_t {
    Jogador = 0,
    Tecnico = 1
};

enum class StatusContrato : uint8_t {
    Ativo = 0,
    Emprestado = 1,
    Encerrado = 2,
    Rescindido = 3
};

struct BonusContratual {
    double bonusPorGol = 0.0;
    double bonusPorPartida = 0.0;
    double bonusPorTitulo = 0.0;
};

struct RegistroRenovacaoContrato {
    uint32_t anoTemporada;
    uint16_t mesesAdicionados;
    double novoSalario;
    double novaMulta;

    RegistroRenovacaoContrato(uint32_t ano = 0, uint16_t meses = 0, double salario = 0.0, double multa = 0.0)
        : anoTemporada(ano), mesesAdicionados(meses), novoSalario(salario), novaMulta(multa) {}
};

} // namespace MLM

#endif // MLM_CONTRATO_TIPOS_H
