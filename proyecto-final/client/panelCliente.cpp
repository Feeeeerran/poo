#include <iostream>
using namespace std;
#include "XmlRpc.h"
using namespace XmlRpc;
#include "panelCliente.h"

bool PanelCliente::identificar(XmlRpc::XmlRpcClient c, string Usuario){
    bool estado;
    XmlRpc::XmlRpcValue arg, result;
    arg=Usuario;
    c.execute("identificar",arg,result);
    cout<<endl;
    cout<<result;
    if (result=="No hay ningun otro usuario controlando el robot."){
        estado= true;
    }else{
        estado= false;
    }
    return estado;
}

bool PanelCliente::conectar(XmlRpc::XmlRpcClient c,int conecta) {
        XmlRpc::XmlRpcValue arg, result;
        arg = conecta;
        c.execute("conectar",arg,result);
        cout << result;
    if (conecta == 1 ) {
        return true;
    } else {
        c.close();
        return false;
    }
}


bool PanelCliente::encender(XmlRpc::XmlRpcClient c,int opc,bool conexion) {
    bool arranque;
    if (conexion){
        XmlRpc::XmlRpcValue arg, result;
        arg = opc;
        c.execute("arrancar",arg,result);
        cout << result;
        arranque=true;
    }else{
        if (opc==1){
            cout<<"No se puede encender porque esta desconectado"<<endl;
            arranque=false;
        }else if (opc==0){
            cout<<"No se puede apagar porque esta desconectado"<<endl;
            arranque=false;
        }
    }  
    return arranque;  
}

void PanelCliente::manual(XmlRpc::XmlRpcClient c,int accion,bool arranque,bool conecta) {
    // Al metodo le entra la accion a realizar, segun el numero es la funcion que llamaremos al servidor
    if (conecta && arranque){
        if (accion == 1) {
        // 1.Mov angular articular
            XmlRpc::XmlRpcValue params, result;
            char mov[20];
            string movi;
            cin.getline(mov,20,'\n');

            // cout << "Para la primer articulacion:" << endl;
            // cout << "V1[mm/s] = ";
            // cin >> mov[0];
            // cout << "S1 [1/0]= ";
            // cin >> mov[1];
            // cout << "q1[º] = ";
            // cin >> mov[2];

            // cout << "Para la segunda articulacion:" << endl;
            // cout << "V2[mm/s] = ";
            // cin >> mov[3];
            // cout << "S2 [1/0]= ";
            // cin >> mov[4];
            // cout << "q2[º] = ";
            // cin >> mov[5];

            // cout << "Para la tercer articulacion:" << endl;
            // cout << "V3[mm/s] = ";
            // cin >> mov[6];
            // cout << "S3 [1/0]= ";
            // cin >> mov[7];
            // cout << "q3[º] = ";
            // cin >> mov[8];
            params=mov;
            cout<<params;
            c.execute("movArticular",params,result);
            cout << result;
            
        } else if (accion == 2) {
        // 2.Mov lineal del efector
            int X,Y,Z,V;
            int movimientos[4];

            XmlRpc::XmlRpcValue result;
            XmlRpc::XmlRpcValue params;

            cout << "Ingrese las coordenadas en milimetros [mm] para mover el efector en el espacio" << endl;
            cout << "X: ";
            cin >> X;
            cout << endl;

            cout << "Y: ";
            cin >> Y;
            cout << endl;

            cout << "Z: ";
            cin >> Z;
            cout << endl;

            cout << "V[mm/s]: ";
            cin >> V;
            cout << endl;

            movimientos[0] = X;
            movimientos[1] = Y;
            movimientos[2] = Z;
            movimientos[3] = V;
            params=movimientos;
            c.execute("movEfector",params,result);
            cout<<result<<endl;

        } else if (accion == 3) {
            // 3.Actividad del efector
            int opc;
            XmlRpc::XmlRpcValue noArgs,arg, result;
            arg=opc;
            c.execute("estadoEfector",arg,result);
            cout << "Estado del efector: " << result;
            cout<<endl;
            cout << "Determine la accion que quiere realizar" << endl;
            cout << "1 --> Abrir la pinza" << endl;
            cout << "0 --> Cerrar la pinza" << endl;
            cin >> opc;


            arg = opc;
            c.execute("efector",arg,result);
            cout << result;

        } else if (accion == 4) {
        // 4.Homing
            XmlRpc::XmlRpcValue arg, result;
            arg=accion;
            c.execute("homing",arg,result);
            cout << result;

        } else if (accion == 5) {
            // 5.Aprendizaje
            XmlRpc::XmlRpcValue arg, result;
            arg=accion;
            c.execute("aprendizaje",arg,result);
            cout << result;
            
        } else {
            cout << "La opcion ingresada no es valida";
        }
    }else if(conecta && arranque==false){
        cout<<"No se puede ejecutar esta accion"<<endl
            <<"El robot se encuentra apagado"<<endl;
    }else if(conecta==false){
        cout<<"No se puede ejecutar esta accion"<<endl
            <<"El robot se encuentra desconectado y apagado"<<endl;
    }
}


void PanelCliente::automatico(XmlRpc::XmlRpcClient c,bool arranque,bool conecta){
    XmlRpc::XmlRpcValue arg, result;
    int largo;
    if (conecta && arranque){
        arg=1;
        cout<<"Modo automatico ACTIVADO"<<endl
            <<endl;
    c.execute("automatico",arg, result);
    largo= result.size();
    cout<<"        EJECUTANDO CODIGO G"<<endl
        <<endl;
    for (int i=0;i<=largo;i++){
        cout<<result[i]<<endl;
    }
    cout<<endl
        <<"        EJECUCION FINALIZADA"<<endl;
    }else if(conecta && arranque==false){
        cout<<"No se puede ejecutar esta accion"<<endl
            <<"El robot se encuentra apagado"<<endl;
    }else if(conecta==false){
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

void PanelCliente::finalizar(XmlRpc::XmlRpcClient c){
    XmlRpc::XmlRpcValue Noargs, result;
    c.execute("finalizar",Noargs, result);
}