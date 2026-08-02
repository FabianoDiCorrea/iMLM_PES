#ifndef MLM_CLUBE_REPOSITORY_MEMORIA_H
#define MLM_CLUBE_REPOSITORY_MEMORIA_H

#include "IClubeRepository.h"
#include <unordered_map>

namespace MLM {

class ClubeRepositoryMemoria : public IClubeRepository {
public:
    ClubeRepositoryMemoria() = default;
    ~ClubeRepositoryMemoria() override = default;

    OperationResult Salvar(const Clube& clube) override {
        m_clubes[clube.ObterId()] = clube;
        return OperationResult::Success();
    }

    Result<Clube> ObterPorId(uint32_t id) override {
        auto it = m_clubes.find(id);
        if (it != m_clubes.end()) {
            return Result<Clube>::Success(it->second);
        }
        return Result<Clube>::Failure("Clube não encontrado.");
    }

    std::vector<Clube> ObterPorLiga(uint32_t ligaId) override {
        std::vector<Clube> lista;
        for (const auto& pair : m_clubes) {
            if (pair.second.ObterLigaId() == ligaId) {
                lista.push_back(pair.second);
            }
        }
        return lista;
    }

    std::vector<Clube> ObterTodos() override {
        std::vector<Clube> lista;
        lista.reserve(m_clubes.size());
        for (const auto& pair : m_clubes) {
            lista.push_back(pair.second);
        }
        return lista;
    }

    OperationResult Remover(uint32_t id) override {
        auto it = m_clubes.find(id);
        if (it != m_clubes.end()) {
            m_clubes.erase(it);
            return OperationResult::Success();
        }
        return OperationResult::Failure("Clube não encontrado para remoção.");
    }

private:
    std::unordered_map<uint32_t, Clube> m_clubes;
};

} // namespace MLM

#endif // MLM_CLUBE_REPOSITORY_MEMORIA_H
