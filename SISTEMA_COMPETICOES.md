# ARQUITETURA DO DOMÍNIO DE COMPETIÇÕES (`MLM`)

**Documentação Técnica da Estrutura e Edições de Torneios**

---

## 1. VISÃO GERAL DO DOMÍNIO

O **Sistema de Competições** do Master League Manager estabelece a separação entre a **Competição Base** (entidade perene que representa a instituição da liga ou copa) e a **Edição de Competição** (instância vinculada a um ano específico de Temporada).

Toda a nomenclatura do domínio está padronizada integralmente em Português (PT-BR).

---

## 2. ESTRUTURA E REGRAS DE NEGÓCIO

### 1. Enumerações e Estruturas (`MLM/Domain/CompeticaoTipos.h`)
- **`TipoCompeticao`:** `Liga`, `Copa`, `Supercopa`, `Mundial`, `Continental`, `Amistoso`.
- **`CategoriaCompeticao`:** `Nacional`, `Continental`, `Mundial`, `InternacionalSelecoes`.
- **`FormatoCompeticao`:** `PontosCorridos`, `MataMata`, `GruposEMataMata`, `LigaComPlayoff`.
- **`FrequenciaCompeticao`:** `Anual` (1 ano), `Bienal` (2 anos), `Quadrienal` (4 anos).
- **`RegrasCompeticao`:** Define vagas continentais, vagas de rebaixamento, premiações financeiras de campeão/vice e prestígio.

### 2. Classe `EdicaoCompeticao` (`MLM/Domain/EdicaoCompeticao.h`)
Representa o torneio em um ano de carreira (ex: *Brasileirão Série A 2026*):
- Armazena a lista de clubes participantes (`ParticipanteEdicao`).
- Registra o Campeão (`m_campeaoId`) e Vice-campeão (`m_viceId`).
- Trava a adição de participantes e alteração de resultados após a edição ser marcada como encerrada.

---

## 3. SEPARAÇÃO DE RESPONSABILIDADES

```text
[Competicao] ── (Possui N) ──► [EdicaoCompeticao (Ano 2026)]
 (Informações fixas)           ├── Participantes Registrados
                               ├── Partidas da Edição
                               └── Resultado Final (Campeão/Vice)
```
