# MOTOR DE EVOLUÇÃO E DESENVOLVIMENTO DOS JOGADORES (`MLM`)

**Documentação Técnica das Curvas e Algoritmos de Evolução**

---

## 1. VISÃO GERAL DO MOTOR

O **Motor de Evolução** é o componente de serviço do Master League Manager (`MLM/Services/MotorEvolucao.h`) responsável por calcular os ajustes anuais no nível dos atletas ao fim de cada temporada.

Este motor funciona **100% desconectado do PES 2021** e baseia-se exclusivamente no domínio de dados do MLM.

---

## 2. CURVAS DE EVOLUÇÃO POR FASE DE CARREIRA

O desenvolvimento do atleta é regido pela idade e pela sua posição de origem:

### Jogadores de Linha

```text
[16 - 20 Anos] Crescimento Jovem  -> Ganho acelerado (até +5/ano) dependente de minutos.
[21 - 25 Anos] Desenvolvimento    -> Crescimento constante rumo ao Potencial.
[26 - 29 Anos] Auge & Estabilidade-> Atleta no ápice físico e técnico. Variação suave por nota.
[30 - 32 Anos] Início do Declínio -> Queda suave de atributos físicos.
[33+ Anos]     Regressão Natural  -> Queda gradual de físico e fôlego (até -4/ano).
```

### Goleiros (Curva Especial)

Goleiros possuem maturidade tardia e longevidade ampliada:
```text
[16 - 21 Anos] Crescimento Jovem
[22 - 26 Anos] Desenvolvimento
[27 - 32 Anos] Auge & Estabilidade (Auge estendido até 32 anos)
[33 - 35 Anos] Início do Declínio
[36+ Anos]     Regressão Natural
```

---

## 3. ATRIBUTO DE POTENCIAL EXCLUSIVO DO MLM

O **Potencial** é um valor exclusivo do MLM (ex: Overall 76 / Potencial 91).

- **Teto Inviolável:** O motor proíbe que qualquer jogador ultrapasse seu Potencial máximo.
- **Aceleração por Desempenho:** Minutagem (30+ partidas) e nota média alta (>= 7.5) fazem o jogador chegar mais rápido ao seu potencial.
- **Lesões:** Lesões graves (> 60 dias inativo) penalizam a taxa de crescimento da temporada.

---

## 4. LIMITES ANUAIS DE VARIAÇÃO

- **Ganho Máximo em 1 Temporada:** `+5` no Overall.
- **Perda Máxima em 1 Temporada:** `-4` no Overall.

---

## 5. EXEMPLOS PRÁTICOS CALCULADOS PELO MOTOR

1. **Jovem Promessa (18 Anos, Overall 72, Potencial 90, 35 partidas):**
   - **Resultado:** Overall sobe de `72` para `77` (`+5`).
2. **Atleta de Linha Veterano (35 Anos, Overall 82, 20 partidas):**
   - **Resultado:** Overall cai de `82` para `79` (`-3`).
3. **Goleiro Experiente (31 Anos, Overall 84, Potencial 88, 38 partidas):**
   - **Resultado:** Mantém estabilidade e evolui suavemente de `84` para `86` (`+2`).
