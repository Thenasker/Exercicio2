/*
*   Exercicio 1.
*
*   Crie uma paleta de cores onde cada pixel é colorido pelo valor de x e y. A saida do
*   programa deve ser uma paleta muito proxima a paleta descrita abaixo:
*
*
*/


/////////////////////////////INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <sstream>
#include <dirent.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <cmath>
#include <bitset>
/////////////////////////////INCLUDES

/////////////////////////////DEFINES
//Definiçao do string do menu
#define menu "\n1-- Criar Paleta\n\n2--Invertir cores\n\n3--Cor que mais aparece\n\n4--Criar imagem com mensagem\n\n5--Ler imagem com mensagem\n\n0--Sair\n\n"//Definiçao do tamaño do nome dos arquivos a salver e do formato deles
#define FORMATO_NOME "Paleta_%Y%m%d_%H%M%S.ppm"
#define TAMANHO_NOME 27
#define FORMATO_NOME_CRIPT "Cripto_%Y%m%d_%H%M%S.ppm"
#define CHAR_VACIO '_'
/////////////////////////////DEFINES

using namespace std;

//Struct que armazena toda a informaçao que compoe a imagem
struct PPM
{
  char magicNum[3];
  int dimx, dimy, maxVal;
  unsigned char * pixels;
};


//Funçao que mostra o menu de opçoes inicial
int mostrarMenu(){

    int opcao=-1;

    //Enquanto o usuario nao escreva uma opçao valida o menu vai se mostrar uma e outra vez.
    while(opcao<0 || opcao>7){
        system("cls");
        //Usamos a variavel definida 'menu'
        cout << menu << endl;
        cin >> opcao;
        if(opcao<0 || opcao>7){
            cout << "Inserir uma opcao valida (0-7)" << endl;
            Sleep(2000);
        }
    }
    return opcao;
}

//funçao para perguntar ao usuario se ele quer finalizar o apicativo o mostrar o menu inicial outra vez
bool finalizar(){

    char fim = CHAR_VACIO;

    //Se pergunta ao usuario se que sair e devolve um valor booleano para a funçao main
    while(fim!='S' && fim!='s' && fim!='N' && fim!='n'){
        cout << "\n\n\nDeseja finalizar o programa? (S/N)" << endl;
        cin >> fim;
    }

    if(fim=='S' || fim == 's'){
        return true;
    }else{
        return false;
    }

}

// Funçao para listar os elementos de um diretorio
void list_dir(string dir){

    DIR *directorio;
    struct dirent *elemento;
    string elem;

    if(directorio = opendir(dir.c_str())){
        while(elemento = readdir(directorio)){
            elem = elemento -> d_name;
            if(elem.length() > 4){
                if(elem.substr(elem.length()-3,elem.length()-1) == "ppm"){
                    cout << elem << endl;
                }
            }
        }
    }
    closedir(directorio);
}

//funçao que cria uma matriz de tuplas RGB e escreve elas num arquivo na disposiçao dos arquivos P6
//Primeiro escreve a cabeceira no arquivo
void criarPaleta(int x, int y){

    unsigned char color[3];
    //Formatamos o nome do arquivo com a data e hora do momento da criaçao do arquivo segundo o formato definido acima.
    char nome[TAMANHO_NOME];
    time_t timestamp = time(0);
    strftime(nome, sizeof(nome), FORMATO_NOME, localtime(&timestamp));

    FILE *fp = fopen(nome, "wb");

    fprintf(fp, "P6\n%d %d\n255\n", x, y);

    for (int i=0; i<x; i++)
    {
        for (int j=0; j<y; j++)
        {
            //original
            color[0] = j;//R
            color[1] = i;//G
            color[2] = (j+i) % 256;//B

            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);

    system(nome);

}

//funçao que recebe um struct PPM e gera uma imagem igual só que cada cor de cada pixel é 255-n para obter a cor inversa
void criarInversa(PPM &ppm){

    unsigned char color[3];
    //Formatamos o nome do arquivo com a data e hora do momento da criaçao do arquivo segundo o formato definido acima.
    char nome[TAMANHO_NOME];
    time_t timestamp = time(0);
    strftime(nome, sizeof(nome), FORMATO_NOME, localtime(&timestamp));

    FILE *fp = fopen(nome, "wb");

    fprintf(fp, "P6\n%d %d\n%d\n", ppm.dimx, ppm.dimy,ppm.maxVal);

    for (int i=0; i<ppm.dimx*ppm.dimy*3; i+=3){

        //original
        color[0] = 255 - static_cast<int>(ppm.pixels[i]);//R
        color[1] = 255 - static_cast<int>(ppm.pixels[i+1]);//G
        color[2] = 255 - static_cast<int>(ppm.pixels[i+2]);//B

        fwrite(color, 1, 3, fp);
    }
    fclose(fp);

    system(nome);

}

//funçao de obtem a informaçao de uma imagem e armazena ela num struct PPM
//Primero le a cabezeira, depois inicializa o array de unsigned char de pixeis em relaçao ao tamanho da imagem. Por ultimo le cada lina e vaiu armazenado os valores no array "pixels"
void lerImagem(PPM &ppmImagem, string dir){

    FILE *fr = fopen(dir.c_str(), "rb");

    if(fr == NULL) {
        cout << "Erro ao abrir o arquivo" << endl;
    }
    fscanf(fr, "%2s", ppmImagem.magicNum);
    ppmImagem.magicNum[2] = '\0';
    if(strcmp(ppmImagem.magicNum, "P6") != 0) {
        cout << "O arquivo nao e do tipo P6" << endl;
    }

    fscanf(fr, "%d", &ppmImagem.dimx);
    fscanf(fr, "%d", &ppmImagem.dimy);
    fscanf(fr, "%d", &ppmImagem.maxVal);

    fseek(fr, 1, SEEK_CUR);

    ppmImagem.pixels = new unsigned char[ppmImagem.dimx * ppmImagem.dimy * 3];

    fread(ppmImagem.pixels, 3*ppmImagem.dimx, ppmImagem.dimy, fr);
}

// funçao que chama as funçoes necessarias para invertir as cores de uma imagem
void invertirCores(string dir){

    PPM ppmImagem;

    lerImagem(ppmImagem,dir);

    criarInversa(ppmImagem);

    system("pause");

}

//funçao que le uma imagem, armazena ela e apois de converter cada valor do array pixels em hexadecimal, armazzena eles em um vetor da clase Vector
//com esse vetor usamos um iterator para contar as repetiçoes das cores e mostramos a que mais se repeter
void contarCores(string dir){

    PPM ppmImagem;
    vector<string> coresHexa;
    map<string,unsigned int> conta;
    int aux=0;
    string hexaAux;

    lerImagem(ppmImagem,dir);

    stringstream ss;

    for (int i=0; i<ppmImagem.dimx*ppmImagem.dimy*3; i+=3){

        ss.str("");

        ss << "#";
        ss<< hex << static_cast<int>(ppmImagem.pixels[i]);//R
        ss<< hex << static_cast<int>(ppmImagem.pixels[i+1]);//G
        ss<< hex << static_cast<int>(ppmImagem.pixels[i+2]);//B

        coresHexa.push_back(ss.str());
    }

    for (int i = 0; i < coresHexa.size(); ++i)
    {
        map<string,unsigned int>::iterator it(conta.find(coresHexa.at(i)));
        if (it != conta.end()){
            it->second++;
        } else {
            conta[coresHexa.at(i)] = 1;
        }
    }

    for(map<string,unsigned int>::iterator it = conta.begin(); it != conta.end(); ++it) {

        if(aux < it->second){
            aux = it -> second;
            hexaAux = it -> first;
        }

    }

    system("cls");
    cout << "A cor que mais se repete e " << hexaAux << ", com " << aux << " pixeis." << endl;


}

//essa funçao adiciona a mensagem as palavras de abertura e fechadura "BACON" e "FRITO" e converte cada carater em um objeto bitset<8>
//depois, cada objeto bitset<8> é copiado bit a bit no ultimo bit de cada cor azul de cada pixel
//Por ultimo esses pixeis sao escritos num arquivo que cria uma imagem.
bool encriptar(string msg, string dir){


    PPM ppmImagem;
    vector < bitset<8> > bytes;
    bitset<8> byte;
    int intByte=0;
    int atual=119;
    unsigned char color[3];

    //Formatamos o nome do arquivo com a data e hora do momento da criaçao do arquivo segundo o formato definido acima.
    char nome[TAMANHO_NOME];
    time_t timestamp = time(0);
    strftime(nome, sizeof(nome), FORMATO_NOME_CRIPT, localtime(&timestamp));

    lerImagem(ppmImagem,dir);

    if(ppmImagem.dimx*ppmImagem.dimy < 120+(msg.size()*8)){
        return false;
    }

    msg.insert(0,"BACON");
    msg.append("FRITO");

    for(int i=0;i<msg.length();i++){
        bytes.push_back(bitset<8>(msg.at(i)));
    }

    for(int i=0; i<bytes.size() ; i++){

        for(int j=0; j<8 ; j++){


            byte = bitset<8>(static_cast<int>(ppmImagem.pixels[atual]));
            byte.set(0,bytes.at(i)[j]);
            intByte = (int)(byte.to_ulong());
            ppmImagem.pixels[atual] = intByte;
            atual += 3;
        }


    }

    FILE *fp = fopen(nome, "wb");

    fprintf(fp, "P6\n%d %d\n%d\n", ppmImagem.dimx, ppmImagem.dimy,ppmImagem.maxVal);

    for (int i=0; i<ppmImagem.dimx*ppmImagem.dimy*3; i+=3){

        //original
        color[0] = static_cast<int>(ppmImagem.pixels[i]);//R
        color[1] = static_cast<int>(ppmImagem.pixels[i+1]);//G
        color[2] = static_cast<int>(ppmImagem.pixels[i+2]);//B

        fwrite(color, 1, 3, fp);
    }
    fclose(fp);

    system(nome);

    return true;
}

//A funçao primeiro procura a palavra de abertura para saber se a iumagem contem uma mensagem lendo cada ultimo bit de cada cor azul.
//Depois continua lendo ate achar a palavra de fechadura e mostra o mensagem em forma de texto.
bool desencriptar(string dir){

    PPM ppmImagem;
    vector < bitset<8> > bytes;
    bitset<8> byte(0);
    bitset<8> rgb(0);
    char msg[150];
    char palavra[5];
    int cont = 0;
    int cont2 = 0;
    string result;

    lerImagem(ppmImagem,dir);

    for(int k=119 ; k<240; k+=3){

        rgb = bitset<8>(static_cast<int>(ppmImagem.pixels[k]));
        byte.set(cont,rgb[0]);
        if(cont == 7){
            palavra[cont2] =(int)(byte.to_ulong());
            cont = -1;
            cont2++;
        }
        cont++;
    }

    if(!strcmp(palavra,"BACON")){
        return false;
    }

    cont=0;
    cont2=0;

    for(int k=119 ; k<ppmImagem.dimx*ppmImagem.dimy*3; k+=3){

        rgb = bitset<8>(static_cast<int>(ppmImagem.pixels[k]));
        byte.set(cont,rgb[0]);
        if(cont == 7){
            msg[cont2] =(int)(byte.to_ulong());
            cont = -1;

            //MUITO FEIO :(
            if(msg[cont2]=='O' && msg[cont2-1]=='T' && msg[cont2-2]=='I' && msg[cont2-3]=='R' && msg[cont2-4]=='F') k=ppmImagem.dimx*ppmImagem.dimy*3;

            cont2++;
        }
        cont++;
    }

    msg[cont2] = '\0';

    system("cls");
    cout << "Mensagem:\n" << endl;
    printf("%.*s\n\n", cont2-10, msg + 5);

    return true;
}

//Funçoes que gerenciam as funcionalidades do programa
bool opcao1(){

    int x=0,y=0;

    system("cls");
    while(x<1 || y<1 || x>800 || y>800){
        cout << "Inserir o tamanho da imagem (no exemplo 200x200):" << endl;
        cout << "largura: ";
        cin >> x;
        cout << "altura: ";
        cin >> y;
    }

    criarPaleta(x,y);

    //devolvemos o valor que vamos obter da funçao finalizar()
    return finalizar();
}

bool opcao2(){

    string dir;
    string nome;

    system("cls");

    cout << "Escreva a rota onde buscar o arquivo (Ex: C\\)\n" << endl;
    cin >> dir;
    list_dir(dir);

    cout << "Escreva o nome do arquivo (Ex: Paleta_20170616_194810.ppm)\n" << endl;
    cin >> nome;

    dir.append("\\");
    dir.append(nome);

    invertirCores(dir);


    //devolvemos o valor que vamos obter da funçao finalizar()
    return finalizar();

}

bool opcao3(){

    string dir;
    string nome;

    system("cls");

    cout << "Escreva a rota onde buscar o arquivo (Ex: C\\)\n" << endl;
    cin >> dir;
    list_dir(dir);

    cout << "Escreva o nome do arquivo (Ex: Paleta_20170616_194810.ppm)\n" << endl;
    cin >> nome;

    dir.append("\\");
    dir.append(nome);

    contarCores(dir);

    system(dir.c_str());

    //devolvemos o valor que vamos obter da funçao finalizar()
    return finalizar();

}

bool opcao4(){


    string dir;
    string nome;
    string msg = "";
    bool val = false;

    while(!val){
        system("cls");

        cout << "Escreva a rota onde buscar o arquivo (Ex: C\\)\n" << endl;
        cin >> dir;

        list_dir(dir);
        cout << "Escreva o nome do arquivo (Ex: Paleta_20170616_194810.ppm)\n" << endl;
        cin >> nome;

        dir.append("\\");
        dir.append(nome);


        system("cls");
        while(msg.empty() || msg.length() > 140){
            cout << "Inserir o texto a encriptar (max. 140 carateres):" << endl;
            getchar();
            getline(cin,msg);
        }

        val = encriptar(msg,dir);
    }

    //devolvemos o valor que vamos obter da funçao finalizar()
    return finalizar();

}

bool opcao5(){


    string dir;
    string nome;

    system("cls");

    cout << "Escreva a rota onde buscar o arquivo (Ex: C\\)\n" << endl;
    cin >> dir;
    list_dir(dir);

    cout << "Escreva o nome do arquivo (Ex: Paleta_20170616_194810.ppm)\n" << endl;
    cin >> nome;

    dir.append("\\");
    dir.append(nome);

    if(!desencriptar(dir)){
        system("cls");
        cout << "A imagem nao contem mensagem" << endl;
    }

    //devolvemos o valor que vamos obter da funçao finalizar()
    return finalizar();

}

//Funçao main
int main(){

    bool finalizar = false;

    //Mostramos a introduçao
    cout << "Linguagem de programacao" << endl;
    cout << "AV2. Questao 2. Paleta." << endl;
    cout << "Pablo Rufat Munoz" << endl;
    cout << "Marcos Marques" << endl;
    cout << "2017/1\n\n" << endl;
    system("pause");

    //Mientras a funçao finalizar() nao devolva true, vamos chamar a funçao que mostra o menu.
    while(!finalizar){
        switch(mostrarMenu()){
            case(0):
                finalizar = true;
                break;
            case(1):
                finalizar = opcao1();
                break;
            case(2):
                finalizar = opcao2();
                break;
            case(3):
                finalizar = opcao3();
                break;
            case(4):
                finalizar = opcao4();
                break;
            case(5):
                finalizar = opcao5();
                break;
            default:
                finalizar = true;
                break;
        };
    }
    return 0;
}
