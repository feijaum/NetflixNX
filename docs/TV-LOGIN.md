# TV Login / Device Pairing

## UX alvo

NetflixNX deve apresentar uma experiência equivalente à de um aparelho de TV:

1. abrir o app;
2. escolher **Entrar**;
3. exibir QR code e instruções;
4. usuário lê o QR no celular;
5. autenticação acontece no fluxo oficial da Netflix;
6. o aparelho recebe confirmação da sessão;
7. seleção de perfil.

A Central de Ajuda da Netflix documenta atualmente login em TV por QR code e também o fluxo por navegador em `netflix.com/tv2`.

## Estado da implementação

A tela visual de login está implementada, mas o QR exibido no protótipo é apenas um placeholder.

**Não gerar QR apontando simplesmente para uma página estática e não simular sucesso de login.** O QR real e o código de pareamento precisam vir de um fluxo de device/TV pairing autorizado pela Netflix, porque a associação da sessão precisa ser reconhecida pelo serviço.

## Próxima investigação

- identificar se o fluxo TV/device pairing é acessível a este tipo de cliente;
- obter documentação/API autorizada para iniciar a sessão de pareamento;
- receber código/URL/QR temporário;
- polling/callback de confirmação;
- armazenamento seguro do estado autenticado;
- expiração/logout;
- tratamento de Netflix Household quando aplicável.

Nenhuma credencial, cookie ou token deve ser capturado de outro aparelho.
