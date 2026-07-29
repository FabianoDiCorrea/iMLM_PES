#ifndef MLM_TEMPORADA_H
#define MLM_TEMPORADA_H

#include <cstdint>

namespace MLM {

enum class SituacaoTemporada : uint8_t {
    Planejada = 0,
    EmAndamento = 1,
    Encerrada = 2
};

class Temporada {
public:
    Temporada(uint16_t ano, uint16_t numeroSequencial);
    ~Temporada() = default;

    // Getters de Leitura de Atributos
    uint16_t ObterAno() const { return m_ano; }
    uint16_t ObterNumeroSequencial() const { return m_numeroSequencial; }
    SituacaoTemporada ObterSituacao() const { return m_situacao; }

    // Consultas de Estado
    bool EstaPlanejada() const { return m_situacao == SituacaoTemporada::Planejada; }
    bool EstaEmAndamento() const { return m_situacao == SituacaoTemporada::EmAndamento; }
    bool EstaEncerrada() const { return m_situacao == SituacaoTemporada::Encerrada; }

    // Métodos de Negócio do Domínio
    bool Iniciar();
    bool Encerrar();
    Temporada CriarProximaTemporada() const;

private:
    uint16_t m_ano;
    uint16_t m_numeroSequencial; // Ex: 1ª Temporada, 2ª Temporada, etc.
    SituacaoTemporada m_situacao;
};

} // namespace MLM

#endif // MLM_TEMPORADA_H
