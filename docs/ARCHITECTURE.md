# Arquitetura NetflixNX

## Princípios

1. **Switch-first:** execução nativa em Atmosphère/title mode.
2. **Sem PC obrigatório:** o console deve ser o cliente final.
3. **Input unificado:** foco lógico único para Joy-Con, Pro Controller e touch.
4. **Playback isolado:** UI nunca depende diretamente de uma implementação de DRM.
5. **Sem bypass:** qualquer integração de playback precisa respeitar autenticação, autorização e DRM exigidos pelo serviço.

## Camadas

### UI
Shell 10-foot 1280×720. Home, Search, My Netflix e Settings. Cards são placeholders até existir uma fonte de catálogo autorizada.

### Input
`libnx` lê Npad e touchscreen. Todos os inputs alteram o mesmo `AppState`; isso evita criar uma UI separada para handheld.

### Platform
SDL2 fornece janela/renderização 2D no MVP. A evolução pode mover vídeo para deko3d/NVDEC se um fluxo de mídia autorizado e compatível for confirmado.

### Playback
Contrato futuro:

```cpp
struct PlaybackBackend {
  virtual ProbeResult probeCapabilities() = 0;
  virtual StartResult start(const AuthorizedMediaDescriptor&) = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual void stop() = 0;
};
```

Nenhuma implementação deve aceitar chave DRM extraída, manifest adulterado ou mídia sem autorização.

## O que estudar no Green-NX

O Green-NX demonstra que um homebrew Switch pode combinar rede, H.264 via NVDEC, deko3d, áudio Opus, UI SDL2 e controle nativo sem PC relay. Para NetflixNX, essas peças são referências de plataforma; autenticação Xbox, catálogo xCloud, signaling xCloud, ICE/WebRTC e data channels Xbox não são componentes Netflix e não devem ser transplantados sem necessidade.

Se código do Green-NX for incorporado, preservar GPL-3.0, avisos e autoria.
