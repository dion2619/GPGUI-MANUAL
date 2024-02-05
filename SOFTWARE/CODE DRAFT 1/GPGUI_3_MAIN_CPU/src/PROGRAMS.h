
void GPGUI::QUAD_DEBUG_PROGRAM()

{
    static int active_Point = 0;
    static float x1 = 20;
    static float y1 = 20;

    static float x2 = 20;
    static float y2 = 120;

    static float x3 = 120;
    static float y3 = 120;

    static float x4 = 20;
    static float y4 = 120;


    float point_Move_Speed = 0.5;

    bool point_Label_En = true;


    while (1)
    {

        switch (read_Keyboard_Port())
        {
        case LEFT_KEY_MAKE_CODE:

            switch (active_Point)
            {
            case 0:
                x1 -= point_Move_Speed;
                x2 -= point_Move_Speed;
                x3 -= point_Move_Speed;
                x4 -= point_Move_Speed;

                break;
            case 1:  x1 -= point_Move_Speed; break;
            case 2:  x2 -= point_Move_Speed; break;
            case 3:  x3 -= point_Move_Speed; break;
            case 4:  x4 -= point_Move_Speed; break;
            }

            break;


        case RIGHT_KEY_MAKE_CODE:

            switch (active_Point)
            {
            case 0:
                x1 += point_Move_Speed;
                x2 += point_Move_Speed;
                x3 += point_Move_Speed;
                x4 += point_Move_Speed;

                break;
            case 1:  x1 += point_Move_Speed; break;
            case 2:  x2 += point_Move_Speed; break;
            case 3:  x3 += point_Move_Speed; break;
            case 4:  x4 += point_Move_Speed; break;
            }

            break;



        case UP_KEY_MAKE_CODE:

            switch (active_Point)
            {
            case 0:
                y1 -= point_Move_Speed;
                y2 -= point_Move_Speed;
                y3 -= point_Move_Speed;
                y4 -= point_Move_Speed;

                break;

            case 1:  y1 -= point_Move_Speed; break;
            case 2:  y2 -= point_Move_Speed; break;
            case 3:  y3 -= point_Move_Speed; break;
            case 4:  y4 -= point_Move_Speed; break;
            }

            break;


        case DOWN_KEY_MAKE_CODE:

            switch (active_Point)
            {
            case 0:
                y1 += point_Move_Speed;
                y2 += point_Move_Speed;
                y3 += point_Move_Speed;
                y4 += point_Move_Speed;

                break;



            case 1:  y1 += point_Move_Speed;  break;
            case 2:  y2 += point_Move_Speed;  break;
            case 3:  y3 += point_Move_Speed;  break;
            case 4:  y4 += point_Move_Speed;  break;
            }

            break;

        case Page_Up_Key_Make_Code:   active_Point++; if (active_Point == 5) active_Point = 0;   beep(500, 200); break;

        case 273:
            active_Point = 0;
            x1 = 20;
            y1 = 20;

            x2 = 20;
            y2 = 120;

            x3 = 120;
            y3 = 120;

            x4 = 20;
            y4 = 120;
            break;

        case 44:




            break;

        };








        drawBackgroundColor(16);



        draw_Quad

        (
            x1, //p1
            y1,

            x2,//p2
            y2,

            x3,//p3
            y3,


            x4,//p4
            y4,
            1,
            yellow
        );



        switch (active_Point)
        {

        case 0:
            setPixel(x1, y1, random(1, 64));
            drawHollowRectangle(x1 - 3, y1 - 3, 6, 6, random(1, 63));
            if (point_Label_En)  printNumberInt(1, x1 + 4, y1, green, black);

            setPixel(x2, y2, random(1, 64));
            drawHollowRectangle(x2 - 3, y2 - 3, 6, 6, random(1, 63));
            if (point_Label_En) printNumberInt(2, x2 + 4, y2, green, black);

            setPixel(x3, y3, random(1, 64));
            drawHollowRectangle(x3 - 3, y3 - 3, 6, 6, random(1, 63));
            if (point_Label_En) printNumberInt(3, x3 + 4, y3, green, black);


            setPixel(x4, y4, random(1, 64));
            drawHollowRectangle(x4 - 3, y4 - 3, 6, 6, random(1, 63));
            if (point_Label_En) printNumberInt(4, x4 + 4, y4, green, black);

            break;




        case 1:
            setPixel(x1, y1, random(1, 64));
            drawHollowRectangle(x1 - 3, y1 - 3, 6, 6, random(1, 63));
            if (point_Label_En) printNumberInt(active_Point, x1 + 4, y1, green, black);
            printNumberDouble(x1, 2, 2, red, black);
            printNumberDouble(y1, 2, 12, red, black);
            break;

        case 2:

            setPixel(x2, y2, random(1, 64));
            drawHollowRectangle(x2 - 3, y2 - 3, 6, 6, random(1, 63));
            if (point_Label_En)  printNumberInt(active_Point, x2 + 4, y2, green, black);
            printNumberDouble(x2, 2, 2, red, black);
            printNumberDouble(y2, 2, 12, red, black);
            break;



        case 3:

            setPixel(x3, y3, random(1, 64));
            drawHollowRectangle(x3 - 3, y3 - 3, 6, 6, random(1, 63));
            if (point_Label_En)  printNumberInt(active_Point, x3 + 4, y3, green, black);
            printNumberDouble(x3, 2, 2, red, black);
            printNumberDouble(y3, 2, 12, red, black);
            break;



        case 4:

            setPixel(x4, y4, random(1, 64));
            drawHollowRectangle(x4 - 3, y4 - 3, 6, 6, random(1, 63));
            if (point_Label_En)  printNumberInt(active_Point, x4 + 4, y4, green, black);
            printNumberDouble(x4, 2, 2, red, black);
            printNumberDouble(y4, 2, 12, red, black);
            break;
        };










        refreshScreen();
    }
}

void GPGUI::texture_Scan_Line_Maping_Debug_Program()
{
    //SCAN LINE
    int x1 = 50;
    int y1 = 50;
    int x2 = 270;
    int y2 = 50;


    //int w
    int xi = 160; //SCREEN SPACE
    int yi = 50;

    float xn = 0; //HOMOGENEOUS SCREEN SPACE 0 - 1

    int tex_Width = 32;
    int texX = 0;
    byte tex[tex_Width];
    bool bit = 1;
    for (int x = 0; x < tex_Width; x++)
    {
        if (bit)
        {
            tex[x] = white; bit = 0;
        }
        else
        {
            tex[x] = black; bit = 1;
        }
    }




    while (1)
    {
        switch (read_Keyboard_Port())
        {
        case LEFT_KEY_MAKE_CODE:
            if (xi > x1) xi--;

            break;


        case RIGHT_KEY_MAKE_CODE:
            if (xi < x2) xi++;
            break;

        case DOWN_KEY_MAKE_CODE:
            if (x2 - x1 > tex_Width)
            {

                x1++;
                x2--;
                xi = x1;
            }
            break;
        case UP_KEY_MAKE_CODE:
            if (x1 > 10 && x2 < 310)
            {

                x1--;
                x2++;
                xi = x1;
            }
            break;

        };







        drawBackgroundColor(16);




        //DRAW SCAN LINE         
        for (int i = x1; i < x2; i++)
        {
            m.normalize(i, x1, x2, &xn);
            m.interp(0, tex_Width, xn, &texX);//get texture x pos
            setPixel(i, yi, tex[texX]);//sample texture for color
        }
        print("xi", 130, y1 - 10, green, black);  printNumberInt(xi, 160, y1 - 10, red, black);
        //




        //DRAW TEXTURE LINE
        for (int i = 0; i < tex_Width; i++) setPixel(i + ((320 - tex_Width) / 2), 120, tex[i]);
        //TEX SAMPLE POINT
        m.normalize(xi, x1, x2, &xn);
        m.interp(0, tex_Width, xn, &texX);//get texture x pos
        drawLine(xi, yi, texX + ((320 - tex_Width) / 2), 120, red);
        print("texX", 130, 130, green, black);  printNumberInt(texX, 160, 130, red, black);
        //



        refreshScreen();
    }
}

void GPGUI::screen_Space_Texture_Mapping_Program()
{
    byte tex[32][32];
    bool bit = true;
    int texWidth = 32;
    int texHeight = 32;
    int tex_Tile_Start_X = 150;
    int tex_Tile_Start_Y = 50;
    int Xa = 10;
    int Xb = 110;
    int ya = 10;
    int yb = 110;
    int yi = 60;
    float ty = 0.0f;
    float xpos = 0;
    float xposEnd = 1;


    for (int y = 0; y < texHeight; y++)
    {
        for (int x = 0; x < texWidth; x++)
        {
            if (bit) {
                tex[y][x] = black;
                bit = false;
            }
            else {
                tex[y][x] = white;
                bit = true;
            }
        }
        if (bit) bit = 0;
        else bit = 1;
    }
    tex[0][0] = green;






    while (1)
    {
        switch (read_Keyboard_Port())
        {
        case F9_KEY_MAKE_CODE:
            if (xpos > 0) xpos -= .01f;
            beep(500, 10);
            break;


        case F10_KEY_MAKE_CODE:
            if (xpos < 1) xpos += .01f;
            beep(500, 10);
            break;

        case F11_KEY_MAKE_CODE:
            if (xposEnd > 0) xposEnd -= .01f;
            beep(500, 10);
            break;

        case F12_KEY_MAKE_CODE:
            if (xposEnd < 1) xposEnd += .01f;
            beep(500, 10);
            break;

        case UP_KEY_MAKE_CODE:
            if (yi > ya + 1) yi--;
            beep(500, 10);
            break;

        case DOWN_KEY_MAKE_CODE:
            if (yi < yb - 1) yi++;
            beep(500, 10);
            break;

        };



        drawBackgroundColor(16);


        //HIGHLIGHT SCREEN SPACE BEING MAPPED
        drawHollowRectangle(Xa, ya, (Xb - Xa), (yb - ya), random(1, 63));

        //m.interpOLATE TEXTURE AND SCREEN SPACE TO DRAW SCANLINE
        m.normalize(yi, ya, yb, &ty);//m.normalize y height
        map_Scanline_From_Texture(Xa, Xb, yi, xpos, xposEnd, ty, tex);




        //DRAW TEXTURE 1:1

        for (int y = 0; y < texHeight; y++)
        {
            for (int x = 0; x < texWidth; x++)
            {
                setPixel(x + tex_Tile_Start_X, y + tex_Tile_Start_Y, tex[y][x]);
            }
        }


        //DRAW SCANLINE REIGON ON TEXTURE 1:1

        int texScreenX;
        m.interp(tex_Tile_Start_X, tex_Tile_Start_X + (texWidth - 1), xpos, &texScreenX);

        int texScreenXend;
        m.interp(tex_Tile_Start_X, tex_Tile_Start_X + (texWidth - 1), xposEnd, &texScreenXend);


        int texScreenY;
        m.interp(tex_Tile_Start_Y, tex_Tile_Start_Y + (texHeight), ty, &texScreenY);

        drawLine(texScreenX, texScreenY, texScreenXend, texScreenY, random(1, 63));
        setPixel(texScreenX, texScreenY, red);
        setPixel(texScreenXend, texScreenY, red);



        refreshScreen();

    }
}

void GPGUI::DEFAULT_PROGRAM()
{

    int zNear_Offset = 10;
    int zFar_Offset = 10;
    int zNear_Plane_Length = 100;
    int zFar_Plane_Length = 50;

    int yStart = 10;
    int xStart = 10;
    int screen_Width = 150;
    int screen_Height = 150;

    byte tex[32][32];

    // tex[random(1, 63)][random(1, 63)];

    while (1)
    {
        tex[random(0, 32)][random(0, 32)] = random(1, 63);
        switch (read_Keyboard_Port())
        {
        case LEFT_KEY_MAKE_CODE:
            break;

        case RIGHT_KEY_MAKE_CODE:
            break;

        case DOWN_KEY_MAKE_CODE:
            break;

        case UP_KEY_MAKE_CODE:
            break;

        case F1_KEY_MAKE_CODE:
            zNear_Offset--;
            beep(500, 10);
            break;

        case F2_KEY_MAKE_CODE:
            zNear_Offset++;
            beep(500, 10);
            break;

        case F3_KEY_MAKE_CODE:
            zFar_Offset--;
            beep(500, 10);
            break;

        case F4_KEY_MAKE_CODE:
            zFar_Offset++;
            beep(500, 10);
            break;

        };




        float vertex[3] =
        {
            2.0f,
            0.0f,
            0.0f
        };




        // m.translate_Matrix(0.5f, 0.5f, 0.5f, vertex);
         //m.scaling_Matrix(0.5f, vertex);
          // m.x_Rotation_Matrix(3.14, vertex);
        // m.y_Rotation_Matrix(.1, vertex);
        // m.z_Rotation_Matrix(3.144, vertex);

        delay(500);

        for (int i = 0; i < 3; i++)
        {
            Serial.println(vertex[i]);
        }
        Serial.println();
        drawBackgroundColor(white);


        // draw_Perspective_Plane(xStart,yStart,screen_Width,screen_Height,zNear_Offset,zFar_Offset,50,100,tex);

        



        refreshScreen();
    }
}

void GPGUI::XY_Plane_Debug_Program()
{

    //vector<vertex3D> world_Vertex_Buffer;


    //
    ////write objects to vector
    //for (int i = 0; i < 10; i++)
    //{   
    //    vertex3D vertex_n;//object template

    //    vertex_n.x = i;  //modify object data
    //    vertex_n.y = 2;
    //    if (i == 500) vertex_n.z = 55;
    //    else vertex_n.z = 0;

    //    world_Vertex_Buffer.push_back(vertex_n);//push object into vector buffer   
    //}


    ////read objects from vector
    //for (int i = 0; i < world_Vertex_Buffer.size(); i++)
    //{
    //    vertex3D  temp = world_Vertex_Buffer[i];//object template copied from vector buffer
    //    float X = temp.x;
    //    float Y = temp.y;
    //    float Z = temp.z;

    //    Serial.println(X);//use data
    //    Serial.println(Y);
    //    Serial.println(Z);
    //    Serial.println();
    //}






    //float cameraX = 0;
    //float cameraY = 0;
    //float cameraZ = 20;

    ////CREATE COPY OF MESH
    //point_Mesh_3D p;
    ////

    ////X Y Z COLOR
    //float world_Space_Verticies_Buffer[20][4];


    ////COPY MESH STRUCTURE TO WORLD SPACE VERTEX BUFFER
    //for (int i = 0; i < 20; i++)
    //{
    //    world_Space_Verticies_Buffer[i][0] = p.vertex_Buffer[i][0];
    //    world_Space_Verticies_Buffer[i][1] = p.vertex_Buffer[i][1];
    //    world_Space_Verticies_Buffer[i][2] = p.vertex_Buffer[i][2];
    //    world_Space_Verticies_Buffer[i][3] = p.vertex_Buffer[i][3];
    //}
    ////

    //while (1)
    //{

    //    switch (read_Keyboard_Port())
    //    {
    //    case LEFT_KEY_MAKE_CODE:
    //        cameraX -= 0.5f;
    //        break;

    //    case RIGHT_KEY_MAKE_CODE:
    //        cameraX += 0.5f;
    //        break;

    //    case UP_KEY_MAKE_CODE:
    //        cameraY -= 0.5f;
    //        break;

    //    case DOWN_KEY_MAKE_CODE:
    //        cameraY += 0.5f;
    //        break;

    //    case HOME_MAKE_CODE:
    //        cameraX = 0;
    //        cameraY = 0;
    //        cameraZ = 20;
    //        break;

    //    case Page_Down_Key_Make_Code:// zoom out
    //        if (cameraZ < 500)
    //        {
    //            cameraZ += .5;
    //        }
    //        break;

    //    case Page_Up_Key_Make_Code://zoom in
    //        if (cameraZ > 1)
    //        {
    //            cameraZ -= .5;
    //        }
    //        break;
    //    };




    //    //MAKE CHANGES TO VERTICIES IN MODEL SPACE

    //    float vertex[3];
    //    //ROTATE EVERY WORLD VERTEX BY ANGLE  delta
    //    for (int i = 0; i < 20; i++)
    //    {
    //        vertex[0] = world_Space_Verticies_Buffer[i][0]; //copy address i
    //        vertex[1] = world_Space_Verticies_Buffer[i][1];
    //        vertex[2] = world_Space_Verticies_Buffer[i][2];
    //        m.z_Rotation_Matrix(2, vertex); //rotate copy
    //        world_Space_Verticies_Buffer[i][0] = vertex[0]; //write over vertex i with rotated in
    //        world_Space_Verticies_Buffer[i][1] = vertex[1];
    //        world_Space_Verticies_Buffer[i][2] = vertex[2];
    //        //repeat for next vertex i
    //    }
    //    // TRANSLATE VERTICES TO SCREEN SPACE

    //    drawBackgroundColor(21);
    //    translate_XY_Veiw_Plane_To_Screen_Plane(0, 0, 240, 240, cameraX, cameraY, cameraZ, black, world_Space_Verticies_Buffer);
    //    print("camera X", 10, 10, green, black);      printNumberInt(cameraX, 70, 10, red, black);
    //    print("camera Y", 10, 20, green, black);     printNumberInt(cameraY, 70, 20, red, black);
    //    print("camera Z", 10, 30, green, black);     printNumberInt(cameraZ, 70, 30, red, black);
    //    refreshScreen();
  //  }
}

void GPGUI::Vertex_Pipeline_Program(bool run)
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
   

    int Z_Cliped = 0;
    int vertex_Counter = 0;
    int bin;
    int av_Count = 0;
    float av_Fps_Count = 0;
    float av_Fps = 0;
    float z = 100;
    int back_Color = 0;
    unsigned long frame_Time;
    float camera_X = 0;
    float camera_Y = 0;
    bool x_Axis_Rotate = false;
    bool y_Axis_Rotate = false;
    bool z_Axis_Rotate = false;
    int projection_Type = 1;

    if (run)
    {   
        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 20, false, &bin);

 
        while (1)
        {
            unsigned long start_Time = millis();
            drawBackgroundColor(back_Color);//CLEAR VGA BACK BUFFER
         
         
            //PASS VERTEX BUFFER TO RENDER PIPELINE
            vertex_Rastor_Pipeline
            (
                40,
                0,
                240,
                camera_X,
                camera_Y,
                z,
                projection_Type,
                black,
                &vertex_Counter,
                &Z_Cliped,
                world_Space_Vertex_Buffer
            );

          

            //CALC FPS###########################################
            unsigned long end_Time = millis();
            frame_Time = end_Time - start_Time;
            float fps = 1000 / frame_Time;
            av_Fps_Count += frame_Time;
            av_Count++;
                if(av_Count > 50) 
                { 
                    av_Fps = 1000 / (av_Fps_Count / 50);
                    av_Fps_Count = 0; 
                    av_Count = 0;
                }
            //###################################################
 
                float zoom_Speed   = 0.01  * frame_Time;
                float rotate_Speed = 0.05  * frame_Time;
                float pan_Speed    = 0.01  * frame_Time;
                switch (read_Keyboard_Port())
                {
                case Page_Up_Key_Make_Code:    if (z > 2) z -= zoom_Speed; break;
                case Page_Down_Key_Make_Code:             z += zoom_Speed; break;
                case F11_KEY_MAKE_CODE:   if (back_Color < 63) back_Color++; beep(1000, 350); break;
                case F12_KEY_MAKE_CODE:   if (back_Color > 0)  back_Color--; beep(800, 350); break;
                case LEFT_KEY_MAKE_CODE:    rotate_Mesh(world_Space_Vertex_Buffer, 1, rotate_Speed); break;//ROTATE RIGHT
                case RIGHT_KEY_MAKE_CODE:   rotate_Mesh(world_Space_Vertex_Buffer, 2, rotate_Speed); break;                
                case UP_KEY_MAKE_CODE:      rotate_Mesh(world_Space_Vertex_Buffer, 0, rotate_Speed); break; //ROTATE FOWORD
                case DOWN_KEY_MAKE_CODE:    break;
                case 16661: camera_X += pan_Speed;   break;//SHIFT DOWN LEFT KEY MAKE     //PAN CAMERA
                case 16662: camera_X -= pan_Speed;   break;//SHIFT DOWN RIGHT KEY MAKE
                case 16663: camera_Y -= pan_Speed;    break;//SHIFT DOWN UP KEY MAKE
                case 16664: camera_Y += pan_Speed;    break;//SHIFT DOWN DOWN KEY MAKE
                }
                print("Vertex Count", 1, 1, red, black); printNumberInt(vertex_Counter, 75, 1, green, black);
                print("FPS", 100, 1, red, black); printNumberInt((int)av_Fps, 120, 1, green, black);
                world_Space_Vertex_Buffer.clear();
        }//END OF RENDER LOOP######################################################################################
    }
}

void GPGUI::color_Pellet_Program()
{




    while (1)
    {
        drawBackgroundColor(0);

        draw_Color_Map_Shaded(true);



        refreshScreen();
    }
}

void GPGUI::SOUNDCARD_TEST_PROGRAM( bool run)
{
    SCC_Enable_Sound(true);
    byte data = 0;
    byte address = 0;
    address_Buff[0] = true;
    int d = 100;
    bool bit = 0;
    byte bit_Pos = 0;
    int x_Pos_Bit_Bar = 20;
    bool byte_Mode = true;
    bool bit_Mode = false;

    while (1)
    {
        switch (read_Keyboard_Port())
        {
        case LEFT_KEY_MAKE_CODE:
            if (address > 0 && byte_Mode)
            {
                d = 100;
                address_Buff[address] = false;
                address--;
                address_Buff[address] = true;
                
                delay(d);
                beep(1500, d/2);
            }

            if (bit_Pos < 7 && bit_Mode)
            {

                bit_Pos++;
                delay(d);
                beep(1500, d / 2);
            }

            break;

        case Left_Key_Shift_Held:
            if (address > 0 && byte_Mode)
            {
                d = 10;
                address_Buff[address] = false;
                address--;
                address_Buff[address] = true;

                delay(d);
                beep(1500, d / 2);
            }
            break;

        case RIGHT_KEY_MAKE_CODE:
            if (address < 95 && byte_Mode)
            {
                d = 100;
                address_Buff[address] = false;
                address++;
                address_Buff[address] = true;
                
                delay(d);
                beep(1500, d / 2);

            }


            if (bit_Pos > 0 && bit_Mode)
            {

                bit_Pos--;
                delay(d);
                beep(1500, d / 2);
            }
            break;

        case Right_Key_Shift_Held:
            if (address < 95 && byte_Mode)
            {
                d = 10;
                address_Buff[address] = false;
                address++;
                address_Buff[address] = true;

                delay(d);
                beep(1500, d / 2);

            }
            break;

        case DOWN_KEY_MAKE_CODE:
            if (data > 0 && byte_Mode)
            {
                d = 100;
                data--;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }
            if (bit_Mode)
            {
                bitWrite(data, bit_Pos, 0);

                d = 100;
                if (data == 255) data = 254;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }
            break;

        case Down_Key_Shift_Held:
            if (data > 0 && byte_Mode)
            {
                d = 10;
                data--;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }

            break;

        case UP_KEY_MAKE_CODE:
            if (data < 254 && byte_Mode)
            { 
                d = 100;
                data++ ;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }

            if (bit_Mode)
            {
                bitWrite(data, bit_Pos, 1);

                d = 100;
                if (data == 255) data = 254;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }
            break;
      

        case ESC_KEY_MAKE_CODE:
            d = 100;
            byte_Mode = true;
            bit_Mode = false;
           
            delay(d);
            beep(1000, d / 2);
           break;

        case ENTER_KEY_MAKE_CODE:
            d = 100;
            byte_Mode = false;
            bit_Mode = true;
            for (int i = 0; i < 8; i++) bitWrite(data, i, sound_reg[address][i]);
            bit_Pos = 0;
            delay(d);
            beep(1000, d / 2);
            break;



        case Up_Key_Shift_Held:
            if (data < 254 && byte_Mode)
            {
                d = 10;
                data++;
                SCC_Write_Byte_To_Reg(address, data);
                delay(d);
                beep(1000, d / 2);
            }
            break;


            //end byte mode




























        case delete_Make_Code:

            data = 0;
            bit_Pos = 0;
            SCC_Write_Byte_To_Reg(address, data);
            delay(d);
            beep(500, d / 2);
            break;

        case Shift_Delete:

            data = 0;
            for (int i = 0; i < 96; i++)
            {
                
                SCC_Write_Byte_To_Reg(i, data);
                address_Buff[i] = false;
                beep(300, 20);
            }
            address = 0;
            address_Buff[0] = true;
            bit_Pos = 0;
            break;



        case HOME_MAKE_CODE:
            address = 0;
            for (int i = 0; i < 96; i++)
            {
                address_Buff[i] = 0;
            }
            address_Buff[0] = 1;
            bit_Pos = 0;
            byte_Mode = true;
            bit_Mode = false;

            break;
     
        };

        
       

        drawBackgroundColor(16);

        x_Pos_Bit_Bar = 10;
        for (int i = 0; i < 8; i++)
        {
            if (sound_reg[address][7 - i])
            {
               
                drawSolidRectangle(x_Pos_Bit_Bar, 185, 20, 50, red);
                printNumberInt(7-i,x_Pos_Bit_Bar, 185, green, red);
                if (7-i == bit_Pos && bit_Mode) drawHollowRectangle(x_Pos_Bit_Bar, 185, 20, 50, green);
            }
            else
            {
             
                drawSolidRectangle(x_Pos_Bit_Bar, 185, 20, 50, black);
                printNumberInt(7 - i, x_Pos_Bit_Bar, 185, green, black);
                if (7-i == bit_Pos && bit_Mode) drawHollowRectangle(x_Pos_Bit_Bar, 185, 20, 50, green);
            }

            x_Pos_Bit_Bar += 23;

        }


        SCC_Registor_Display(sound_reg,address_Buff, true);
        print("ADDRESS", 1, 160, red, black);    printNumberHex(address, 50, 160,  green, black);
        print("BIT POS", 100, 160, red, black);  printNumberInt(bit_Pos, 150, 160, green, black);
        print("DATA", 1, 170, red, black);       printNumberHex(data,    50, 170,  red, black);
        refreshScreen();
       
    }
}

void GPGUI::mouse_Test_Program(bool run)
{





    int controller = 0;
    bool mouse_Left_Click_Flag = false;
    float mouseX = 12;
    float mouseY = 12;

    bool button_Active = false;
    static int active_Button_Id = 0;
    int number_of_buttons = 5;
     char* tag_List[number_of_buttons] =
    {
        "TAG 0",
        "TAG 1",
        "TAG 2",
        "TAG 3",
        "TAG 4",
    };

     int button_Pos_List[number_of_buttons][4] =
     {
         {50,10,50,10},//xStart, yStart, xSize, ysize
         {60,30,50,10},
         {70,50,50,10},
         {80,70,50,10},
         {90,110,34,34}
     };

    vector<button> buttons_Buff;


    //fill vector with buttons
    for (int i = 0; i < number_of_buttons; i++)
    {
            button buttonN;
            buttonN.xStart = button_Pos_List[i][0];//xStart
            buttonN.yStart = button_Pos_List[i][1];//yStart
            buttonN.xSize  = button_Pos_List[i][2];             //xSize
            buttonN.ySize  = button_Pos_List[i][3];             //ySize
            buttonN.onColor = green;          //on color
            buttonN.offColor = grey;            //off color
            buttonN.id = i;  
            buttonN.text = tag_List[i];
            buttons_Buff.push_back(buttonN);
    }

    ////single button
    //button buttonN;
    //buttonN.xStart = random(50, 200);//xStart
    //buttonN.yStart = random(50, 200);//yStart
    //buttonN.xSize = 50;             //xSize
    //buttonN.ySize = 10;             //ySize
    //buttonN.onColor = green;          //on color
    //buttonN.offColor = grey;            //off color
    //buttonN.id = 0;
    //buttonN.text = "TAG";
    //buttons_Buff.push_back(buttonN);





    int ypos = 10;

    if (run)
    {
        while (1)
        {
            drawBackgroundColor(16);




            ypos = 10;

            for (int i = 0; i < 10; i++)
            {
                printNumberInt_7Seg(i, 10, ypos, i);

                ypos += 8;

            }
            printNumberInt_7Seg(random(0,1000), 30, 10, green);

            //draw cursor and buttons, read controller then check cursor pos against button list
            // and return the id of active button
            //mouse_Icon_List_Handle(buttons_Buff, &active_Button_Id, mouseX, mouseY);      
            //mouse_Cursor_Controller_Handle(&mouseX, &mouseY, button_Active, &mouse_Left_Click_Flag, controller);
            //draw_Mouse_Cursor(mouseX, mouseY, button_Active);

            if (active_Button_Id == 999) { button_Active = false; }
            if ( active_Button_Id < 999) { button_Active = true; }
            delay(500);



            //left click event handle
            if (mouse_Left_Click_Flag)
            {
                switch (active_Button_Id)
                {
                case 1: beep(1000, 100); break;//button id 1 flag   
                };

                mouse_Left_Click_Flag = false;
            }


            








           // printNumberInt(mouseX, 10, 10, red, black);
            //printNumberInt(mouseY, 10, 20, red, black);
            //printNumberInt(active_Button_Id, 10, 30, red, black);
            refreshScreen();
        }

    }
}

void GPGUI::perspective_Grid_3D_Program()
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
    int vertex_Count;
    int bin;

    //WORLD VIEW CAMERA SETTINGS
    int projection_Type = 1;
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;
    float z = 255; //camera zoom
    float FOV = 60;


    //GRID SETTINGS
    float plane_Size = 100;
    float y_Offset = 0;
    float z_Offset = 0;
    int back_Color = black;
    int n_Lines    = 14;
    int grid_Color = green;
    bool grid_Color_Highlight = true;

    while (1)
    {
        int line_Id = 0;
        unsigned long current_Time = millis();
        switch (read_Keyboard_Port())
        {
        case Page_Up_Key_Make_Code :
           if(z>255) z-=10;
            beep(500, 5);
            break;


        case Page_Down_Key_Make_Code:
            z+= 10;
            beep(500, 5);
            break;

        case LEFT_KEY_MAKE_CODE:
           
            y_angle += 1.5;
            if(y_angle > 85)   y_angle = 85;
            break;

        case RIGHT_KEY_MAKE_CODE:

            y_angle -= 1.5;      
            if (y_angle < -85) y_angle = -85;
                          
                          break;


        case UP_KEY_MAKE_CODE:
            x_angle -= 1.5;
            if (x_angle < -85) x_angle = -85;

            break;

        case DOWN_KEY_MAKE_CODE:
            x_angle += 1.5;
            if (x_angle > 85) x_angle = 85;

            break;

        case F1_KEY_MAKE_CODE:
            if (n_Lines > 0) n_Lines--;
            beep(800, 300);
            break;

        case F2_KEY_MAKE_CODE:

            if (n_Lines < 50) n_Lines++;
            beep(800, 300);
            break;

        case HOME_MAKE_CODE:
           
            x_angle = 20;
            y_angle = 0;
           if(projection_Type == 1) z = 500;
           if (projection_Type == 0) z = 250;
            beep(1500, 250);
            break;

        case F3_KEY_MAKE_CODE:
            projection_Type = 0;
            beep(750,100);
            break;

        case F4_KEY_MAKE_CODE:
            projection_Type = 1;
            beep(750, 100);
            break;


        case F5_KEY_MAKE_CODE:
            
            break;

        case F6_KEY_MAKE_CODE:
         
            break;
        };

     

        drawBackgroundColor(back_Color);//CLEAR VGA BACK BUFFER
        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 4, true, &bin);

        write_Grid_Plane_To_Vector_XZ
        (
            -plane_Size,//x pa
            -plane_Size,//z
             plane_Size,//x pb
            -plane_Size,//z
            -plane_Size,//x pc
                      0,//z
             plane_Size,//x pd
                      0,//z
               y_Offset,//y offset
                n_Lines,
             grid_Color,
   grid_Color_Highlight,
                line_Id,
               &line_Id,
            world_Space_Vertex_Buffer
        );


        write_Grid_Plane_To_Vector_XY
        (
            -plane_Size,//x pa
            -plane_Size,//y
             plane_Size,//x pb
            -plane_Size,//y
            -plane_Size,//x pc
                      0,//y
             plane_Size,//x pd
                      0,//y
               z_Offset,//z offset
                n_Lines,
             grid_Color,
   grid_Color_Highlight,
                line_Id,
               &line_Id,
            world_Space_Vertex_Buffer
        );



        //LASER LINE
        vertex3D p1, p2;
        p1.c = red;
        p1.id = line_Id;
        p1.mesh_Type = 1;
        p1.x = 0;
        p1.y = 0;
        p1.z = -plane_Size;
        //
        p2.c = red;
        p2.id = line_Id;
        p2.mesh_Type = 1;
        p2.x =  -plane_Size;
        p2.y =  -plane_Size;
        p2.z = 0;
        line_Id++;
        world_Space_Vertex_Buffer.push_back(p1);
        world_Space_Vertex_Buffer.push_back(p2);

  

        //ROTATE WORLD 
        rotate_Mesh(world_Space_Vertex_Buffer, 0, x_angle);
        rotate_Mesh(world_Space_Vertex_Buffer, 1, y_angle);
        rotate_Mesh(world_Space_Vertex_Buffer, 2, z_angle);

        //PASS VERTEX BUFFER TO RENDER PIPELINE
        vertex_Rastor_Pipeline
        (
            0,
            0,
            320,
            240,
            z,
            FOV,
            projection_Type,          
            back_Color,
            &vertex_Count,
            &bin,
            world_Space_Vertex_Buffer
        );

      
        //print("VERTEX COUNT", 0, 0, red, black);   printNumberInt(vertex_Count, 90, 0, green, black);
        //print("Camera Z", 0, 10, red, black);     printNumberDouble(z, 90, 10, green, black);
     
        world_Space_Vertex_Buffer.clear();
        FPS_Counter(current_Time,black);
        refreshScreen();
  
    }//END OF RENDER LOOP######################################################################################


}

void GPGUI::scene_3D_World_Template_Program()
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
    vector<triangle> world_Triangle_Buffer;
    vector<triangle>          OBJ_1_Buffer;
    vector<triangle>          OBJ_2_Buffer;

    int vertex_Count;
    int bin;
    



    //WORLD VIEW CAMERA SETTINGS
    int projection_Type = 1;
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;
    float z = 130; //camera zoom
    float FOV = 60;


    //GRID SETTINGS

   // 1000/1000 = 1 meter
    float ground_Plane_Size = 100000;//100 meters
    float plane_Size = (ground_Plane_Size/1000)/2;
    float y_Offset = plane_Size;
    float x_Offset = 0;
    float z_Offset = 0;

    
    int back_Color = black;//36 sky blue
    int n_Lines = 17;
    int grid_Color = green;
    bool grid_Color_Highlight = true;



    //object position and angle
    float x_angle_OBJ1 = 0.0;
    float y_angle_OBJ1 = 0.0;
    float z_angle_OBJ1 = 0.0;
    float xPos1 = -plane_Size/2;
    float yPos1 = 0;
    float zPos1 = -plane_Size / 2;



    //object position and angle
    float x_angle_OBJ2 = 0.0;
    float y_angle_OBJ2 = 0.0;
    float z_angle_OBJ2 = 0.0;
    float xPos2 = plane_Size / 2;
    float yPos2 = 0;
    float zPos2 = plane_Size / 2;















    while (1)
    {
        
        int line_Id = 0;
        unsigned long current_Time = millis();
        switch (read_Keyboard_Port())
        {
        case Page_Up_Key_Make_Code:
            if (z > 130) z -= 1.8;
            beep(500, 5);
            break;


        case Page_Down_Key_Make_Code:
            z += 1.8;
            beep(500, 5);
            break;

        case LEFT_KEY_MAKE_CODE:

            y_angle += 1.5;
            if (y_angle > 360)   y_angle = 0;
            break;

        case RIGHT_KEY_MAKE_CODE:

            y_angle -= 1.5;
            if (y_angle < 0) y_angle = 359;

            break;


        case DOWN_KEY_MAKE_CODE:
            x_angle -= 1.5;
            if (x_angle < 0) x_angle = 0;

            break;

        case UP_KEY_MAKE_CODE:
            x_angle += 1.5;
            if (x_angle > 90) x_angle = 90;

            break;

        case F1_KEY_MAKE_CODE:
            if (n_Lines > 0) n_Lines--;
            beep(800, 300);
            break;

        case F2_KEY_MAKE_CODE:

            if (n_Lines < 50) n_Lines++;
            beep(800, 300);
            break;

        case HOME_MAKE_CODE:

            x_angle = 0;
            y_angle = 315;
            if (projection_Type == 1) z = 300;
            if (projection_Type == 0) z = 300;
            beep(1500, 250);
            break;

        case F3_KEY_MAKE_CODE:
            projection_Type = 0;
            beep(750, 100);
            break;

        case F4_KEY_MAKE_CODE:
            projection_Type = 1;
            beep(750, 100);
            break;


        case F5_KEY_MAKE_CODE:
            grid_Color_Highlight = true;
            break;

        case F6_KEY_MAKE_CODE:
            grid_Color_Highlight = false;
            break;

        case F7_KEY_MAKE_CODE:
            if (back_Color > 0) back_Color--;
            beep(380, 100);
            break;

        case F8_KEY_MAKE_CODE:
            if (back_Color < 48) back_Color++;
            beep(380, 100);
            break;



        case F9_KEY_MAKE_CODE:
            if (xPos1 < 100) xPos1+= 0.5;
            
            break;


        case F10_KEY_MAKE_CODE:
            if (xPos1 > -100) xPos1 -= 0.5;

            break;


        case F11_KEY_MAKE_CODE:
            if (zPos1 < 100) zPos1 += 0.5;

            break;


        case F12_KEY_MAKE_CODE:
            if (zPos1 > -100) zPos1 -= 0.5;

            break;

        };





        //FRAME LOOP
       // drawBackgroundColor(16);//CLEAR VGA BACK BUFFER

        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 10, true, &bin);

        //GROUND PLANE
        write_Grid_Plane_To_Vector_XZ
        (
            -plane_Size,//x pa
            -plane_Size,//z
            plane_Size,//x pb
            -plane_Size,//z
            -plane_Size,//x pc
            plane_Size,//z
            plane_Size,//x pd
            plane_Size,//z
            y_Offset,//y offset
            n_Lines,
            grid_Color,
            grid_Color_Highlight,
            line_Id,
            &line_Id,
            world_Space_Vertex_Buffer
        );



        ////wall
        //write_Grid_Plane_To_Vector_XY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    z_Offset,//z offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);

        ////WALL
        //write_Grid_Plane_To_Vector_ZY
        //(
        //    -plane_Size,//x pa
        //    -plane_Size,//z
        //    plane_Size,//x pb
        //    -plane_Size,//z
        //    -plane_Size,//x pc
        //    plane_Size,//z
        //    plane_Size,//x pd
        //    plane_Size,//z
        //    x_Offset,//y offset
        //    n_Lines,
        //    grid_Color,
        //    grid_Color_Highlight,
        //    line_Id,
        //    &line_Id,
        //    world_Space_Vertex_Buffer
        //);

        // LOAD OBJECT DATA INTO MODEL SPACE     
      //  translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
      //  translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_2_Buffer);
    
        // SCALE OBJECTS IN MODEL SPACE
        static float SF = 1; 
        if (SF < 1) SF = 1;
        scale_Triangle_Buffer(SF, SF, SF, OBJ_1_Buffer);
        scale_Triangle_Buffer(SF, SF, SF, OBJ_2_Buffer);
        SF = sin(m.deg_To_Rad(y_angle_OBJ1))*2;

       // ROTATE OBJECTS IN MODEL SPACE
        rotate_Obj_Triangle(x_angle_OBJ1,y_angle_OBJ1,z_angle_OBJ1,OBJ_1_Buffer);
        rotate_Obj_Triangle(x_angle_OBJ2,y_angle_OBJ2,z_angle_OBJ2,OBJ_2_Buffer);

        //MOVE OBJECTS IN MODEL SPACE
        translate_Move_Triangle_Buffer(xPos1, sin(m.deg_To_Rad(y_angle_OBJ1)) * (plane_Size / 4), zPos2, OBJ_1_Buffer);
        translate_Move_Triangle_Buffer(xPos2, sin(m.deg_To_Rad(y_angle_OBJ2)) * (plane_Size / 4), zPos2, OBJ_2_Buffer);
     
       // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
        move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer,world_Triangle_Buffer);
        move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_2_Buffer,world_Triangle_Buffer);

        //GLOBAL ROTATE WORLD SPACE AND MOVE VERTEX BUFFER IN WORLD SPACE
        rotate_World_Vertex(x_angle, y_angle, z_angle,world_Space_Vertex_Buffer);
        rotate_Obj_Triangle(x_angle,y_angle,z_angle,world_Triangle_Buffer);

        //PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
        vertex_Rastor_Pipeline
        (
            0,
            0,
            320,
            240,
            z,
            FOV,
            projection_Type,
            back_Color,
            &vertex_Count,
            &bin,
            world_Space_Vertex_Buffer
        );
    
           
        triangle_Rastor_Pipeline
        (
            0,                           // WINDOW START X
            0,                           // WINDOW START Y
            320,                         // WINDOW SIZE X
            240,                         // WINDOW SIZE Y
            z,                           // CAMERA Z
            FOV,                         // CAMMERA FOV
            projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
            0,                           // LIGHT X
            0,                           // LIGHT Y
            0,                           // LIGHT Z
            true,                        // ENABLE BACKFACE CULLING
            false,                       // LIGHT ENABLE
            false,                       // TRIANGLE FILL ENABLE
            &bin,                        // TRIANGLES DRAWN
            world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
        );


        //AUTO ROTATE OBJECTS IN MODEL SPACE
        x_angle_OBJ1 += 1.0;
        y_angle_OBJ1 += 1.5;
        if (x_angle_OBJ1 > 359) x_angle_OBJ1 = 0.0;
        if (y_angle_OBJ1 > 359) y_angle_OBJ1 = 0.0;
        //    
        x_angle_OBJ2 += 1.4;
        y_angle_OBJ2 += 2.5;
        if (x_angle_OBJ2 > 359) x_angle_OBJ2 = 0.0;
        if (y_angle_OBJ2 > 359) y_angle_OBJ2 = 0.0;

          

        

        //CLEAR BUFFERS FOR NEXT FRAME
        world_Space_Vertex_Buffer.clear();
        world_Triangle_Buffer.clear();
        OBJ_1_Buffer.clear();
        OBJ_2_Buffer.clear();
        FPS_Counter(current_Time, black);
        refreshScreen();

    }//END OF RENDER LOOP######################################################################################


}

void GPGUI::camera_Control()
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
    vector<triangle> world_Triangle_Buffer;
    vector<triangle>          OBJ_1_Buffer;
    vector<triangle>          OBJ_2_Buffer;

    int vertex_Count;
    int bin;

    bool dump = false;


    //WORLD VIEW CAMERA SETTINGS
    int projection_Type = 1;
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;
    float z = 130; //camera zoom
    float FOV = 60;



    //GRID SETTINGS

   // 1000/1000 = 1 meter
    float ground_Plane_Size = 100000;//100 meters
    float plane_Size = (ground_Plane_Size / 1000) / 2;
    float y_Offset = plane_Size;
    float x_Offset = 0;
    float z_Offset = 0;


    int back_Color = black;//36 sky blue
    int n_Lines = 17;
    int grid_Color = green;
    bool grid_Color_Highlight = true;



    //object position and angle
    float x_angle_OBJ1 = 0.0;
    float y_angle_OBJ1 = 0.0;
    float z_angle_OBJ1 = 0.0;
    float xPos1 = -plane_Size / 2;
    float yPos1 = 0;
    float zPos1 = -plane_Size / 2;



    //object position and angle
    float x_angle_OBJ2 = 0.0;
    float y_angle_OBJ2 = 0.0;
    float z_angle_OBJ2 = 0.0;
    float xPos2 = plane_Size / 2;
    float yPos2 = 0;
    float zPos2 = plane_Size / 2;

    float cameraX = 0;
    float cameraY = 0;
    float cameraZ = 0;
    float camera_Dir_X = 0;
    float camera_Dir_Z = 1;
    float dirX = 0;
    float dirZ = 1;


    vec3D dir;
    dir.x = 0;
    dir.y = 0;
    dir.z = 1;
    vec3D dirStep;



    float rotate_Speed = .1;


    while (1)
    {

        int line_Id = 0;
        unsigned long current_Time = millis();
        switch (read_Keyboard_Port())
        {
        case Page_Up_Key_Make_Code:
            cameraY += 1.4;
            break;


        case Page_Down_Key_Make_Code:
            cameraY -= 1.4;
            break;

        case LEFT_KEY_MAKE_CODE:

            y_angle += 0.5;
            if (y_angle > 359) y_angle = 0;

            break;

        case RIGHT_KEY_MAKE_CODE:

            y_angle -= 0.5;
            if (y_angle < 0) y_angle = 359;

            break;


        case DOWN_KEY_MAKE_CODE:
            cameraX += dirStep.x * 0.5;
            cameraY += dirStep.y * 0.5;
            cameraZ += dirStep.z * 0.5;
            break;

        case UP_KEY_MAKE_CODE:
            cameraX -= dirStep.x * 0.5;
            cameraY -= dirStep.y * 0.5;
            cameraZ -= dirStep.z * 0.5;
            break;

        case F1_KEY_MAKE_CODE:
            if (n_Lines > 0) n_Lines--;
            beep(800, 300);
            break;

        case F2_KEY_MAKE_CODE:

            if (n_Lines < 50) n_Lines++;
            beep(800, 300);
            break;

        case HOME_MAKE_CODE:

            x_angle = 0;
            y_angle = 315;
            if (projection_Type == 1) z = 300;
            if (projection_Type == 0) z = 300;
            beep(1500, 250);
            break;

        case F3_KEY_MAKE_CODE:
            projection_Type = 0;
            beep(750, 100);
            break;

        case F4_KEY_MAKE_CODE:
            projection_Type = 1;
            beep(750, 100);
            break;


        case F5_KEY_MAKE_CODE:
            grid_Color_Highlight = true;
            break;

        case F6_KEY_MAKE_CODE:
            grid_Color_Highlight = false;
            break;

        case F7_KEY_MAKE_CODE:
            if (back_Color > 0) back_Color--;
            beep(380, 100);
            break;

        case F8_KEY_MAKE_CODE:
            if (back_Color < 48) back_Color++;
            beep(380, 100);
            break;



        case F9_KEY_MAKE_CODE:
            if (xPos1 < 100) xPos1 += 0.5;

            break;


        case F10_KEY_MAKE_CODE:
            if (xPos1 > -100) xPos1 -= 0.5;

            break;


        case F11_KEY_MAKE_CODE:

            break;


        case F12_KEY_MAKE_CODE:

            break;

        };



        dirStep = m.vector_Rotate_Y(dir, 0);

        //FRAME LOOP
       // drawBackgroundColor(16);//CLEAR VGA BACK BUFFER

        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 10, true, &bin);

        //GROUND PLANE
        write_Grid_Plane_To_Vector_XZ
        (
            -plane_Size,//x pa
            -plane_Size,//z
            plane_Size,//x pb
            -plane_Size,//z
            -plane_Size,//x pc
            plane_Size,//z
            plane_Size,//x pd
            plane_Size,//z
            y_Offset,//y offset
            n_Lines,
            grid_Color,
            grid_Color_Highlight,
            line_Id,
            &line_Id,
            world_Space_Vertex_Buffer
        );




        // LOAD OBJECT DATA INTO MODEL SPACE     
       // translate_Vertex_Array_To_Triangle_Buff_UNIT_CUBE(UNIT_CUBE_vertex_Array, UNIT_CUBE_faces, OBJ_1_Buffer);
      //  translate_Vertex_Array_To_Triangle_Buff_UNIT_ICOSAHEDRON(UNIT_ICOSAHEDRON_MESH_Vertex_Array, UNIT_ICOSAHEDRON_MESH_face_Array, OBJ_2_Buffer);


        // ROTATE OBJECTS IN MODEL SPACE
       // rotate_Obj_Triangle(x_angle_OBJ1, y_angle_OBJ1, z_angle_OBJ1, OBJ_1_Buffer);
       // rotate_Obj_Triangle(x_angle_OBJ2, y_angle_OBJ2, z_angle_OBJ2, OBJ_2_Buffer);

        //MOVE OBJECTS IN MODEL SPACE
       // translate_Move_Triangle_Buffer(xPos1, sin(m.deg_To_Rad(y_angle_OBJ1)) * (plane_Size / 4), zPos2, OBJ_1_Buffer);
       // translate_Move_Triangle_Buffer(xPos2, sin(m.deg_To_Rad(y_angle_OBJ2)) * (plane_Size / 4), zPos2, OBJ_2_Buffer);

        // MOVE OBJECTS FROM MODEL SPACE TO WORLD SPACE
       // move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_1_Buffer, world_Triangle_Buffer);
       // move_Obj_Triangle_Buffer_To_World_Triangle_Buffer(OBJ_2_Buffer, world_Triangle_Buffer);

        //GLOBAL ROTATE WORLD SPACE AND MOVE VERTEX BUFFER IN WORLD SPACE
       //  rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);





       // rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);


       // rotate_Obj_Triangle(x_angle, y_angle, z_angle, world_Triangle_Buffer);
        rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);

        // translate_Move_Triangle_Buffer(cameraX, cameraY, cameraZ, world_Triangle_Buffer);
        translate_Move_Vertex_Buffer(cameraX, cameraY, cameraZ, world_Space_Vertex_Buffer);


        //PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
        vertex_Rastor_Pipeline
        (
            0,
            0,
            320,
            240,
            z,
            FOV,
            projection_Type,
            back_Color,
            &vertex_Count,
            &bin,
            world_Space_Vertex_Buffer
        );


        //triangle_Rastor_Pipeline
        //(
        //    0,                           // WINDOW START X
        //    0,                           // WINDOW START Y
        //    320,                         // WINDOW SIZE X
        //    240,                         // WINDOW SIZE Y
        //    z,                           // CAMERA Z
        //    FOV,                         // CAMMERA FOV
        //    projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
        //    0,                           // LIGHT X
        //    0,                           // LIGHT Y
        //    0,                           // LIGHT Z
        //    true,                        // ENABLE BACKFACE CULLING
        //    false,                       // LIGHT ENABLE
        //    false,                       // TRIANGLE FILL ENABLE
        //    &bin,                        // TRIANGLES DRAWN
        //    world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
        //);


        //AUTO ROTATE OBJECTS IN MODEL SPACE
        x_angle_OBJ1 += 1.0;
        y_angle_OBJ1 += 1.5;
        if (x_angle_OBJ1 > 359) x_angle_OBJ1 = 0.0;
        if (y_angle_OBJ1 > 359) y_angle_OBJ1 = 0.0;
        //    
        x_angle_OBJ2 += 1.4;
        y_angle_OBJ2 += 2.5;
        if (x_angle_OBJ2 > 359) x_angle_OBJ2 = 0.0;
        if (y_angle_OBJ2 > 359) y_angle_OBJ2 = 0.0;





        //CLEAR BUFFERS FOR NEXT FRAME
        world_Space_Vertex_Buffer.clear();
        world_Triangle_Buffer.clear();
        OBJ_1_Buffer.clear();
        OBJ_2_Buffer.clear();
        FPS_Counter(current_Time, black);
        refreshScreen();

    }//END OF RENDER LOOP######################################################################################


}

void GPGUI::plane_Intersect_Program()
{
    //CREATE BUFFER TO HOLD ALL WORLD VERTICIES
    vector<vertex3D> world_Space_Vertex_Buffer;
    vector<triangle> world_Triangle_Buffer;




    int vertex_Count;
    int bin;
    bool dump = false;

    //WORLD VIEW CAMERA SETTINGS
    int projection_Type = 1;
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;
    float z = 130; //camera zoom
    float FOV = global_FOV;



    //GRID SETTINGS  
    float ground_Plane_Size = 100000;//100 meters
    float plane_Size = (ground_Plane_Size / 1000) / 2;
    float y_Offset = plane_Size;


    int back_Color = black;//36 sky blue
    int n_Lines = 17;
    int grid_Color = green;
    bool grid_Color_Highlight = true;







    float cameraX = 0;
    float cameraY = 0;
    float cameraZ = 0;
    float camera_Dir_X = 0;
    float camera_Dir_Z = 1;
    float dirX = 0;
    float dirZ = 1;


    vec3D dir;
    dir.x = 0;
    dir.y = 0;
    dir.z = 1;
    vec3D dirStep;



    float rotate_Speed = .1;


    while (1)
    {

        int line_Id = 0;
        unsigned long current_Time = millis();
        switch (read_Keyboard_Port())
        {
        case Page_Up_Key_Make_Code:
            cameraY += .2;
            break;


        case Page_Down_Key_Make_Code:
            cameraY -= .2;
            break;

        case LEFT_KEY_MAKE_CODE:

            y_angle += 0.5;
            if (y_angle > 359) y_angle = 0;

            break;

        case RIGHT_KEY_MAKE_CODE:

            y_angle -= 0.5;
            if (y_angle < 0) y_angle = 359;

            break;


        case DOWN_KEY_MAKE_CODE:
            cameraX -= dirStep.x * 0.5;
            cameraY -= dirStep.y * 0.5;
            cameraZ -= dirStep.z * 0.5;
            break;

        case UP_KEY_MAKE_CODE:
            cameraX += dirStep.x * 0.5;
            cameraY += dirStep.y * 0.5;
            cameraZ += dirStep.z * 0.5;
            break;

        case F1_KEY_MAKE_CODE:
            if (n_Lines > 0) n_Lines--;
            beep(800, 300);
            break;

        case F2_KEY_MAKE_CODE:

            if (n_Lines < 50) n_Lines++;
            beep(800, 300);
            break;

        case HOME_MAKE_CODE:

            x_angle = 0;
            y_angle = 315;
            if (projection_Type == 1) z = 300;
            if (projection_Type == 0) z = 300;
            beep(1500, 250);
            break;

        case F3_KEY_MAKE_CODE:
            projection_Type = 0;
            beep(750, 100);
            break;

        case F4_KEY_MAKE_CODE:
            projection_Type = 1;
            beep(750, 100);
            break;


        case F5_KEY_MAKE_CODE:
            grid_Color_Highlight = true;
            break;

        case F6_KEY_MAKE_CODE:
            grid_Color_Highlight = false;
            break;

        case F7_KEY_MAKE_CODE:
            if (back_Color > 0) back_Color--;
            beep(380, 100);
            break;

        case F8_KEY_MAKE_CODE:
            if (back_Color < 48) back_Color++;
            beep(380, 100);
            break;



        case F9_KEY_MAKE_CODE:


            break;


        case F10_KEY_MAKE_CODE:


            break;


        case F11_KEY_MAKE_CODE:

            break;


        case F12_KEY_MAKE_CODE:
            dump = true;
            break;

        };



        dirStep = m.vector_Rotate_Y(dir, 0);

        //FRAME LOOP
       // drawBackgroundColor(16);//CLEAR VGA BACK BUFFER


        vec3D P1;
        P1.x = 0;
        P1.y = 0;
        P1.z = 0;

        vec3D P2;
        P2.x = 0;
        P2.y = 0;
        P2.z = -10;

        vec3D P3;
        P3.x = -10;
        P3.y = 0;
        P3.z = 0;



        triangle t;
        t.p1.x = P1.x;
        t.p1.y = P1.y;
        t.p1.z = P1.z;

        t.p2.x = P2.x;
        t.p2.y = P2.y;
        t.p2.z = P2.z;

        t.p3.x = P3.x;
        t.p3.y = P3.y;
        t.p3.z = P3.z;
        t.face_Color = green;
        t.id = 1;








        write_Origin_Cross_Mesh_To_Vector(world_Space_Vertex_Buffer, 10, true, &bin);
        world_Triangle_Buffer.push_back(t);
        vec3D plane_Norm = m.get_Plane_Normal_Vector(P1, P2, P3);
















        rotate_Triangle_Vector(world_Triangle_Buffer, 1, y_angle);
        translate_Move_Triangle_Buffer(cameraX, cameraY, cameraZ, world_Triangle_Buffer);
        rotate_World_Vertex(x_angle, y_angle, z_angle, world_Space_Vertex_Buffer);
        translate_Move_Vertex_Buffer(cameraX, cameraY, cameraZ, world_Space_Vertex_Buffer);


        //PASS VERTEX AND TRIANGLE BUFFER TO RENDER PIPELINE
        vertex_Rastor_Pipeline
        (
            0,
            0,
            320,
            240,
            z,
            FOV,
            projection_Type,
            back_Color,
            &vertex_Count,
            &bin,
            world_Space_Vertex_Buffer
        );





        if (dump)
        {
            world_Triangle_Buffer_Serial_Dump(world_Triangle_Buffer);
            dump = false;
            delay(500);
        }

        triangle_Rastor_Pipeline
        (
            0,                           // WINDOW START X
            0,                           // WINDOW START Y
            320,                         // WINDOW SIZE X
            240,                         // WINDOW SIZE Y
            z,                           // CAMERA Z
            FOV,                         // CAMMERA FOV
            projection_Type,             // 0 = ORTHO 1 = PERSPECTIVE
            0,                           // LIGHT X
            0,                           // LIGHT Y
            0,                           // LIGHT Z
            false,                        // ENABLE BACKFACE CULLING
            false,                       // LIGHT ENABLE
            false,                       // TRIANGLE FILL ENABLE
            &bin,                        // TRIANGLES DRAWN
            world_Triangle_Buffer        // WORLD SPACE TRIANGLE BUFFER
        );






        //CLEAR BUFFERS FOR NEXT FRAME
        world_Triangle_Buffer.clear();
        world_Space_Vertex_Buffer.clear();
        FPS_Counter(current_Time, black);
        refreshScreen();

    }//END OF RENDER LOOP######################################################################################


}
