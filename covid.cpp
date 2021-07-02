#include <bits/stdc++.h>
using namespace std;

enum {obitos = 0, casos};

void clean_stdin()
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' and c != EOF);
}

struct Dado
{
    int numeros[2];
    string data;
    Dado(int novos_obitos, int novos_casos, string data): numeros {novos_obitos, novos_casos}, data(data){}
};

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

    int getNumeros(int dia, int intervalo, int tipo)
    {
        if(intervalo - 1 > dia || intervalo <= 0 || dia >= (int)calendar.size())
        {
            throw(1);
        }

        int aux = 0;
        for(int i = dia-intervalo + 1; i <= dia; i++)
        {
            aux += calendar[i].numeros[tipo];
        }
        return aux;
    }

    double media(int dia, int intervalo, int tipo)
    {
        return (double)getNumeros(dia, intervalo, tipo)/intervalo;
    }

    double growth(int dia, int intervalo_media, int intervalo_crescimento, int tipo)
    {
        if(intervalo_crescimento-1 >dia || intervalo_crescimento<=0)
        {
            throw(2);
        }
        double num = media(dia, intervalo_media, tipo)-media(dia-intervalo_crescimento, intervalo_media, tipo);
        double den = media(dia-intervalo_crescimento, intervalo_media, tipo);
        if(den == 0) return 1e18;
        return 100*(num/den);
    }

    double desvioPadrao(int dia, int intervalo, int tipo){
        double resp = 0;
        double med = media(dia, intervalo, tipo);
        for(int i = dia-intervalo + 1; i <= dia; i++)
        {
            resp += (double)(calendar[i].numeros[tipo] - med)*(calendar[i].numeros[tipo] - med);
        }
        return sqrt(resp/intervalo);
    }

    void inserirDado(Dado d)
    {
        calendar.push_back(d);
    }

    void atualizar(int novos_obitos, int novos_casos, int dia)
    {
        calendar[dia].numeros[casos] += novos_casos;
        calendar[dia].numeros[obitos] += novos_obitos;
    }
};

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

void salvarDados(Local &pais, vector<Local> &regioes, vector<Local> &estados, vector<Local> &cidades, string reg, string est, string cid, int id_UF, int id_cidade, string data, int novos_casos, int novos_obitos)
{
    //Se o vector do local for vazio ou o ultimo local for diferente do atual, queremos adicionar um local novo.
    if(cidades.empty() || cidades.back().getID() != id_cidade)
        cidades.push_back(Local (cid, id_cidade));
    if(estados.empty() || estados.back().getID() != id_UF)
        estados.push_back(Local (est, id_UF));
    if(regioes.empty() || regioes.back().getNome() != reg)
        regioes.push_back(Local (reg, 0));
    //Adiciona os dados, verificando se a data ja existe para as regioes, estados e pais.
    cidades.back().inserirDado(Dado (novos_obitos, novos_casos, data));
    if(estados.back().getCalendarSize()>=cidades.back().getCalendarSize())
        estados.back().atualizar(novos_obitos, novos_casos, cidades.back().getCalendarSize()-1);
    else
        estados.back().inserirDado(Dado (novos_obitos, novos_casos, data));
    if(regioes.back().getCalendarSize()>=cidades.back().getCalendarSize())
        regioes.back().atualizar(novos_obitos, novos_casos, cidades.back().getCalendarSize()-1);
    else
        regioes.back().inserirDado(Dado (novos_obitos, novos_casos, data));
    if(pais.getCalendarSize()>=cidades.back().getCalendarSize())
        pais.atualizar(novos_obitos, novos_casos, cidades.back().getCalendarSize()-1);
    else
        pais.inserirDado(Dado (novos_obitos, novos_casos, data));
}

void readFile(Local &pais, vector<Local> &regioes, vector<Local> &estados, vector<Local> &cidades)
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
        string novos_casos;
        string novos_obitos;
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
                novos_casos.push_back(c);
            if (cont_pontvirg == 'N' - 'A')
                novos_obitos.push_back(c);
            if (cont_pontvirg == 'O' - 'A')
                break;
        }
        if(cidade.empty()) continue;

        int id_UF = stoi(cod_UF);
        int id_cidade = stoi(cod_cidade);
        int numero_casos = stoi(novos_casos);
        int numero_obitos = stoi(novos_obitos);
        salvarDados(pais, regioes, estados, cidades, regiao, estado, cidade, id_UF, id_cidade, data, numero_casos, numero_obitos);
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
            clean_stdin();
            switch (op)
            {
            case 1:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                clean_stdin();
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                clean_stdin();
                valor = atual.getNumeros(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, casos);
                cout << "\nTotal de casos novos no intervalo de " << ini << " a " << fim << ": "
                    << valor << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 2:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                clean_stdin();
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                clean_stdin();
                valor = atual.getNumeros(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, obitos);
                cout << "\nTotal de obitos no intervalo de " << ini << " a " << fim << ": "
                    << valor << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 3:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                clean_stdin();
                cout << "Digite o intervalo de tempo em dias: ";
                cin >> intervalo;
                clean_stdin();
                media = atual.media(queDiaFoiEsse(fim), intervalo, casos);
                cout << "\nMedia movel de casos novos no dia " << fim << " com intervalo de " << intervalo << " dias: "
                    << media << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 4:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                clean_stdin();
                cout << "Digite o intervalo de tempo em dias: ";
                cin >> intervalo;
                clean_stdin();
                media = atual.media(queDiaFoiEsse(fim), intervalo, obitos);
                cout << "\nMedia movel de obitos no dia " << fim << " com intervalo de " << intervalo << " dias: "
                    << media << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 5:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                clean_stdin();
                cout << "Digite o intervalo da media movel em dias: ";
                cin >> intervalo;
                clean_stdin();
                cout << "Digite com quantos dias anteriores deseja comparar: ";
                cin >> intervalo_growth;
                clean_stdin();
                media = atual.growth(queDiaFoiEsse(fim), intervalo, intervalo_growth, casos);
                cout << "\nTendencia de crescimento de casos novos no dia " << fim
                    << "\ncom intervalo de media movel igual a " << intervalo
                    << " dias\nem relação a " << intervalo_growth << " dias anteriores: ";
                if(media == 1e18) cout << "INF" << endl;
                else cout << setprecision(2) << fixed << media << "%" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 6:
                cout << "Digite o dia (DD/MM): ";
                cin >> fim;
                clean_stdin();
                cout << "Digite o intervalo da media movel em dias: ";
                cin >> intervalo;
                clean_stdin();
                cout << "Digite com quantos dias anteriores deseja comparar: ";
                cin >> intervalo_growth;
                clean_stdin();
                media = atual.growth(queDiaFoiEsse(fim), intervalo, intervalo_growth, obitos);
                cout << "\nTendencia de crescimento de obitos no dia " << fim
                    << "\ncom intervalo de media movel igual a " << intervalo
                    << " dias\nem relação a " << intervalo_growth << " dias anteriores: ";
                if(media == 1e18) cout << "INF" << endl;
                else cout << setprecision(2) << fixed << media << "%" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 7:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                clean_stdin();
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                clean_stdin();
                media = atual.media(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, casos);
                desvio = atual.desvioPadrao(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, casos);
                cout << "\nMedia de casos novos no intervalo de " << ini << " a " << fim << ": "
                    << media << endl;
                cout << "Desvio padrao: " << desvio << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
            break;

            case 8:
                cout << "Digite o dia inicial do periodo (DD/MM): ";
                cin >> ini;
                clean_stdin();
                cout << "Digite o dia final do periodo (DD/MM): ";
                cin >> fim;
                clean_stdin();
                media = atual.media(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, obitos);
                desvio = atual.desvioPadrao(queDiaFoiEsse(fim), queDiaFoiEsse(fim) - queDiaFoiEsse(ini) + 1, obitos);
                cout << "\nMedia de obitos no intervalo de " << ini << " a " << fim << ": "
                    << media << endl;
                cout << "Desvio padrao: " << desvio << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
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
                clean_stdin();
                break;
            case 2:
                cout << "Intervalo de crescimento invalido" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
                break;
            case 3:
                cout << "Data Invalida" << endl;
                cout << "\nDigite 0 para continuar: ";
                cin >> ini;
                clean_stdin();
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
        cout << "Digite o nome do local que voce deseja verificar, ou digite 1 para fazer a pesquisa por NUMERO." << endl;
        cout << "Para retornar ao menu, digite 0." << endl;
        getline(cin, c);
        if(c=="0")
            return 0;
        if(c=="1")
        {
            for(i = 0; i < l_pesquisa.size(); i++)
            {
                cout << setw(4) << i+1 << " - " << l_pesquisa[i].getNome() << endl;
            }
            cout << "\n   0 - Retornar a pesquisa por nome." << endl;
            cout << "Digite o NUMERO de onde deseja verificar os dados." << endl;
            cin >> n;
            clean_stdin();
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

void menu(Local &pais, vector<Local> &regioes, vector<Local> &estados, vector<Local> &cidades)
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
        clean_stdin();
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
    Local pais("Brasil", 1);
    vector<Local> regioes, estados, cidades;
    readFile(pais, regioes, estados, cidades);
    menu(pais, regioes, estados, cidades);
}
