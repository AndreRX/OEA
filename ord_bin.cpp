#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>

#pragma pack(1)
using namespace std;
typedef struct _Endereco Endereco;

struct _Endereco
{
  char logradouro[72];
  char bairro[72];
  char cidade[72];
  char uf[72];
  char sigla[2];
  char cep[8];
  char lixo[2];
};

int compara(const void *e1, const void *e2)
{
  return strncmp(((Endereco *)e1)->cep, ((Endereco *)e2)->cep, 8);
}

void ordena(Endereco *e, long tamanho, FILE *arq, FILE *f)
{
  fread(e, sizeof(Endereco), tamanho, f);
  qsort(e, tamanho, sizeof(Endereco), compara);
  fwrite(e, sizeof(Endereco), tamanho, arq);
  fclose(arq);
}

int intercala(int index_1, int index_2, int index_3)
{
  FILE *a, *b, *saida;
  Endereco ea, eb;
  char arquivo_1[100];
  char arquivo_2[100];
  char arquivo_novo[100];

  sprintf(arquivo_1, "cep_%i.dat", index_1);
  sprintf(arquivo_2, "cep_%i.dat", index_2);
  sprintf(arquivo_novo, "cep_%i.dat", index_3);

  a = fopen(arquivo_1, "r");
  b = fopen(arquivo_2, "r");
  saida = fopen(arquivo_novo, "w");

  fread(&ea, sizeof(Endereco), 1, a);
  fread(&eb, sizeof(Endereco), 1, b);

  while (!feof(a) && !feof(b))
  {
    if (compara(&ea, &eb) < 0)
    {
      fwrite(&ea, sizeof(Endereco), 1, saida);
      fread(&ea, sizeof(Endereco), 1, a);
    }
    else
    {
      fwrite(&eb, sizeof(Endereco), 1, saida);
      fread(&eb, sizeof(Endereco), 1, b);
    }
  }

  while (!feof(a))
  {
    fwrite(&ea, sizeof(Endereco), 1, saida);
    fread(&ea, sizeof(Endereco), 1, a);
  }

  while (!feof(b))
  {
    fwrite(&eb, sizeof(Endereco), 1, saida);
    fread(&eb, sizeof(Endereco), 1, b);
  }
  fclose(a);
  fclose(b);
  fclose(saida);
}

int main(int argc, char **argv)
{
  FILE *f, *saida;
  Endereco *e;
  int div = 8, aux_1 = div, aux_2 = 0, count = 0;
  long posicao, qtd, particao;

  f = fopen("cep.dat", "r");
  fseek(f, 0, SEEK_END);

  posicao = ftell(f);
  qtd = posicao / sizeof(Endereco);
  particao = qtd / div;

  char fileName[100];

  e = (Endereco *)malloc(particao * sizeof(Endereco));
  rewind(f);

  for (long i = 0; i < qtd; i += particao + 1)
  {
    sprintf(fileName, "cep_%i.dat", count);
    FILE *arq = fopen(fileName, "w");

    ordena(e, particao, arq, f);
    count++;
  }
  int k = 0;
  int flag = 0;
  while (flag == 0)
  {
    intercala(k, k + 1, aux_1);
    aux_1++;
    aux_2++;
    if (k + 2 == div && aux_2 != 1)
    {
      div += aux_2;
      aux_2 = 0;
    }
    else if (k + 2 == div && aux_2 == 1)
    {
      flag = 1;
      break;
    }
    k += 2;
  }

  fclose(f);
  int ticks = clock();

  cout << (float)ticks / CLOCKS_PER_SEC;

  system("pause");
  return 0;
}