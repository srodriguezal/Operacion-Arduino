#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Variables
int cambiaturno = 1;
int finalJuego= 1;              //controla el numero de turnos
int servoOn = 0;                //controla si debe usar el servo

int puntuacion1 = 0;            //inicializamos la puntuacion del J1
int puntuacion2 = 0;            //inicializamos la puntuacion del J2
int puntuacion3 = 0;            //inicializamos la puntuacion del J3
int puntuacion4 = 0;            //inicializamos la puntuacion del J4
int ganador;                    //el jugador que ha ganado
int jug = 2;                    //inicializamos el numero de jugadores que habra en la partida
String dificultad = "facil";    //inicializamos el nivel de dificultad que habra en la partida
int turno;                      //inicializamos el turno
int ptos = 25;                  //inicializamos los puntos de la pieza a sacar
int contador = 20;              //esta es la cuenta atras de la dificultad media

//Botones
int estado_anteriorBtnIzq;      //seleccion jugador
int estado_anteriorBtnDer;      //seleccion jugador

int estado_anteriorBtnIzqPieza;       //boton izquierda en la seleccion de la pieza
int estado_anteriorBtnDerPieza;       //boton derecha en la seleccion de la pieza
int estado_anteriorBtnOkPieza = HIGH; //boton "ok" en la seleccion de la pieza

int estado_anteriorBtnIzqDificultad;        //boton izquierda en la seleccion de dificultad
int estado_anteriorBtnDerDificultad;        //boton derecha en la seleccion de dificultad
int estado_anteriorBtnOkDificultad = HIGH;  //boton "ok" en la seleccion de dificultad

int estado_anteriorBtnOk;
int estado_anteriorBtnRuleta;
int sensorValBtnOk;
int seleccionadoJugador = 0;      //Indica si se ha seleccionado el numero de jugadores
int seleccionadaDificultad = 0;   //Indica si se ha seleccionado el nivel de dificultad

Servo servoMotor; //Servo

void seleccionJugador() { //En este metodo la pantalla mostrara el numero posible de jugadores para que el susario elija cuantos jugaran en la partida

  lcd.clear();              //Quita todo lo que hubiera escrito en la pantalla
  lcd.home();               //Pone el puntero de la pantalla en la primera posicion de esta
  lcd.print("Jugadores:");  //Escribe en la primera fila
  lcd.setCursor(0, 1);      //Pone el puntero en la segunda fi�a
  lcd.print(">2   3   4");  //Escribe en la segunda fila
  estado_anteriorBtnOk = HIGH;
  
  do {
    int sensorValBtnIzq = digitalRead(2);//Indicamos donde esta el pin del boton izquierda
    int sensorValBtnDer = digitalRead(3);//Indicamos donde esta el pin del boton derecha
    sensorValBtnOk = digitalRead(4);//BtnOk
    if (sensorValBtnIzq != estado_anteriorBtnIzq) {//Si el boton izquierda es pulsado, dependiendo de donde este la flecha en la pantalla la desplaza a una opcion u otra
      if (sensorValBtnIzq == LOW  && jug == 2) {
        jug = 4;    //cambiamos el numero de jugadores que van a participar
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(" 2   3  >4");
      } else if (sensorValBtnIzq == LOW && jug == 3) {
        jug = 2;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(">2   3   4");
      } else if (sensorValBtnIzq == LOW && jug == 4) {
        jug = 3;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(" 2  >3   4");
      }
    } else if (sensorValBtnDer != estado_anteriorBtnDer) {//Si el boton derecha es pulsado, dependiendo de donde este la flecha en la pantalla la desplaza a una opcion u otra
      if (sensorValBtnDer == LOW && jug == 2) {
        jug = 3;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(" 2  >3   4");
      } else if (sensorValBtnDer == LOW && jug == 3) {
        jug = 4;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(" 2   3  >4");
      } else if (sensorValBtnDer == LOW  && jug == 4) {
        jug = 2;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Jugadores:");
        lcd.setCursor(0, 1);
        lcd.print(">2   3   4");
      }
    }
    estado_anteriorBtnIzq = sensorValBtnIzq;
    estado_anteriorBtnDer = sensorValBtnDer;
    sensorValBtnOk = digitalRead(4);    //Indicamos donde esta el pin del boton "ok"
  } while (sensorValBtnOk == estado_anteriorBtnOk);   //Terminamos cuando se pulsa el boton "ok"
  estado_anteriorBtnOk = sensorValBtnOk;
  turno = 1;
  delay(600);
}

void seleccionDificultad() { //En este metodo la pantalla mostrara los distintos niveles de dificultad para que el usuario elija con cual jugaran
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dificutad:>facil");
  lcd.setCursor(0, 1);
  lcd.print(" medio   dificil");
  int sensorValBtnOkDificultad;
  estado_anteriorBtnOkDificultad = HIGH;
  
  do {
    int sensorValBtnIzqDificultad = digitalRead(2);
    int sensorValBtnDerDificultad = digitalRead(3);
    sensorValBtnOkDificultad = digitalRead(4);
    
    if (sensorValBtnIzqDificultad != estado_anteriorBtnIzqDificultad) {//Si el boton izquierda es pulsado, dependiendo de donde este la flecha en la pantalla la desplaza a una opcion u otra
      if (sensorValBtnIzqDificultad == LOW  && dificultad == "facil") {
        dificultad = "dificil";   //cambia el nivel de dificultad con el que se juega
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad: facil");
        lcd.setCursor(0, 1);
        lcd.print(" medio  >dificil");
      } else if (sensorValBtnIzqDificultad == LOW && dificultad == "medio") {
        dificultad = "facil";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad:>facil");
        lcd.setCursor(0, 1);
        lcd.print(" medio   dificil");
      } else if (sensorValBtnIzqDificultad == LOW && dificultad == "dificil") {
        dificultad = "medio";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad: facil");
        lcd.setCursor(0, 1);
        lcd.print(">medio   dificil");
      }
    } else if (sensorValBtnDerDificultad != estado_anteriorBtnDerDificultad) {//Si el boton derecha es pulsado, dependiendo de donde este la flecha en la pantalla la desplaza a una opcion u otra
      if (sensorValBtnDerDificultad == LOW && dificultad == "facil") {
        dificultad = "medio";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad: facil");
        lcd.setCursor(0, 1);
        lcd.print(">medio   dificil");
      } else if (sensorValBtnDerDificultad == LOW && dificultad == "medio") {
        dificultad = "dificil";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad: facil");
        lcd.setCursor(0, 1);
        lcd.print(" medio  >dificil");
      } else if (sensorValBtnDerDificultad == LOW  && dificultad == "dificil") {
        dificultad = "facil";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dificutad:>facil");
        lcd.setCursor(0, 1);
        lcd.print(" medio   dificil");
      }
    }
    estado_anteriorBtnIzqDificultad = sensorValBtnIzqDificultad;
    estado_anteriorBtnDerDificultad = sensorValBtnDerDificultad;
    sensorValBtnOkDificultad = digitalRead(4);
  } while (sensorValBtnOkDificultad == estado_anteriorBtnOkDificultad);   //Terminamos cuando se pulsa el boton "ok"
  estado_anteriorBtnOkDificultad = sensorValBtnOkDificultad;
  delay(600);
}

void mostrarPuntos() { //En este metodo la pantalla mostrara las puntuaciones totales cada final de ronda
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Puntos totales");
  lcd.setCursor(0, 1);
  lcd.print("de la partida");
  delay(2000);

 //Dependiendo del numero de jugadores lo imprimo en la pantalla de una forma u otra
  if (jug == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("J1:" + String(puntuacion1));
    lcd.setCursor(0, 1);
    lcd.print("J2:" + String(puntuacion2));
  } else if (jug == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("J1:" + String(puntuacion1) + " J2:" + String(puntuacion2));
    lcd.setCursor(0, 1);
    lcd.print("J3:" + String(puntuacion3));
  } else if (jug == 4) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("J1:" + String(puntuacion1) + " J2:" + String(puntuacion2));
    lcd.setCursor(0, 1);
    lcd.print("J3:" + String(puntuacion3) + " J4:" + String(puntuacion4));
  }
  delay(2000);


}

void seleccionPieza() { //En este metodo la pantalla mostrara el numero posible de jugadores para que el susario elija cuantos jugaran en la partida

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">25  30  40  50");
  lcd.setCursor(0, 1);
  lcd.print(" 55  60  75  90");

  int sensorValBtnOkPieza;
  estado_anteriorBtnOkPieza = HIGH;
  do {
    
    int sensorValBtnIzqPieza = digitalRead(2);
    int sensorValBtnDerPieza = digitalRead(3);
    sensorValBtnOkPieza = digitalRead(4);

    if (sensorValBtnIzqPieza != estado_anteriorBtnIzqPieza) {
      if (sensorValBtnIzqPieza == LOW  && ptos == 25) {
        ptos = 90;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75 >90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 30) {
        ptos = 25;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 40) {
        ptos = 30;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25 >30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 50) {
        ptos = 40;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30 >40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 55) {
        ptos = 50;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40 >50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 60) {
        ptos = 55;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(">55  60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 75) {
        ptos = 60;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55 >60  75  90");
      } else if (sensorValBtnIzqPieza == LOW && ptos == 90) {
        ptos = 75;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60 >75  90");
      }
    } else if (sensorValBtnDerPieza != estado_anteriorBtnDerPieza) {
      if (sensorValBtnDerPieza == LOW && ptos == 25) {
        ptos = 30;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25 >30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 30) {
        ptos = 40;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30 >40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 40) {
        ptos = 50;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40 >50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 50) {
        ptos = 55;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(">55  60  75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 55) {
        ptos = 60;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55 >60  75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 60) {
        ptos = 75;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60 >75  90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 75) {
        ptos = 90;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75 >90");
      } else if (sensorValBtnDerPieza == LOW && ptos == 90) {
        ptos = 25;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">25  30  40  50");
        lcd.setCursor(0, 1);
        lcd.print(" 55  60  75  90");
      }
    }
    estado_anteriorBtnIzqPieza = sensorValBtnIzqPieza;
    estado_anteriorBtnDerPieza = sensorValBtnDerPieza;
    sensorValBtnOkPieza = digitalRead(4);
  } while (sensorValBtnOkPieza == estado_anteriorBtnOkPieza);
  estado_anteriorBtnOkPieza = sensorValBtnOkPieza;
  delay(600);
}

void nuevaPuntuacion() { //En este metodo se actualizan las puntuaciones de los jugadores
  if (turno == 1) {
    puntuacion1 = puntuacion1 + ptos;
  } else if (turno == 2) {
    puntuacion2 = puntuacion2 + ptos;
  } else if (turno == 3) {
    puntuacion3 = puntuacion3 + ptos;
  } else {
    puntuacion4 = puntuacion4 + ptos;
  }

  // Reiniciamos el valor de las puntuaciones
  ptos = 25;
}

void ruleta() { //En este metodo se indica que se debe pulsar el boton de la ruleta
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gira la ruleta");
  lcd.setCursor(0, 1);
  lcd.print("      --->");
  delay(2000);
}

void zumbador() { //En este emtodo se controlan los zumbadores
  digitalWrite( 12 , HIGH);
  tone(13, 500);
  delay(1000);
  noTone(13);
  digitalWrite( 12 , LOW);
  delay (1000);

}

//Dificultad Facil

void programaPrincipalFacil() {

  if (cambiaturno == 1) {
    cambiaturno = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turno jugador: ");
    lcd.setCursor(0, 1);
    lcd.print(turno);
    delay(2000);

    ruleta();
    seleccionPieza();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saca la pieza");
    delay(2000);
  }
  if (digitalRead(4) == HIGH) {
    if (digitalRead(0) == LOW) { //No se pulsa ok, lo que quiere decir que ha fallado
      zumbador();

      cambiaturno = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error:");
      lcd.setCursor(0, 1);
      lcd.print("cortocircuito");
      delay(2000);

      delay(600);
      if (turno == jug) {
        turno = 1;
        finalJuego++;
        mostrarPuntos();
      }
      else {
        turno++;
      }
    }

  } else { //Se pulsa ok, lo que quiere decir que se coge la pieza

    nuevaPuntuacion();
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("Pieza extraida");
      lcd.setCursor(0, 1);
      lcd.print("con exito!");
    delay(2000);
    if (turno == jug) {
      turno = 1;
      finalJuego++;
      mostrarPuntos();
    }
    else {
      turno++;
    }
    cambiaturno = 1;
    delay(600);
  }
  delay(600);
}

//Dificultad Media
void programaPrincipalMedio() {

  if (cambiaturno == 1) {
    cambiaturno = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turno jugador: ");
    lcd.setCursor(0, 1);
    lcd.print(turno);
    delay(2000);

    ruleta();
    seleccionPieza();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saca la pieza");
    delay(2000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tiempo restante:");
    lcd.setCursor(0, 1);
    lcd.print(contador);
    lcd.print(" segundos");
    delay(1000);
    contador--;
    if (digitalRead(4) == HIGH) {
      if (digitalRead(0) == LOW || contador == 0) { //No se pulsa ok, lo que quiere decir que ha fallado
        zumbador();

        cambiaturno = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error:");
        lcd.setCursor(0, 1);
        lcd.print("cortocircuito");
        delay(2000);

        delay(600);
        if (turno == jug) {
          turno = 1;
          finalJuego++;
          mostrarPuntos();
        }
        else {
          turno++;
        }
        contador = 20; //Reinicializamos la cuenta atras
      }

    } else { //se pulsa ok, lo que quiere decir que se coge la pieza

      nuevaPuntuacion();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pieza extraida");
      lcd.setCursor(0, 1);
      lcd.print("con exito!");
      delay(2000);
      if (turno == jug) {
        turno = 1;
        finalJuego++;
        mostrarPuntos();
      }
      else {
        turno++;
      }
      contador = 20; //reinicializamos la cuenta atras
      cambiaturno = 1;
      delay(600);
    }
  }
  delay(600);
}

//Dificultad Dificil
void programaPrincipalDificil() {
  if (cambiaturno == 1) {
    cambiaturno = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turno jugador: ");
    lcd.setCursor(0, 1);
    lcd.print(turno);
    delay(2000);

    ruleta();

    seleccionPieza();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saca la pieza");
    delay(2000);
    servoOn = 1; //permite entrar en la zona del servo
  } else if (servoOn == 1) {

    //CODIGO DEL SERVO
    servoMotor.write(0);
    delay(1000);

    servoMotor.write(90);
    delay(1000);
    servoMotor.write(-90);


    if (digitalRead(4) == HIGH) {
      if (digitalRead(0) == LOW || contador == 0) { //No se pulsa ok, lo que quiere decir que ha fallado
        zumbador();

        cambiaturno = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error:");
        lcd.setCursor(0, 1);
        lcd.print("cortocircuito");
        delay(2000);

        delay(600);
        if (turno == jug) {
          turno = 1;
          finalJuego++;
          mostrarPuntos();
        }
        else {
          turno++;
        }
      }

    } else { //Se pulsa ok, lo que quiere decir que se coge la pieza

      nuevaPuntuacion();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pieza extraida");
      lcd.setCursor(0, 1);
      lcd.print("con exito!");
      delay(2000);
      if (turno == jug) {
        turno = 1;
        finalJuego++;
        mostrarPuntos();
      }
      else {
        turno++;
      }
      cambiaturno = 1;
      delay(600);
    }
  }
  delay(600);
}

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);//Boton izq
  pinMode(3, INPUT_PULLUP);//Boton der
  pinMode(4, INPUT_PULLUP);// Boton ok
  pinMode(13, OUTPUT);//Zumbador
  pinMode(12, OUTPUT);//Led
  servoMotor.attach(11);
  lcd.begin(16, 2);
  lcd.clear();
  seleccionadoJugador = 0;
  seleccionadaDificultad = 0;
}

void loop()
{
  if (seleccionadoJugador == 0) { //Seleccion de jugadores

    seleccionJugador();
    seleccionadoJugador = 1;

  } else if (seleccionadoJugador == 1) { //Seleccion de dificultad

    seleccionDificultad();
    seleccionadoJugador = -1;
    seleccionadaDificultad = 1;

  }
  else if (seleccionadaDificultad == 1) { //Programa principal
    if (finalJuego <= 10) {//Elige el tipo de juego
      if (dificultad == "facil") {
        programaPrincipalFacil();
      } else if (dificultad == "medio") {
        programaPrincipalMedio();
      } else if (dificultad == "dificil") {
        programaPrincipalDificil();
      }
    } else if (finalJuego > 10) {//Acaba la partida al llegar a 10
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Se acabo");
      lcd.setCursor(0, 1);
      lcd.print("el juego");
      delay(2000);

      mostrarPuntos();
      delay(2000);
    }
  }
}