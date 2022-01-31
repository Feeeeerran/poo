#  El módulo xmlrpc.server proporciona un marco de servidor básico para servidores XML-RPC escritos en Python

from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler
from panel_control_server import Panel_control_server
import socket
import os

# Restrict to a particular path.
# class RequestHandler(SimpleXMLRPCRequestHandler):
#     rpc_paths = ('/RPC2',)

PanelControl = Panel_control_server()

ip = '127.0.0.1'
port = 8000


# Creamos el servidor
with SimpleXMLRPCServer((ip, port)) as server:
    server.register_introspection_functions()

    # No necesarias
    # server.register_function(PanelControl.conexion_desconexion,'conectar')

    # Funciones basicas
    server.register_function(PanelControl.verificar,'verificar')
    server.register_function(PanelControl.desconectar,'desconectar')
    server.register_function(PanelControl.estado_robot,'estadoRobot')
    server.register_function(PanelControl.do_activar_desactivar,'arrancar')

    
    # Funciones de movimiento del robot
    server.register_function(PanelControl.do_mc_articulaciones,'movArticular')
    server.register_function(PanelControl.do_ml_efector,'movEfector')
    server.register_function(PanelControl.do_actividad_efector,'efector')
    server.register_function(PanelControl.do_estado_efector,'estadoEfector')
    server.register_function(PanelControl.do_homing,'homing')
    server.register_function(PanelControl.do_aprendizaje,'aprendizaje')
    server.register_function(PanelControl.do_automatico,'automatico')
    server.register_function(PanelControl.do_mostrar_reporte,'reporte')
    
    os.system("clear")
    print('\n////////////////////////\n')
    print('Servidor iniciado en')
    print('IP: ',ip)
    print('Puerto: ',port)
    print('\n//////////////////////// \n')

    # Iniciamos al servidor
    server.serve_forever()

