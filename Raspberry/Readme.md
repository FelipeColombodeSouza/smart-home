# Configurando e instalando  o Raspberry PI

Primeiramente para instalar o sistema operacional do Raspberry, você precisará de um cartão SD, onde ficará armazenado a imagem do SO. Acessando o https://www.raspberrypi.com/software/ você pode baixar o programa e configurá-lo.

### Instalando o Mosquitto Broker no Raspberry Pi OS

Ative a conexão por SSH, abra o terminal e execute os seguintes comandos:

```bash
sudo apt update && sudo apt upgrade
```

Pressione Y e Enter . Levará algum tempo para atualizar.

Para instalar o Mosquitto Broker digite os próximos comandos:

```bash
sudo apt install -y mosquitto mosquitto-clients
```

Para fazer o Mosquitto iniciar automaticamente quando o Raspberry Pi inicializar, você precisa executar o seguinte comando:

```bash
sudo systemctl enable mosquitto.service
```

Agora teste a instalação executando o seguinte comando:

```bash
mosquitto -v
```

### Permissão do acesso remoto (sem autenticação)

Execute o seguinte comando para abrir o arquivo `mosquito.conf`.

```bash
sudo nano /etc/mosquitto/mosquitto.conf
```

Vá até o final do arquivo usando as teclas de seta e cole as duas linhas a seguir:

```bash
listener 1883
allow_anonymous true
```
Em seguida, pressione  CTRL-X  para sair e salvar o arquivo. Pressione  Y  e  Enter .
 
Reinicie o Mosquitto para que as alterações tenham efeito.

```bash
sudo systemctl restart mosquitto
```


### Instalando o Node-RED no Raspberry Pi

Na sua conexão SSH com o Raspberry, execute o seguinte comando:

```bash
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

#### Definindo as configurações iniciais

```bash
node-red admin init
```

- Pressione Enter para criar um arquivo de configurações do Node-RED em `/home/pi/.node-red/settings.js`
- Você deseja configurar a segurança do usuário? Sim .
- Digite um nome de usuário e pressione Enter.
- Digite uma senha e pressione Enter.
- Deseja ativar o recurso Projetos? Não.
- Insira um nome para seu arquivo de fluxos. Pressione Enter para selecionar o nome padrão flows.json.
- Forneça uma senha para criptografar seu arquivo de credenciais. 
- Selecione um tema para o editor. Basta pressionar Enter para selecionar o padrão.
- Pressione Enter novamente para selecionar o editor de texto padrão.
- Permitir que nós de função carreguem módulos externos? Sim .

#### Iniciando o Node-RED

```bash
node-red-start
```

Colocar o Node-RED para iniciar automaticamente:

```bash
sudo systemctl enable nodered.service
```

Reinicie para salvar as alterações:

```bash
sudo reboot
```

Agora o Node-RED está servindo na porta 1880. Logo, para você acessar, insira o IP do Raspberry juntamente com a porta do Node-RED. Exemplo:

```bash
192.168.1.0:1880
```

### Instalando o Node-RED Dashboard

Para instalar o Node-RED Dashboard, você precisará acessar no menu direito a opção `Manage Palette - Palette - Install` e pesquisar por `node-red-dashboard` e instalar.

A interface será carregada com a seguinte URL:

```bash
192.168.1.0:1880/ui
```

## Importando Flow do projeto

Para importar o arquivo do projeto, acesse o menu superior do projeto e vá em `Import - Clipboard`.

Abra o arquivo flows.json, copie todo o código e cole na aba de importação. Logo após, clique em `Deploy`.

Ao final o fluxo deverá estar conforme a imagem abaixo:

![alt Imagem fluxo node-red](https://github.com/Josiane-R/smart-home/blob/main/Raspberry/fluxo-node-red.jpg)

E pronto, o projeto estará rodando :tw-1f604: