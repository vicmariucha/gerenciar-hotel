#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct composto {
  char sigla;
  int num;
} composto;

typedef union estado {
  char sigla;
  composto campo;
} estado;

typedef struct hospede {
  int numreg;
  char nome[80];
  int acompanhante;
  char categoria;
  int dias;
  estado tabela;
} hospede;

typedef struct quarto {
  int num;
  char categoria;
  estado status;
} quarto;

void aloca_quarto(quarto **pq, int qq);
void aloca_hospede(hospede **ph, int qh);
int verifica_arq_quarto();
int verifica_arq_hospede();
void cadastro_quarto(quarto *pq, int qq);
void cadastro_hospede(hospede *ph, quarto *pq);
void grava_hospede(hospede *ph);
int busca_quarto(quarto *pq, char letra, int qq, int numreg);
void busca_hospede(hospede *ph, int numreg);
void encerra_quarto(hospede *ph, quarto *pq, int qq, int n_quarto);
void mostra_hospede(hospede *ph, int n_quarto);
void mostra_quarto(quarto *pq, int qq);

int main() {
  quarto *q = NULL;
  hospede *hosp = NULL;
  int op, n_quarto, qq;

  aloca_quarto(&q, 1);
  qq = verifica_arq_quarto();
  if (qq == 0) // se o arquivo de quartos nao existir - deverá ser feito o
    cadastro_quarto(q, 15);
  aloca_hospede(&hosp, 1);

  do {
    system("cls");
    printf("[1] Check-in \n[2] Check-out \n[3] Mostra o hospede \n[4] Mostra quarto \n[5] Finalizar \n");
    scanf("%i", &op);
    fflush(stdin);

    switch (op) {
    case 1:
      cadastro_hospede(hosp, q); // passa o ponteiro de hospede e de quarto, pq
                                 // teremos que buscar quarto vago
      system("pause");
      break;
    case 2:
      printf("\nQuarto a ser encerrado: ");
      scanf("%i", &n_quarto);
      fflush(stdin);
      mostra_hospede(hosp, n_quarto);
      encerra_quarto(hosp, q, 15, n_quarto);
      break;
    case 3:
      mostra_hospede(hosp, 0); // o para indicar TODOS
      system("pause");
      break;
    case 4:
      mostra_quarto(q, 15);
      system("pause");
      break;
    } // switch
  } while (op != 5);
} // main

void aloca_quarto(quarto **pq, int qq) {
  if (((*pq = (quarto *)realloc(*pq, qq * sizeof(quarto))) == NULL))
    exit(1);
} // aloca quarto

void aloca_hospede(hospede **ph, int qh) {
  if (((*ph = (hospede *)realloc(*ph, qh * sizeof(hospede))) == NULL))
    exit(1);
} // aloca hospede

int verifica_arq_quarto() {
  long int cont = 0;
  FILE *fquarto = NULL;
  if ((fquarto = fopen("quartos.bin", "rb")) == NULL)
    return cont;
  else {
    fseek(fquarto, 0, 2); // fseek -> posiciona no fim do arquivo
    cont = ftell(fquarto) / sizeof(quarto); // ftell -> quantidade de elementos
    fclose(fquarto);                        // dentro do else por conta do "rb"
    return cont;
  } // else
} // verifica arq quarto

int verifica_arq_hospede() {
  long int cont = 0;
  FILE *fhosp = NULL;
  if ((fhosp = fopen("hospedes.bin", "rb")) == NULL)
    return cont;
  else {
    fseek(fhosp, 0, 2);
    cont = ftell(fhosp) / sizeof(hospede);
    fclose(fhosp);
    return cont;
  } // else
} // verifica arq hospede

void cadastro_quarto(quarto *pq, int qq) {
  FILE *fquarto = NULL;
  int i;

  if ((fquarto = fopen("quartos.bin", "ab")) == NULL)
    printf("\nERRO NO ARQUIVO");
  else
    for (i = 0; i < qq; i++) {
      if (i < 5)
        pq->categoria = 'S';
      else
        pq->categoria = 'F';
      pq->num = i + 1; // pra nao começar o primeiro quarto como zero
      pq->status.sigla = 'L';
      fseek(fquarto, i * sizeof(quarto), 0);
      fwrite(pq, sizeof(quarto), 1, fquarto);
    }              // for
  fclose(fquarto); // fora do else por conta do "ab"
} // cadastra quarto

void cadastro_hospede(hospede *ph, quarto *pq) {
  int qh, aux_quarto;

  qh = verifica_arq_hospede(); // descobre quantos hospedes tem no arquivo
  system("cls");
  ph->numreg = ++qh; // 1 a mais que a quantidade de hospedes
  printf("\nRegistro: %i\n", ph->numreg);
  printf("\nNome: ");
  gets(ph->nome);
  fflush(stdin);

  do {
    printf("\nQuantidade de acompanhantes: - max-3: ");
    scanf("%i", &(ph->acompanhante));
    fflush(stdin);
  } while (ph->acompanhante < 0 || ph->acompanhante > 3);
  if (ph->acompanhante == 0) {
    aux_quarto = busca_quarto(
        pq, 'S', 15, ph->numreg); // retorna o numero do quarto disponivel para
    ph->categoria = 'S';
  } // if
  else {
    aux_quarto = busca_quarto(
        pq, 'F', 15, ph->numreg); // retorna o numero do quarto disponivel para
    ph->categoria = 'F';
  } // else
  if (aux_quarto == -1)
    printf("\nHotel Lotado\n\n");
  else {
    ph->tabela.campo.sigla = 'O';
    ph->tabela.campo.num = aux_quarto; // recebe o numero do quarto
    printf("\nQuarto: %i \n", ph->tabela.campo.num);
    printf("\nCategoria: %c \n", ph->categoria);
    printf("\nQuantos dias: ");
    scanf("%i", &(ph->dias));
    fflush(stdin);
    grava_hospede(ph);
  }
}

void grava_hospede(hospede *ph) {
  FILE *fhosp = NULL;

  if ((fhosp = fopen("hospedes.bin", "ab")) == NULL)
    printf("\nERRO AO ABRIR O ARQUIVO");
  else
    fwrite(ph, sizeof(hospede), 1, fhosp);
  fclose(fhosp); // fora do else por conta do ab
} // grava hospede

int busca_quarto(quarto *pq, char letra, int qq, int numreg) {
  FILE *fquarto = NULL;
  int i;
  if ((fquarto = fopen("quartos.bin", "rb+")) == NULL)
    printf("\nImpossivel abrir o arquivo!!!!!");
  else
    for (i = 0; i < qq; i++) {
      fseek(fquarto, i * sizeof(quarto), 0);
      fread(pq, sizeof(quarto), 1, fquarto);
      if (pq->categoria == letra &&
          pq->status.sigla == 'L') { // se encontrou algum quarto na categoria

        pq->status.campo.sigla = 'O';
        pq->status.campo.num = numreg; // registro do hospede
        fseek(fquarto, i * sizeof(quarto), 0);
        fwrite(pq, sizeof(quarto), 1, fquarto);
        fclose(fquarto);
        return (pq->num);
      }
    }              // for
  fclose(fquarto); // fora do else por conta do rb+
  return -1;       // nao encontrou quarto disponivel na categoria desejada
} // busca quarto

void busca_hospede(hospede *ph, int numreg) {
  FILE *fhosp = NULL;
  int i, qh, achou = 0;

  qh = verifica_arq_hospede(); // descobre a quantidade de hospedes no arquivo
  if ((fhosp = fopen("hospedes.bin", "rb+")) == NULL)
    printf("\nARQUIVO NAO ENCONTRADO");
  else {
    for (i = 0; i < qh; i++) {
      fseek(fhosp, i * sizeof(hospede), 0);
      fread(ph, sizeof(hospede), 1, fhosp);
      if (ph->numreg == numreg) {
        achou = 1;
        ph->tabela.sigla = 'L'; // atualiza tabela do e para LIVRE
        ph->tabela.campo.num =
            -1; // indica que nao existe quarto vinculado ao hospede
        fseek(fhosp, i * sizeof(hospede), 0);
        fwrite(ph, sizeof(hospede), 1,
               fhosp); // atualiza os dados do hospede no arquivo
        i = qh; // forçar a saída do for para ficar com as informações do
                // hospede na memoria
      }         // if
    }           // for
    if (achou == 0) {
      printf("\nNão foi encontrado hospede com esse registro\n\n");
      system("pause");
    } // if
  }   // else
  fclose(fhosp);
} // busca hospede

void encerra_quarto(hospede *ph, quarto *pq, int qq, int n_quarto) {
  FILE *fquarto = NULL;
  int i, n_hosp;
  float total;

  if ((fquarto = fopen("quartos.bin", "rb+")) == NULL) // arquivo será
                                                       // atualizado
    printf("\nARQUIVO NAO ENCONTRADO");
  else
    for (i = 0; i < qq; i++) {
      fseek(fquarto, i * sizeof(quarto), 0);
      fread(pq, sizeof(quarto), 1, fquarto);
      if (pq->num == n_quarto) {       // achou o quarto a ser encerrado
        n_hosp = pq->status.campo.num; // numero do resgistro do hospede
        busca_hospede(ph,
                      n_hosp); // traz para a memoria as informações do hospede
        if (pq->categoria == 'S')
          total = ph->dias * 85;
        else
          total = ph->dias * 45 * ph->acompanhante + 1;
        printf("\nValor a ser pago: %.2f\n", total);
        system("pause");
        pq->status.sigla = 'L'; // atualiza o status para LIVRE
        pq->status.campo.num =
            -1; // indica que nao há hospede vinculado ao quarto
        fseek(fquarto, i * sizeof(quarto), 0);
        fwrite(pq, sizeof(quarto), 1, fquarto);
        // if
      } // for
    }
  fclose(fquarto); // por conta do rb+
} // encerra quarto

void mostra_hospede(hospede *ph, int n_quarto) {
  FILE *fhosp = NULL;
  int i, qh;

  qh = verifica_arq_hospede(); // descobre a quantidade de hospedes no arquivo
  if ((fhosp = fopen("hospedes.bin", "rb")) == NULL)
    printf("\nERRO NO ARQUIVO\n");
  else {
    for (i = 0; i < qh; i++) {
      fseek(fhosp, i * sizeof(hospede), 0);
      fread(ph, sizeof(hospede), 1, fhosp);
      if (n_quarto == 0) { // mostra todos os quartos
        printf("\nRegistro: %i \nNome: %s \nAcompanhantes: %i \nCategoria: %c "
               "\nDias: %i \nTabela: %c",
               ph->numreg, ph->nome, ph->acompanhante, ph->categoria, ph->dias,
               ph->tabela.campo.sigla);
        if (ph->tabela.campo.sigla == 'O')
          printf("\n Numero do quarto: %i\n\n\n", ph->tabela.campo.num);
        else
          printf("\n\n\n"); // só pular linha para nao ficar colado uma coisa na
                            // outra
      }    // if
      else // mostra só o quarto solicitado
          if (ph->tabela.campo.num == n_quarto) {
        printf("\nRegistro: %i \nNome: %s \nAcompanhantes: %i \nCategoria: %c "
               "\nDias: %i \nTabela: %c",
               ph->numreg, ph->nome, ph->acompanhante, ph->categoria, ph->dias,
               ph->tabela.campo.sigla);
        if (ph->tabela.campo.sigla == 'O')
          printf("\n Numero do quarto: %i\n\n\n", ph->tabela.campo.num);
        else
          printf("\n\n\n");
      }            // if
    }              // for
    fclose(fhosp); // dentro do else por conta do rb
  }                // else
} // mostra hospede

void mostra_quarto(quarto *pq, int qq) {
  FILE *fquarto = NULL;
  int i;

  if ((fquarto = fopen("quartos.bin", "rb")) == NULL)
    printf("\nARQUIVO NAO EXISTE");
  else {
    for (i = 0; i < qq; i++) {
      fseek(fquarto, i * sizeof(quarto), 0);
      fread(pq, sizeof(quarto), 1, fquarto);
      printf("\nQuarto: %i \nCategoria: %c \nStatus: %c", pq->num,
             pq->categoria, pq->status.campo.sigla);
      if (pq->status.campo.sigla == 'O')
        printf("\nHospede: %i \n", pq->status.campo.num);
      else
        printf("\n\n");
    }                // for
    fclose(fquarto); // dentro do else por causa do rb
  }                  // else
} // mostra quarto