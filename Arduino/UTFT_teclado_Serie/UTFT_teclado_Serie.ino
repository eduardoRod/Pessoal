/*
Teclado qwerty com envio por porta série
*/

#include<UTFT.h>
#include<UTouch.h>

extern uint8_t BigFont[];
//extern uint8_t SmallFont[];
const char ALF[32] = "QWERTYUIOPASDFGHJKL  ZXCVBNM";
char temp[1];

UTFT        myGLCD(ITDB32S, 38,39,40,41);
UTouch      myTouch(6,5,4,3,2);

// Variaveis para recolha de dados
int x, y;
char RActual[21] = "";
char EActual[21] = "";
int EActualLen = 0;
char EAnterior[21] = "";

void DesenhaBotoes(){
  // teclado de x = 0 y = 32 a x = 319 y = 239
  // cada botao tem 32x64
  for(y = 0; y < 3; y++){
    for(x = 0; x < 10; x++){
      if(!(x==9 && y==1) && !(x==0 && y==2) && !(x==8 && y==2) && !(x==9 && y==2)){
        myGLCD.setColor(0, 0, 255);
        myGLCD.fillRoundRect((x*31), 32+(y*52),(x*31)+31,84+(y*52));
        myGLCD.setColor(255, 255, 255);
        myGLCD.drawRoundRect((x*31), 32+(y*52),(x*31)+31,84+(y*52));
        temp[0] = ALF[x+(10*y)];
        temp[1] = 0;
        myGLCD.print(temp, 8+(x*31), 50+(y*52));
      }
    }
  }
  // Desenha botões de Enter e Clear
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(0,188,160,239);
  myGLCD.fillRoundRect(160,188,319,239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(0,188,160,239);
  myGLCD.drawRoundRect(160,188,319,239);
  myGLCD.print("Clear", 40, 205);
  myGLCD.print("Enter", 200, 205);
  myGLCD.setBackColor (0, 0, 0);
}

void esperaBotao(int x1, int y1, int x2, int y2){
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect(x1,y1,x2,y2);
  while(myTouch.dataAvailable()){}
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}

void actualizaStr(char novo){
  if (EActualLen < 20){
    EActual[EActualLen] = novo;
    EActualLen++;
    EActual[EActualLen] = '\0';
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(EActual, LEFT, 16);
  }else{
    myGLCD.setColor(255, 0, 0);
    myGLCD.print("NAO ACHAS DEMAIS?", CENTER, 16);
    delay(500);
    myGLCD.print("                 ", CENTER, 16);
    delay(500);
    myGLCD.print("NAO ACHAS DEMAIS?", CENTER, 16);
    delay(500);
    myGLCD.print("                 ", CENTER, 16);
    
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(EActual, LEFT, 16);
  }
}

char recolheBotao(){
  myTouch.read();
  x = myTouch.getX();
  y = myTouch.getY();
  
  if((y >= 32) && (y <= 84)){  // Linha 1
    if((x >= 0) && (x <= 31)){  // Coluna 1 ("Q")
      esperaBotao(0, 32, 31, 84);
      actualizaStr('Q');
    }else if((x >= 31) && (x <= 62)){  // Coluna 2 ("W")
      esperaBotao(31, 32, 62, 84);
      actualizaStr('W');
    }else if((x >= 62) && (x <= 93)){  // Coluna 3 ("E")
      esperaBotao(62, 32, 93, 84);
      actualizaStr('E');
    }else if((x >= 93) && (x <= 124)){  // Coluna 4 ("R")
      esperaBotao(93, 32, 124, 84);
      actualizaStr('R');
    }else if((x >= 124) && (x <= 155)){  // Coluna 5 ("T")
      esperaBotao(124, 32, 155, 84);
      actualizaStr('T');
    }else if((x >= 155) && (x <= 186)){  // Coluna 6 ("Y")
      esperaBotao(155, 32, 186, 84);
      actualizaStr('Y');
    }else if((x >= 186) && (x <= 217)){  // Coluna 7 ("U")
      esperaBotao(186, 32, 217, 84);
      actualizaStr('U');
    }else if((x >= 217) && (x <= 248)){  // Coluna 8 ("I")
      esperaBotao(217, 32, 248, 84);
      actualizaStr('I');
    }else if((x >= 248) && (x <= 279)){  // Coluna 8 ("O")
      esperaBotao(248, 32, 279, 84);
      actualizaStr('O');
    }else if((x >= 279) && (x <= 310)){  // Coluna 9 ("P")
      esperaBotao(279, 32, 310, 84);
      actualizaStr('P');
    }
  }else if((y >= 84) && (y <= 136)){  // Linha 2
    if((x >= 0) && (x <= 31)){  // Coluna 1 ("A")
      esperaBotao(0, 84, 31, 136);
      actualizaStr('A');
    }else if((x >= 31) && (x <= 62)){  // Coluna 2 ("S")
      esperaBotao(31, 84, 62, 136);
      actualizaStr('S');
    }else if((x >= 62) && (x <= 93)){  // Coluna 3 ("D")
      esperaBotao(62, 84, 93, 136);
      actualizaStr('D');
    }else if((x >= 93) && (x <= 124)){  // Coluna 4 ("F")
      esperaBotao(93, 84, 124, 136);
      actualizaStr('F');
    }else if((x >= 124) && (x <= 155)){  // Coluna 5 ("G")
      esperaBotao(124, 84, 155, 136);
      actualizaStr('G');
    }else if((x >= 155) && (x <= 186)){  // Coluna 6 ("H")
      esperaBotao(155, 84, 186, 136);
      actualizaStr('H');
    }else if((x >= 186) && (x <= 217)){  // Coluna 7 ("J")
      esperaBotao(186, 84, 217, 136);
      actualizaStr('J');
    }else if((x >= 217) && (x <= 248)){  // Coluna 8 ("K")
      esperaBotao(217, 84, 248, 136);
      actualizaStr('K');
    }else if((x >= 248) && (x <= 279)){  // Coluna 8 ("L")
      esperaBotao(248, 84, 279, 136);
      actualizaStr('L');
    }
  }else if((y >= 136) && (y <= 188)){  // Linha 3
    if((x >= 31) && (x <= 62)){  // Coluna 2 ("Z")
      esperaBotao(31, 136, 62, 188);
      actualizaStr('Z');
    }else if((x >= 62) && (x <= 93)){  // Coluna 3 ("X")
      esperaBotao(62, 136, 93, 188);
      actualizaStr('X');
    }else if((x >= 93) && (x <= 124)){  // Coluna 4 ("C")
      esperaBotao(93, 136, 124, 188);
      actualizaStr('C');
    }else if((x >= 124) && (x <= 155)){  // Coluna 5 ("V")
      esperaBotao(124, 136, 155, 188);
      actualizaStr('V');
    }else if((x >= 155) && (x <= 186)){  // Coluna 6 ("B")
      esperaBotao(155, 136, 186, 188);
      actualizaStr('B');
    }else if((x >= 186) && (x <= 217)){  // Coluna 7 ("N")
      esperaBotao(186, 136, 217, 188);
      actualizaStr('N');
    }else if((x >= 217) && (x <= 248)){  // Coluna 8 ("M")
      esperaBotao(217, 136, 248, 188);
      actualizaStr('M');
    }
  }
  
  if((y >= 188) && (y <= 239)){  // Linha 4
    if((x >= 0) && (x <= 160)){  // Coluna 1 ("CLEAR")
      if(EActualLen > 0){
        esperaBotao(0, 188, 160, 239);
        EActual[EActualLen-1] = '\0';
        EActualLen = EActualLen-1;
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(0, 16, 319, 31);
        myGLCD.setColor(0, 255, 0);
        myGLCD.print(EActual, LEFT, 16);
      }
    }else if((x >= 160) && (x <= 309)){
      if(EActualLen > 0){
        esperaBotao(160, 188, 319, 239);
        for(x = 0; x <= EActualLen; x++){
          EAnterior[x] = EActual[x];
        }
        EActual[0] = '\0';
        EActualLen = 0;
        myGLCD.setColor(0, 0, 0);
        myGLCD.fillRect(0, 0, 319, 31);
        myGLCD.setColor(0, 255, 0);
        myGLCD.print(EAnterior, LEFT, 0);
        Serial.println(EAnterior);
      }
    }
  }
}

void setup(){
  // Inicia LCD
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);
  // Inicia Touch
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);
  
  DesenhaBotoes();
  Serial.begin(115200);
}

void loop(){
  recolheBotao();
}
