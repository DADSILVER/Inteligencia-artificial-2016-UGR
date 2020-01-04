#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

/*
double EMPATA(const Environment & T,int fil, int col, int jugador){
    
    int derecha=1,abajo=1;
    int contador=1;
    int punt=1;
    while(fil+derecha < 7 && (int) T.See_Casilla(fil+derecha, col) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        derecha++;  
    }
    
    contador=1;
    derecha=1;
    
	while(col+abajo < 7 && (int) T.See_Casilla(fil, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        abajo++;
    }
	
	contador=1;
    abajo=1;
     
	while(fil-derecha < -1 && col+abajo < 7 && (int) T.See_Casilla(fil-derecha, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        abajo++;
        derecha++;
    }
	
	contador=1;
	derecha=1;
    abajo=1;
    
    
    while(fil+derecha < 7 && col+abajo < 7 && (int) T.See_Casilla(fil+derecha, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        derecha++;
        abajo++;
    }
    return punt;
}


*/


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta


// ------------------- Los tres metodos anteriores no se pueden modificar

double puntuaPos(const Environment & T,int fil, int col, int jugador){
    
    int derecha=1,abajo=1;
    int contador=1;
    int punt=1;
    while(fil+derecha < 7 && (int) T.See_Casilla(fil+derecha, col) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        derecha++;  
    }
    
    contador=1;
    derecha=1;
    
	while(col+abajo < 7 && (int) T.See_Casilla(fil, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        abajo++;
    }
	
	contador=1;
    abajo=1;
     
	while(fil-derecha < -1 && col+abajo < 7 && (int) T.See_Casilla(fil-derecha, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        abajo++;
        derecha++;
    }
	
	contador=1;
	derecha=1;
    abajo=1;
    
    
    while(fil+derecha < 7 && col+abajo < 7 && (int) T.See_Casilla(fil+derecha, col+abajo) % 3==jugador){
        punt+=contador; 
        contador+=contador*3;
        derecha++;
        abajo++;
    }
    return punt;
}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador,int num_hijos,int prof){
	
	int ganador = estado.RevisarTablero();
	double punt = 0;
	int resta =10;
	if(estado.JuegoTerminado() && 8==estado.Last_Action(jugador))
		num_hijos=0;
			
		resta=resta-num_hijos;
	
    if (ganador==jugador)
       return 9999989.0+resta+prof*2; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
           	return -9999989.0+resta+prof*2; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
           return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else{
    
    	for(int f=0;f<7;f++){
    		for(int c=0;c<7;c++){
    			if((int) estado.See_Casilla(f, c)%3!=jugador)
    				punt+=puntuaPos(estado,f,c,(int) estado.See_Casilla(f, c)%3);
    			else
    				punt=punt - puntuaPos(estado,f,c,(int) estado.See_Casilla(f, c)%3)*3;
    			
    		}
    	}
    	return punt;
    }
}




double Poda_AlfaBeta(const Environment & T , int jugador, int prof, Environment::ActionType & accion, double alpha, double beta,int num_hijos){

	if(T.JuegoTerminado())
		return Valoracion(T,jugador,num_hijos,prof);
	
	if(prof==0){
	
		return Valoracion(T,jugador,num_hijos,prof);
	}
	else{
	
		int ultacc = -1;
		double valor;
		Environment hijo=T.GenerateNextMove(ultacc);
		Environment::ActionType kk;
		bool act[8];
		int num_hijos;
		
		while(ultacc < 8 ){
			valor=Poda_AlfaBeta(hijo,jugador,prof-1,kk,alpha,beta,T.possible_actions(act));
			if(prof%2==0){
				if(valor>alpha){
					alpha=valor;
				 	accion= static_cast< Environment::ActionType > (ultacc);
				 	//num_hijos=hijo.possible_actions(act);
				 	if(alpha>=beta )
						ultacc=8;
					
				}/*else if(valor==alpha && (num_hijos>hijo.possible_actions(act) || hijo.JuegoTerminado())){
					accion= static_cast< Environment::ActionType > (ultacc);
					if(hijo.JuegoTerminado())
						num_hijos=0;
					else
				 		num_hijos=hijo.possible_actions(act);
				}*/
			}
			else{
				if(valor<beta){
					beta=valor;
				 	accion= static_cast< Environment::ActionType > (ultacc);
				 	if(beta<=alpha)
						ultacc=8;
				}
			
			}
				
			hijo=T.GenerateNextMove(ultacc);
		}
			
			if(prof%2==0)
				return alpha;
			else
				return beta;
				
	}

}


// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

/*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui

*/
    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf,0);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}
























