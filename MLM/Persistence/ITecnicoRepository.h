#ifndef MLM_ITECNICO_REPOSITORY_H
#define MLM_ITECNICO_REPOSITORY_H

#include <vector>
#include <memory>
#include <cstdint>
#include "../Domain/Tecnico.h"
#include "../Core/Result.h"

namespace MLM {

class ITecnicoRepository {
public:
    virtual ~ITecnicoRepository() = default;

    virtual OperationResult Salvar(const Tecnico& tecnico) = 0;
    virtual Result<Tecnico> ObterPorId(uint32_t id) = 0;
    virtual std::vector<Tecnico> ObterTodos() = 0;
    virtual std::vector<Tecnico> ObterDesempregados() = 0;
    virtual OperationResult Remover(uint32_t id) = 0;
};

} // namespace MLM

#endif // MLM_ITECNICO_REPOSITORY_H
