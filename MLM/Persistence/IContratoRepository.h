#ifndef MLM_ICONTRATO_REPOSITORY_H
#define MLM_ICONTRATO_REPOSITORY_H

#include <vector>
#include <memory>
#include <cstdint>
#include "../Domain/Contrato.h"
#include "../Core/Result.h"

namespace MLM {

class IContratoRepository {
public:
    virtual ~IContratoRepository() = default;

    virtual OperationResult Salvar(const Contrato& contrato) = 0;
    virtual Result<Contrato> ObterPorId(uint32_t id) = 0;
    virtual Result<Contrato> ObterContratoAtivoEntidade(TipoContrato tipo, uint32_t entidadeId) = 0;
    virtual std::vector<Contrato> ObterContratosPorClube(uint32_t clubeId) = 0;
    virtual std::vector<Contrato> ObterTodos() = 0;
    virtual OperationResult Remover(uint32_t id) = 0;
};

} // namespace MLM

#endif // MLM_ICONTRATO_REPOSITORY_H
