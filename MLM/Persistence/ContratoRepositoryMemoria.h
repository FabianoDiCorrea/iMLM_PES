#ifndef MLM_CONTRATO_REPOSITORY_MEMORIA_H
#define MLM_CONTRATO_REPOSITORY_MEMORIA_H

#include "IContratoRepository.h"
#include <unordered_map>
#include <algorithm>

namespace MLM {

class ContratoRepositoryMemoria : public IContratoRepository {
public:
    ContratoRepositoryMemoria() = default;
    ~ContratoRepositoryMemoria() override = default;

    OperationResult Salvar(const Contrato& contrato) override {
        // Validação de Consistência: Verificar se já existe outro contrato ativo para esta entidade
        for (const auto& pair : m_contratos) {
            if (pair.first != contrato.ObterId()) {
                if (!contrato.ECompativelCom(pair.second)) {
                    return OperationResult::Failure("Conflito de contrato: Entidade já possui um contrato ativo incompatível.");
                }
            }
        }

        m_contratos[contrato.ObterId()] = contrato;
        return OperationResult::Success();
    }

    Result<Contrato> ObterPorId(uint32_t id) override {
        auto it = m_contratos.find(id);
        if (it != m_contratos.end()) {
            return Result<Contrato>::Success(it->second);
        }
        return Result<Contrato>::Failure("Contrato não encontrado.");
    }

    Result<Contrato> ObterContratoAtivoEntidade(TipoContrato tipo, uint32_t entidadeId) override {
        for (const auto& pair : m_contratos) {
            if (pair.second.ObterTipo() == tipo &&
                pair.second.ObterEntidadeId() == entidadeId &&
                (pair.second.ObterStatus() == StatusContrato::Ativo || pair.second.ObterStatus() == StatusContrato::Emprestado)) {
                return Result<Contrato>::Success(pair.second);
            }
        }
        return Result<Contrato>::Failure("Nenhum contrato ativo encontrado para a entidade.");
    }

    std::vector<Contrato> ObterContratosPorClube(uint32_t clubeId) override {
        std::vector<Contrato> lista;
        for (const auto& pair : m_contratos) {
            if (pair.second.ObterClubeId() == clubeId && pair.second.ObterStatus() == StatusContrato::Ativo) {
                lista.push_back(pair.second);
            }
        }
        return lista;
    }

    std::vector<Contrato> ObterTodos() override {
        std::vector<Contrato> lista;
        lista.reserve(m_contratos.size());
        for (const auto& pair : m_contratos) {
            lista.push_back(pair.second);
        }
        return lista;
    }

    OperationResult Remover(uint32_t id) override {
        auto it = m_contratos.find(id);
        if (it != m_contratos.end()) {
            m_contratos.erase(it);
            return OperationResult::Success();
        }
        return OperationResult::Failure("Contrato não encontrado para remoção.");
    }

private:
    std::unordered_map<uint32_t, Contrato> m_contratos;
};

} // namespace MLM

#endif // MLM_CONTRATO_REPOSITORY_MEMORIA_H
