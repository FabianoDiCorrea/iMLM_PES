#ifndef MLM_IFINANCEIRO_REPOSITORY_H
#define MLM_IFINANCEIRO_REPOSITORY_H

#include <vector>
#include <memory>
#include <cstdint>
#include "../Domain/FinancasClube.h"
#include "../Core/Result.h"

namespace MLM {

class IFinanceiroRepository {
public:
    virtual ~IFinanceiroRepository() = default;

    virtual OperationResult Salvar(const FinancasClube& financas) = 0;
    virtual Result<FinancasClube> ObterPorClubeId(uint32_t clubeId) = 0;
    virtual std::vector<FinancasClube> ObterTodos() = 0;
    virtual OperationResult Remover(uint32_t clubeId) = 0;
};

} // namespace MLM

#endif // MLM_IFINANCEIRO_REPOSITORY_H
