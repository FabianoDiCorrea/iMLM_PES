#ifndef MLM_FINANCEIRO_REPOSITORY_MEMORIA_H
#define MLM_FINANCEIRO_REPOSITORY_MEMORIA_H

#include "IFinanceiroRepository.h"
#include <unordered_map>

namespace MLM {

class FinanceiroRepositoryMemoria : public IFinanceiroRepository {
public:
    FinanceiroRepositoryMemoria() = default;
    ~FinanceiroRepositoryMemoria() override = default;

    OperationResult Salvar(const FinancasClube& financas) override {
        m_financas[financas.ObterClubeId()] = financas;
        return OperationResult::Success();
    }

    Result<FinancasClube> ObterPorClubeId(uint32_t clubeId) override {
        auto it = m_financas.find(clubeId);
        if (it != m_financas.end()) {
            return Result<FinancasClube>::Success(it->second);
        }
        return Result<FinancasClube>::Failure("Finanças do clube não encontradas.");
    }

    std::vector<FinancasClube> ObterTodos() override {
        std::vector<FinancasClube> lista;
        lista.reserve(m_financas.size());
        for (const auto& pair : m_financas) {
            lista.push_back(pair.second);
        }
        return lista;
    }

    OperationResult Remover(uint32_t clubeId) override {
        auto it = m_financas.find(clubeId);
        if (it != m_financas.end()) {
            m_financas.erase(it);
            return OperationResult::Success();
        }
        return OperationResult::Failure("Finanças do clube não encontradas para remoção.");
    }

private:
    std::unordered_map<uint32_t, FinancasClube> m_financas;
};

} // namespace MLM

#endif // MLM_FINANCEIRO_REPOSITORY_MEMORIA_H
