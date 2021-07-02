#include <bits/stdc++.h>
using namespace std;

enum {obitos, casos};

struct Dado
{
    int obitos, casos;
    string data;
    Dado(int obitos, int casos, string data):obitos(obitos), casos(casos), data(data){}
};

// struct Obitos
// {
//     int valor;
// };

// struct Casos
// {
//     int valor;
// };

class Local
{
private:
    string nome;
    int id;
    vector<Dado> calendar;
public:
    Local(string nome = "", int id = 0):nome(nome), id(id){}

    int getCalendarSize() const
    {
        return (int) calendar.size();
    }
    string getNome() const
    {
        return nome;
    }
    int getID() const
    {
        return id;
    }
    int getObitos(int dia, int intervalo)
    {
        if(intervalo - 1 > dia || intervalo <= 0 || dia >= (int)calendar.size())
        {
            throw(1);
        }

        int aux = 0;
        for(int i = dia-intervalo + 1; i <= dia; i++)
        {
            aux += calendar[i].obitos;
        }
        return aux;
    }
    int getCasos(int dia, int intervalo)
    {
        if(intervalo-1 >dia || intervalo <= 0 || dia >= (int)calendar.size())
        {
            throw(1);
        }

        int aux = 0;
        for(int i = dia-intervalo+1; i<=dia; i++)
        {
            aux += calendar[i].casos;
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
            throw(2);
        }
        return 100*((mediaObitos(dia, intervalo_media)-mediaObitos(dia-intervalo_crescimento, intervalo_media))/mediaObitos(dia-intervalo_crescimento, intervalo_media));
    }
    double growthCasos(int dia, int intervalo_media, int intervalo_crescimento)
    {
        if(intervalo_crescimento-1 >dia || intervalo_crescimento<=0)
        {
            throw(2);
        }
        return 100*((mediaCasos(dia, intervalo_media)-mediaCasos(dia-intervalo_crescimento, intervalo_media))/mediaCasos(dia-intervalo_crescimento, intervalo_media));
    }
    double desvioPadraoCasos(int dia, int intervalo){
        double resp = 0;
        double media = mediaCasos(dia, intervalo);
        for(int i = dia-intervalo + 1; i <= dia; i++)
        {
            resp += (double)(calendar[i].casos - media)*(calendar[i].casos - media);
        }
        return sqrt(resp/intervalo);
    }
    double desvioPadraoObitos(int dia, int intervalo){
        double resp = 0;
        double media = mediaObitos(dia, intervalo);
        for(int i = dia-intervalo + 1; i <= dia; i++)
        {
            resp += (calendar[i].obitos - media)*(calendar[i].obitos - media);
        }
        return sqrt(resp/intervalo);
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
};

Local pais("Brasil", 1);
vector<Local> regioes, estados, cidades;

int queDiaFoiEsse(string data)
{
    //DD/MM para AAAA-MM-DD se nao for AAAA-MM-DD
    if((int)data.size() < 10){
        string tmp = "2021-";
        tmp += data[3];
        tmp += data[4];
        tmp += "-";
        tmp += data[0];
        tmp += data[1];
        data = tmp;
    }

    int dia = 10*(data[8]-'0') + data[9]-'0';
    int mes = 10*(data[5]-'0') + data[6]-'0';
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(dia <= 0 or mes <= 0 or mes > 12 or dia > dias_mes[mes - 1]){
        throw(3);
    }
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
        if(cidade.empty()) continue;

        int id_UF = stoi(cod_UF);
        int id_cidade = stoi(cod_cidade);
        int numero_casos = stoi(casos);
        int numero_mortes = stoi(mortes);
        salvarDados(regiao, estado, cidade, id_UF, id_cidade, data, numero_casos, numero_mortes);
    }
    arq.close();
}

void informar(Local atual)
{
    int op = 1, intervalo = 0, intervalo_growth = 0, valor = 0;
    double media = 0, desvio = 0;
    string ini, fim;
    try{
        while(op != 0)
        {
            cout << "O local que esta avaliando eh: " << atual.getNome() << endl;
            cout << "Digite o ID do dado que deseja:" << endl;
            cout << setw(2) << 1 << " - Total de CASOS  num periodo." << endl;
            cout << setw(2) << 2 << " - Total de OBITOS num periodo." << endl;
            cout << setw(2) << 3 << " - Media Movel de CASOS  num periodo." << endl;
            cout << setw(2) << 4 << " - Media Movel de OBITOS num periodo." << endl;
            cout << setw(2) << 5 << " - Tendencia de crescimento entre duas Medias Moveis de CASOS." << endl;
            cout << setw(2) << 6 << " - Tendencia de crescimento entre duas Medias Moveis de OBITOS." << endl;
            cout << setw(2) << 7 << " - Media e desvio padrao de CASOS num periodo." << endl;
            cout << setw(2) << 8 << " - Media e desvio padrao de OBITO num periodo." << endl;
            cout << endl;
            cout << setw(2) << 0 << " - Retornar ao menu anterior." << endl;
            cin >> op;
            fflush(stdin);
            switch (op)
            {
            case 1:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                fflush(stdin);
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                valor = atual.getCasos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                cout << "\nTotal de casos novos no intervalo de " << ini << " a " << fim << ": "
                    << valor << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 2:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                fflush(stdin);
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                valor = atual.getObitos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                cout << "\nTotal de obitos no intervalo de " << ini << " a " << fim << ": "
                    << valor << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 3:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                cout << "Digite o intervalo de tempo em dias: ";
                cin >> intervalo;
                fflush(stdin);
                media = atual.mediaCasos(queDiaFoiEsse(fim), intervalo);
                cout << "\nMedia movel de casos novos no dia " << fim << " com intervalo de " << intervalo << " dias: "
                    << media << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 4:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                cout << "Digite o intervalo de tempo em dias: ";
                cin >> intervalo;
                fflush(stdin);
                media = atual.mediaObitos(queDiaFoiEsse(fim), intervalo);
                cout << "\nMedia movel de obitos no dia " << fim << " com intervalo de " << intervalo << " dias: "
                    << media << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 5:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                cout << "Digite o intervalo da media movel em dias: ";
                cin >> intervalo;
                fflush(stdin);
                cout << "Digite com quantos dias anteriores deseja comparar: ";
                cin >> intervalo_growth;
                fflush(stdin);
                media = atual.growthCasos(queDiaFoiEsse(fim), intervalo, intervalo_growth);
                cout << "\nTendencia de crescimento de casos novos no dia " << fim
                    << " com intervalo de media movel igual a " << intervalo
                    << " dias em relação a " << intervalo_growth << " dias anteriores: "
                    << setprecision(2) << fixed << media << "%" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 6:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                cout << "Digite o intervalo da media movel em dias: ";
                cin >> intervalo;
                fflush(stdin);
                cout << "Digite com quantos dias anteriores deseja comparar: ";
                cin >> intervalo_growth;
                fflush(stdin);
                media = atual.growthObitos(queDiaFoiEsse(fim), intervalo, intervalo_growth);
                cout << "\nTendencia de crescimento de obitos no dia " << fim
                    << " com intervalo de media movel igual a " << intervalo
                    << " dias em relação a " << intervalo_growth << " dias anteriores: "
                    << setprecision(2) << fixed << media << "%" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 7:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                fflush(stdin);
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                media = atual.mediaCasos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                desvio = atual.desvioPadraoCasos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                cout << "\nMedia de casos novos no intervalo de " << ini << " a " << fim << ": "
                    << media << endl;
                cout << "Desvio padrao: " << desvio << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 8:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                fflush(stdin);
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                fflush(stdin);
                media = atual.mediaObitos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                desvio = atual.desvioPadraoObitos(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1);
                cout << "\nMedia de obitos no intervalo de " << ini << " a " << fim << ": "
                    << media << endl;
                cout << "Desvio padrao: " << desvio << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
            break;

            case 0:
                //Retornar.
            break;

            default:
                cout << "Valor Invalido." << endl;
            }
        }
    }
    catch(int err){
        switch(err){
            case 1:
                cout << "Intervalo invalido" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
                break;
            case 2:
                cout << "Intervalo de crescimento invalido" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
                break;
            case 3:
                cout << "Data Invalida" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                fflush(stdin);
                break;
            default:
                cout << "Erro." << endl;
        }
    }

}

int enumerar(const vector<Local> &l_pesquisa)
{
    string c = "1";
    int n = 1;
    int i = 0;
    bool found = false;
    while(c != "0")
    {
        cout << "Digite o nome do local que voce deseja verificar, ou digite 1 para fazer a pesquisa por ID." << endl;
        cout << "Para retornar ao menu, digite 0." << endl;
        getline(cin, c);
        fflush(stdin);
        if(c=="0")
            return 0;
        if(c=="1")
        {
            for(i = 0; i < l_pesquisa.size(); i++)
            {
                cout << setw(4) << i+1 << " - " << l_pesquisa[i].getNome() << endl;
            }
            cout << "\n   0 - Retornar a pesquisa por nome." << endl;
            cout << "Digite o ID de onde deseja verificar os dados." << endl;
            cin >> n;
            fflush(stdin);
            if(n > 0 && n < i+1)
                informar(l_pesquisa[n-1]);
            else
            {
                if(n != 0)
                    cout << "Valor Invalido." << endl;
            }
        }
        else
        {
            for(i = 0; i < l_pesquisa.size(); i++)
            {
                if(l_pesquisa[i].getNome() == c)
                {
                    informar(l_pesquisa[i]);
                    found = true;
                }
            }
            if(found)
                found = false;
            else
                cout << "Nome nao encontrado. Talvez nao seja possivel verificar nomes com acento ou com letras maiusculas diferentes das salvas." << endl;
        }
    }
    return 0;
}

void menu()
{
    cout << "As estatisticas foram carregadas." << endl;
    int op = 1;
    while(op != 0)
    {
        cout << "Digite o ID do item que deseja avaliar:" << endl;
        cout << setw(2) << 1 << " - Fazer busca no BRASIL." << endl;
        cout << setw(2) << 2 << " - Fazer busca por REGIAO." << endl;
        cout << setw(2) << 3 << " - Fazer busca por ESTADO." << endl;
        cout << setw(2) << 4 << " - Fazer busca por CIDADE." << endl;
        cout << endl;
        cout << setw(2) << 0 << " - Encerrar programa." << endl;
        cin >> op;
        fflush(stdin);
        switch (op)
        {
           case 1:
             informar(pais);
           break;

           case 2:
             enumerar(regioes);
           break;

           case 3:
             enumerar(estados);
           break;

           case 4:
             enumerar(cidades);
           break;

           case 0:
             //Programa encerrado.
           break;

           default:
             cout << "Valor Invalido." << endl;
        }
    }

}

int main()
{
    readFile();
    // cout << estados[0].getObitos(queDiaFoiEsse("2021-01-01"), 1) << endl;
    // cout << estados[0].getCasos(queDiaFoiEsse("2021-01-02"), 2) << endl;
    // cout << estados[0].growthObitos(queDiaFoiEsse("2021-01-10"), 3, 4) << endl;
    // cout << estados[0].growthCasos(queDiaFoiEsse("2021-01-10"), 3, 4) << endl;
    menu();
}
