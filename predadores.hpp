#ifndef PREDADORES_HPP
#define PREDADORES_HPP

#include <iostream>
#include <string>

using namespace std;

#define MAX_POINTS 7000
#define TAMANHO 0
#define ATAQUE 1
#define DEFESA 2
#define INTELIGENCIA 3
#define AGILIDADE 4
#define FUGA 5
#define CACA 6
#define N_ATR 7

int get_int(int n);

class Predador{
private:
	int atr[N_ATR];
	int vida;
	int alimentacao;
	int idade;
	string nome;

public:
	Predador();
	Predador(int* atr, string nome);

	void set_atr(int ind, int val);
	void set_vida(int v);
	void set_alimentacao(int a);
	void set_idade(int i);
	void set_nome(string nome);

	string get_nome();
	int get_idade();
	int get_atr(int ind);
	int get_vida();
	int get_alimentacao();

	void aniversario();
	int analisar(Predador p);
	int fugir(Predador p);
	int receber_dano(int dano);
	int atacar(Predador &p);
	int morreu();
	int comer(Predador &p);
	int ser_comido(int fome);
	Predador reproduzir(Predador p);
	void criar();
	void print();
	void recuperar_vida();

	//friend ostream& operator<<(ostream& os, Predador p);
};

#endif