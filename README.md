# Aplicação de Banco de Dados para Registro e Manipulação do Histórico de Ações na Bolsa de Valores

**Grupo de Desenvolvimento:**

- Artur Webber: [https://github.com/artieeez](https://github.com/artieeez)
- Isaac do Canto: [https://github.com/isaacbueno27](https://github.com/isaacbueno27)
- Luís França: [https://github.com/Yeahitori](https://github.com/Yeahitori)

**Introdução:**

Este trabalho foi realizado como parte da disciplina de Classificação e Pesquisa de Dados na Universidade Federal do Rio Grande do Sul. O objetivo principal é desenvolver uma aplicação que funcione como um banco de dados para armazenar e manipular o histórico de ações na bolsa de valores. O projeto é dividido em duas partes distintas: o backend, implementado em C++, e a aplicação de interface gráfica, desenvolvida em Electron.js, React e Material UI.

**Arquitetura do Projeto:**

1. **Backend em C++:**
    - **Database:** Camada de baixo nível responsável por operações básicas de CRUD em arquivos. Livra as camadas superiores de preocupações relacionadas à implementação de I/O na memória secundária. Salva o "Record<T>".
    - **Índice:** Possui estruturas de indexação e utiliza a classe Database::Context para escrever arquivos. Tem como objetivo receber uma chave indexada e retornar o endereço onde o Record está no arquivo principal.
    - **Controller:** Camada mais externa onde reside toda a lógica de negócios. Comunica-se tanto com os índices quanto com a database. Aqui estão as funções chamadas pela main, bem como as funções expostas na API para Node.js.
    - **NAPI:** Módulo responsável pela serialização das controllers para addons do Node.js.
    - **Main:** Ponto de partida da aplicação, demonstra as controllers para aqueles que desejam executar na linha de comando sem a necessidade do frontend em Electron.
2. **Aplicação em Electron.js:**
    - Desenvolvida com Node.js, Electron.js, React e Material UI.
    - Importa o backend em C++ e consome a API para permitir a visualização e a execução de operações CRUD na interface gráfica.

**Dependências:**

- **Backend em C++:**
    - Não possui dependências externas.
- **Aplicação em Electron.js:**
    - Node.js
    - Electron.js
    - React
    - Material UI
    - node-gyp para a construção do addon nativo.

**Estrutura do Repositório:**

- **Backend em C++:**
    - [https://github.com/artieeez/dbms-cpp-backend](https://github.com/artieeez/dbms-cpp-backend)
- **Aplicação em Electron.js:**
    - [https://github.com/artieeez/dbms-electron-ui](https://github.com/artieeez/dbms-electron-ui)

## Instruções

### Requisitos

- CMake: [https://cmake.org/download/](https://cmake.org/download/)
- MSVC: [https://visualstudio.microsoft.com/downloads/?q=build+tools#build-tools-for-visual-studio-2022](https://visualstudio.microsoft.com/downloads/?q=build+tools#build-tools-for-visual-studio-2022)

### Compilação

- Crie uma pasta `build` na raiz do projeto:

```bash
mkdir build
cd build
```

- Rode o **`CMake`** dentro da pasta build:

```bash
cmake ..
```

- Faça o build preferencialmente utilizando MSVC:

```bash
cmake --build .
```

O código compilado faz uso da nossa API e serve de demonstração das implementações.