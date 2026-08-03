# Fórmula real parcial de overall PES 2021

Fonte: texto fornecido pelo Fabiano durante pesquisa do iMLM.

## Estrutura informada

O overall não fica salvo no EDIT. O jogo calcula em tempo real.

A fórmula real informada usa pesos inteiros por posição:

1. Cada atributo contribui como:

```text
(valor - 25) * peso
```

2. As contribuições são somadas.
3. O total é dividido por 100.
4. Um bônus de posição é adicionado.
5. O resultado é misturado em proporção 60/40 com uma média ponderada.

A parte ainda ambígua é exatamente o item 5: precisamos descobrir, por teste, qual é a segunda média ponderada e a ordem exata do arredondamento.

## Pesos reais informados

### PTE/PTD - LWF/RWF

- ball_ctrl: 19
- atk: 17
- drib: 15
- speed: 15
- exp_pwr/aceleração: 15
- finish: 11
- loftpass: 9
- tight_pos: 7
- stamina: 6
- weak_acc: 47
- bônus posição: +10

### MLE/MLD - LMF/RMF

- speed: 24
- exp_pwr/aceleração: 21
- drib: 17
- ball_ctrl: 15
- stamina: 13
- loftpass: 12
- weak_acc: 56
- bônus posição: +8

## Observação importante

O peso de `weak_acc` parece grande, mas o atributo usa escala 1-4. Então o impacto prático total é baixo, aproximadamente 1 ponto.

## Próximo teste

Usar Robinho Júnior como alvo PTD/RWF:

- PES: 73
- iMLM atual: 73
- stats conhecidas pelo banco GOGOSZ

Objetivo: descobrir qual interpretação do 60/40 reproduz o número sem chute.
