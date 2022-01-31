#include <iostream>
using namespace std;
#include "XmlRpc.h"
using namespace XmlRpc;
#include "panelCliente.h"


//  Funcion para verificar si hay un usuario usando el robot
// La misma obtiene datos del lado del servidor como el nombre del usuario que esta usando el robot
bool PanelCliente::verificar(XmlRpcClient c){
    system("clear");
    XmlRpcValue arg, result;
    c.execute("verificar",usuario,result);
    cout << result[0];
    while (result[1] != "1") {
        cout << endl;
        cout << "Presione enter para intentar nuevamente" << endl;
        cin.ignore();
        system("clear");

        c.execute("verificar",usuario,result);
        cout << result[0];
    }

    return true;
}


// opc = 1 entonces encender
// opc = 0 entonces apagar
void PanelCliente::encender(XmlRpcClient c,int opc) {
    XmlRpcValue arg, result;
    c.execute("arrancar",opc,result);
    cout << result[0];

    if (result[1] == "1") {
        robot = true;
    } else {
        robot = false;
    }
}

void PanelCliente::manual(XmlRpcClient c,int accion) {
    // Al metodo le entra la accion a realizar, segun el numero es la funcion que llamaremos al servidor
    if (conexion && robot){
        if (accion == 1) {
        // 1.Mov angular articular
            XmlRpcValue params ,result;
            string mov[9];

            cout << "Para la primer articulacion:" << endl;
            cout << "V1[mm/s] = ";
            cin >> mov[0];
            cout << "S1 [1/0]= ";
            cin >> mov[1];
            cout << "q1[º] = ";
            cin >> mov[2];
            cout << endl;

            cout << "Para la segunda articulacion:" << endl;
            cout << "V2[mm/s] = ";
            cin >> mov[3];
            cout << "S2 [1/0]= ";
            cin >> mov[4];
            cout << "q2[º] = ";
            cin >> mov[5];
            cout << endl;

            cout << "Para la tercer articulacion:" << endl;
            cout << "V3[mm/s] = ";
            cin >> mov[6];
            cout << "S3 [1/0]= ";
            cin >> mov[7];
            cout << "q3[º] = ";
            cin >> mov[8];
            cout << endl;

            // Para poder enviar el todo junto tengo que concatenar todo los strings
            string all = mov[0];
            for (int i = 1; i < 9; i++) {
                all = all + " " +mov[i];
            }

            // params = mov[0];
            c.execute("movArticular",all,result);
            cout << result;
            
        } else if (accion == 2) {
        // 2.Mov lineal del efector
            string mov[4];

            XmlRpc::XmlRpcValue result;
            XmlRpc::XmlRpcValue params;

            cout << "Ingrese las coordenadas en milimetros [mm] para mover el efector en el espacio" << endl << endl;
            cout << " Coordenada X: ";
            cin >> mov[0];
            cout << endl;

            cout << " Coordenada Y: ";
            cin >> mov[1];
            cout << endl;

            cout << " Coordenada Z: ";
            cin >> mov[2];
            cout << endl;

            cout << " Velocidad [mm/s]: ";
            cin >> mov[3];
            cout << endl;

            string all = mov[0];
            for (int i = 1; i < 4; i++) {
                all = all + " " +mov[i];
            }

            c.execute("movEfector",all,result);
            cout << result << endl;

        } else if (accion == 3) {
            // 3.Actividad del efector
            system("clear");
            int opc;
            XmlRpc::XmlRpcValue noArgs,arg, result;
            arg=opc;
            c.execute("estadoEfector",arg,result);
            cout << "Estado del efector: " << result << endl;
            cout<<endl;
            cout << "Determine la accion que quiere realizar" << endl;
            cout << "1 --> Abrir la pinza" << endl;
            cout << "0 --> Cerrar la pinza" << endl;
            cout << "   >> ";
            cin >> opc;


            arg = opc;
            c.execute("efector",arg,result);
            cout << endl << result;

        } else if (accion == 4) {
        // 4.Homing
            XmlRpc::XmlRpcValue arg, result;
            c.execute("homing",accion,result);
            cout << result;

        } else if (accion == 5) {
            // 5.Aprendizaje
            XmlRpc::XmlRpcValue arg, result;
            c.execute("aprendizaje",accion,result);
            cout << result;
            
        } else {
            cout << "La opcion ingresada no es valida";
        }
    } else if (conexion && ~robot){
        cout << "No se puede ejecutar esta accion" << endl
            << "El robot se encuentra apagado" << endl;
    } else if (~conexion){
        cout << "No se puede ejecutar esta accion" << endl
            << "El robot se encuentra desconectado y apagado" << endl;
    }
}


void PanelCliente::automatico(XmlRpcClient c){
    XmlRpc::XmlRpcValue arg, result;
    int largo;
    if (conexion && robot){
        arg=1;
        cout<<"Modo automatico ACTIVADO" << endl <<endl;
    c.execute("automatico",arg, result);
    largo= result.size();
    cout<<"        EJECUTANDO CODIGO G"<<endl
        <<endl;
    for (int i=0;i<=largo;i++){
        cout<<result[i]<<endl;
    }
    cout<<endl
        <<"        EJECUCION FINALIZADA"<<endl;
    }else if(conexion && ~robot) {
        cout<<"No se puede ejecutar esta accion"<<endl
            <<"El robot se encuentra apagado"<<endl;
    }else if(~conexion){
        cout<<"No se puede ejecutar esta accion"<<endl
            <<"El robot se encuentra desconectado y apagado"<<endl;
    }

}

void PanelCliente::reporte(XmlRpc::XmlRpcClient c,int opc){
    int largo;
    XmlRpc::XmlRpcValue arg, result;
    arg=opc;
    c.execute("reporte",arg,result);
    largo= result.size();
    for (int i=0;i<=largo;i++){
        cout<<result[i]<<endl;
    }
}

