#ifndef MLM_HISTORICO_TECNICO_H
#define MLM_HISTORICO_TECNICO_H

#include <string>
#include <cstdint>

namespace MLM {

enum class TipoRegistroHistoricoTecnico : uint8_t {
    AssumiuClube = 0,
    DeixouClube = 1,
    AssumiuSelecao = 2,
    DeixouSelecao = 3,
    TituloConquistado = 4,
    PremioRecebido = 5,
    Demissao = 6,
    PedidoDemissao = 7,
    RenovacaoContrato = 8,
    Aposentadoria = 9
};

struct RegistroHistoricoTecnico {
    uint32_t anoTemporada;
    TipoRegistroHistoricoTecnico tipo;
    uint32_t entidadeId; // ID do Clube, Seleção ou Competição vinculada
    std::wstring descricao;

    RegistroHistoricoTecnico(uint32_t ano = 0,
                             TipoRegistroHistoricoTecnico t = TipoRegistroHistoricoTecnico::AssumiuClube,
                             uint32_t id = 0,
                             const std::wstring& desc = L"")
        : anoTemporada(ano), tipo(t), entidadeId(id), descricao(desc) {}
};

} // namespace MLM

#endif // MLM_HISTORICO_TECNICO_H
