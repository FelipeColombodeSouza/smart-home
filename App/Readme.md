# App e Banco

Foram criandos um aplicativo em React Native e um banco MySql, que foi colocado no Google Cloud.

### Criando um banco local

O banco utilizado para o projeto foi colocado em uma conta pessoal no Google Cloud, por isso é recomendável criar um banco em caso de usos futuros.
Segue query para criação do banco, tabela e registros usados no projeto:

**Criação do banco:**
```bash
CREATE DATABASE trabalho_iot;
```
Assim, será criado o banco "trabalho_iot". Para a adição da tabela utilizada:

**Criação da tabela:**
```bash
CREATE TABLE lampadas (
  `idlampadas` INT NOT NULL AUTO_INCREMENT,
  `estado` TINYINT NOT NULL,
  `comodo` VARCHAR(45) NULL,
  PRIMARY KEY (`idlampadas`),
  UNIQUE INDEX `idlampadas_UNIQUE` (`idlampadas` ASC) VISIBLE);
```
As lâmpadas serão controladas pelo campo *"estado"*, que dirá se uma lâmpada está apagada em caso de 0 e acesa caso o campo esteja como 1. Agora, a adição dos registros das lampadas da cozinha, sacada, quarto e banheiro:

**Adição de registros:**
```bash
INSERT INTO lampadas
    (idlampadas,estado,comodo)
VALUES
    (1,0,"Cozinha"),
    (2,0,"Sacada"),
    (3,0,"Quarto"),
    (4,0,"Banheiro");
```

É possível adicionar mais registros para novos equipamentos ligados pelo banco. Necessário que seja por ID sequencial.

### App

A conexão com o banco no App foi feita em **Controller.js**. Para mudar para um banco local ou outro banco:

```bash
const con = mysql.createConnection({
    host:'ip do host',
    user:'nome do usuario',
    password:'senha do usuario',
    database:'nome do banco'
});
```
