# DRM / Playback Feasibility

Este documento é intencionalmente conservador.

## Pergunta

É possível reproduzir Netflix diretamente em um homebrew de Nintendo Switch usando uma conta/assinatura válida, sem PC relay e sem contornar DRM?

## Estado

**Não confirmado.**

A existência de H.264 hardware decode no Switch não resolve por si só a cadeia de playback. É necessário confirmar autenticação, aquisição de manifest/licença, CDM/DRM suportado, política de dispositivo e formato de mídia de forma autorizada.

## Regras da investigação

- Não extrair CDM de navegador/Android/console.
- Não usar chaves de conteúdo obtidas por terceiros.
- Não modificar challenge/licença para burlar política.
- Não reutilizar cookies/tokens capturados de forma insegura.
- Não publicar segredos.
- Separar fatos confirmados, hipóteses e testes.

## Saída esperada

Um relatório com:
1. capacidades reais do Switch/homebrew;
2. requisitos atuais do serviço;
3. incompatibilidades;
4. APIs/documentação oficial disponível;
5. conclusão técnica reproduzível.

Até essa conclusão, o repositório mantém apenas shell/UI e testes com mídia própria/não protegida.
