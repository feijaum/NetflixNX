# NetflixNX

Cliente homebrew experimental para Nintendo Switch (Atmosphère) com foco em uma experiência de streaming **TV/console**, navegação por Joy-Con/Pro Controller e suporte a toque no modo portátil.

> Status: **Fase 0/1 — shell nativo + investigação de viabilidade do playback oficial**.

## Objetivo

O NetflixNX pretende ser um cliente para uso pessoal com uma assinatura Netflix válida. O projeto **não** fornece conteúdo, não contorna assinatura, geobloqueio, DRM/CDM ou controles de acesso e não inclui chaves, cookies, tokens ou credenciais.

A prioridade é funcionar diretamente no Switch, sem PC/Android/servidor de vídeo pago como requisito permanente.

## UX alvo

- Interface 1280×720 pensada para TV/console.
- D-pad / analógico: mover foco.
- A: selecionar.
- B: voltar.
- L/R: trocar seções.
- +: menu/saída.
- Touchscreen: tocar nos mesmos elementos focáveis no modo portátil.
- Controles físicos e toque coexistem.
- Navegação inspirada na ergonomia de apps de mídia do Switch, sem copiar assets proprietários.

## Arquitetura planejada

```
src/
  app/        ciclo de vida e estado
  ui/         shell 10-foot UI
  input/      Joy-Con/Pro Controller/touch
  platform/   integração libnx/SDL
  playback/   interface de playback (sem implementação DRM)
docs/
  ARCHITECTURE.md
  ROADMAP.md
  DRM-FEASIBILITY.md
```

A pesquisa técnica usa o Green-NX como referência arquitetural para partes genéricas do Switch: execução em title mode, UI/input, decode por hardware e renderização. Qualquer código GPL incorporado no futuro deverá manter compatibilidade de licença e atribuição.

## Build

Requer devkitPro/devkitA64, libnx e SDL2 para Switch.

```bash
make
```

Saída esperada:

```
netflixnx.nro
```

## Escopo atual

O primeiro executável é deliberadamente um **shell de UI**: navegação por controle + touch e telas Home/Search/My Netflix/Settings. O módulo de playback fica atrás de uma interface e só será conectado depois de confirmar um caminho compatível com o serviço oficial e o DRM exigido.

## Regra de segurança do projeto

Não implementar:
- bypass de Widevine/PlayReady ou outro DRM;
- extração/uso de CDM ou chaves de terceiros;
- captura/reuso indevido de cookies/tokens;
- endpoints para conteúdo sem autorização;
- redistribuição de vídeo da Netflix.

Se o playback nativo oficial não for tecnicamente possível no ambiente homebrew, isso será documentado como blocker em vez de substituir por um bypass.

## Licença

GPL-3.0-or-later. Veja `LICENSE`.

Netflix, Nintendo Switch e demais marcas pertencem aos respectivos titulares. Este projeto não é afiliado, aprovado ou suportado por Netflix ou Nintendo.
