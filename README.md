# NetflixNX

Cliente homebrew experimental para Nintendo Switch (Atmosphère), desenhado como uma experiência de streaming **TV/console** com Joy-Con/Pro Controller e touchscreen no modo portátil.

> Status: **UI prototype 0.2 + investigação de viabilidade do playback oficial**.

## UI 0.2

O shell agora inclui:
- home em estilo 10-foot TV, com hero e duas fileiras de mídia;
- foco visual para navegação por controle;
- D-pad e analógico esquerdo;
- L/R para alternar Home, Search, My Netflix e Settings;
- A para abrir/selecionar e B para voltar;
- tela de detalhes;
- shell de player com play/pause;
- touchscreen com tap-to-focus e segundo toque para abrir;
- layout 1280×720 para dock/handheld.

Os cards continuam sendo placeholders: nenhuma arte, catálogo ou asset proprietário da Netflix é distribuído.

## Build

Requer devkitPro/devkitA64, libnx e SDL2 para Switch.

```bash
make
```

Saída esperada: `netflixnx.nro`.

## Controles

| Controle | Ação |
|---|---|
| D-pad / L-stick | mover foco |
| A | selecionar / play-pause no shell |
| B | voltar |
| L / R | seção anterior/próxima |
| + | sair |
| Touch | selecionar; tocar novamente abre |

## Limite atual

A interface e a navegação são independentes do playback. A reprodução Netflix só será conectada se for encontrado um caminho autorizado e compatível com os requisitos atuais do serviço. O projeto não implementa bypass de DRM, extração de CDM/chaves, captura indevida de tokens ou redistribuição de conteúdo.

## Próximas etapas

1. adicionar texto/fontes e ícones próprios;
2. carrosséis com scroll e animação de foco;
3. busca/teclado e perfis;
4. capability probe de rede, codecs e hardware decode com mídia própria;
5. investigar autenticação/DRM oficial e documentar blockers;
6. conectar playback somente se a etapa anterior for viável.

Veja `docs/ROADMAP.md` e `docs/DRM-FEASIBILITY.md`.

## Licença

GPL-3.0-or-later.

Netflix e Nintendo Switch são marcas de seus respectivos titulares. Projeto não afiliado à Netflix ou Nintendo.
