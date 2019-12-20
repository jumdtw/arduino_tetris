  #include <VGAX.h>

//image generated from 2BITIMAGE - by Sandro Maffiodo
#define pixel_width 120
#define pixel_height 60 // side_space_width IMG_PIRATE_HEIGHT
#define side_space_width 42
#define field_width 12
#define field_height 20
#define IMG_PIRATE_BWIDTH 30
#define field_pixel_width 10
#define MINO_ANGLE 4
#define MINO_HEIGHT_WIDTH 4
#define MINO_TYPE 1

byte mino[MINO_TYPE][MINO_ANGLE][MINO_HEIGHT_WIDTH][MINO_HEIGHT_WIDTH] = {
            // t mino 0
        0,0,0,0,
        0,1,0,0,
        1,1,1,0,
        0,0,0,0,
        // t mino 90
        0,0,0,0,
        0,1,0,0,
        0,1,1,0,
        0,1,0,0,
        //t mino 180
        0,0,0,0,
        0,0,0,0,
        1,1,1,0,
        0,1,0,0,
        //t mino 270
        0,0,0,0,
        0,1,0,0,
        1,1,0,0,
        0,1,0,0,
};

byte pixel_field[field_pixel_width*pixel_height];
byte static_field[field_height][field_width];
byte display_field[field_height][field_width];

VGAX vga;

int count = 0;
byte mino_type = 0,mino_angle = 0,mino_x = 3 ,mino_y = 0;
const int right = 7,left = 8,angle = 10;

void static_field_init(){
    int i;
    for(i=0;i<field_height;i++){
        static_field[i][0] = 1;
        static_field[i][field_width-1] = 1;
    }
    for(i=0;i<field_width;i++){
        static_field[field_height-1][i] = 1;
    }
}

void display_field_init(){
    int i;
    for(i=0;i<field_height;i++){
        display_field[i][0] = 1;
        display_field[i][field_width-1] = 1;
    }
    for(i=0;i<field_width;i++){
        display_field[field_height-1][i] = 1;
    }
}

void display(){
    byte buf;
    for (int y=0; y!=VGAX_HEIGHT; y++) {
        for (int x=0; x!=VGAX_BWIDTH; x++) {
            buf = 0;
            if(x>=10 && x<=19){
                buf = pixel_field[(10*y+(x-10))];
            }
            vga.putpixel4(x, y, buf);
        }
    }
}

// テトリス画面と実際の画面の転換
void field_to_display(){
    int i,k,height_index=0;
    byte buf;

    for(i=0;i<field_height;i++){
        // 外枠残り２ドットとfield 端の2ドットを埋めてる
        pixel_field[field_pixel_width*height_index + 0] = 0b00000101;
        pixel_field[field_pixel_width*(height_index+1) + 0] = 0b00000101;
        pixel_field[field_pixel_width*(height_index+2) + 0] = 0b00000101;
        // 36/4 == 9 
        for(k=0;k<8;k++){
            buf = 0;
            if(k==0){
                buf = 0b01000000;
                if(display_field[i][1]>=1){
                    buf = buf + 0b00010101;
                }
                pixel_field[field_pixel_width*height_index + 1] = buf;
                pixel_field[field_pixel_width*(height_index+1) + 1] = buf;
                pixel_field[field_pixel_width*(height_index+2) + 1] = buf;
                continue;
            }else if(k==7){
                buf = 0b00000001;
                if(display_field[i][10]==1){
                    buf = buf + 0b01010100;
                }
                pixel_field[field_pixel_width*height_index + 8] = buf;
                pixel_field[field_pixel_width*(height_index+1) + 8] = buf;
                pixel_field[field_pixel_width*(height_index+2) + 8] = buf;
                continue;
            }else{
                if(k==1){
                    if(display_field[i][2]>=1){
                        buf = 0b01010100;
                    }else if(display_field[i][2]==0){
                        buf = 0b00000000;
                    }
                    if(display_field[i][2+1]>=1){
                        buf = buf + 0b00000001;
                    }
                }else if(k==2){
                    if(display_field[i][3]>=1){
                        buf = 0b01010000;
                    }else if(display_field[i][3]==0){
                        buf = 0b00000000;
                    }
                    if(display_field[i][3+1]>=1){
                        buf = buf + 0b00000101;
                    }
                }else if(k==3){
                    if(display_field[i][4]==1){
                        buf = 0b01000000;
                    }else{
                        buf = 0b00000000;
                    }
                    if(display_field[i][4+1]==1){
                        buf = buf + 0b00010101;
                    }
                }else if(k==4){
                    if(display_field[i][6]==1){
                        buf = 0b01010100;
                    }else{
                        buf = 0b00000000;
                    }
                    if(display_field[i][6+1]==1){
                        buf = buf + 0b00000001;
                    }
                }else if(k==5){
                    if(display_field[i][7]==1){
                        buf = 0b01010000;
                    }else{
                        buf = 0b00000000;
                    }
                    if(display_field[i][7+1]==1){
                        buf = buf + 0b00000101;
                    }
                }else if(k==6){
                    if(display_field[i][8]==1){
                        buf = 0b01000000;
                    }else{
                        buf = 0b00000000;
                    }
                    if(display_field[i][8+1]==1){
                        buf = buf + 0b00010101;
                    }
                }else{
                    //Serial.println("error field to display");
                }
                pixel_field[field_pixel_width*height_index + k + 1] = buf;
                pixel_field[field_pixel_width*(height_index+1) + k + 1] = buf;
                pixel_field[field_pixel_width*(height_index+2) + k + 1] = buf;  
            }
        }
        // ラスト２ドットは確定 壁と余白２ドット
        pixel_field[field_pixel_width*height_index + 9] = 0b01010000;
        pixel_field[field_pixel_width*(height_index+1) + 9] = 0b01010000;
        pixel_field[field_pixel_width*(height_index+2) + 9] = 0b01010000;
        height_index = height_index + 3;
    }
}

void cp_static_to_display(){
    for(int y=0;y<field_height;y++){
        for(int x=0;x<field_width;x++){
            display_field[y][x] = static_field[y][x];
        }
    }
}

void cp_display_to_static(){
    for(int y=0;y<field_height;y++){
        for(int x=0;x<field_width;x++){
            static_field[y][x] = display_field[y][x];
        }
    }
}

void update(){
    cp_static_to_display();
    // mino print 
    for(int i=0;i<MINO_HEIGHT_WIDTH;i++){
        for(int k=0;k<MINO_HEIGHT_WIDTH;k++){
            display_field[mino_y+i][mino_x+k] = display_field[mino_y+i][mino_x+k] | mino[mino_type][mino_angle][i][k];
        }
    }

}

//----------------------------------------------------------------------------------
void castline(int y){
    for(int i=y;i>=0;i--){
        for(int k=1;k<field_width-1;k++){
            if(i==0){
                static_field[i][k] = 0;
                continue;
            }
            static_field[i][k] = static_field[i-1][k];
        }
    }
}

void deleteline(int y){
    for(int i=1;i<field_width-1;i++){
        static_field[y][i] = 0;
    }
    castline(y);
}

void checkline(){
    boolean flag = true;
    for(int i=0;i<field_height-1;i++){
        for(int k=1;k<field_width-1;k++){
            if(static_field[i][k]==0){
                flag = false;
            }
        }
        if(flag){
            deleteline(i);
        }else{
            flag = true;
        }
    }
    
}

//----------------------------------------------------------------------------------------

boolean hitcheck(byte minoX,byte minoY,byte minotype,byte minoangle){
    for(int i=0;i<MINO_HEIGHT_WIDTH;i++){
        for(int k=0;k<MINO_HEIGHT_WIDTH;k++){
            if(mino[minotype][minoangle][i][k]&&static_field[minoY+i][minoX+k]){
                return false;
            }
        }
    }
    return true;
}


void setup() {
    pinMode(right,INPUT);
    pinMode(left,INPUT);
    pinMode(angle,INPUT);
    static_field_init();
    display_field_init();
    //Serial.begin(9600);
    vga.begin();
}

void loop(){
    if(count==1){
        /*
        int buf = 0;
        for (int y=0; y!=VGAX_HEIGHT; y++) {
            for (int x=0; x!=VGAX_BWIDTH; x++) {
                buf = 0;
                if(x>=10 && x<=19){
                    buf = pixel_field[(10*y+(x-10))];
                }
                Serial.print(buf);
                Serial.print(", ");
            }
            Serial.print("\n");
        }
        */
                
       /*
       for(int y=0;y<field_height;y++){
           for(int x=0;x<field_width;x++){
               Serial.print(display_field[y][x]);
           }
           Serial.print("\n");
       }
       */
       
    }
    if(count%20==0){
        if(digitalRead(right)==LOW){
            if(hitcheck(mino_x+1,mino_y,mino_type,mino_angle)){
                mino_x++;
            }
        }
        if(digitalRead(left)==LOW){
            if(hitcheck(mino_x-1,mino_y,mino_type,mino_angle)){
                mino_x = mino_x - 1;
            }
        }
        if(digitalRead(angle)==LOW){
            if(hitcheck(mino_x,mino_y,mino_type,mino_angle+1)){
                mino_angle++;
                if(mino_angle>=4){
                    mino_angle = 0;
                }
            }
        }
    }

    if(count%100==0){
        if(hitcheck(mino_x,mino_y+1,mino_type,mino_angle)){
            mino_y++;
            update();
        }else{
            cp_display_to_static();
            mino_y = 0;
            mino_x = 2;
        }
        checkline();
    }
    
    field_to_display();
    display();
    count++;
    if(count>=10000){
        count = 0;
    }
}