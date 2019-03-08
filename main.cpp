#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "predadores.hpp"
#include <vector>
#include <algorithm>
#include <fstream>
#include <thread>
using namespace std;

#define principal "PoPuLaCao.txt"

bool melhor(Predador &p1, Predador &p2){
	if(p1.get_idade()>=p2.get_idade())
		return true;
	return false;
}

vector<Predador> inicializar_populacao(int n){
	Predador p;
	vector<Predador> pop;
	for(int i=0;i<n;i++){
		p = Predador();
		pop.push_back(p);
	}
	return pop;
}

void limpar_mortos(vector<Predador> &pop, string seguir){
	
	for(int i=0;i<(int)pop.size();i++){

		if(pop[i].get_alimentacao()<(pop[i].get_atr(TAMANHO)) || pop[i].get_vida()<1){
			if(pop[i].get_nome()==seguir and pop[i].get_vida()>0){
				cout<<seguir<<" morreu de fome"<<endl;
			}
			pop.erase(pop.begin()+i);
			i--;
		}
	}
}

void reproduzir_populacao(vector<Predador> &pop, int num, string seguir){
	int pai, mae;
	Predador filho;
	vector<int> roleta;
	int idade;

	for(int i=0;i<(int)pop.size();i++){
		if(pop[i].get_vida()>0)
			pop[i].aniversario();

		idade = pop[i].get_idade();
		for(int j=0;j<idade;j++){
			roleta.push_back(i);
		}
	}
	int n = num-pop.size();
	for(int i=0;i<n-(num/5);i++){
		pai = rand()%roleta.size();
		mae = rand()%roleta.size();
		if(pop[roleta[pai]].get_nome()==seguir or pop[roleta[mae]].get_nome()==seguir)
			cout<<seguir<<" acaba de se reproduzir"<<endl;
		filho = pop[roleta[pai]].reproduzir(pop[roleta[mae]]);
		pop.push_back(filho);
	}
	for(int i=0;i<num/5;i++){
		filho = Predador();
		pop.push_back(filho);
	}
}

int brigar(Predador &p1, Predador &p2, int &res, string seguir){

	// p1.print();
	// p2.print();

	if(p1.analisar(p2)==0 and p2.analisar(p1)==0){
		res=0;
		//cout<<"nenhum quis lutar"<<endl;
		return 0;
	}
	if(p1.analisar(p2)==0 and p1.fugir(p2)==1){
		res=0;
		//cout<<"p1 nao quis lutar e fugiu"<<endl;
		if(p1.get_nome()==seguir)
			cout<<seguir<<" fugiu de uma briga"<<endl;
		return 0;
	}
	if(p2.analisar(p1)==0 and p2.fugir(p1)==1){
		res=0;
		//cout<<"p2 nao quis lutar e fugiu"<<endl;
		if(p2.get_nome()==seguir)
			cout<<seguir<<" fugiu de uma briga"<<endl;
		return 0;
	}
	//cout<<"comeca luta"<<endl;
	int a=0;
	while(p1.morreu()==0 and p2.morreu()==0){
		if(p1.get_atr(AGILIDADE) >= p2.get_atr(AGILIDADE)){

			//cout<<"p1 ataca ";
			//cout<<p2.get_vida()<<" ->";
			p1.atacar(p2);
			//cout<<p2.get_vida()<<endl;

			//cout<<"p2 ataca ";
			//cout<<p1.get_vida()<<" ->";
			p2.atacar(p1);
			//cout<<p1.get_vida()<<endl;
		}
		else{
			//cout<<"p2 ataca ";
			//cout<<p1.get_vida()<<" ->";
			p2.atacar(p1);
			//cout<<p1.get_vida()<<endl;
			
			//cout<<"p1 ataca ";
			//cout<<p2.get_vida()<<" ->";
			p1.atacar(p2);
			//cout<<p2.get_vida()<<endl;
		}
		a++;
		if(a>10000){
			cout<<"loop infinito"<<endl;
			return 0;
		}
	}
	p1.recuperar_vida();
	p2.recuperar_vida();
	if(p1.morreu()==0 and p2.morreu()==1){
		p1.comer(p2);
		res=2;
		return 2;
	}
	else if(p1.morreu()==1 and p2.morreu()==0){
		p2.comer(p1);
		res=1;
		return 1;
	}
	res=0;
	return 0;
}

void temporada_de_caca(vector<Predador> &pop, int num, string seguir){
	int p1, p2, perdeu;

	for(int i=0;i<num;i++){
		for(int j=0;j<(int)pop.size();j++){
			
			p1=j;

			do{
				p1 = (p1+1)%pop.size();
			}while(pop[p1].get_vida()<1);

			p2 = rand()%pop.size();
			do{
				p2 = (p2+1)%pop.size();
			}while(p2==p1);

			brigar(pop[p1], pop[p2], perdeu, seguir);
			
			if(perdeu==1){
				if(p1==0){
					cout<<pop[p1].get_nome()<<" (Top1) morreu e esta sendo devorado"<<endl;
				}
				if(pop[p2].get_nome()==seguir)
					cout<<seguir<<" ganhou uma briga"<<endl;
				if(pop[p1].get_nome()==seguir)
					cout<<seguir<<" morreu e esta sendo devorado"<<endl;
				if(pop[p1].get_atr(TAMANHO)<1){
					pop.erase(pop.begin()+p1);
				}
			}
			else if(perdeu==2){
				if(p2==0){
					cout<<pop[p2].get_nome()<<" (Top1) morreu e esta sendo devorado"<<endl;
				}
				if(pop[p1].get_nome()==seguir)
					cout<<seguir<<" ganhou uma briga"<<endl;
				if(pop[p2].get_nome()==seguir)
					cout<<seguir<<" morreu e esta sendo devorado"<<endl;
				if(pop[p2].get_atr(TAMANHO)<1){
					pop.erase(pop.begin()+p2);
				}
			}
					
		}	
	}
}

void mostrar_populacao(vector<Predador> pop, string arquivo){
	if(arquivo==""){
		cout<<pop.size()<<" individuos"<<endl;
		for(int i=0;i<(int)pop.size();i++){
			cout<<"Nome: "<<pop[i].get_nome()<<":"<<endl;
			cout<<"Idade: "<<pop[i].get_idade()<<endl;
			cout<<"Vida: "<<pop[i].get_vida()<<endl;
			cout<<"Tamanho: "<<pop[i].get_atr(TAMANHO)<<endl;
			cout<<"Ataque: "<<pop[i].get_atr(ATAQUE)<<endl;
			cout<<"Defesa: "<<pop[i].get_atr(DEFESA)<<endl;
			cout<<"Inteligencia: "<<pop[i].get_atr(INTELIGENCIA)<<endl;
			cout<<"Agilidade: "<<pop[i].get_atr(AGILIDADE)<<endl;
			cout<<"Fuga: "<<pop[i].get_atr(FUGA)<<endl;
			cout<<"Caça: "<<pop[i].get_atr(CACA)<<endl;
			cout<<endl;
		}
	}
	else{
		ofstream arq(arquivo.c_str());
		arq<<pop.size()<<" individuos"<<endl;
		for(int i=0;i<(int)pop.size();i++){
			arq<<"Nome: "<<pop[i].get_nome()<<":"<<endl;
			arq<<"Idade: "<<pop[i].get_idade()<<endl;
			arq<<"Vida: "<<pop[i].get_vida()<<endl;
			arq<<"Tamanho: "<<pop[i].get_atr(TAMANHO)<<endl;
			arq<<"Ataque: "<<pop[i].get_atr(ATAQUE)<<endl;
			arq<<"Defesa: "<<pop[i].get_atr(DEFESA)<<endl;
			arq<<"Inteligencia: "<<pop[i].get_atr(INTELIGENCIA)<<endl;
			arq<<"Agilidade: "<<pop[i].get_atr(AGILIDADE)<<endl;
			arq<<"Fuga: "<<pop[i].get_atr(FUGA)<<endl;
			arq<<"Caça: "<<pop[i].get_atr(CACA)<<endl;
			arq<<endl;
		}
		arq.close();
	}


	
}

void salvar_populacao(vector<Predador> pop, string arquivo){
	if(arquivo==""){
		arquivo = principal;
	}
	ofstream arq(arquivo.c_str());
	arq<<pop.size()<<endl;
	for(int i=0;i<(int)pop.size();i++){
		arq<<pop[i].get_nome()<<endl;
		arq<<pop[i].get_idade()<<endl;
		arq<<pop[i].get_vida()<<endl;
		for(int j=0;j<N_ATR;j++){
			arq<<pop[i].get_atr(j)<<endl;
		}
	}
	arq.close();
	
}

void recuperar_populacao(vector<Predador> &pop, string arquivo){
	if(arquivo==""){
		arquivo = principal;
	}
	pop.clear();
	ifstream arq(arquivo.c_str());

	int val;
	string nome;
	int num;
	getline(arq, nome);
	
	num = atoi(nome.c_str());

	Predador p;

	for(int i=0;i<num;i++){
		p = Predador();
		getline(arq, nome);
		p.set_nome(nome);

		getline(arq, nome);
		val = atoi(nome.c_str());
		p.set_idade(val);

		getline(arq, nome);
		val = atoi(nome.c_str());
		p.set_vida(val);

		for(int j=0;j<N_ATR;j++){
			getline(arq, nome);
			val = atoi(nome.c_str());
			p.set_atr(j, val);
		}
		pop.push_back(p);
	}
	arq.close();
}

void ciclo_natural(vector<Predador> &pop, int num, int vezes, string seguir){

	for(int i=0;i<vezes;i++){
		cout<<i<<" ";
		temporada_de_caca(pop, 15, seguir);
		cout<<endl;

		limpar_mortos(pop, seguir);
		//cout<<pop.size()<<" sobreviventes"<<endl;
		reproduzir_populacao(pop, num, seguir);	
	}

}

void mudar_numero_populacao(vector<Predador> &pop, int num){
	Predador p;
	while((int)pop.size() > num){
		pop.pop_back();
	}
	while((int)pop.size() < num){
		p = Predador();
		pop.push_back(p);
	}
	salvar_populacao(pop, "");
}

void tela_inicial(){

	vector<Predador> pop;
	recuperar_populacao(pop, "");

	int opcao=0;
	string nome;
	string seguir;
	Predador p;
	do{
		cout<<"1 - 1 ciclo"<<endl;
		cout<<"2 - Varios ciclos"<<endl;
		cout<<"3 - Mostrar populacao"<<endl;
		cout<<"4 - Definir numero da populacao"<<endl;
		cout<<"5 - Inserir novos predadores"<<endl;
		cout<<"6 - Reiniciar populacao"<<endl;
		cout<<"7 - Salvar populacao"<<endl;
		cout<<"8 - Restaurar populacao"<<endl;
		cout<<"9 - Seguir predador"<<endl;
		cout<<"0 - Sair"<<endl;

		opcao = get_int(-1);

		if(opcao==1){
			ciclo_natural(pop, (int)pop.size(), 1, seguir);
			salvar_populacao(pop, "");
		}
		else if(opcao==2){
			cout<<"Quantos ciclos?"<<endl;
			opcao = get_int(-1);
			ciclo_natural(pop, (int)pop.size(), opcao, seguir);
			salvar_populacao(pop, "");
			opcao = 2;
		}
		else if(opcao==3){
			cout<<"Arquivo?"<<endl;
			getline(cin, nome);
			mostrar_populacao(pop, nome);
		}
		else if(opcao==4){
			cout<<"Numero?"<<endl;
			opcao = get_int(-1);
			if(opcao>1){
				mudar_numero_populacao(pop, opcao);
			}
			opcao = 4;
		}
		else if(opcao==5){
			cout<<"1 - Criar predador"<<endl;
			cout<<"2 - Aleatorio"<<endl;
			cout<<"0 - Cancelar"<<endl;

			opcao = get_int(-1);
			if(opcao == 1){
				p.criar();
				cout<<"Quantos?"<<endl;
				opcao = get_int(1);
				for(int i=0;i<opcao;i++)
					pop.push_back(p);
			}
			else if(opcao==2){
				cout<<"Quantos?";
				opcao = get_int(1);
				for(int i=0;i<opcao;i++){
					p = Predador();
					pop.push_back(p);
				}
			}
			salvar_populacao(pop, "");
			opcao = 5;
		}
		else if(opcao==6){
			cout<<"Numero da populacao?"<<endl;
			opcao = get_int(-1);
			if(opcao>0)
				pop = inicializar_populacao(opcao);
			opcao = 6;
		}
		else if(opcao==7){
			cout<<"Arquivo?"<<endl;
			getline(cin, nome);
			salvar_populacao(pop, nome);
		}
		else if(opcao==8){
			cout<<"Arquivo?"<<endl;
			getline(cin, nome);
			recuperar_populacao(pop, nome);
		}
		else if(opcao==9){
			cout<<"Seguir qual predador?"<<endl;
			getline(cin, seguir);
		}
	}while(opcao!=0);
}

int main(){

	srand(time(NULL));

	tela_inicial();

	// int res;
	// string nome;
	// do{
	// 	cout<<string(50, '\n');
	// 	Predador p1 = Predador();
	// 	Predador p2 = Predador();

	// 	p1.print();
	// 	p2.print();
	// 	brigar(p1, p2, res);
	// 	getline(cin, nome);
	// }while(nome=="");


	return 0;
}