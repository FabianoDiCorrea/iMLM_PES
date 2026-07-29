#ifndef MLM_PARTIDA_H
#define MLM_PARTIDA_H

#include <cstdint>
#include <string>

namespace MLM {

enum class SituacaoPartida : uint8_t {
    Agendada = 0,
    EmAndamento = 1,
    Finalizada = 2,
    Cancelada = 3
};

enum class ResultadoPartida : uint8_t {
    VitoriaMandante = 0,
    VitoriaVisitante = 1,
    Empate = 2,
    Indefinido = 255
};

class Partida {
public:
    Partida(uint32_t id,
            uint32_t mandanteId,
            uint32_t visitanteId,
            uint16_t anoTemporada,
            uint32_t competicaoId,
            uint16_t rodada);

    ~Partida() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterId() const { return m_id; }
    uint32_t ObterMandanteId() const { return m_mandanteId; }
    uint32_t ObterVisitanteId() const { return m_visitanteId; }
    uint16_t ObterAnoTemporada() const { return m_anoTemporada; }
    uint32_t ObterCompeticaoId() const { return m_competicaoId; }
    uint16_t ObterRodada() const { return m_rodada; }
    uint16_t ObterGolsMandante() const { return m_golsMandante; }
    uint16_t ObterGolsVisitante() const { return m_golsVisitante; }
    SituacaoPartida ObterSituacao() const { return m_situacao; }

    // Consultas de Estado do Domínio
    bool EstaAgendada() const { return m_situacao == SituacaoPartida::Agendada; }
    bool EstaEmAndamento() const { return m_situacao == SituacaoPartida::EmAndamento; }
    bool EstaFinalizada() const { return m_situacao == SituacaoPartida::Finalizada; }
    bool EstaCancelada() const { return m_situacao == SituacaoPartida::Cancelada; }
    bool Empatada() const;
    ResultadoPartida ObterResultado() const;
    uint32_t ObterVencedorId() const; // Retorna 0 em caso de empate ou não finalizada

    // Métodos de Negócio do Domínio
    bool Iniciar();
    bool RegistrarGolMandante();
    bool RegistrarGolVisitante();
    bool DefinirPlacarDirecto(uint16_t golsMandante, uint16_t golsVisitante); // Preparado para importação rápida / OCR futuro
    bool Finalizar();
    bool Cancelar();

private:
    uint32_t m_id;
    uint32_t m_mandanteId;
    uint32_t m_visitanteId;
    uint16_t m_anoTemporada;
    uint32_t m_competicaoId;
    uint16_t m_rodada;

    uint16_t m_golsMandante;
    uint16_t m_golsVisitante;
    SituacaoPartida m_situacao;
};

} // namespace MLM

#endif // MLM_PARTIDA_H
