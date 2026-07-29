#ifndef MLM_COMPETICAO_H
#define MLM_COMPETICAO_H

#include <string>
#include <vector>
#include <cstdint>
#include "Clube.h"
#include "Partida.h"

namespace MLM {

enum class TipoCompeticao : uint8_t {
    Liga = 0,
    Copa = 1,
    Supercopa = 2,
    Mundial = 3,
    Continental = 4,
    Amistoso = 5,
    Outros = 255
};

enum class SituacaoCompeticao : uint8_t {
    Planejada = 0,
    EmAndamento = 1,
    Encerrada = 2
};

class Competicao {
public:
    Competicao(uint32_t id,
               const std::wstring& nome,
               const std::string& nomeAbreviado,
               uint16_t anoTemporada,
               TipoCompeticao tipo,
               size_t limiteMaximoClubes);

    virtual ~Competicao() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterId() const { return m_id; }
    std::wstring ObterNome() const { return m_nome; }
    std::string ObterNomeAbreviado() const { return m_nomeAbreviado; }
    uint16_t ObterAnoTemporada() const { return m_anoTemporada; }
    TipoCompeticao ObterTipo() const { return m_tipo; }
    SituacaoCompeticao ObterSituacao() const { return m_situacao; }
    size_t ObterLimiteMaximoClubes() const { return m_limiteMaximoClubes; }

    // Consultas de Estado
    bool EstaPlanejada() const { return m_situacao == SituacaoCompeticao::Planejada; }
    bool EstaEmAndamento() const { return m_situacao == SituacaoCompeticao::EmAndamento; }
    bool EstaEncerrada() const { return m_situacao == SituacaoCompeticao::Encerrada; }

    // Transição de Estados
    bool Iniciar();
    bool Encerrar();

    // Gestão de Clubes Participantes
    bool AdicionarClube(uint32_t clubeId);
    bool RemoverClube(uint32_t clubeId);
    bool PossuiClube(uint32_t clubeId) const;
    size_t QuantidadeClubes() const { return m_clubesIds.size(); }
    const std::vector<uint32_t>& ObterClubes() const { return m_clubesIds; }

    // Gestão de Partidas
    bool AdicionarPartida(const Partida& partida);
    bool PossuiPartida(uint32_t partidaId) const;
    size_t QuantidadePartidas() const { return m_partidas.size(); }
    const std::vector<Partida>& ObterPartidas() const { return m_partidas; }

private:
    uint32_t m_id;
    std::wstring m_nome;
    std::string m_nomeAbreviado;
    uint16_t m_anoTemporada;
    TipoCompeticao m_tipo;
    SituacaoCompeticao m_situacao;
    size_t m_limiteMaximoClubes;

    std::vector<uint32_t> m_clubesIds;
    std::vector<Partida> m_partidas;
};

} // namespace MLM

#endif // MLM_COMPETICAO_H
