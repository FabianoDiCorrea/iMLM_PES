#include "MotorTemporada.h"
#include <algorithm>

namespace MLM {

MotorTemporada::MotorTemporada(Temporada& temporadaAtual)
    : m_temporada(temporadaAtual),
      m_faseAtual(FaseTemporada::PreTemporada)
{
    if (m_temporada.EstaPlanejada()) {
        m_temporada.Iniciar();
    }
}

bool MotorTemporada::AvancarFase()
{
    switch (m_faseAtual) {
        case FaseTemporada::PreTemporada:
            m_faseAtual = FaseTemporada::JanelaTransferencias;
            return true;
        case FaseTemporada::JanelaTransferencias:
            m_faseAtual = FaseTemporada::EmCompeticao;
            return true;
        case FaseTemporada::EmCompeticao:
            m_faseAtual = FaseTemporada::PosTemporada;
            return true;
        case FaseTemporada::PosTemporada:
            m_faseAtual = FaseTemporada::Encerrada;
            m_temporada.Encerrar();
            return true;
        case FaseTemporada::Encerrada:
            return false; // Já está encerrada
        default:
            return false;
    }
}

bool MotorTemporada::ExecutarPreTemporada(std::vector<Jogador>& jogadores, const std::vector<DesempenhoTemporada>& desempenhos)
{
    if (m_faseAtual != FaseTemporada::PreTemporada) {
        return false; // Fase inválida
    }

    // 1. Atualização de Idades e Aplicação do Motor de Evolução
    for (size_t i = 0; i < jogadores.size(); ++i) {
        jogadores[i].FazerAniversario(); // Incrementa idade

        DesempenhoTemporada desp = (i < desempenhos.size()) ? desempenhos[i] : DesempenhoTemporada{};
        ResultadoEvolucao res = m_motorEvolucao.CalcularEvolucaoAnual(jogadores[i], desp);

        // Aplica o ajuste de Overall no objeto de domínio Jogador
        if (res.deltaOverall > 0) {
            jogadores[i].AtualizarForma(jogadores[i].ObterOverall() + res.deltaOverall);
        } else if (res.deltaOverall < 0) {
            uint8_t novoOv = (jogadores[i].ObterOverall() > (uint8_t)(-res.deltaOverall)) ? 
                             jogadores[i].ObterOverall() + res.deltaOverall : 40;
            jogadores[i].AtualizarForma(novoOv);
        }
    }

    return true;
}

bool MotorTemporada::ProcessarJanelaTransferencias(const std::vector<PropostaTransferencia>& propostas,
                                                  const std::vector<Jogador>& jogadores,
                                                  const std::vector<Clube>& clubes,
                                                  std::vector<RespostaTransferencia>& outRespostas)
{
    if (m_faseAtual != FaseTemporada::JanelaTransferencias && m_faseAtual != FaseTemporada::PreTemporada) {
        return false;
    }

    outRespostas.clear();
    outRespostas.reserve(propostas.size());

    for (const auto& prop : propostas) {
        // Encontra o Jogador
        const Jogador* jogPtr = nullptr;
        for (const auto& j : jogadores) {
            if (j.ObterId() == prop.jogadorId) { jogPtr = &j; break; }
        }

        // Encontra Clube Vendedor e Comprador
        const Clube* vendPtr = nullptr;
        const Clube* compPtr = nullptr;
        for (const auto& c : clubes) {
            if (c.ObterId() == prop.clubeVendedorId) vendPtr = &c;
            if (c.ObterId() == prop.clubeCompradorId) compPtr = &c;
        }

        if (jogPtr && vendPtr && compPtr) {
            RespostaTransferencia res = m_motorTransferencias.AvaliarProposta(prop, *jogPtr, *vendPtr, *compPtr);
            outRespostas.push_back(res);
        }
    }

    return true;
}

bool MotorTemporada::ExecutarCompeticoes(std::vector<Competicao>& competicoes, ModoResolucaoCompeticao modo)
{
    if (m_faseAtual != FaseTemporada::EmCompeticao) {
        return false;
    }

    for (auto& comp : competicoes) {
        if (comp.EstaPlanejada()) {
            comp.Iniciar();
        }

        if (modo == ModoResolucaoCompeticao::ModoHumano) {
            // No modo humano, apenas aguarda importação de partidas registradas
        } else {
            // No modo simulado, finaliza as partidas existentes
        }

        comp.Encerrar();
    }

    return true;
}

bool MotorTemporada::ExecutarPosTemporada()
{
    if (m_faseAtual != FaseTemporada::PosTemporada) {
        return false;
    }

    // Execução de premiações, encerramento financeiro e preparação de estatísticas anuais
    return true;
}

Temporada MotorTemporada::ConcluirEVirarTemporada()
{
    if (m_faseAtual != FaseTemporada::Encerrada) {
        m_faseAtual = FaseTemporada::Encerrada;
        m_temporada.Encerrar();
    }

    return m_temporada.CriarProximaTemporada();
}

} // namespace MLM
