#ifndef MLM_TECNICO_SERIALIZER_H
#define MLM_TECNICO_SERIALIZER_H

#include "../Domain/Tecnico.h"
#include <sstream>
#include <string>

namespace MLM {

class TecnicoSerializer {
public:
    // Serialização simples em formato delimitado (CSV/String Stream) para persistência leve de save
    static std::string Serializar(const Tecnico& t) {
        std::stringstream ss;
        ss << t.ObterId() << ";"
           << t.ObterPaisId() << ";"
           << (int)t.ObterIdade() << ";"
           << (int)t.ObterReputacao() << ";"
           << (int)t.ObterPotencial() << ";"
           << (int)t.ObterEstiloPreferencial() << ";"
           << (int)t.ObterPersonalidade() << ";"
           << t.ObterClubeAtualId() << ";"
           << t.ObterSelecaoAtualId() << ";"
           << t.ObterSalario() << ";"
           << t.ObterValorContratual() << ";"
           << t.ObterTempoContratoMeses() << ";"
           << (int)t.ObterSituacao() << ";"
           << (t.EHumano() ? 1 : 0) << ";"
           << t.ObterPesEditId();
        return ss.str();
    }

    static Result<Tecnico> Deserializar(uint32_t id, const std::wstring& nome, const std::string& dataStr) {
        std::stringstream ss(dataStr);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 15) {
            return Result<Tecnico>::Failure("Dados de serialização inválidos.");
        }

        try {
            uint32_t paisId = std::stoul(tokens[1]);
            uint8_t idade = static_cast<uint8_t>(std::stoul(tokens[2]));
            uint8_t reputacao = static_cast<uint8_t>(std::stoul(tokens[3]));
            uint8_t potencial = static_cast<uint8_t>(std::stoul(tokens[4]));
            EstiloPreferencial estilo = static_cast<EstiloPreferencial>(std::stoul(tokens[5]));
            PersonalidadeTecnico personalidade = static_cast<PersonalidadeTecnico>(std::stoul(tokens[6]));
            uint32_t clubeId = std::stoul(tokens[7]);
            uint32_t selecaoId = std::stoul(tokens[8]);
            double salario = std::stod(tokens[9]);
            uint16_t tempoMeses = static_cast<uint16_t>(std::stoul(tokens[11]));
            bool ehHumano = (std::stoul(tokens[13]) == 1);
            uint32_t pesEditId = std::stoul(tokens[14]);

            Tecnico t(id, nome, paisId, idade, reputacao, potencial, estilo, personalidade);
            t.DefinirHumano(ehHumano);
            t.DefinirPesEditId(pesEditId);

            if (clubeId > 0) {
                t.AssumirClube(clubeId, salario, tempoMeses, 2026);
            }
            if (selecaoId > 0) {
                t.AssumirSelecao(selecaoId, 2026);
            }

            return Result<Tecnico>::Success(t);
        } catch (...) {
            return Result<Tecnico>::Failure("Erro ao converter tipos na deserialização.");
        }
    }
};

} // namespace MLM

#endif // MLM_TECNICO_SERIALIZER_H
