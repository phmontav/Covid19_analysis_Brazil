#include <bits/stdc++.h>
using namespace std;

struct Dado
{
    int obitos, casos;
    string data;
    Dado(int obitos, int casos, string data):obitos(obitos), casos(casos), data(data)
    {

    }
};

class Local
{
    string nome;
    int id;
    vector<Dado> calendar;
public:
    Local()
    {
        nome = "";
        id = 0;
    }
    Local(string nome, int id):nome(nome), id(id)
    {

    }
    string getNome()
    {
        return nome;
    }
    int getID()
    {
        return id;
    }
    double getObitos(int dia, int intervalo)
    {
        if(intervalo-1 >dia || intervalo<=0)
        {
            cout << "Intervalo Invalido\n";
            return -1;
        }
        if(intervalo == 1)
            return calendar[dia].obitos;
        int aux = 0;
        for(int i=max(dia-intervalo+1, 0); i<=dia; i++)
        {
            aux+=getObitos(i, 1);
        }
        return aux;
    }
    int getCasos(int dia, int intervalo)
    {
        if(intervalo-1 >dia || intervalo<=0)
        {
            cout << "Intervalo Invalido\n";
            return -1;
        }
        if(intervalo == 1)
            return calendar[dia].casos;
        int aux = 0;
        for(int i=max(dia-intervalo+1, 0); i<=dia; i++)
        {
            aux+=getCasos(i, 1);
        }
        return aux;
    }
    double mediaObitos(int dia, int intervalo)
    {
        return (double)getObitos(dia, intervalo)/intervalo;
    }
    double mediaCasos(int dia, int intervalo)
    {
        return (double)getCasos(dia, intervalo)/intervalo;
    }
    double growthObitos(int dia, int intervalo_media, int intervalo_crescimento)
    {
        if(intervalo_crescimento-1 >dia || intervalo_crescimento<=0)
        {
            cout << "Intervalo de Crescimento Invalido\n";
            return -1;
        }
        return 100*((mediaObitos(dia, intervalo_media)-mediaObitos(dia-intervalo_crescimento, intervalo_media))/mediaObitos(dia-intervalo_crescimento, intervalo_media));
    }
    double growthCasos(int dia, int intervalo_media, int intervalo_crescimento)
    {
        if(intervalo_crescimento-1 >dia || intervalo_crescimento<=0)
        {
            cout << "Intervalo de Crescimento Invalido\n";
            return -1;
        }
        return 100*((mediaCasos(dia, intervalo_media)-mediaCasos(dia-intervalo_crescimento, intervalo_media))/mediaCasos(dia-intervalo_crescimento, intervalo_media));
    }
    void inserirDado(Dado d)
    {
        calendar.push_back(d);
    }
    void atualizar(int mortes, int casos, int dia)
    {
        calendar[dia].casos+=casos;
        calendar[dia].obitos+=mortes;
    }
    int getCalendarSize()
    {
        return calendar.size();
    }
};

Local pais;
vector<Local> regioes;
vector<Local> estados;
vector<Local> cidades;

int queDiaFoiEsse(string data)
{
    int dia = 10*(data[8]-'0') + data[9]-'0';
    int mes = 10*(data[5]-'0') + data[6]-'0';
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int aux = dia;
    for(int i=0; i<mes-1; i++)
    {
        aux+=dias_mes[i];
    }
    return aux-1;
}

void salvarDados(string reg, string est, string cid, int id_UF, int id_cidade, string data, int casos, int mortes)
{
    //Se o vector do local for vazio ou o ultimo local for diferente do atual, queremos adicionar um local novo.
    if(cidades.empty() || cidades.back().getID() != id_cidade)
        cidades.push_back(Local (cid, id_cidade));
    if(estados.empty() || estados.back().getID() != id_UF)
        estados.push_back(Local (est, id_UF));
    if(regioes.empty() || regioes.back().getNome() != reg)
        regioes.push_back(Local (reg, 0));
    //Adiciona os dados, verificando se a data ja existe para as regioes, estados e pais.
    cidades.back().inserirDado(Dado (mortes, casos, data));
    if(estados.back().getCalendarSize()>=cidades.back().getCalendarSize())
        estados.back().atualizar(mortes, casos, cidades.back().getCalendarSize()-1);
    else
        estados.back().inserirDado(Dado (mortes, casos, data));
    if(regioes.back().getCalendarSize()>=cidades.back().getCalendarSize())
        regioes.back().atualizar(mortes, casos, cidades.back().getCalendarSize()-1);
    else
        regioes.back().inserirDado(Dado (mortes, casos, data));
    if(pais.getCalendarSize()>=cidades.back().getCalendarSize())
        pais.atualizar(mortes, casos, cidades.back().getCalendarSize()-1);
    else
        pais.inserirDado(Dado (mortes, casos, data));
}

void readFile()
{
    string linha;
    ifstream arq("HIST_PAINEL_COVIDBR_Parte3_22jun2021.csv");
    int i = 0;
    while (getline(arq, linha))
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        string regiao;
        string estado;
        string cidade;
        string data;
        string casos;
        string mortes;
        string cod_UF;
        string cod_cidade;
        int cont_pontvirg = 0;
        for (auto c : linha)
        {
            if (c == ';')
            {
                cont_pontvirg++;
                continue;
            }
            if (cont_pontvirg == 0)
                regiao.push_back(c);
            if (cont_pontvirg == 1)
                estado.push_back(c);
            if (cont_pontvirg == 2)
                cidade.push_back(c);
            if (cont_pontvirg == 'D' - 'A')
                cod_UF.push_back(c);
            if (cont_pontvirg == 'E' - 'A')
                cod_cidade.push_back(c);
            if (cont_pontvirg == 'H' - 'A')
                data.push_back(c);
            if (cont_pontvirg == 'L' - 'A')
                casos.push_back(c);
            if (cont_pontvirg == 'N' - 'A')
                mortes.push_back(c);
            if (cont_pontvirg == 'O' - 'A')
                break;
        }
        if(cidade.empty())
            continue;
        int id_UF = stoi(cod_UF);
        int id_cidade = stoi(cod_cidade);
        int numero_casos = stoi(casos);
        int numero_mortes = stoi(mortes);
        salvarDados(regiao, estado, cidade, id_UF, id_cidade, data, numero_casos, numero_mortes);
    }
}

int main()
{
    readFile();
    cout << estados[0].getObitos(queDiaFoiEsse("2021-01-01"), 1) << endl;
    cout << estados[0].getCasos(queDiaFoiEsse("2021-01-02"), 2) << endl;
    cout << estados[0].growthObitos(queDiaFoiEsse("2021-01-10"), 3, 4) << endl;
    cout << estados[0].growthCasos(queDiaFoiEsse("2021-01-10"), 3, 4) << endl;

}
