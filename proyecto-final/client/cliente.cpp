#include <iostream>
#include <stdlib.h>
using namespace std;

// Clase PanelCliente
#include "panelCliente.h"

// Incluyendo la libreria para xmlrpc
#include "XmlRpc.h"
using namespace XmlRpc;

// Funcion para verificar lo ingresado por el usuario
int verificar(int num,int max);

int main() {
    system("clear");
    int opc=0;
    bool i=true;
    bool estado,conexion,arranque;
    char Usuario[20];
    // Para entrar en bucle al menu
    bool flag=true;
    // Conectamos al servidor pero deberiamos tener una fucnion que conecte --> A lo sumo que sea una propiedad de la clase panelCliente generar dicha conexion
    int port = 8000;
    XmlRpcClient client("127.0.0.1", port);
    

    // Instanciamos un objeto mainPanel de la clase panelCliente
    PanelCliente panel;

    cout<<"Hola usuario. Por favor identifiquese a contuacion:"<<endl;
    cout<<">>";
    cin.getline(Usuario,20,'\n');
    // Loop para el menu
    while(flag){

        cout << endl;
        cout<<"|============================================================|"<<endl
            <<"|                        MENU PRINCIPAL                      |"<<endl
            <<"|                                                            |"<<endl
            <<"| 1--> Conectar / Desconectar   SERVIDOR                     |"<<endl
            <<"| 2--> Encender / Apagar        ROBOT                        |"<<endl
            <<"| 3--> Modo manual                                           |"<<endl
            <<"| 4--> Modo automatico                                       |"<<endl
            <<"| 5--> Reporte                                               |"<<endl
            <<"| 0--> Finalizar                                             |"<<endl
            <<"|============================================================|"<<endl;

        // Tomamos la opcion del usuario
        cout << "Ingrese su opcion por favor" << endl;
        cout << "   >> ";
        cin >> opc;
        cout << endl;

        // Funcion que verifica que el numero ingresado por el usuario este dentro de las opciones
        verificar(opc,5);
        system("clear");

        // Entramos al menu y las opciones las validamos con un switch
        switch (opc){
            case 1:
                cout << "Usted ha elegido conectar/desconectar con el servidor" << endl << endl;
                cout << "1 --> Conectar" << endl;
                cout << "0 --> Desconectar" << endl;
                cin >> opc;
                verificar(opc,1);
                conexion=panel.conectar(client,opc);
                 if (i){
                     estado=panel.identificar(client,Usuario);
                     i=false;
                    if (estado==false){
                        flag= false;
                    }
                 }
            break;

            case 2:
                cout << "Usted ha elegido encender/apagar el robot" << endl << endl;
                cout << "1 --> Encender el robot" << endl;
                cout << "0 --> Apagar el robot" << endl;
                cin >> opc;
                arranque=panel.encender(client,opc,conexion);
            break;

            case 3:
                int manual;
                cout<<"|=========================================================|"<<endl
                    <<"|                         MODO MANUAL                     |"<<endl
                    <<"|                                                         |"<<endl
                    <<"| 1 --> Movimiento angular articular                      |"<<endl
                    <<"| 2 --> Movimiento lineal del efector                     |"<<endl
                    <<"| 3 --> Actividad del efector                             |"<<endl
                    <<"| 4 --> Homing                                            |"<<endl
                    <<"| 5 --> Aprendizaje                                       |"<<endl
                    <<"| 0 --> Salir del modo                                    |"<<endl
                    <<"|=========================================================|"<<endl;

                cout << "Ingrese su opcion del modo manual por favor" << endl;
                cout << "   >> ";
                cin >> manual;
                cout << endl;

                // Verificamos lo ingresado
                verificar(opc,5);

                // Como al metodo manual de la clase panel de control le entra la accion a realizar, directamente llamamos a la misma
                if (manual !=0) {
                    panel.manual(client,manual,conexion,arranque);
                }

            break;


            // Modo automatico
            case 4:
                panel.automatico(client,arranque,conexion);
            break;


            // Reporte
            case 5:
                cout << endl;
                cout << endl;
                cout<< "           REPORTE"<<endl;
                cout << endl;
                panel.reporte(client,opc);
            break;


            // Finalizar
            case 0:
                system("clear");
                cout << endl;
                cout << endl;
                cout << "          |==================================================|"<<endl;
                cout << "          |                PROGRAMA FINALIZADO               |" << endl;
                cout<<  "          |==================================================|"<<endl;
                cout << endl;
                cout << endl;
                panel.finalizar(client);
                flag=false;
            break;


            // Opcion incorrecta (?
            default:
            break;
        }
    }      
return 0;
}


int verificar(int num,int max) {
    // entra el num del usuario y entra el max
    // Verificar si el num > max lo metemos dentro de un bucle que no sale hasta colocar la resp correcta

    while (num > max) {
        cout << "Por favor ingrese una opcion valida :)" << endl;
        cin >> num;
    }
    return num;
}