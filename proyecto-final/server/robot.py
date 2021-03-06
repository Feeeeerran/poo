import sys
from excepciones import VA_lim
from excepciones import VL_lim
from excepciones import PA_lim
from excepciones import PL_lim
import math
import serial, time

class Robot:

    def __init__(self, d_an=1685, d_p=1685, d_al=2310, vl=5000, va=60):     #vl[mm/s]
        self.tipo = "3DF"
        self.configuracion = "ABB IRB 640"
        self.efector_final = "Pinza"
        self.dim_anchura = d_an
        self.dim_profundidad = d_p
        self.dim_altura = d_al
        self.eslabones = [900, 950, 1840]
        self.VL_max = vl
        self.VA_max = va
        self.LA = 180
        self.estadoEfector = False


    def ejectutar_orden(codigoG,opc):
        arduino = serial.Serial("COM5", 9600)
        time.sleep(2)
        s = str(codigoG)
        arduino.write(s.encode())

        rta = arduino.readline()
        a = rta.splitlines()
        b=str(a[0])
        c=b.replace("b","")
        d=c.replace("'","")
        arduino.close()

        return d


    def generar_codigoG(self,cond,x=0,y=0,z=0,f=0):
        if cond==1:
            return "{ G01 X" + str(x) + " Y" + str(y) + " Z" + str(z) + " F" + str(f) + " }"
        elif cond==2:
            return "{ G29 }"            #G29 abre la pinza
        elif cond==3:
            return "{ G30 }"            #G30 cierra la pinza 
        elif cond==4:
            return "{ M17 }"            #M17 habilita los motores
        elif cond==5:
            return "{ M18 }"            #M18 deshabilita los motores


    def verificar(self,m,v1,v2=0,v3=0,v4=0,v5=0,v6=0,v7=0,v8=0,v9=0):
        
        
        v2 = int(v2)
        v3 = int(v3)
        v4 = bool(int(v4))
        v5 = bool(int(v5))
        v6 = bool(int(v6))
        v7 = int(v7)
        v8 = int(v8)
        v9 = int(v9)

        # mc articulaciones
        if m==1:
            v1 = int(v1)
            try:
                if (v1>self.VA_max)|(v2>self.VA_max)|(v3>self.VA_max):
                    raise VA_lim
                elif (v7>self.LA)|(v8>self.LA)|(v9>self.LA):
                    raise PA_lim

            except VA_lim:
                return 'Error1'

            except PA_lim:
                return 'Error2'

            else:
                x = (self.eslabones[1]*math.cos(v8)+self.eslabones[2]*math.cos(v9+v8))*math.cos(v7)
                y = (self.eslabones[1]*math.cos(v8)+self.eslabones[2]*math.cos(v9+v8))*math.sin(v7)
                z = self.eslabones[0]+self.eslabones[1]*math.sin(v8)+self.eslabones[2]*math.sin(v8+v9)
                return self.generar_codigoG(1,x,y,z,v3)

        # ml efector
        elif m==2:
            v1 = int(v1)
            try:
                if (v4>self.VL_max):
                    raise VL_lim
                elif (v1>self.dim_anchura)|(v2>self.dim_profundidad)|(v3>self.dim_altura):   
                    raise PL_lim

            except VL_lim:
                return 'Error1'

            except PL_lim:
                return 'Error2'

            else:
                return self.generar_codigoG(1,v1,v2,v3,v4)

        #actividad_efector
        elif m==3:          
            if v1=='1' and self.estadoEfector == False:
                self.estadoEfector = True
                return self.generar_codigoG(2)
            elif v1=='0' and self.estadoEfector == True:
                self.estadoEfector = False
                return self.generar_codigoG(3)
        # encender motores
        elif m==4:
            if v1=='1':
                return self.generar_codigoG(4)            
            elif v1=='0':
                return self.generar_codigoG(5)            