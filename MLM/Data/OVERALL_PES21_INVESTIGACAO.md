# Overall PES 2021 - estado da investigação

## Conclusão confirmada

O overall exibido pelo PES 2021 não aparece como um campo simples gravado no registro binário do jogador dentro do `EDIT00000000`.

Foi criado e executado o inspetor `MLM/Tools/InspecionarOverallNoEdit.cpp`, comparando 8 jogadores com overall conhecido no PES:

- Robinho Júnior: 73
- Neymar: 83
- Gabriel Brazão: 79
- Bento: 83
- Bremer: 85
- Ederson: 86
- Danilo: 80
- Carlos Cuesta: 80

Resultado do teste:

- candidatos 7-bit iguais ao overall PES em todos os alvos: 0
- candidatos 8-bit iguais ao overall PES em todos os alvos: 0

Portanto, o iMLM não deve tratar `overall` como dado lido diretamente do EDIT. O número precisa ser reproduzido pela mesma lógica de cálculo usada pelo PES.

## Regra de qualidade para o iMLM

A fórmula atual do projeto não deve ser considerada oficial enquanto não passar por validação massiva por posição.

O nome correto do campo, até a fórmula ser comprovada, é:

- `overallEstimado`

O campo só deve voltar a ser chamado de `overallPesJogo` quando a fórmula for validada contra uma base grande de jogadores reais do PES.

## Caminho correto

1. Coletar amostras reais com:
   - posição registrada;
   - atributos completos;
   - overall exibido pelo PES;
   - ratings por posição, quando disponíveis.
2. Ajustar uma fórmula por posição.
3. Validar por erro absoluto:
   - meta mínima: 95% dos jogadores com diferença 0;
   - divergências restantes no máximo 1 ponto;
   - nenhum caso crítico de mercado com diferença maior que 1.
4. Só então congelar a fórmula como base oficial do iMLM.

## Fontes úteis

O PESDB 2021 informa que seus jogadores foram exportados do PES 2021 Data Pack 7.00 e LiveUpdate 2021/08/26, e expõe atributos + overall por jogador. Ele serve como base pública para engenharia reversa da fórmula, mas não substitui validação no EDIT customizado usado pelo iMLM.
