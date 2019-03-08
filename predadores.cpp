#include "predadores.hpp"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

#define MULTVIDA 15

int get_int(int erro){
	//pega inteiro da entrada
	//se inteiro for invalido, retorna valor de erro passado
	string aux;
	getline(cin, aux);

	int t = aux.size();
	for(int i=0;i<t;i++){
		if(aux[i]<'0' || aux[i]>'9')
			return erro;
	}
	if(t>0)
		return atoi(aux.c_str());

	return erro;
}

void consoante_aleat(char &letra1){	//atribui uma consoante aleatoria ao char passado
	char letra;
	do{
		letra = 97 + (rand()%26);
	}while(letra=='a' or letra=='e' or letra=='i' or letra=='o' or letra=='u' or letra=='q');
	letra1 = letra;
}

void vogal_aleat(char &letra2){	//atribui uma vogal aleatoria ao char passado
	char letra;
	letra = rand()%5;
	if(letra==0)
		letra='a';
	else if(letra==1)
		letra='e';
	else if(letra==2)
		letra='i';
	else if(letra==3)
		letra='o';
	else if(letra==4)
		letra='u';

	letra2 = letra;
}

string nome_aleat(){	//cria um nome aleatorio
	char letra1, letra2;
	int tam = 4+(rand()%7);	//tamanho aleatorio entre 4 e MULTVIDA
	if(tam%2==1)
		tam--;

	string nome(tam, ' ');
	for(int i=0;i<tam;i+=2){
		thread let1(consoante_aleat, std::ref(letra1));	//cria consoantes e vogais aleatorias em paralelo
		thread let2(vogal_aleat, std::ref(letra2));
		let1.join();	//sincroniza
		let2.join();
		nome[i]=letra1;	//atribui ao nome
		nome[i+1]=letra2;
	}
	nome[0]-=32;	//transforma primeira letra em maiuscula

	return nome;
}

Predador::Predador(){
	int t=0;
	this->alimentacao=0;
	for(int i=0;i<N_ATR;i++){
		this->atr[i]= rand()%MAX_POINTS;
		t+=this->atr[i];
	}

	for(int i=0;i<N_ATR;i++){
		this->atr[i] = ((this->atr[i]*MAX_POINTS)/t)+1;
	}
	this->vida=this->atr[TAMANHO]*MULTVIDA;
	this->idade=0;
	this->nome = nome_aleat();
}

Predador::Predador(int* atr, string nome){
	int t=0;
	if(atr!=NULL){
		for(int i=0;i<N_ATR;i++){
			this->atr[i] = atr[i];
			t+=atr[i];
		}
		for(int i=0;i<N_ATR;i++){
			this->atr[i] = ((this->atr[i]*MAX_POINTS)/t)+1;
		}
		this->vida=MULTVIDA*this->atr[TAMANHO];
		this->alimentacao=0;
		this->idade=0;
		this->nome = nome;
	}
	else{
		throw -1;
		
	}
}

void Predador::set_atr(int ind, int val){
	this->atr[ind] = val;
}

void Predador::set_vida(int v){
	this->vida = v;
}

void Predador::set_alimentacao(int a){
	this->alimentacao=a;
}

void Predador::set_idade(int i){
	this->idade=i;
}

void Predador::set_nome(string nome){
	this->nome = nome;
}

string Predador::get_nome(){
	return this->nome;
}

int Predador::get_idade(){
	return this->idade;
}

void Predador::aniversario(){
	this->idade++;
	this->vida = this->atr[TAMANHO]*MULTVIDA;
	this->alimentacao=0;
}

int Predador::get_atr(int ind){
	return this->atr[ind];
}

int Predador::get_vida(){
	return this->vida;
}

int Predador::get_alimentacao(){
	return this->alimentacao;
}

int Predador::analisar(Predador p){
	int vidaop;

	if(this->alimentacao >= this->atr[TAMANHO])
		return 0;

	if(this->vida<1)
		return 0;

	vidaop = ((((p.get_vida()))*this->get_atr(INTELIGENCIA))/MAX_POINTS);

	if(vidaop > (this->vida))
		return 0;
	return 1;
}

int Predador::fugir(Predador p){
	if((p.get_atr(CACA)) >= (this->atr[FUGA]))
		return 0;
	return 1;
}

int Predador::receber_dano(int d){
	int defesa = this->atr[DEFESA];
	if(this->vida>0){
		d -= ((d*defesa)/MAX_POINTS);
		if(d<1)
			d=1;
		this->vida-=(d);
		return 1;
	}
	return 0;
}

int Predador::atacar(Predador &p){

	int dano = this->atr[ATAQUE];
	if(this->vida>0){

		p.receber_dano(dano);
		return 1;
	}
	return 0;
}

int Predador::morreu(){
	if(this->vida<1){
		this->atr[FUGA]=0;
		this->atr[CACA]=0;
		this->atr[AGILIDADE]=0;
		this->atr[ATAQUE]=0;
		this->atr[DEFESA]=0;
		this->atr[INTELIGENCIA]=0;
		return 1;
	}
	return 0;
}

int Predador::ser_comido(int fome){
	if(this->vida>0)
		return 0;
	if(fome >= this->atr[TAMANHO]){
		this->atr[TAMANHO]=0;
	}
	else{
		this->atr[TAMANHO]-=fome;
	}
	return 1;
}

int Predador::comer(Predador &p){
	int fome = this->atr[TAMANHO]-this->alimentacao;
	if(p.get_atr(TAMANHO)>0){
		if(fome >= p.get_atr(TAMANHO)){
			this->alimentacao+=p.get_atr(TAMANHO);
		}
		else{
			this->alimentacao=this->atr[TAMANHO];	
		}
		p.ser_comido(fome);
		return 1;
	}
	return 0;
}

void Predador::recuperar_vida(){
	// int vida_perdida = (this->atr[TAMANHO]*MULTVIDA)-this->vida;
	// int recuperar = this->alimentacao*30;
	if(this->vida > 0){
		this->vida = this->atr[TAMANHO]*MULTVIDA;
		/*if(recuperar <= vida_perdida){
			this->vida+=recuperar;
			this->alimentacao=0;
		}
		else{
			this->alimentacao-=(vida_perdida/30);
			this->vida = this->atr[TAMANHO]*MULTVIDA;
		}*/
	}
}

Predador Predador::reproduzir(Predador p){
	int min;
	//int max;
	int dif;
	int val;
	int t=0;

	Predador filho;

	dif=MAX_POINTS/20;
	for(int i=0;i<N_ATR;i++){

		val = (this->atr[i]+p.get_atr(i))/2;

		// if(this->atr[i]<p.get_atr(i)){
		// 	min = this->atr[i]-dif;
		// 	max = p.get_atr(i)+dif;
		// }
		// else{
		// 	min = p.get_atr(i)-dif;
		// 	max = this->atr[i]+dif;
		// }

		// if(min<1)
		// 	min=1;
		// if(max>MAX_POINTS)
		// 	max=MAX_POINTS;

		// if(max==min)
		// 	max++;

		if(val<1)
			val=1;
		filho.set_atr(i, val);
		t+=val;
	}

	min = rand()%N_ATR;
	val = filho.get_atr(min);
	if(rand()%2)
		val+=dif;
	else
		val-=dif;
	if(val<1)
		val=1;
	filho.set_atr(min, val);

	

	for(int i=0;i<N_ATR;i++){
		val = filho.get_atr(i);
		val = ((val*MAX_POINTS)/t);
		if(val<1)
			val=1;
		filho.set_atr(i, val);
	}



	filho.set_vida(filho.get_atr(TAMANHO)*MULTVIDA);
	filho.set_alimentacao(0);
	filho.set_nome(nome_aleat());
	return filho;
}

void Predador::criar(){
	string nome;
	double t=0;
	double val;
	cout<<"Nome: ";

	getline(cin, nome);
	this->nome = nome;

	cout<<"Tamanho: ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[TAMANHO] = val;
	t+=val;

	cout<<"Ataque ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[ATAQUE] = val;
	t+=val;

	cout<<"Defesa ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[DEFESA] = val;
	t+=val;

	cout<<"Inteligencia: ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[INTELIGENCIA] = val;
	t+=val;

	cout<<"Agilidade: ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[AGILIDADE] = val;
	t+=val;

	cout<<"Fuga: ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;
	this->atr[FUGA] = val;
	t+=val;

	cout<<"Caça: ";
	val = get_int(rand()%MAX_POINTS);
	if(val<1)
		val=1;

	for(int i=0;i<N_ATR;i++){
		val = this->atr[i];
		val = ((val*MAX_POINTS)/t);
		if(val<1)
			val=1;
		if(val>MAX_POINTS)
			val=MAX_POINTS;
		this->atr[i] = val;
	}

	this->vida = this->atr[TAMANHO]*MULTVIDA;
	this->idade=0;
	this->alimentacao=0;
}

void Predador::print(){
	cout<<"Nome: "<<this->get_nome()<<":"<<endl;
	cout<<"Idade: "<<this->get_idade()<<endl;
	cout<<"Vida: "<<this->get_vida()<<endl;
	cout<<"Tamanho: "<<this->get_atr(0)<<endl;
	cout<<"Ataque: "<<this->get_atr(1)<<endl;
	cout<<"Defesa: "<<this->get_atr(2)<<endl;
	cout<<"Inteligencia: "<<this->get_atr(3)<<endl;
	cout<<"Agilidade: "<<this->get_atr(4)<<endl;
	cout<<"Fuga: "<<this->get_atr(5)<<endl;
	cout<<"Caça: "<<this->get_atr(6)<<endl;
	cout<<endl;
}