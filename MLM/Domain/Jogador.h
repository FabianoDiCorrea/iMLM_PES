#ifndef MLM_JOGADOR_H
#define MLM_JOGADOR_H

#include <string>
#include <vector>
#include <cstdint>
#include "../Core/Tipos.h"

namespace MLM {

class Jogador {
public:
    Jogador(uint32_t id, 
            const std::wstring& nome, 
            const std::string& nomeCamisa, 
            uint8_t idade, 
            uint32_t paisId, 
            PosicaoCampo posicaoPrincipal, 
            uint8_t overallBase, 
            uint8_t potencial);

    ~Jogador() = default;

    // Getters de Leitura de Atributos
    uint32_t ObterId() const { return m_id; }
    std::wstring ObterNome() const { return m_nome; }
    std::string ObterNomeCamisa() const { return m_nomeCamisa; }
    uint8_t ObterIdade() const { return m_idade; }
    uint32_t ObterPaisId() const { return m_paisId; }
    PosicaoCampo ObterPosicaoPrincipal() const { return m_posicaoPrincipal; }
    uint8_t ObterOverall() const { return m_overall; }
    uint8_t ObterPotencial() const { return m_potencial; }
    uint32_t ObterClubeId() const { return m_clubeAtualId; }
    uint16_t ObterNumeroCamisa() const { return m_numeroCamisa; }
    float ObterMoral() const { return m_moral; }
    float ObterFormaFisica() const { return m_formaFisica; }
    double ObterValorMercado() const { return m_valorMercado; }

    // Métodos de Negócio de Domínio
    void AlterarClube(uint32_t novoClubeId, uint16_t novoNumeroCamisa);
    void DesvincularClube();
    void AlterarNumeroCamisa(uint16_t novoNumeroCamisa);
    void FazerAniversario();
    void AtualizarMoral(float deltaMoral);
    void AtualizarForma(float novaForma);
    void AtualizarValorMercado(double novoValor);

private:
    uint32_t m_id;
    std::wstring m_nome;
    std::string m_nomeCamisa;
    uint8_t m_idade;
    uint32_t m_paisId;
    PosicaoCampo m_posicaoPrincipal;
    uint8_t m_overall;
    uint8_t m_potencial;
    
    // Estado Dinâmico de Carreira no MLM
    uint32_t m_clubeAtualId; // 0 se for Agente Livre
    uint16_t m_numeroCamisa;
    float m_moral;       // Escala de 0.0 a 100.0
    float m_formaFisica; // Escala de 0.0 a 100.0
    double m_valorMercado;
};

} // namespace MLM

#endif // MLM_JOGADOR_H
