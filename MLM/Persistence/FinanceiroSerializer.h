#ifndef MLM_FINANCEIRO_SERIALIZER_H
#define MLM_FINANCEIRO_SERIALIZER_H

#include "../Domain/FinancasClube.h"
#include <sstream>
#include <string>

namespace MLM {

class FinanceiroSerializer {
public:
    static std::string Serializar(const FinancasClube& f) {
        std::stringstream ss;
        ss << f.ObterClubeId() << ";"
           << f.ObterSaldoAtual() << ";"
           << f.ObterOrcamentoTransferencias() << ";"
           << f.ObterFolhaSalarialMensal() << ";"
           << f.ObterReceitasAcumuladasTotal() << ";"
           << f.ObterDespesasAcumuladasTotal();
        return ss.str();
    }

    static Result<FinancasClube> Deserializar(const std::string& dataStr) {
        std::stringstream ss(dataStr);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 6) {
            return Result<FinancasClube>::Failure("Dados de serialização financeira inválidos.");
        }

        try {
            uint32_t clubeId = std::stoul(tokens[0]);
            double saldo = std::stod(tokens[1]);
            double orcamentoTransf = std::stod(tokens[2]);
            double folhaSalarial = std::stod(tokens[3]);

            FinancasClube f(clubeId, saldo, orcamentoTransf, folhaSalarial);
            return Result<FinancasClube>::Success(f);
        } catch (...) {
            return Result<FinancasClube>::Failure("Erro ao desconstruir finanças serializadas.");
        }
    }
};

} // namespace MLM

#endif // MLM_FINANCEIRO_SERIALIZER_H
