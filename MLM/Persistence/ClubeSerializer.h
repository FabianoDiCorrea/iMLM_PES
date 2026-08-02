#ifndef MLM_CLUBE_SERIALIZER_H
#define MLM_CLUBE_SERIALIZER_H

#include "../Domain/Clube.h"
#include <sstream>
#include <string>

namespace MLM {

class ClubeSerializer {
public:
    static std::string Serializar(const Clube& c) {
        std::stringstream ss;
        ss << c.ObterId() << ";"
           << c.ObterPaisId() << ";"
           << c.ObterEstadioId() << ";"
           << c.ObterLigaId() << ";"
           << c.ObterTecnicoId() << ";"
           << c.ObterSaldo() << ";"
           << c.ObterOrcamentoTransferencias() << ";"
           << c.ObterFolhaSalarialMensal() << ";"
           << (int)c.ObterReputacao() << ";"
           << (int)c.ObterTradicaoInstitucional() << ";"
           << (c.EstaAtivo() ? 1 : 0);
        return ss.str();
    }

    static Result<Clube> Deserializar(uint32_t id, const std::wstring& nome, const std::string& nomeAbreviado, const std::string& dataStr) {
        std::stringstream ss(dataStr);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 11) {
            return Result<Clube>::Failure("Dados de serialização do clube inválidos.");
        }

        try {
            uint32_t paisId = std::stoul(tokens[1]);
            uint32_t estadioId = std::stoul(tokens[2]);
            uint32_t ligaId = std::stoul(tokens[3]);
            uint32_t tecnicoId = std::stoul(tokens[4]);
            double saldo = std::stod(tokens[5]);
            double orcamentoTransf = std::stod(tokens[6]);
            double folhaSalarial = std::stod(tokens[7]);
            uint8_t reputacao = static_cast<uint8_t>(std::stoul(tokens[8]));
            uint8_t tradicao = static_cast<uint8_t>(std::stoul(tokens[9]));
            bool ativo = (std::stoul(tokens[10]) == 1);

            Clube c(id, nome, nomeAbreviado, paisId, L"", estadioId, ligaId);
            c.ReceberValor(saldo);
            c.DefinirOrcamentoTransferencias(orcamentoTransf);
            c.DefinirFolhaSalarialMensal(folhaSalarial);
            c.AlterarReputacao(reputacao);
            c.DefinirTradicaoInstitucional(tradicao);
            if (tecnicoId > 0) {
                c.AlterarTecnico(tecnicoId);
            }
            if (!ativo) {
                c.Desativar();
            }

            return Result<Clube>::Success(c);
        } catch (...) {
            return Result<Clube>::Failure("Erro ao desconstruir clube serializado.");
        }
    }
};

} // namespace MLM

#endif // MLM_CLUBE_SERIALIZER_H
