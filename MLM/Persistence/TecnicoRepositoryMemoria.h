#ifndef MLM_TECNICO_REPOSITORY_MEMORIA_H
#define MLM_TECNICO_REPOSITORY_MEMORIA_H

#include "ITecnicoRepository.h"
#include <unordered_map>
#include <algorithm>

namespace MLM {

class TecnicoRepositoryMemoria : public ITecnicoRepository {
public:
    TecnicoRepositoryMemoria() = default;
    ~TecnicoRepositoryMemoria() override = default;

    OperationResult Salvar(const Tecnico& tecnico) override {
        m_tecnicos[tecnico.ObterId()] = tecnico;
        return OperationResult::Success();
    }

    Result<Tecnico> ObterPorId(uint32_t id) override {
        auto it = m_tecnicos.find(id);
        if (it != m_tecnicos.end()) {
            return Result<Tecnico>::Success(it->second);
        }
        return Result<Tecnico>::Failure("Técnico não encontrado.");
    }

    std::vector<Tecnico> ObterTodos() override {
        std::vector<Tecnico> lista;
        lista.reserve(m_tecnicos.size());
        for (const auto& pair : m_tecnicos) {
            lista.push_back(pair.second);
        }
        return lista;
    }

    std::vector<Tecnico> ObterDesempregados() override {
        std::vector<Tecnico> lista;
        for (const auto& pair : m_tecnicos) {
            if (pair.second.ObterSituacao() == SituacaoTecnico::Desempregado) {
                lista.push_back(pair.second);
            }
        }
        return lista;
    }

    OperationResult Remover(uint32_t id) override {
        auto it = m_tecnicos.find(id);
        if (it != m_tecnicos.end()) {
            m_tecnicos.erase(it);
            return OperationResult::Success();
        }
        return OperationResult::Failure("Técnico não encontrado para remoção.");
    }

private:
    std::unordered_map<uint32_t, Tecnico> m_tecnicos;
};

} // namespace MLM

#endif // MLM_TECNICO_REPOSITORY_MEMORIA_H
