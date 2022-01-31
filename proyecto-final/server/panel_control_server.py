from cmd import Cmd
from io import open
import re
import time
from robot import Robot
import sys

miRobot = Robot()

class Panel_control_server(Cmd):

    def __init__(self, arg = False):
        Cmd.__init__(self)
        self.nombreUsuario = str
        self.serv = arg
        # estadoRobot false = que esta apagado (inicialmente false)
        self.estadoRobot = False
        # estadoServer false = que no hay un usuario usando el robot (inic false)
        self.estadoServer = False
        self.aprendizaje = False


    # Funcion para verificar si ya hay algun usuario
    def verificar(self,usuario):
        if self.estadoServer == False:
            self.estadoServer = True
            self.nombreUsuario = usuario
            print("\nSe conecto {} al servidor".format(usuario))
            return "Bienvenido al control del robot {} \n".format(usuario),"1"
        else:
            print("\nAcceso denegado para {}, {} esta conectado en este momento".format(usuario,self.nombreUsuario))
            return "\nEl usuario {} esta controlando el robot en estos momentos.\nIntente nuevamente mas tarde\n".format(self.nombreUsuario),"0"


    # Le decimos al servidor que el usuario se ha desconectado
    def desconectar(self):
        if self.serv:
            print("Ejecucion finalizada")
            raise SystemExit
        else:
            self.estadoServer = False
            print("\nSe desconecto {} del servidor".format(self.nombreUsuario))
            self.nombreUsuario = ""

    # Retorna estado del robot
    def estado_robot(self):
        if (self.estadoRobot == True):
            return "1"
        else:
            return "0"

    # Enciende o apaga al robot + condicionales
    def do_activar_desactivar(self,arg):
        """Ingresar 1 para encender el robot y 0 para apagarlo"""
        arg = str(arg)
        if arg == "1" and self.estadoRobot == False:
            sms1 = miRobot.verificar(4,arg)
            self.estadoRobot = True            
            sms2 = "\nRobot ABB IRB 460 ENCENDIDO"
            self.do_llenar_reporte(sms2 + "\n" + sms1)
            # sms3 = miRobot.ejectutar_orden(sms1)
            sms3 = " "
            self.estadoServer=True
            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms1)
            if self.serv:
                print (sms3+'\n'+sms2)
            return sms2,"1"

        elif arg == '1' and self.estadoRobot == True:
            if self.serv:
                print ("El robot ya esta encendido")
            else:
                return "\nEl robot ya esta encendido","1"

        elif arg == '0' and self.estadoRobot == False:
            if self.serv:
                print ("El robot ya esta apagado")
            else:
                return "\nEl robot ya esta apagado","0"

        elif arg == '0' and self.estadoRobot == True:
            sms1 = miRobot.verificar(4,arg)
            self.estadoRobot=False 
            sms2 = "\nRobot ABB IRB 460 APAGADO"
            self.do_llenar_reporte(sms2 + "\n" + sms1)
            # sms3 = miRobot.ejectutar_orden(sms1)
            sms3 = " "
            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms1)

            if self.serv:
                print (sms3+'\n'+sms2)
            else:
                return sms2, "0"
        

    # Movimiento de los distintos grados de libertad
    def do_mc_articulaciones(self,args):
        """Movimiento circular: ingrese velocidad, sentido y angulo de las 3 articulaciones en el siguiente formato: V1,S1,q1,V2,S2,q2,V3,S3,q3"""

        args=args.split()
        sms1 = miRobot.verificar(1,args[0],args[3],args[6],args[1],args[4],args[7],args[2],args[5],args[8])

        if  sms1 == "Error1":
            if self.serv:
                print ("Velocidad angular fuera de rango, velocidad maxima: ", miRobot.VA_max)
            else:
                return "Velocidad angular fuera de rango, velocidad maxima: ", miRobot.VA_max
        elif  sms1 == "Error2":
            if self.serv:
                print ("Limites articulares excedidos, angulo maximo ", miRobot.LA)
            else:
                return "Limites articulares excedidos, angulo maximo ", miRobot.LA
        else:
            self.do_llenar_reporte(sms1)
            # sms2 = miRobot.ejectutar_orden(sms1)
            sms2 = "Orden ejecutada sin problemas"
            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms1)

            if self.serv:
                print (sms2)
            else:
                return sms2


    # Movimientos lineales del efector
    def do_ml_efector(self,args):
        """Movimiento lineal: ingrese coordenadas del efector final y velocidad en el siguiente formato: X,Y,Z,V"""

        print(args)

        args=args.split()
        sms1 = miRobot.verificar(2,int(args[0]),int(args[1]),int(args[2]),int(args[3]))
        
        if  sms1 == "Error1":
            if self.serv:
                print ("Velocidad fuera de rango, velocidad maxima ", miRobot.VL_max)
            else:
                return "Velocidad fuera de rango, velocidad maxima ", miRobot.VL_max
        elif  sms1 == "Error2":
            if self.serv:
                print ("Punto final fuera del espacio de trabajo, Limites maximos:""\n x: 0-",miRobot.dim_anchura,"\n y: 0-",miRobot.dim_profundidad,"\n z: 0-",miRobot.dim_altura)
            else:
                return "Punto final fuera del espacio de trabajo, Limites maximos:""\n x: 0-",miRobot.dim_anchura,"\n y: 0-",miRobot.dim_profundidad,"\n z: 0-",miRobot.dim_altura
        else:
            self.do_llenar_reporte(sms1)
            # sms2 = miRobot.ejectutar_orden(sms1)
            sms2 = "Orden ejecutada sin problemas"

            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms1)

            if self.serv:
                print (sms2)
            else:
                return sms2


    # Devuelve el estado del efector
    def do_estado_efector(self,opc):

        if miRobot.estadoEfector:
            if self.serv:
                print ('La pinza se encuentra abierta')
            else:
                return "La pinza se encuentra abierta"
        else:
            if self.serv:
                print ('La pinza se encuentra cerrada') 
            else: 
                return "La pinza se encuentra cerrada"


    # Para abrir o cerrar el efector
    def do_actividad_efector(self,arg):
        """Actividad efector final: Ingrese 1 para abrir la pinza y 0 para cerrarla"""

        arg=str(arg)
        if arg == '1' and miRobot.estadoEfector == False:
            sms = miRobot.verificar(3,arg)
            miRobot.estadoEfector = True
            self.do_llenar_reporte('Abriendo pinza\n' + sms)
            # sms2 = miRobot.ejectutar_orden(sms)
            sms2 = ""
            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms)

            if self.serv:
                print (sms2 + '\nAbriendo pinza') 
            else: 
                return 'Abriendo pinza'

        elif arg == '1' and miRobot.estadoEfector == True:
            if self.serv:
                print ('La pinza ya esta abierta')
            else:
                return 'La pinza ya esta abierta'

        elif arg == '0' and miRobot.estadoEfector == False:
            if self.serv:
                print ('La pinza ya esta cerrada')
            else:
                return 'La pinza ya esta cerrada'

        elif arg == '0' and miRobot.estadoEfector == True:
            sms = miRobot.verificar(3,arg)
            miRobot.estadoEfector=False 
            self.do_llenar_reporte('Cerrando pinza\n' + sms)
            # sms2 = miRobot.ejectutar_orden(sms)
            sms2 = ""
            if self.aprendizaje:
                self.do_llenar_aprendizaje(sms)

            if self.serv:
                print (sms2 + '\nCerrando pinza')
            else:
                return 'Cerrando pinza'


    # A posicion de homing
    def do_homing(self,opc):
        """Lleva al robot a su posicion de offset"""

        sms ="{ G01 X0000 Y0000 Z3690 F"+str(miRobot.VL_max/2)+" } "
        self.do_llenar_reporte(sms)

        if self.aprendizaje:
            self.do_llenar_aprendizaje(sms)

        if self.serv:
            print ("Posicion de origen "+sms+" alcanzada") 
        else: 
            return "Posicion de origen "+sms+" alcanzada"


    def do_aprendizaje(self,opc):
        """Activa/Desactiva el modo aprendizaje, para grabar los movimientos realizados"""
        
        if self.aprendizaje == False:
            self.aprendizaje = True
            ap = open('aprendizaje.txt','w')
            ap.write('<<ModoAprendizaje>>\n')     
            ap.close()

            self.do_llenar_reporte("Modo aprendizaje ACTIVADO")

            if self.serv:
                print ("Modo aprendizaje activado") 
            else: 
                return("Modo aprendizaje Activado")

        elif self.aprendizaje == True:
            ap = open('aprendizaje.txt','a')
            ap.write('<<FIN>>\n')      
            ap.close()
            self.aprendizaje=False 

            self.do_llenar_reporte("Modo aprendizaje DESACTIVADO")

            if self.serv:
                print ("Modo aprendizaje desactivado") 
            else: 
                return("Modo aprendizaje Desactivado")


    def do_automatico(self,opc):
        """Ejecuta movimientos predeterminados"""

        archivo = open("mod_automatic.txt")
        lineas = archivo.readlines()
        archivo.close()
        j=0
        c=1
        lista=[]

        for i in lineas:        
            if (c % 2==0):
                j=j+1
                large_str  = lineas[c] 
                p = large_str.find('G0')
                G = int(large_str[p+2])
                p = large_str.find('X')
                x = int(large_str[p+1:p+5])
                p = large_str.find('Y')
                y = int(large_str[p+1:p+5])
                p = large_str.find('Z')
                z = int(large_str[p+1:p+5])
                p = large_str.find('F')
                f = int(large_str[p+1:p+5])
                sms =" 	<<{ G0"+str(G)+" X"+str(x)+" Y"+str(y)+" Z"+str(z)+" F"+str(f)+" }>>"
                lista.append(sms)
                if self.serv:
                    print(sms)

            if c<=len(lineas[c]):
                c=c+1      

        self.do_llenar_reporte("Modo automatico ejecutado")

        if self.serv:
            print ("Trayectoria automática realizada. Se realizaron "+str(j)+" movimientos") 
        else: 
            return lista  


    def do_mostrar_reporte(self,args):
        """Muestra en consola un reporte de toda la actividad desde que encendio"""

        with open("Reporte.txt") as archivo:
            a = archivo.readlines()
            if self.serv:
                for i in a:
                    print (i) 
            else: 
                return(a)


    def do_llenar_reporte(self,orden):
        operacion='a'
        if self.estadoServer==False:
            operacion = 'w'

        reporte = open('Reporte.txt',operacion)
        reporte.write(orden+'\n')
        reporte.write("     Fecha: ")
        reporte.write(time.strftime("%d/%m/%y "))
        reporte.write("Hora: ")
        reporte.write(time.strftime("%H:%M:%S\n\n"))
        reporte.close()


    def do_llenar_aprendizaje(self,orden):
        if self.aprendizaje:
            ap = open('aprendizaje.txt','a')    
            ap.write('<<mov>>\n    <<'+orden+'>>\n') 
            ap.close() 


if __name__=="__main__":
    interprete=Panel_control_server(True)
    interprete.prompt='\nPor favor operario, ingrese una orden. Para conocer las funciones ingrese -help-: \n '
    interprete.doc_header = "Comandos con ayuda disponible"
    interprete.undoc_header = "Otros comandos"
    interprete.cmdloop('Servidor iniciado')