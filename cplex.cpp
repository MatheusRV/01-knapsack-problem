#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <sstream>  


typedef IloArray<IloNumVarArray> IloNumVarMatrix;

using namespace std;

int main(int argc, char *argv[])
{
    int verbose;
    verbose = atoi(argv[1]);

    time_t t = time(NULL);
    struct tm *timePtr = localtime(&t);
    // Declarando váriveis livres do problema
    // W -> Certo valor inteiro da idade máxima de todos os acadêmicos
    // K -> Número máximo de acadêmicos selecionados
    int NrAcademicos, W, K;

    // Dados de entrada dependentes dos conjuntos
    vector<float> Potencial; // chamado de p no problema
    vector<int> AnoAcademico;
    vector<int> IdadeAcademica; // chamado de a no problema
    vector<int> Citacoes;       // chamado de c no problema
    vector<int> Indice;         // chamado de h no problema

    // Leitura do Arquivo
    FILE *fp;
    fp = fopen(argv[2], "r");

    if (fp == NULL)
    {
        if (verbose == 1)
        {
            printf("Erro ao abrir o arquivo!\n");
        }
        exit(1);
    }

    // Leitura dos conjuntos
    fscanf(fp, "%d %d %d\n", &NrAcademicos, &W, &K);
    // Definindo os dados de entrada de acordo com o tamanho dos conjuntos lidos
    // Por Academico
    Citacoes = vector<int>(NrAcademicos);
    Indice = vector<int>(NrAcademicos);
    AnoAcademico = vector<int>(NrAcademicos);
    IdadeAcademica = vector<int>(NrAcademicos);
    Potencial = vector<float>(NrAcademicos);

    // Após a declaração dos tamanhos dos dados de entrada, realizando a leitura
    // Por Academico
    for (int i = 0; i < NrAcademicos; i++)
    {
        fscanf(fp, "%d %d %d\n", &Citacoes[i], &Indice[i], &AnoAcademico[i]);
        IdadeAcademica[i] = (((timePtr->tm_year) + 1900) - AnoAcademico[i]);
        Potencial[i] = ((Citacoes[i] + Indice[i]) / IdadeAcademica[i]);
    }
    //AnoAcademico delete;

    // Impressão para Verificação dos dados
    if (verbose == 1)
    {

        printf("Numero de Academico: %d\n", NrAcademicos);
        printf("W: %d\n", W);
        printf("K: %d\n", K);

        printf("Acadêmicos:\n");
        printf("i \t Citacoes \t Indice \t Idade Academica \t Potencial\n");
        for (int i = 0; i < NrAcademicos; i++)
        {
            printf("%d\t\t%d\t\t%d\t\t%d\t\t%f\n", (i + 2), Citacoes[i], Indice[i], IdadeAcademica[i], Potencial[i]);
        }
    }
    IloEnv env;
    IloModel model(env);
    IloCplex cplex(model);

    // Restrução 1
    IloNumVarArray x(env, NrAcademicos, 0, 1, ILOBOOL);

    // FO
    IloExpr FuncaoObjetivo(env);
    for (int i = 0; i < NrAcademicos; i++)
    {
        FuncaoObjetivo += Potencial[i] * x[i];
    }
    model.add(IloMaximize(env, FuncaoObjetivo));

    // Restrição 2
    IloExpr SomatorioIdades(env);
    for (int i = 0; i < NrAcademicos; i++)
    {
        SomatorioIdades += IdadeAcademica[i] * x[i];
    }
    stringstream var_SomatorioIdades;
    var_SomatorioIdades << "SomatorioIdades:";
    IloRange rest_SomatorioIdades(env, 0, SomatorioIdades, W);
    rest_SomatorioIdades.setName(var_SomatorioIdades.str().c_str());
    model.add(rest_SomatorioIdades);

    // Restrição 3
    IloExpr SomatorioPesos(env);
    for (int i = 0; i < NrAcademicos; i++)
    {
        SomatorioPesos += x[i];
    }
    stringstream var_SomatorioPesos;
    var_SomatorioPesos << "SomatorioPesos:";
    IloRange rest_SomatorioPesos(env, 0, SomatorioPesos, K);
    rest_SomatorioPesos.setName(var_SomatorioPesos.str().c_str());
    model.add(rest_SomatorioPesos);

    // Extração do Modelo
    cplex.extract(model);
    //cplex.exportModel("instancia.lp");
    cplex.solve();

    // Exibindo Solução
    cout << "FO:  " << cplex.getObjValue() << endl;
    if (verbose == 1)
    {
        if (cplex.getObjValue() >= 1)
        {
            cout << "Acadêmico" << "\t" << "Idade Acadêmica" << "\t" << "Potencial" << endl;
            for (int i = 0; i < NrAcademicos; i++)
            {
                if (cplex.getValue(x[i]) == 1)
                {
                    cout << "Academico[" << (i + 1) << "]\t" << IdadeAcademica[i] << "\t\t" << Potencial[i] << endl;
                }
            }
        }
    }
    env.end();

    return 0;
}