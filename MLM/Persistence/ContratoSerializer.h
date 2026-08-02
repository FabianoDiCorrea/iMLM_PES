#ifndef MLM_CONTRATO_SERIALIZER_H
#define MLM_CONTRATO_SERIALIZER_H

#include "../Domain/Contrato.h"
#include <sstream>
#include <string>

namespace MLM {

class ContratoSerializer {
public:
    static std::string Serializar(const Contrato& c) {
        std::stringstream ss;
        ss << c.ObterId() << ";"
           << (int)c.ObterTipo() << ";"
           << c.ObterEntidadeId() << ";"
           << c.ObterClubeId() << ";"
           << c.ObterClubeOrigemEmprestimoId() << ";"
           << c.ObterAnoInicio() << ";"
           << c.ObterDuracaoMeses() << ";"
           << c.ObterSalarioMensal() << ";"
           << c.ObterMultaRescisoria() << ";"
           << (int)c.ObterStatus() << ";"
           << (c.ERenovavel() ? 1 : 0) << ";"
           << c.ObterBonus().bonusPorGol << ";"
           << c.ObterBonus().bonusPorPartida << ";"
           << c.ObterBonus().bonusPorTitulo;
        return ss.str();
    }

    static Result<Contrato> Deserializar(const std::string& dataStr) {
        std::stringstream ss(dataStr);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 14) {
            return Result<Contrato>::Failure("Dados de serialização do contrato inválidos.");
        }

        try {
            uint32_t id = std::stoul(tokens[0]);
            TipoContrato tipo = static_cast<TipoContrato>(std::stoul(tokens[1]));
            uint32_t entidadeId = std::stoul(tokens[2]);
            uint32_t clubeId = std::stoul(tokens[3]);
            uint32_t clubeOrigemEmprestimoId = std::stoul(tokens[4]);
            uint32_t anoInicio = std::stoul(tokens[5]);
            uint16_t duracaoMeses = static_cast<uint16_t>(std::stoul(tokens[6]));
            double salarioMensal = std::stod(tokens[7]);
            double multaRescisoria = std::stod(tokens[8]);
            StatusContrato status = static_cast<StatusContrato>(std::stoul(tokens[9]));
            bool renovavel = (std::stoul(tokens[10]) == 1);

            BonusContratual bonus;
            bonus.bonusPorGol = std::stod(tokens[11]);
            bonus.bonusPorPartida = std::stod(tokens[12]);
            bonus.bonusPorTitulo = std::stod(tokens[13]);

            Contrato c(id, tipo, entidadeId, clubeId, anoInicio, duracaoMeses, salarioMensal, multaRescisoria, bonus);
            c.DefinirRenovavel(renovavel);

            if (status == StatusContrato::Emprestado && clubeOrigemEmprestimoId > 0) {
                c.ProcessarEmprestimo(clubeId);
            } else if (status == StatusContrato::Encerrado) {
                c.Encerrar();
            } else if (status == StatusContrato::Rescindido) {
                c.Rescindir();
            }

            return Result<Contrato>::Success(c);
        } catch (...) {
            return Result<Contrato>::Failure("Erro ao desconstruir contrato serializado.");
        }
    }
};

} // namespace MLM

#endif // MLM_CONTRATO_SERIALIZER_H
