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

## Considerações Finais

Este projeto representou não apenas a conclusão de um desafio acadêmico, mas também um período valioso de aprendizado para toda a equipe. Abaixo estão os principais aprendizados obtidos durante o desenvolvimento:

- **CMake e Compilação em C++:** Adquirimos conhecimento prático no uso do CMake, uma ferramenta essencial para a compilação e construção de projetos em C++. Compreendemos os intricados detalhes do processo de compilação de código nativo.
- **Domínio em C++:** Para alguns membros da equipe, este projeto marcou o primeiro contato com a linguagem C++. Aprendemos os conceitos fundamentais da linguagem, desde a sintaxe até estruturas mais avançadas.
- **Modularização em C++:** Experimentamos a complexidade e importância da modularização de projetos em C++, entendendo como organizar e dividir o código de maneira eficiente para facilitar a manutenção e expansão.
- **Estruturas de Dados em Memória Secundária:** Exploramos o uso de estruturas de dados em memória secundária, como tries e listas ligadas, compreendendo suas aplicações e impactos no desempenho do sistema.
- **Métodos de Persistência de Dados:** Ganhamos experiência prática na implementação de métodos de persistência de dados, essenciais para garantir a integridade e durabilidade das informações armazenadas.
- **Uso Avançado do Visual Studio Code com C++:** Aprendemos a otimizar o uso das ferramentas fornecidas pelo Visual Studio Code para o desenvolvimento em C++, aumentando a eficiência e facilitando a depuração e análise de código.
- **Compilação de C++ em Addons para Node.js:** Desenvolvemos habilidades na compilação de código C++ como addons nativos para Node.js, proporcionando integração eficiente entre o backend em C++ e a aplicação em Electron.js.
- **Electron.js:** Adquirimos conhecimento prático na utilização do Electron.js para o desenvolvimento de interfaces gráficas, compreendendo como integrar efetivamente o frontend com o backend em C++.

### Dificuldades

A principal dificuldade enfrentada foi o aprendizado e implementação em C++. Para alguns membros do grupo, esta foi a primeira experiência com a linguagem, e o processo de modularização do projeto revelou-se desafiador. O tempo foi dedicado não apenas ao desenvolvimento do projeto, mas também à assimilação das ferramentas de compilação necessárias.

### Limitações

A limitação mais significativa foi o fator temporal. O projeto demandou uma gestão cuidadosa do tempo, resultando na necessidade de cortar diversas features inicialmente planejadas para uma versão mais robusta da aplicação.

### Faltou Fazer

Infelizmente, não conseguimos implementar a importação de mais dados para a aplicação. Além disso, a inclusão da entidade "Company" não foi possível dentro do prazo estabelecido. Esses são pontos que identificamos como melhorias desejadas para uma versão futura do projeto.