/*  
 *   MSL 2024
 *   By Mauro
 */

/* Librerías */
#include <Servo.h>
//#include <NewPing.h>

/* Variables */
// Guarda información acerca de un objecto detectado por el sensor ultrasonico
struct OBJETO
{
  int detectado;
  int distancia; // En cm
};

// Acciones que debe tomar luego de revisar los infrarojos
enum IFMOVER
{
  DERECHA = 3,
  IZQUIERDA,
  DETENER,
  ADELANTE
};

// *** Control de los motores DC ***

#define M_IN1 11  // Motor Derecho
#define M_IN2 12
#define M_IN3 13 // Motor Izquierdo
#define M_IN4 8
#define V_MD 9
#define V_MI 10

// *** Sensores ópticos ***
#define OS_LEFT 3    // Sensor izquierdo
#define OS_RIGHT 2 // Sensor derecho

// ** Servo motor de la plataforma ***
#define SERVO_P 7

// ** Ultrasonico  frontal **/
#define US_ECHO 8
#define US_TRIG 9
#define DISTANCIA_MAX 50 // Hasta 1 metro

Servo Plataforma;
//NewPing Ultrasonico(US_TRIG, US_ECHO,DISTANCIA_MAX);
 
/* Funciones */
void MoverAdelante()
{
  digitalWrite(M_IN1, HIGH); // Adelante
  digitalWrite(M_IN2, LOW);
  digitalWrite(M_IN3, HIGH); // Adelante
  digitalWrite(M_IN4, LOW);
  Serial.println("[MOVER ADELANTE]");
}

void MoverAtras()
{
  digitalWrite(M_IN1, LOW); // Atras
  digitalWrite(M_IN2, HIGH);
  digitalWrite(M_IN3, LOW); // Atras
  digitalWrite(M_IN4, HIGH);
  Serial.println("[MOVER ATRAS]");
}

void MoverIzquierda()
{
  digitalWrite(M_IN1, LOW); // Iz atras
  digitalWrite(M_IN2, HIGH);
  digitalWrite(M_IN3, HIGH); // Adelante
  digitalWrite(M_IN4, LOW);
  Serial.println("[MOVER DERECHA]");
}

void MoverDerecha()
{
  digitalWrite(M_IN1, HIGH); // Iz adelante
  digitalWrite(M_IN2,LOW);
  digitalWrite(M_IN3, LOW); // Atras
  digitalWrite(M_IN4, HIGH);
  Serial.println("[MOVER IZQUIERDA]");
}

void Detener()
{
  digitalWrite(M_IN1, LOW); // Detenido
  digitalWrite(M_IN2,LOW);
  digitalWrite(M_IN3, LOW); // Detenido
  digitalWrite(M_IN4, LOW);
  Serial.println("[MOVER DETENIDO]");
}

void SubirPlataforma()
{
  Plataforma.write(180);
  Serial.println("[SUBIR PLATAFORMA]");
}

void BajarPlataforma()
{
  Plataforma.write(0);
  Serial.println("[BAJAR PLATAFORMA]");
}

IFMOVER LeerInfrarojos()
{
  // Leemos los valores del sensores infrarojos
  int izquierdo = 0;
  int derecho = 0;

  izquierdo = digitalRead(OS_LEFT);
  derecho = digitalRead(OS_RIGHT);

  // Procesamos 
  if(izquierdo == HIGH && derecho == HIGH)
  {
    // Si ambos estan sobre la linea negra
    Serial.println("[LEER INFRAROJOS] -> DETENIDO");
    return DETENER;
  }else if(izquierdo == LOW && derecho == LOW)
  {
    // Ambos sobre linea blanca adelante
    Serial.println("[LEER INFRAROJOS] -> ADELANTE");
    return ADELANTE;
  }
  else if(izquierdo == LOW && derecho == HIGH)
  {
    // Izquierdo en la linea
    Serial.println("[LEER INFRAROJOS] -> DERECHA");
    return DERECHA;
  }
  else if(izquierdo == HIGH && derecho == LOW)
  {
    // Derecho en la linea
    Serial.println("[LEER INFRAROJOS] -> IZQUIERDA");
    return IZQUIERDA;
  }
  Serial.println("[LeerInfrarojos] Detenido por falla en las condicionales");
  return DETENER;
}

/*OBJETO LeerUltrasonico()
{
  
  OBJETO obj = {LOW, LOW}; // Nuestro objeto
  int dist = 0;
  dist = Ultrasonico.ping_cm();
  delay(50);
  if(dist == 0)
  {
    return obj;
  }
  else
  {
    obj.detectado = true;
    obj.distancia = dist;
  }
  Serial.println("[LEER ULTRASONICO] Ok");
  return obj;
}*/

// ** FUNCIONES PARA PRUEBAS **
void PruebaMovimiento()
{
  delay(1000);
  MoverAdelante();
  delay(1000);
  MoverAtras();
  delay(1000);
  MoverDerecha();
  delay(1000);
  MoverIzquierda();
  delay(1000);
  Detener();
}

void PruebaServo()
{
  SubirPlataforma();
  delay(3000);
  BajarPlataforma();
  delay(3000);
}

/* Inicialización */
void setup()
{
  Serial.begin(115200);
  // Establecemos los pines para controlar el motor como salidas
  pinMode(M_IN1, OUTPUT);
  pinMode(M_IN2, OUTPUT);
  pinMode(M_IN3, OUTPUT);
  pinMode(M_IN4, OUTPUT);
  pinMode(V_MD, OUTPUT);
  pinMode(V_MI, OUTPUT);
  
  // Ahora colocamos como entradas los pines para los sensores ópticos
  pinMode(OS_LEFT, INPUT);
  pinMode(OS_RIGHT, INPUT);

  // Ahora la salida para el servo Motor
  //Plataforma.attach(SERVO_P);
  analogWrite(V_MD, 130);
  analogWrite(V_MI, 100);
  Serial.println("[SETUP]");
  Detener();
}

/* Bucle principal */
void loop()
{
  /*OBJETO obj1 = LeerUltrasonico();
  if(obj1.detectado)
  {
    Serial.print("Objecto detectado! a: ");
    Serial.print(obj1.distancia);
    Serial.print("\n");
  }*/
  Serial.print("Sensor Izq:");
  Serial.print(digitalRead(OS_LEFT));
  Serial.print("\n");
  Serial.print("Sensor dere:");
  Serial.print(digitalRead(OS_RIGHT));
  Serial.print("\n");

  IFMOVER mov = LeerInfrarojos();
  if(mov == ADELANTE)
  {
    MoverAdelante();
  }else if(mov == DETENER)
  {
    Detener();;
  }else if(mov == IZQUIERDA)
  {
    MoverIzquierda();
  }else if(mov == DERECHA)
  {
    MoverDerecha();;
  }

  
  //PruebaMovimiento();
  //MoverAdelante();
  //delay(5);
}
