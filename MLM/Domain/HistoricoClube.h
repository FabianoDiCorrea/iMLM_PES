#ifndef MLM_HISTORICO_CLUBE_H
#define MLM_HISTORICO_CLUBE_H

#include <string>
#include <vector>
#include <cstdint>

namespace MLM {

struct RegistroTituloClube {
    uint32_t anoTemporada;
    uint32_t competicaoId;
    std::wstring nomeCompeticao;

    RegistroTituloClube(uint32_t ano = 0, uint32_t id = 0, const std::wstring& nome = L"")
        : anoTemporada(ano), competicaoId(id), nomeCompeticao(nome) {}
};

struct RegistroCampanhaClube {
    uint32_t anoTemporada;
    uint32_t competicaoId;
    std::wstring nomeCompeticao;
    uint16_t posicaoFinal; // Para ligas
    std::wstring faseAlcancada; // Para copas (ex: "Campeão", "Finalista", "Semi-final")
    uint16_t jogos;
    uint16_t vitorias;
    uint16_t empates;
    uint16_t derrotas;

    RegistroCampanhaClube(uint32_t ano = 0, uint32_t compId = 0, const std::wstring& nomeComp = L"",
                           uint16_t pos = 0, const std::wstring& fase = L"",
                           uint16_t j = 0, uint16_t v = 0, uint16_t e = 0, uint16_t d = 0)
        : anoTemporada(ano), competicaoId(compId), nomeCompeticao(nomeComp),
          posicaoFinal(pos), faseAlcancada(fase), jogos(j), vitorias(v), empates(e), derrotas(d) {}
};

} // namespace MLM

#endif // MLM_HISTORICO_CLUBE_H
