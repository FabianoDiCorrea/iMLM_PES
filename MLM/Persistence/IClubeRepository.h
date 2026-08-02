#ifndef MLM_ICLUBE_REPOSITORY_H
#define MLM_ICLUBE_REPOSITORY_H

#include <vector>
#include <memory>
#include <cstdint>
#include "../Domain/Clube.h"
#include "../Core/Result.h"

namespace MLM {

class IClubeRepository {
public:
    virtual ~IClubeRepository() = default;

    virtual OperationResult Salvar(const Clube& clube) = 0;
    virtual Result<Clube> ObterPorId(uint32_t id) = 0;
    virtual std::vector<Clube> ObterPorLiga(uint32_t ligaId) = 0;
    virtual std::vector<Clube> ObterTodos() = 0;
    virtual OperationResult Remover(uint32_t id) = 0;
};

} // namespace MLM

#endif // MLM_ICLUBE_REPOSITORY_H
