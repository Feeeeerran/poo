#ifndef PANELCLIENTE_H
#define PANELCLIENTE_H

#include "XmlRpc.h"
using namespace XmlRpc;

// Cambiar la devolucion de las funciones


class PanelCliente {
    public:
        string usuario;
        bool conexion = false;
        bool robot;

        bool verificar(XmlRpcClient c);
        void encender(XmlRpcClient c,int opc);

        void manual(XmlRpcClient c,int accion);
        void automatico(XmlRpcClient c);

        void reporte(XmlRpcClient c,int opc);

};

#endif