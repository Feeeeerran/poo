#include <iostream>
#include <stdlib.h>
using namespace std;

// Clase PanelCliente
#include "panelCliente.h"

// Incluyendo la libreria para xmlrpc
#include "XmlRpc.h"
using namespace XmlRpc;

// Declaracion de la funcion para verificar lo ingresado por el usuario
int verificarOpc(int num,int max);

int main() {
    system("clear");
    int opc = 0;
    string estadoRobot, estadoServidor;
    // Para entrar en bucle al menu
    int port = 8000;
    bool flag = false;


    // Instanciamos un objeto mainPanel de la clase panelCliente
    PanelCliente panel;

    cout << "Hola usuario!" << endl;
    cout << "Por favor, ingrese su nombre" << endl;
    cout << ">> ";
    getline(cin,panel.usuario);

    system("clear");
    cout << " >>>>>>>>>>> Bienvenido "<< panel.usuario << " <<<<<<<<<<<<<" << endl;
    cout << endl;



    XmlRpcClient client("127.0.0.1", port);
    

    // En caso de que el servidor este ocupado con otro usuario debemos pedirle al usuario que espere
    panel.conexion = panel.verificar(client);

    // Una vez se accede al panel, se ubica el estado del robot
    XmlRpcValue arg,result;
    client.execute("estadoRobot",arg,result);
    if (result == "1") {
        // El robot esta encendido
        panel.robot = true;
    } else {
        // El robot esta apagado
        panel.robot = false;
    }




    // Loop para el menu
    while (flag == false){
        // Para dejar los estados en el panel
        if (panel.robot == true) {
            estadoRobot = "encendido";
        } else {
            estadoRobot = "apagado  ";
        }

        if (panel.conexion == true) {
            estadoServidor = "conectado   ";
        } else {
            estadoServidor = "desconectado";
        }

        
        cout << endl << endl << endl;
        cout<<"|============================================================|"<<endl
            <<"|                        MENU PRINCIPAL                      |"<<endl
            <<"|                                                            |"<<endl
            <<"| 1--> Conectar / Desconectar   SERVIDOR                     |"<<endl
            <<"| 2--> Encender / Apagar        ROBOT                        |"<<endl
            <<"| 3--> Modo manual                                           |"<<endl
            <<"| 4--> Modo automatico                                       |"<<endl
            <<"| 5--> Reporte                                               |"<<endl
            <<"| 0--> Finalizar                                             |"<<endl
            <<"|                                                            |"<<endl
            <<"| Estado del robot: "<<estadoRobot<<"                                |"<<endl
            <<"| Estado de conexion: "<<estadoServidor<<"                           |"<<endl
            <<"|============================================================|"<<endl;

        // Tomamos la opcion del usuario
        cout << "\nIngrese su opcion por favor" << endl;
        cout << "   >> ";
        cin >> opc;
        cout << endl;

        // Funcion que verifica que el numero ingresado por el usuario este dentro de las opciones
        opc = verificarOpc(opc,5);
        system("clear");

        // Entramos al menu y las opciones las validamos con un switch
        switch (opc) {
            case 1:
                cout << "Usted ha elegido conectar/desconectar con el servidor" << endl << endl;
                cout << "1 --> Conectar" << endl;
                cout << "0 --> Desconectar" << endl;
                cin >> opc;
                opc = verificarOpc(opc,1);

                if (opc == 1 && panel.conexion == false) {
                    XmlRpcClient client("127.0.0.1",port);
                    panel.conexion = panel.verificar(client);
                } else if (opc == 1 && panel.conexion == true) {
                    cout << "Ya se encuentra conectado al servidor" << endl;
                } else if (opc == 0 && panel.conexion == false) {
                    cout << "Ya se encuentra desconectado del servidor" << endl;
                } else {
                    XmlRpcValue arg, result;
                    client.execute("desconectar",arg,result);
                    client.close();
                    panel.conexion = false;
                    cout << "Se ha desconectado del servidor de forma exitosa";
                }
            break;

            case 2:
                cout << "Usted ha elegido encender/apagar el robot" << endl << endl;
                cout << "1 --> Encender el robot" << endl;
                cout << "0 --> Apagar el robot" << endl;
                cout << "   >> ";
                cin >> opc;
                opc = verificarOpc(opc,1);
                panel.encender(client,opc);
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

                cout << "\nIngrese su opcion del modo manual por favor" << endl;
                cout << "   >> ";
                cin >> manual;
                cout << endl;

                // Verificamos lo ingresado
                opc = verificarOpc(opc,5);
                system("clear");
                // Como al metodo manual de la clase panel de control le entra la accion a realizar, directamente llamamos a la misma
                if (manual != 0) {
                    panel.manual(client,manual);
                }
            break;


            // Modo automatico
            case 4:
                // panel.automatico(client,arranque,conexion);
            break;


            // Reporte
            case 5:
                cout << endl;
                cout << endl;
                cout<< "           REPORTE"<<endl;
                cout << endl;
                // panel.reporte(client,opc);
            break;


            // Finalizar
            case 0:
                system("clear");
                cout << endl;
                cout << endl;
                cout << "       |==================================================|"<<endl;
                cout << "       |                PROGRAMA FINALIZADO               |"<<endl;
                cout<<  "       |==================================================|"<<endl;
                cout << endl;
                cout << endl;
            break;


            // Opcion incorrecta (?
            default:
                cout << endl;
                cout << "La opcion ingresada no es valida, intente nuevamente" << endl;
                cout << endl;
            break;
        }

        if (opc == 0) {
            XmlRpcValue arg, result;
            flag = true;
            client.execute("desconectar",arg,result);
            client.close();
            cout << flag;
        }
    }
    return 0;
}


int verificarOpc(int num,int max) {
    // entra el num del usuario y entra el max
    // Verificar si el num > max lo metemos dentro de un bucle que no sale hasta colocar la resp correcta

    while (num > max) {
        cout << "Por favor ingrese una opcion valida :)" << endl;
        cin >> num;
    }
    return num;
}