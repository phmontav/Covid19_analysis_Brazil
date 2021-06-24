#include <bits/stdc++.h>
using namespace std;

/* class vector_casos
{
    protected:
     vector < pair < string,vector < pair < int,string > > > >   novos_casos;
    public:
    /* vector_casos(string nome, int num, string data)
    {
        novos_casos[0].first = nome;
        novos_casos[0].second.push_back(make_pair(num,data));
    } */

//};

class base //: public vector_casos
{
protected:
    
    vector<pair<string, vector<pair<int, string>>>> novos_casos;
    vector<pair<string, vector<pair<int, string>>>> novas_mortes;
    
    

public:
    base()
    {
    }
    void incluiDado(string nome, int casos, int mortes, string data)
    {
        
        
        if (novos_casos.size() != 0 && novos_casos.back().first == nome )
        {
           
            novos_casos.back().second.push_back(make_pair(casos, data));
            novas_mortes.back().second.push_back(make_pair(mortes, data));
        }

        else
        {
            novos_casos.emplace_back();
            novos_casos.back().first = nome;
            novos_casos.back().second.push_back(make_pair(casos, data));
            novas_mortes.emplace_back();
            novas_mortes.back().first = nome;
            novas_mortes.back().second.push_back(make_pair(mortes, data));
        }
    }
    void imprime()
    {
        for (auto aux : novas_mortes)
        {
            cout << aux.first << " ";
        }
        cout << endl;
    }
    
    
    
};
class regiao : public base
{
public:
    void incluiDado(string nome, int casos, int mortes, string data)
    {
        int i = 0;
        bool found = false;
        bool has_date = false;
        for (auto aux : novos_casos)
        {
            if (aux.first == nome)
            {
                found = true;
                int j = 0;
                for (auto aux2 : novos_casos[i].second)
                {
                    if (aux2.second == data)
                    {
                        novos_casos[i].second[j].first += casos;
                        novas_mortes[i].second[j].first += mortes;
                        has_date = true;
                    }
                    j++;
                }
                if (has_date == false)
                {
                    novos_casos[i].second.push_back(make_pair(casos, data));
                    novas_mortes[i].second.push_back(make_pair(mortes, data));
                    break;
                }
            }
            i++;
        }
        if (found == false)
        {
            novos_casos.emplace_back();
            novos_casos.back().first = nome;
            novos_casos.back().second.push_back(make_pair(casos, data));
            novas_mortes.emplace_back();
            novas_mortes.back().first = nome;
            novas_mortes.back().second.push_back(make_pair(mortes, data));
        }
    }
};

class estado : public base
{
   
};

class cidade : public base
{
    
};

void readFile(estado &es, cidade &cd, regiao &rg)
{
    string linha;

    ifstream arq("HIST_PAINEL_COVIDBR_Parte3_09jun2021.csv");
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
            if (cont_pontvirg == 'H' - 'A')
                data.push_back(c);
            if (cont_pontvirg == 'L' - 'A')
                casos.push_back(c);
            if (cont_pontvirg == 'N' - 'A')
                mortes.push_back(c);
            if (cont_pontvirg == 'O' - 'A')
                break;
        }
        int numero_casos = stoi(casos);
        int numero_mortes = stoi(mortes);
        //rg.incluiDado(regiao,numero_casos,numero_mortes,data);
        if (estado.size() == 0 && cidade.size() == 0)
        {
            rg.incluiDado(regiao, numero_casos, numero_mortes, data);
        }
        if (cidade.size() == 0 && estado.size() != 0)
        {
            rg.incluiDado(regiao, numero_casos, numero_mortes, data);
            es.incluiDado(estado, numero_casos, numero_mortes, data);
        }
        if (cidade.size() != 0)
        {
            cd.incluiDado(cidade,numero_casos,numero_mortes,data);
        }
    }
}

int main()
{
    int a;
    cidade cd;
    regiao rg;
    estado es;
    readFile(es, cd, rg);
    es.imprime();
    rg.imprime();
    cd.imprime();
}
