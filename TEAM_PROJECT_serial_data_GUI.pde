import processing.net.*;
import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

ListBox serial_list_box;
Serial myPort;
Serial serial_port = null;

Client my_client;

Textarea serial_text_area;
Textarea connection_text;

int num_serial_ports = 0;     //사용가능한 시리얼 포트의 개수 저장
int selected_serial_port = 0; //선택된 시리얼포트 index 저장
int mouse_flag = 0;

int sensor;
int buf[] = new int[5]; //java statement
int x,y;

String text = "";

void setup()
{
  size(810, 600);
  background(40);
  noStroke();
  
  //myPort = new Serial(this, "COM5", 9600);
  
  //소켓생성
  //my_client = new Client(this, "192.168.30.16", 5000); // server ip adr, port
  ////준비여부 즉 클라이언트가 소켓이 생성되었는지 체크 = 소케 유효성확인
  //if(my_client.active() == false)
  //{
  //  println("연결안됨, 활성화 안됨, Connect Error");
  //  exit();
  //}
  //else println("Server에 연결 됨 = OK");
  
  //textSize(20);
  //fill(255,0,0);
 
  cp5 = new ControlP5(this);
  serial_list_box = cp5.addListBox("SerialPortList")
  .setPosition(10, 100)
  .setSize(120, 200);
  serial_list_box.setFont(createFont("arial", 11));
  serial_list_box.setItemHeight(25);
  serial_list_box.setBarHeight(25);
  
  //커넥트 버튼 추가
  cp5.addButton("Connect")
    .setPosition(10, 10)
    .setFont(createFont("arial", 11))
    .setSize(120, 20);
  //디스커넥트 버튼 추가
  cp5.addButton("Disconnect")
    .setPosition(10, 40)
    .setFont(createFont("arial", 11))
    .setSize(120, 20);
  connection_text = cp5.addTextarea("txt")
    .setPosition(10,70)
    .setSize(120,20)
    .setFont(createFont("arial", 10))
    .setColor(color(120))
    .setColorBackground(color(220))
    .setText("Not Connected");
  //시리얼 통신 연결 버튼 및 포트넘버 목록창 버튼
  String serial_port_str;
  num_serial_ports = Serial.list().length;
  for(int i=0; i<num_serial_ports; i++)
  {
    serial_port_str = Serial.list()[i]; // i번째 시리얼 포트이름을 저장
    serial_list_box.addItem(serial_port_str, i); //시리얼포트이름을 리스트 박스에 추가
  }     
  //방향키 버튼 생성
  cp5.addButton("GO")
     .setValue(0)
     .setPosition(700, 210)
     .setFont(createFont("arial", 20))
     .setSize(50,50)
     .setId(1);
  cp5.addButton("BACK")
     .setValue(0)
     .setPosition(700,310)
     .setFont(createFont("arial", 15))
     .setSize(50,50)
     .setId(2);
  cp5.addButton("STOP")
     .setValue(0)
     .setPosition(700,260)
     .setFont(createFont("arial", 15))
     .setSize(50,50)
     .setId(3);
  cp5.addButton("LEFT")
     .setValue(0)
     .setPosition(650,260)
     .setFont(createFont("arial", 15))
     .setSize(50,50);
  cp5.addButton("RIGHT")
     .setValue(0)
     .setPosition(750,260)
     .setFont(createFont("arial", 15))
     .setSize(50,50);
  //데이터 표시 박스
  fill(240);
  rect(140,50,500,350);
  strokeWeight(8);
  stroke(#969696);

}
//시리얼포트리스트 버튼 누를시 
void SerialPortList(int value)
{
  selected_serial_port = value;
  println("선택된 번호 = " + value);
}

void draw()
{
  //println(mouseX);
  //println(mouseY);
  
  if(serial_port != null) //시리얼 포트가 연결되어 있는경우
  {
    if(serial_port.available() > 0)  //수신된 데이터가 있는 경우
    {
      int val = serial_port.read(); //수신된 데이터를 1byte 읽음
      if(val == '\n')
      {
        serial_text_area.setText(text);
        text = "";
      }
      else
      {
        text = text + char(val);
      }
    }
  }
  
  if((mouseX >= 700) && (mouseX <= 750))
  {
    if((mouseY >= 210) && (mouseY <= 260))
    {
      cursor(HAND);
      mouse_flag = 1;
    }
    else if((mouseY >= 260) && (mouseY <=310))
    {
      cursor(HAND);
      mouse_flag = 2;
    }
    else if((mouseY >= 310) && (mouseY <= 360))
    {
      cursor(HAND);
      mouse_flag = 3;
    }
    else{cursor(ARROW); mouse_flag = 0;}
  }
  else if((mouseY >= 260) && (mouseY <= 310))
  {
    if((mouseX >= 650) && (mouseX <= 700))
    {
      cursor(HAND);
      mouse_flag = 4;
    }
    //else if((mouseX > 456) && (mouseX <= 456)) {cursor(ARROW); mouse_flag = 0;}
    else if((mouseX >= 750) && (mouseX <= 800))
    {
      cursor(HAND);
      mouse_flag = 5;
    }
    else{cursor(ARROW); mouse_flag = 0;}
  }
  else{cursor(ARROW); mouse_flag = 0;}
  delay(50);

  
  
  //ip 서버에 연결하기 
  //if(my_client.active() == true) //클라이언트 소켓이 준비되었으면, 서버와 연결되었으면..
  //{
  //  //데이터 수신여부 체크
  //  if(my_client.available() > 0) //수신 byte 수를 가지고 있음, 0 = 수신데이터가 없음
  //  {
  //    //수신 데이터가 있으면
  //    //방법1 = max 수신 할 배열을 정하고 byte 형식으로 받는 방법
  //    //byte[] byte_buf = my_client.readBytes(20);
  //    byte[] byte_buf = my_client.readBytes(my_client.available());
  //    println(byte_buf.length + "byte를 받았습니다");
      
  //    //방법2 = 문자열로 받는 방법
  //    //String rx_d = my_client.readString();
      
  //    //방법3 = 정해진 문자까지 데이터를 받는 경우 = default
  //    //String rx_d = my_client.readStringUntil('\n');
  //  }
    
  //  delay(500);
    
    //data tx
    //my_client.write('k');
    //my_client.write("Server Test OK\n");
  //}
  //joystick 
  //if(myPort.available() > 0)
  //{
  //  int c = myPort.read();
  //  if(c == 0x7E) //0x7E = 126
  //  {
  //    int i = 0;
      
  //    while((buf[i++]=myPort.read()) != 0x7E)
  //    {
  //      if( i==5)
  //       break;
  //    }
      
  //    x = (buf[1] << 8) + buf[2];
  //    println("x:" + x);
      
  //    y = (buf[3] << 8) + buf[4];
  //    println("y:" + y);
  //  }
    
  //  delay(100);
  //}
}

//클라이언트 연결시 주고받는 데이터 
//void clientEvent(Client ho)
//{
//  while(true)
//  {
//    if(ho.available() > 0)
//    {
//      //byte[] d_buf = ho.readBytes(ho.available());  //  readBytes(20);
//      String d_buf = ho.readString();
//      println(d_buf);
//      fill(0,0,255);
//      textSize(50);
//      text("Smart Factory", 200, 100);
//      fill(156,0,0);
//      text(d_buf, 240, 200);
//    }
//    else
//    {
//      break;
//    }
//  }
//}
//void client_close()
//{
//  my_client.stop();
//}

//Connect 클릭
void Connect()  
{
  if(serial_port == null) //연결된 시리얼 포트가 없는 경우
  {
    String port_string = serial_list_box.getItem(selected_serial_port).get("text").toString();  //포트번호를 가져옴
    println(port_string);
    serial_port = new Serial(this, port_string, 9600);  //포트번호에 연결
    connection_text.setText("CONNECTED");
  }
}
//disconnect 클릭
void Disconnect()
{
  if(serial_port != null)  //시리얼 포트가 연결된 경우
  {
    serial_port.stop();  //연결정지
    serial_port = null;
    println("DISCONNECT");
    connection_text.setText("NOT CONNECTED");
  }
}

void Send()
{
  String value = cp5.get(Textfield.class, "input").getText();
  println(value);
  serial_port.write(value);
}
//버튼 기능 넣기 앞뒤이동 기타등등...
void mousePressed()
{ 
  int go_buf = 0; 
  int back_buf = 0;
  
  if((mouseX >= 700) && (mouseX <=750) && (mouseY >= 210) && (mouseY <= 257))
  {
    println("GO");
    go_buf = 1;
    //println(go_buf);
    //back_buf = 0;
    
  }
  else if((mouseX >= 700) && (mouseX <= 750) && (mouseY >= 310) && (mouseY <= 360)) 
  {
    println("BACK");
    back_buf = 1;
  }
  else if((mouseX >= 650) && (mouseX <= 700) && (mouseY >= 260) && (mouseY <= 310))
  {
    println("LEFT");
  }
  else if((mouseX >= 750) && (mouseX <= 800) && (mouseY >= 260) && (mouseY <= 310))
  {
    println("RIGHT");
  }
  else if((mouseX >= 700) && (mouseX <= 750) && (mouseY >= 260) && (mouseY <= 310))
  {
    println("STOP");
  }
}
