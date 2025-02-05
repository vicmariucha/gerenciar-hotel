# Gerenciar hotel 🏨

**Gerenciar hotel** é um sistema de gerenciamento de hotel desenvolvido em C. Ele oferece funcionalidades para lidar com check-ins, check-outs e controle de disponibilidade de quartos. O sistema armazena dados em arquivos binários e gerencia dinamicamente a alocação de memória para os registros de hóspedes e quartos. O sistema foi feito como parte da nota para a minha disciplina "Programação estruturada" na faculdade de Engenharia da Computação.

## 📌 Funcionalidades

- **Sistema de check-in**: Registra novos hóspedes e atribui quartos disponíveis.
- **Sistema de check-out**: Processa saídas e atualiza a disponibilidade dos quartos.
- **Gerenciamento de quartos**: Controla o status dos quartos e categoriza os tipos de acomodação.
- **Registro de hóspedes**: Armazena detalhes dos hóspedes e duração da estadia.

## 🛠️ Tecnologias utilizadas

- **Linguagem C** → Implementação principal do programa.
- **Manipulação de arquivos (Arquivos binários)** → Armazena e recupera os dados de hóspedes e quartos.
- **Alocação dinâmica de memória** → Usa `malloc()` e `realloc()` para gerenciamento de memória.

## 🗂️ Estrutura do projeto

O projeto é composto pelos seguintes componentes:

- **`main.c`** – Ponto de entrada do programa que lida com a interação do usuário.
- **`quarto.h` & `quarto.c`** – Gerencia a alocação de quartos e atualizações de status.
- **`hospede.h` & `hospede.c`** – Lida com os registros de hóspedes e atribuições de quartos.
- **`files.c`** – Gerencia operações de arquivos para armazenar e recuperar dados.

## 📝 Licença

Este projeto foi desenvolvido para fins educacionais. Sinta-se à vontade para modificar e aprimorar conforme necessário.

📩 Se tiver dúvidas ou sugestões, entre em contato!
