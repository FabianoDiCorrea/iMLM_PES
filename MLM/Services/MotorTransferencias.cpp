#include "MotorTransferencias.h"
#include <algorithm>
#include <cmath>

namespace MLM {

double MotorTransferencias::CalcularValorMercado(const Jogador& jogador) const
{
    // 1. Base por Overall
    double baseOverall = std::pow(jogador.ObterOverall() / 10.0, 3.8) * 1000.0; // Ex: 80 overall -> ~2.7M, 90 overall -> ~13.5M

    // 2. Multiplicador de Idade
    double multIdade = 1.0;
    uint8_t idade = jogador.ObterIdade();
    if (idade <= 21) multIdade = 1.6;
    else if (idade <= 25) multIdade = 1.3;
    else if (idade <= 29) multIdade = 1.0;
    else if (idade <= 32) multIdade = 0.7;
    else multIdade = 0.3; // 33+ anos

    // 3. Multiplicador de Potencial
    double multPotencial = 1.0 + ((double)jogador.ObterPotencial() - (double)jogador.ObterOverall()) * 0.05;

    return (baseOverall * multIdade * multPotencial);
}

double MotorTransferencias::CalcularSalarioExigido(const Jogador& jogador) const
{
    double valMercado = CalcularValorMercado(jogador);
    // Salário base anual proporcional ao valor de mercado (~5% a 8% a.a.)
    return (valMercado * 0.06);
}

bool MotorTransferencias::AvaliarDecisaoClube(const PropostaTransferencia& proposta,
                                              const Jogador& jogador,
                                              const Clube& clubeVendedor,
                                              double valorMercado,
                                              std::wstring& outMotivo,
                                              double& outValorExigido) const
{
    double multiplicadorExigencia = 1.0;

    // Fator Ídolo / Importância no Elenco (Overall alto no clube)
    if (jogador.ObterOverall() >= 85) {
        multiplicadorExigencia += 0.35; // +35% de sobrepreço para liberar ídolo
    }

    // Fator Jovem Promessa (Potencial >= 88 e Idade <= 21)
    if (jogador.ObterPotencial() >= 88 && jogador.ObterIdade() <= 21) {
        multiplicadorExigencia += 0.40; // +40% para joia da base
    }

    // Fator Reputação do Clube Vendedor
    if (clubeVendedor.ObterReputacao() >= 85) {
        multiplicadorExigencia += 0.15;
    }

    outValorExigido = valorMercado * multiplicadorExigencia;

    if (proposta.valorOferecido < outValorExigido) {
        if (jogador.ObterOverall() >= 85) {
            outMotivo = L"O clube nao aceita vender seu principal idolo por este valor.";
        } else {
            outMotivo = L"A proposta financeira esta abaixo da avaliacao do clube vendedor.";
        }
        return false;
    }

    outMotivo = L"Proposta aceita pela diretoria do clube vendedor.";
    return true;
}

bool MotorTransferencias::AvaliarDecisaoJogador(const PropostaTransferencia& proposta,
                                                const Jogador& jogador,
                                                const Clube& clubeVendedor,
                                                const Clube& clubeComprador,
                                                double salarioBase,
                                                std::wstring& outMotivo,
                                                double& outSalarioExigido) const
{
    // 1. Reputação do Clube Comprador vs Vendedor
    if (clubeComprador.ObterReputacao() < (clubeVendedor.ObterReputacao() - 15)) {
        // Exige um salário 50% maior para aceitar jogar em clube menor
        outSalarioExigido = salarioBase * 1.5;
        if (proposta.salarioOferecido < outSalarioExigido) {
            outMotivo = L"O jogador nao tem interesse em se transferir para um clube de reputacao inferior.";
            return false;
        }
    } else {
        outSalarioExigido = salarioBase * 1.05; // 5% de aumento padrão
    }

    if (proposta.salarioOferecido < outSalarioExigido) {
        outMotivo = L"O salario oferecido esta abaixo das pretensoes salariais do atleta.";
        return false;
    }

    outMotivo = L"O jogador aceitou os termos contratuais propostos.";
    return true;
}

RespostaTransferencia MotorTransferencias::AvaliarProposta(const PropostaTransferencia& proposta,
                                                           const Jogador& jogador,
                                                           const Clube& clubeVendedor,
                                                           const Clube& clubeComprador) const
{
    RespostaTransferencia resposta;
    resposta.status = StatusProposta::Pendente;
    resposta.clubeAceitou = false;
    resposta.jogadorAceitou = false;

    // 1. Validação Financeira do Comprador
    if (clubeComprador.ObterSaldo() < proposta.valorOferecido) {
        resposta.status = StatusProposta::SaldoInsuficienteComprador;
        resposta.motivoClube = L"O clube comprador nao possui saldo em orcamento suficiente.";
        resposta.motivoJogador = L"Transferencia cancelada por falta de fundos.";
        return resposta;
    }

    double valorMercado = CalcularValorMercado(jogador);
    double salarioBase = CalcularSalarioExigido(jogador);

    // 2. Avalia Decisão do Clube Vendedor
    resposta.clubeAceitou = AvaliarDecisaoClube(proposta, jogador, clubeVendedor, valorMercado, resposta.motivoClube, resposta.valorMinimoExigido);
    if (!resposta.clubeAceitou) {
        resposta.status = StatusProposta::RecusadaPeloClube;
        return resposta;
    }

    // 3. Avalia Decisão do Jogador
    resposta.jogadorAceitou = AvaliarDecisaoJogador(proposta, jogador, clubeVendedor, clubeComprador, salarioBase, resposta.motivoJogador, resposta.salarioMinimoExigido);
    if (!resposta.jogadorAceitou) {
        resposta.status = StatusProposta::RecusadaPeloJogador;
        return resposta;
    }

    // Ambas as partes aceitaram
    resposta.status = StatusProposta::AceitaClubeEJogador;
    return resposta;
}

std::vector<PropostaTransferencia> MotorTransferencias::GerarMercadoReativo(const std::vector<Clube>& clubes,
                                                       const std::vector<Jogador>& todosJogadores,
                                                       const std::vector<uint32_t>& clubesRebaixadosIds,
                                                       const std::vector<uint32_t>& clubesCampeoesIds) const {
    std::vector<PropostaTransferencia> propostasGeradas;

    // 1. Reação a Clubes Rebaixados:
    // Jogadores com Overall >= 78 em clubes rebaixados pedem pra sair e atraem propostas de clubes grandes
    for (uint32_t rebaixadoId : clubesRebaixadosIds) {
        for (const auto& jog : todosJogadores) {
            if (jog.ObterClubeId() == rebaixadoId && jog.ObterOverall() >= 78) {
                // Procura um clube comprador (não rebaixado e com boa reputação)
                for (const auto& comprador : clubes) {
                    bool eRebaixado = std::find(clubesRebaixadosIds.begin(), clubesRebaixadosIds.end(), comprador.ObterId()) != clubesRebaixadosIds.end();
                    if (!eRebaixado && comprador.ObterId() != rebaixadoId && comprador.ObterReputacao() >= 75) {
                        
                        PropostaTransferencia prop;
                        prop.jogadorId = jog.ObterId();
                        prop.clubeVendedorId = rebaixadoId;
                        prop.clubeCompradorId = comprador.ObterId();
                        
                        // Oferta com desconto porque o clube foi rebaixado (~15% a 20% abaixo da pedida)
                        double valMercado = CalcularValorMercado(jog);
                        prop.valorOferecido = valMercado * 0.85;
                        prop.salarioOferecido = CalcularSalarioExigido(jog) * 1.1; // Oferece salário maior pro jogador aceitar sair
                        prop.anosContratoOferecidos = 3;

                        propostasGeradas.push_back(prop);
                        break; // Uma proposta por jogador de destaque rebaixado
                    }
                }
            }
        }
    }

    // 2. Reação a Clubes Campeões:
    // Campeões buscam jovens promessas (Potencial >= 84 e Idade <= 22) de outros clubes para se reforçarem
    for (uint32_t campeaoId : clubesCampeoesIds) {
        for (const auto& jog : todosJogadores) {
            if (jog.ObterClubeId() != campeaoId && jog.ObterPotencial() >= 84 && jog.ObterIdade() <= 22) {
                PropostaTransferencia prop;
                prop.jogadorId = jog.ObterId();
                prop.clubeVendedorId = jog.ObterClubeId();
                prop.clubeCompradorId = campeaoId;
                
                // Campeão faz proposta agressiva com sobrepreço (+25%)
                double valMercado = CalcularValorMercado(jog);
                prop.valorOferecido = valMercado * 1.25;
                prop.salarioOferecido = CalcularSalarioExigido(jog) * 1.2;
                prop.anosContratoOferecidos = 4;

                propostasGeradas.push_back(prop);
                break; // Limita uma contratação de peso por campeão por janela
            }
        }
    }

    return propostasGeradas;
}

} // namespace MLM
