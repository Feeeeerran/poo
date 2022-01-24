#ifndef PANELCLIENTE_H
#define PANELCLIENTE_H

#include "XmlRpc.h"
using namespace XmlRpc;

// Cambiar la devolucion de las funciones


class PanelCliente {
    public:
        bool conectar(XmlRpc::XmlRpcClient c,int conecta);
        bool encender(XmlRpc::XmlRpcClient c,int opc,bool conexion);
        void manual(XmlRpc::XmlRpcClient c,int accion,bool arranque,bool conecta);
        void automatico(XmlRpc::XmlRpcClient c,bool arranque,bool conecta);
        void reporte(XmlRpc::XmlRpcClient c,int opc);
        bool identificar(XmlRpc::XmlRpcClient c,string Usuario);
        void finalizar(XmlRpc::XmlRpcClient c);
       // int verificar(int num,int max);
        // string finalizar();
};

#endif