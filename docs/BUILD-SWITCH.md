# Build e teste no Nintendo Switch

O artefato executável do projeto é `netflixnx.nro`.

## GitHub Actions

Cada push em `main` executa o workflow **Build Switch NRO** usando a imagem oficial `devkitpro/devkita64`, instala `switch-sdl2`, compila o projeto e publica o artefato **NetflixNX-Switch**.

## Instalação para teste

Extraia o artefato e copie:

`netflixnx.nro -> /switch/NetflixNX/NetflixNX.nro`

Abra pelo Homebrew Menu. Para ter mais memória disponível em testes posteriores de vídeo, prefira iniciar o Homebrew Menu em title mode em vez do applet mode.

## Estado deste build

Este primeiro NRO testável valida shell/UI, Joy-Con/Pro Controller, touchscreen, telas de login/perfis/browse/details/player e capability probe básico. Ele **ainda não reproduz Netflix**: autenticação Netflix, receiver reconhecido pela Netflix e DRM continuam como etapas separadas de viabilidade.
