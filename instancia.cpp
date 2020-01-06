#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void GerarInstancia(int seed, int NrAcademicos, int W, int K, const char fileName[])
{
    vector<int> AnoAcademico;
    vector<int> Citacoes;
    vector<int> Indice;

    AnoAcademico = vector<int>(NrAcademicos);
    Citacoes = vector<int>(NrAcademicos);
    Indice = vector<int>(NrAcademicos);

    FILE *fp;
    fp = fopen(fileName, "w");

    fprintf(fp, "%d %d %d\n", NrAcademicos, W, K);

    for (int i = 0; i < NrAcademicos; i++)
    {
        Citacoes[i] = (50000 + rand() % 100000);
        Indice[i] = (10 + rand() % 130);
        AnoAcademico[i] = (1980 + rand() % 39);
        fprintf(fp, "%d %d %d\n", Citacoes[i], Indice[i], AnoAcademico[i]);
    }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    int seed, NrAcademicos, W, K;

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL)
    {
        cout << "API:\nArg1: int Seed\nArg2: int NrAcademicos\nArg3: int K\nArg4: string Filename" << endl;
        exit(1);
    }

    seed = atoi(argv[1]);
    NrAcademicos = atoi(argv[2]);
    K = atoi(argv[3]);

    W = NrAcademicos * K;
    GerarInstancia(seed, NrAcademicos, W, K, argv[4]);
    return 0;
}