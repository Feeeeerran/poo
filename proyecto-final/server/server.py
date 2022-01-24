#  El módulo xmlrpc.server proporciona un marco de servidor básico para servidores XML-RPC escritos en Python

from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler
from panel_control_server import panel_control_server
import socket

# Restrict to a particular path.
# class RequestHandler(SimpleXMLRPCRequestHandler):
#     rpc_paths = ('/RPC2',)

panelControl = panel_control_server()


# Creamos el servidor
with SimpleXMLRPCServer(('localhost', 8000)) as server:
    server.register_introspection_functions()


    # Funciones del panel
    server.register_function(panelControl.conexion_desconexion,'conectar')
    server.register_function(panelControl.do_activar_desactivar,'arrancar')
    server.register_function(panelControl.do_fin,'finalizar')
    
    # Funciones de movimiento
    server.register_function(panelControl.do_mc_articulaciones,'movArticular')
    server.register_function(panelControl.do_ml_efector,'movEfector')
    server.register_function(panelControl.do_estado_efector,'estadoEfector')
    server.register_function(panelControl.do_actividad_efector,'efector')
    server.register_function(panelControl.do_homing,'homing')
    server.register_function(panelControl.do_aprendizaje,'aprendizaje')
    server.register_function(panelControl.do_automatico,'automatico')
    server.register_function(panelControl.do_mostrar_reporte,'reporte')
    server.register_function(panelControl.identificar,'identificar')
    
    server.serve_forever()

