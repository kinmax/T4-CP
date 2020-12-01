# Fases Paralelas

Trabalho Prático 4 da disciplina de Computação Paralela (4645J-04) do 7º semestre do curso de Bacharelado em Ciência da Computação (4/606) da Escola Politécnica da Pontifícia Universidade Católica do Rio Grande do Sul (PUCRS).

Professor: Cesar Augusto F De Rose

Turma: 128

Semestre: 2020/2

Aluno: Kin Max Piamolini Gusmão - 16104046-4 - kin.gusmao@edu.pucrs.br - Grupo 6

## Dependências

Informações de compilação e execução referentes a sistemas operacionais GNU/Linux.

É necessário instalar o MPI para compilar e executar. Tente utilizar o comando mpirun no terminal para verificar se este está instalado. Caso não esteja, o sistema deve informar o pacote necessário para utilizar os comandos.

Além disso, é necessário que o `gcc` esteja instalado.

## Compilação

Para compilar a versão sequencial: `make seq`

Para compilar a versão MPI com fases paralelas: `make mpi`

Para compilar a versão MPI com fases paralelas no LAD: `make lad`

Para excluir os executáveis: `make clean`

Para limpar executáveis e compilar sequencial e MPI fora do LAD: `make build`

Para limpar executáveis e compilar sequencial e MPI no LAD: `make build_lad`

## Execução

Primeiro certifique-se de que a versão que se deseja executar está corretamente compilada e que seu executável está na pasta.

Para executar a versão sequencial: `./bubble`

Para executar a versão MPI com fases paralelas:

`mpirun -np <número de processos> ./bubble_fases`

Para executar a versão MPI com fases paralelas no LAD:

`ladrun -np <número de processos> ./bubble_fases`

Ao alterar o número de processos, é necessário alterar no código o valor da macro `TROCA_SIZE`. Este precisa ser menor do que `1/np`, onde np é o número de processos. Recomenda-se o uso de potências de 2 para o número de processos, até 32. Para além de 32, recomenda-se aumentar o tamanho do vetor total.