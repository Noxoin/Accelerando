#include "song_player.h"


const int song_MaxWidth = 8191;//MAX VALUE. TEXUTRE LIMIT

const int bitMapCellW = 45;
const int bitMapCellH = 75;

const int SCREEN_WIDTH = 1280;//TEMP
const int SCREEN_HEIGHT = 720;//TEMP

const int BACKGROUND = 0;
const int BAR = 1;
const int MUSIC_SHEET = 2;

const int LINE_GAP = 15;
const int EIGTHCHAIN_X_OFFSET = 23;
const int EIGTHCHAIN_X_THICKNESS = LINE_GAP/2 + 3;

const int startPoint = 30;
const int headWidth = 3*bitMapCellW;
const int renderHeight = 170;


SDL_Rect copy;
SDL_Rect paste;
SDL_Rect draw;


//Blit the notes to the music sheet
void SongPlayer::copySymbolCellToSurface( SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int row, int col, int x, int y )
{
    copy.x = (col-1)*bitMapCellW;
    copy.y = (row-1)*bitMapCellH;
    copy.w = bitMapCellW;
    copy.h = bitMapCellH;

    paste.x = x;
    paste.y = y;
    paste.w = bitMapCellW;
    paste.h = bitMapCellH;
    
    SDL_RenderCopy(gRenderer, SymTexture, &copy, &paste);
}

//Set background with clef, key sig, and time sig
void SongPlayer::setPlayscreenBackground ( SDL_Renderer *gRenderer, LTexture* gBuffer )
{ 
    SDL_RenderCopy(gRenderer, gBuffer[BACKGROUND].mTexture, NULL, NULL);
    SDL_RenderPresent( gRenderer );
}

//reset the note value. Add empty value b/w "half keys"
int SongPlayer::resetNoteValue(Note note)
{
    int noteValue;
    if (note.value >=60 && note.value <=64){
        noteValue = note.value;
    }
    else if (note.value >=65 && note.value <= 71){
        noteValue = note.value + 1;
    }
    else if (note.value >=72 && note.value <=76){
        noteValue = note.value + 2;
    }
    else if (note.value >=77 && note.value <=83){
        noteValue = note.value + 3;
    }
    else if (note.value >= 84){
        noteValue = note.value + 4;
    }
    else{
        noteValue = -2000;
    }
    return noteValue;
}

//Hardcode the note positions 
int SongPlayer::lookUpNote(int rowHeight, int currNoteValue, int sharpTrue)
{
    int notePos;
    switch(currNoteValue){
        case 60:
            notePos = rowHeight + 27;
            break;
        case 61:
            notePos = rowHeight + 27 + 7*(sharpTrue - 1);
            break;
        case 62:
            notePos = rowHeight + 20;
            break;
        case 63:
            notePos = rowHeight + 20 + 7*(sharpTrue - 1);
            break;        
        case 64:
            notePos = rowHeight + 13;
            break;
        case 66:
            notePos = rowHeight + 4;
            break;
        case 67:
            notePos = rowHeight + 4 + 6*(sharpTrue - 1);
            break;
        case 68:
            notePos = rowHeight - 2;
            break;
        case 69:
            notePos = rowHeight -2 + 9*(sharpTrue - 1);
            break;
        case 70:
            notePos = rowHeight - 11;
            break;
        case 71:
            notePos = rowHeight - 11 + 7*(sharpTrue - 1);
            break;
        case 72:
            notePos = rowHeight - 18;
            break;
        case 74:
            notePos = rowHeight - 25;
            break;
        case 75:
            notePos = rowHeight - 25 + 8*(sharpTrue - 1);
            break;
        case 76:
            notePos = rowHeight - 33;
            break;
        case 77:
            notePos = rowHeight - 33 + 7*(sharpTrue - 1);
            break;
        case 78:
            notePos = rowHeight - 40;
            break;
        case 80:
            notePos = rowHeight - 48;
            break;
        case 81:
            notePos = rowHeight - 48 + 7*(sharpTrue - 1);
            break;
        case 82:
            notePos = rowHeight - 55;
            break;
        case 83:
            notePos = rowHeight - 55 + 7*(sharpTrue - 1);;
            break;
        case 84:
            notePos = rowHeight - 62;
            break;
        case 85:
            notePos = rowHeight - 62 + 7*(sharpTrue - 1);;
            break;
        case 86:
            notePos = rowHeight - 69;
            break;
        case 88:
            notePos = rowHeight - 76;
            break;
        default:
            notePos = rowHeight + LINE_GAP*2  - bitMapCellH/2 + (68-int(currNoteValue/2)*2)*(LINE_GAP/4) + 6;
    }
    return notePos;
}

void SongPlayer::drawExtraLines(SDL_Renderer *gRenderer, int currNoteValue, int extraLineX, int notePos, int rowHeight, bool sharpTrue)
{
    int notePos_84;
    if(currNoteValue == 60 || currNoteValue == 84 || currNoteValue == 88 || (currNoteValue == 61 && sharpTrue)){
        draw.x = extraLineX;
        draw.y = notePos + bitMapCellH/2+12;
        draw.w = bitMapCellW/2+5;
        draw.h = 2;
        SDL_RenderFillRect(gRenderer, &draw);
    }
    if(currNoteValue == 86 || currNoteValue == 88 || currNoteValue == 85 || (currNoteValue == 83 && !sharpTrue)){
        notePos_84 = lookUpNote(rowHeight, 84, 1);
        draw.x = extraLineX;
        draw.y = notePos_84 + bitMapCellH/2+12;
        draw.w = bitMapCellW/2+5;
        draw.h = 2;
        SDL_RenderFillRect(gRenderer, &draw);
    }
}

void SongPlayer::drawAccKey(SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int keyNum, int currNoteValue, int x, int y, bool goingUp)
{
    bool drawAccKey = false;
    bool inKeySig = false;
    bool natTrue = false;
    int keyPos;

    if(keyNum == 0){
        if(goingUp && (currNoteValue%2 == 1)){
            keyPos = lookUpNote(y, currNoteValue, 1);
            copySymbolCellToSurface(gRenderer, SymTexture, 6, 1, x-30, keyPos+30);
        }
        if(!goingUp && (currNoteValue%2 == 1)){
            keyPos = lookUpNote(y, currNoteValue, 1);
            copySymbolCellToSurface(gRenderer, SymTexture, 6, 3, x-30, keyPos+30);
        }
    }
    else if(keyNum > 0){
        for(int i = 0; i < keyNum; i++){
            if(i == 0){//G major
                if(currNoteValue == 81 || currNoteValue == 67 ){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if((currNoteValue == 80 || currNoteValue == 66) && (keyNum < 6)){
                    keyPos = lookUpNote(y, currNoteValue, 1);//4
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 1){//D major
                if(currNoteValue == 75 || currNoteValue == 61){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if((currNoteValue == 74 || currNoteValue == 60 || currNoteValue == 88) &&(keyNum < 7)){
                    keyPos = lookUpNote(y, currNoteValue, 1);//1
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 2){//A major
                if(currNoteValue == 83 || currNoteValue == 69){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 82 || currNoteValue == 68){
                    keyPos = lookUpNote(y, currNoteValue, 1);//5
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 3){//E major
                if(currNoteValue == 77 || currNoteValue == 63){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 76 || currNoteValue == 62){
                    keyPos = lookUpNote(y, currNoteValue, 1);//2
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 4){//B major
                if(currNoteValue == 71 || currNoteValue == 85){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 70 || currNoteValue == 84){
                    keyPos = lookUpNote(y, currNoteValue, 1);//6
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 5){//F# major
                //if(currNoteValue == 80 or 66){
                //    remap = true;
                //}
                if(currNoteValue == 78 || currNoteValue == 64){
                    keyPos = lookUpNote(y, currNoteValue, 1);//3
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 6){//C# major
                //if(currNoteValue == 74 or 88){
                //    remap = true;
                //}
                if(currNoteValue == 72 || currNoteValue == 86){
                    keyPos = lookUpNote(y, currNoteValue, 1);//7
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(!inKeySig && (i == keyNum -1) && currNoteValue%2 == 1){
                drawAccKey = true;
                keyPos = lookUpNote(y, currNoteValue, 1);
            }
            if(drawAccKey){
                copySymbolCellToSurface(gRenderer, SymTexture, 6, 1 + 1*natTrue, x-30, keyPos+30);
                drawAccKey = false;
            }
        }
        inKeySig = false;
    }
    else{
        int maxFlats = -keyNum;
        for(int i = 0; i < maxFlats; i++){
            if(i == 0){//F major
                if(currNoteValue == 71 || currNoteValue == 85){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if((currNoteValue == 72 || currNoteValue == 86) && (maxFlats < 6) ){
                    keyPos = lookUpNote(y, currNoteValue, 0);//7
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 1){//Bb major
                if(currNoteValue == 63 || currNoteValue == 77){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if((currNoteValue == 64 || currNoteValue == 78) && (maxFlats < 7)){
                    keyPos = lookUpNote(y, currNoteValue, 0);//3
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 2){//Eb major
                if(currNoteValue == 69 || currNoteValue == 83){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 70 || currNoteValue == 84){
                    keyPos = lookUpNote(y, currNoteValue, 0);//6
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 3){//Ab major
                if(currNoteValue == 75 || currNoteValue == 61){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 76 || currNoteValue == 62){
                    keyPos = lookUpNote(y, currNoteValue, 0);//2
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 4){//Db major
                if(currNoteValue == 67 || currNoteValue == 81){
                    drawAccKey = false;
                    inKeySig = true;
                }
                if(currNoteValue == 68 || currNoteValue == 82){
                    keyPos = lookUpNote(y, currNoteValue, 0);//5
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 5){//Gb major
                //if(currNoteValue == 72){
                //    drawAccKey = false;
                //}
                if(currNoteValue == 74 || currNoteValue == 60 || currNoteValue == 88){
                    keyPos = lookUpNote(y, currNoteValue, 0);//1
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(i == 6){//Cb major
                //if(currNoteValue == 64){
                //    drawAccKey = false;
                //}
                if(currNoteValue == 66 || currNoteValue == 80){
                    keyPos = lookUpNote(y, currNoteValue, 0);//4
                    drawAccKey = true;
                    natTrue = true;
                }
            }
            if(!inKeySig &&  (i == maxFlats -1) && (currNoteValue%2 == 1)){
                drawAccKey = true;
                keyPos = lookUpNote(y, currNoteValue, 0);
            }
            if(drawAccKey){
                copySymbolCellToSurface(gRenderer, SymTexture, 6, 3 - 1*natTrue, x-30, keyPos+30);
                drawAccKey = false;
            }
        }
        inKeySig = false;
    }
}

void SongPlayer::drawKeySig(SDL_Renderer *gRenderer, SDL_Texture *SymTexture, int keyNum, int x, int y)
{
    int keyPos;
    if(keyNum > 0){
        for(int i = 0; i < keyNum; i++){
            if(i == 0){//G major
                keyPos = lookUpNote(y, 80, 1);//4
            }
            if(i == 1){//D major
                keyPos = lookUpNote(y, 74, 1);//1
            }
            if(i == 2){//A major
                keyPos = lookUpNote(y, 82, 1);//5
            }
            if(i == 3){//E major
                keyPos = lookUpNote(y, 76, 1);//2
            }
            if(i == 4){//B major
                keyPos = lookUpNote(y, 70, 1);//6
            }
            if(i == 5){//F# major
                keyPos = lookUpNote(y, 78, 1);//3
            }
            if(i == 6){//C# major
                keyPos = lookUpNote(y, 72, 1);//7
            }
            copySymbolCellToSurface(gRenderer, SymTexture, 6, 1, x-20+i*10, keyPos+30);
        }
    }
    else{
        int maxFlats = -keyNum;
        for(int i = 0; i < maxFlats; i++){
            if(i == 0){//F major
                keyPos = lookUpNote(y, 72, 0);//7
            }
            if(i == 1){//Bb major
                keyPos = lookUpNote(y, 78, 0);//3
            }
            if(i == 2){//Eb major
                keyPos = lookUpNote(y, 70, 0);//6
            }
            if(i == 3){//Ab major
                keyPos = lookUpNote(y, 76, 0);//2
            }
            if(i == 4){//Db major
               keyPos = lookUpNote(y, 68, 0);//5
            }
            if(i == 5){//Gb major
                keyPos = lookUpNote(y, 74, 0);//1
            }
            if(i == 6){//Cb major
                keyPos = lookUpNote(y, 66, 0);//4
            }
            copySymbolCellToSurface(gRenderer, SymTexture, 6, 3, x-20+i*10, keyPos+30);
        }
    }
}

//Projecting Image
void SongPlayer::createMusicSurface ( SDL_Renderer *gRenderer, LTexture gSymbol, LTexture* gBuffer, Song* song )
{
    screen_width = SCREEN_WIDTH;
    screen_height = SCREEN_HEIGHT;
    int xRender = headWidth + startPoint;
    int barWidth = bitMapCellW*(song->bars[0].tsig_numerator*8/song->bars[0].tsig_denominator);
    int eigthChainDouble = 0;
    int eigthChain = 0;
    int restDuration = 0;
    
    int keyNum;

    Note prevNote;
    Note currNote;
    Note nextNote;
    Note nextnextNote;
    
    int currNoteValue;
    int nextNoteValue;
    int nextNextNoteValue;

    int notePos;
    int nextNotePos;
    int nextNextNotePos;

    int eigthChainX;
    int extraLineX;

    LTexture gBackground;

    int sharpTrue = 1;
    bool goingUp = true;

    int rowNum = 0;
    int pageNum = 0;
    int ROW_Y[4];
    ROW_Y[0] = 25 + 37;//First row, first line
    ROW_Y[1] = ROW_Y[0] + 98 + 10 + 25 + 37;//Second row, first line
    ROW_Y[2] = ROW_Y[1] + 98 + 10 + 25 + 37;//Third row, first line
    ROW_Y[3] = ROW_Y[2] + 98 + 10 + 25 + 37;//Fourth row, first line
    //printf("ROW 0 = %d\n", ROW_Y[0]);
    //printf("ROW 1 = %d\n", ROW_Y[1]);
    //printf("ROW 2 = %d\n", ROW_Y[2]);
    //printf("ROW 3 = %d\n", ROW_Y[3]);
    
    ROW_X = int((screen_width - headWidth - 20)/barWidth) * barWidth;
    
    //Load the background image
    if( !gBackground.loadFromFile( gRenderer, "res/images/bg.png" ) ){
        printf( "Failed to load background image\n" );
    }

	//Load symbols' map
    if( !gSymbol.loadFromFile( gRenderer, "res/images/symbolsMap.png" ) ){
        printf( "Failed to load music symbols\n" );
    }
    
    //Create a blank target texture for loading background
    if( !gBuffer[BACKGROUND].createBlank( gRenderer, screen_width, screen_height, SDL_TEXTUREACCESS_TARGET ) ){
        printf( "Failed to create blank texture for loading background!\n" );
    }

    //Load the bar image
    if( !gBuffer[BAR].loadFromFile( gRenderer, "res/images/bar.png" ) ){
        printf( "Failed to load bar!\n" );
    }
    
    //Create a blank target texture for loading music sheet
    if( !gBuffer[MUSIC_SHEET].createBlank( gRenderer, song_MaxWidth, screen_height, SDL_TEXTUREACCESS_TARGET ) ){
        printf( "Failed to create blank tuexture for loading music sheet!\n" );
    }

    //Turn on Alpha(Render + Texture). Important
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
    gBuffer[BACKGROUND].setBlendMode(SDL_BLENDMODE_BLEND);
    //Set gBuffer[BACKGROUND] as render target
    SDL_SetRenderTarget( gRenderer, gBuffer[BACKGROUND].mTexture );

    //Clear screen with transparent background
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0x0 );//transparent
    SDL_RenderClear( gRenderer );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );//black, non-transparent
    
    gBackground.render(gRenderer, 0,0);


    SDL_SetRenderTarget( gRenderer, NULL );
    //Turn on Alpha(Texture). Important
    gBuffer[MUSIC_SHEET].setBlendMode(SDL_BLENDMODE_BLEND);
    //Set gBuffer[MUSIC_SHEET] as render target
    SDL_SetRenderTarget( gRenderer, gBuffer[MUSIC_SHEET].mTexture );

    //Clear screen with transparent background
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0x0 );//transparent
    SDL_RenderClear( gRenderer );

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );//black, non-transparent

    //key signature information
    keyNum = song->bars[0].key;
    printf("the keySig is %d\n", keyNum);
    //if(keyNum < 0){
    if(keyNum >= 249){
        sharpTrue = 0;
        keyNum = keyNum - 256;
    }


    //Apply notes onto play texture
    for (int bar = 0; bar < song->length; bar++){
        //printf( "bar key sig = %d\n", song->bars[bar].key );
        if ( (xRender+barWidth) > (screen_width - 20) ) {
            xRender = headWidth + startPoint;
            rowNum += 1;
            
            if (rowNum == 4){
                rowNum = 0;
                pageNum++;
            }
        }
        //bars with only rests
        if (song->bars[bar].length == 0){
            int fullRests = (song->bars[bar].tsig_numerator * 8/song->bars[bar].tsig_denominator)/8;
            int lastRestDuration = (song->bars[bar].tsig_numerator * 8/song->bars[bar].tsig_denominator)%8;

            for (int i = 0; i < fullRests; i++ ){
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 2, 8, pageNum*screen_width + xRender + bitMapCellW*i*8, ROW_Y[rowNum] + LINE_GAP*2 - bitMapCellH/2);
            }
            if (lastRestDuration != 0){
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 2, lastRestDuration, pageNum*screen_width + xRender + bitMapCellW*fullRests*8, ROW_Y[rowNum] + LINE_GAP*2 - bitMapCellH/2);
            }			
        }
        else{
            //if the bar begins with a rest
            restDuration = song->bars[bar].notes[0].time;
            if (restDuration != 0){
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 2, restDuration, pageNum*screen_width + xRender + bitMapCellW*currNote.time, ROW_Y[rowNum] + LINE_GAP*2 - bitMapCellH/2);
            }

            for (int note = 0; note < song->bars[bar].length; note++){
                prevNote = song->bars[bar].notes[note-1];
                currNote = song->bars[bar].notes[note];
                nextNote = song->bars[bar].notes[note+1];
                nextnextNote = song->bars[bar].notes[note+2];
                restDuration = nextNote.time - currNote.duration - currNote.time;
   
                if((currNote.value <= nextNote.value) || (currNote.value >= prevNote.value)){
                    goingUp = true;
                }
                else{
                    goingUp = false;
                }

                //reset the note value. Add empty value b/w "half keys"
                currNoteValue = resetNoteValue(currNote);
                //loop up the hardcoded note position
                notePos = lookUpNote(ROW_Y[rowNum], currNoteValue, sharpTrue);

                if (currNote.duration == 1 && ((nextNote.time == currNote.time && nextnextNote.duration == 1 && nextnextNote.time == currNote.time+1) || eigthChainDouble !=0)){
                    copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 1, 2, pageNum*screen_width + xRender + bitMapCellW*currNote.time, notePos);
                    drawAccKey(gRenderer, gSymbol.mTexture, keyNum, currNoteValue, pageNum*screen_width + xRender + bitMapCellW*currNote.time, ROW_Y[rowNum], goingUp);
                    //draw extra lines for specific notes
                    extraLineX = pageNum*screen_width + xRender + bitMapCellW*currNote.time + 1;
                    drawExtraLines(gRenderer, currNoteValue, extraLineX, notePos, ROW_Y[rowNum], sharpTrue);

                    if (eigthChainDouble==0 && currNoteValue>=60){
                        //reset the note value. Add empty value b/w "half keys"
                        nextNextNoteValue = resetNoteValue(nextnextNote);
                        nextNextNotePos = lookUpNote(ROW_Y[rowNum], nextNextNoteValue, sharpTrue);
                        eigthChainX = pageNum*screen_width + xRender + bitMapCellW*currNote.time + EIGTHCHAIN_X_OFFSET;
                        for(int m = 0; m < EIGTHCHAIN_X_THICKNESS; m++){
                            SDL_RenderDrawLine(gRenderer, eigthChainX, notePos+m, eigthChainX+bitMapCellW, nextNextNotePos+m);
                        }
                        eigthChainDouble = 3;
                    }
                    else{
                        eigthChainDouble--;
                    }
                }
                else if (currNote.duration == 1 && ((nextNote.duration == 1 && nextNote.time == currNote.time+1) || eigthChain != 0)){
                    copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 1, 2, pageNum*screen_width + xRender + bitMapCellW*currNote.time, notePos);
                    drawAccKey(gRenderer, gSymbol.mTexture, keyNum, currNoteValue, pageNum*screen_width + xRender + bitMapCellW*currNote.time, ROW_Y[rowNum], goingUp);
                    //draw extra lines for specific notes
                    extraLineX = pageNum*screen_width + xRender + bitMapCellW*currNote.time + 1;
                    drawExtraLines(gRenderer, currNoteValue, extraLineX, notePos, ROW_Y[rowNum], sharpTrue);

                    if (eigthChain==0 && currNoteValue>=60){
                        //reset the note value. Add empty value b/w "half keys"
                        nextNoteValue = resetNoteValue(nextNote);
                        nextNotePos = lookUpNote(ROW_Y[rowNum], nextNoteValue, sharpTrue);
                        eigthChainX = pageNum*screen_width + xRender + bitMapCellW*currNote.time + EIGTHCHAIN_X_OFFSET;
                        for(int m = 0; m < EIGTHCHAIN_X_THICKNESS; m++){
                            SDL_RenderDrawLine(gRenderer, eigthChainX, notePos+m, eigthChainX+bitMapCellW, nextNotePos+m);
                        }
                        eigthChain = 1;
                    }
                    else{
                        eigthChain--;
                    }
                }
                else{
                    copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 1, currNote.duration, pageNum*screen_width + xRender + bitMapCellW*currNote.time, notePos);
                    drawAccKey(gRenderer, gSymbol.mTexture, keyNum, currNoteValue, pageNum*screen_width + xRender + bitMapCellW*currNote.time, ROW_Y[rowNum], goingUp);
                    //draw extra lines for specific notes
                    extraLineX = pageNum*screen_width + xRender + bitMapCellW*currNote.time + 1;
                    drawExtraLines(gRenderer, currNoteValue, extraLineX, notePos, ROW_Y[rowNum], sharpTrue);
                    //eigthChain = 0;
                }

                if (restDuration > 0){
                    copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 2, restDuration, pageNum*screen_width + xRender + bitMapCellW*(currNote.time+currNote.duration), ROW_Y[rowNum] + LINE_GAP*2 - bitMapCellH/2);
                    //eigthChain = 0;
                }
            }
        }
        eigthChainDouble = 0;
        eigthChain = 0;
        xRender = xRender + barWidth;

        //draw the bar
        draw.x = pageNum*screen_width + xRender - 2;// - 5;
        draw.y = ROW_Y[rowNum];// + LINE_GAP*2 - bitMapCellH/4;
        draw.w = 2;
        draw.h = LINE_GAP*4;
        SDL_RenderFillRect(gRenderer, &draw);
    }

    //Draw a line at end of score
    draw.x = xRender + pageNum*screen_width  + 4;
    draw.y = ROW_Y[rowNum];
    draw.w = 4;
    draw.h = LINE_GAP*4 + 3;
    SDL_RenderFillRect(gRenderer, &draw);

    for (int i = 0; i <= pageNum; i++){
        if (i == pageNum){//last page
            for (int j = 0; j <= rowNum; j++){//rowNum per page
            	draw.x = startPoint - 3 + i*screen_width;
                draw.y = ROW_Y[j];
                draw.w = 3;
                draw.h = LINE_GAP*4 + 3;
                SDL_RenderFillRect(gRenderer, &draw);

            	//Apply clef onto bliting background surface
                copy.x = 0;
                copy.y = 2*bitMapCellH;
                copy.w = bitMapCellW;
                copy.h = 2*bitMapCellH;

                paste.x = startPoint + i*screen_width;
                paste.y = ROW_Y[j] + LINE_GAP*2 - bitMapCellH;
                paste.w = bitMapCellW;
                paste.h = 2*bitMapCellH;

                SDL_RenderCopy(gRenderer, gSymbol.mTexture, &copy, &paste);

                //Apply key sig onto bliting background surface
                drawKeySig(gRenderer, gSymbol.mTexture, keyNum, startPoint + bitMapCellW + i*screen_width, ROW_Y[j]);
                
            	//Apply time sig onto bliting background surface
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 5, song->bars[0].tsig_numerator, startPoint + bitMapCellW*2 + i*screen_width, ROW_Y[j] + LINE_GAP*2-bitMapCellH/2);
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 5, song->bars[0].tsig_denominator, startPoint + bitMapCellW*2 + i*screen_width, ROW_Y[j] + LINE_GAP*2);

                //Apply 5 lines onto bliting background surface
                for (int k = 0; k < 5; k++){
                    //Draw the line going through the notes
                    draw.x = startPoint + i*screen_width;//+3*bitMapCellW;
                    draw.y = ROW_Y[j] + LINE_GAP*k;//screen_height/2 - 29;
                    if (j == rowNum ){
                        draw.w = xRender + headWidth - 160;//stop at the end of the song
                    }
                    else{
                        draw.w = ROW_X + headWidth;
                    }
                    draw.h = 3;
                    SDL_RenderFillRect(gRenderer, &draw);
                }//end of for k
            }//end of for j
        }//end of if
        else{
            for (int p = 0; p < 4; p++){//4 rows per page
                draw.x = startPoint - 3 + i*screen_width;
                draw.y = ROW_Y[p];
                draw.w = 3;
                draw.h = LINE_GAP*4 + 3;
                SDL_RenderFillRect(gRenderer, &draw);

                //Apply clef onto bliting background surface
                copy.x = 0;
                copy.y = 2*bitMapCellH;
                copy.w = bitMapCellW;
                copy.h = 2*bitMapCellH;

                paste.x = startPoint + i*screen_width;
                paste.y = ROW_Y[p] + LINE_GAP*2 - bitMapCellH;
                paste.w = bitMapCellW;
                paste.h = 2*bitMapCellH;

                SDL_RenderCopy(gRenderer, gSymbol.mTexture, &copy, &paste);

                //Apply key sig onto bliting background surface
                drawKeySig(gRenderer, gSymbol.mTexture, keyNum, startPoint + bitMapCellW + i*screen_width, ROW_Y[p]);

            	//Apply time sig onto bliting background surface
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 5, song->bars[0].tsig_numerator, startPoint + bitMapCellW*2 + i*screen_width, ROW_Y[p] + LINE_GAP*2-bitMapCellH/2);
                copySymbolCellToSurface(gRenderer, gSymbol.mTexture, 5, song->bars[0].tsig_denominator, startPoint + bitMapCellW*2 + i*screen_width, ROW_Y[p] + LINE_GAP*2);
                
                //Apply 5 lines onto bliting background surface
                for (int q = 0; q < 5; q++){
                    //Draw the line going through the notes
                    draw.x = startPoint + i*screen_width;//+3*bitMapCellW;
                    draw.y = ROW_Y[p] + LINE_GAP*q;//screen_height/2 - 29;
                    draw.w = ROW_X + headWidth;
                    draw.h = 3;
                    SDL_RenderFillRect(gRenderer, &draw);
                }//end of for q
            }//end of for p
        }//end of else
    }//end of for i
    
    //Reset render target
    SDL_SetRenderTarget( gRenderer, NULL );
    total_page_num = pageNum;
    //free the LTexture
    gBackground.free();

}

//Update the renderer
void SongPlayer::updateMusicSurface( SDL_Renderer *gRenderer, LTexture* gBuffer, int xCoord, int oldXCoord )
{
    int barW = gBuffer[BAR].getWidth();
    int barH = gBuffer[BAR].getHeight();
    bool nextRow = false;
    
    //Blit page 0 notes onto windows at the beginning
    if ( xCoord == oldXCoord){
        copy.x = 0;
        copy.y = 0;
        copy.w = screen_width;
        copy.h = screen_height;

        paste.x = 0;
        paste.y = 0;
        paste.w = screen_width;
        paste.h = screen_height;
        
        SDL_RenderCopy(gRenderer, gBuffer[MUSIC_SHEET].mTexture, &copy, &paste);
    }

    //update the bar position
    if (oldXCoord%ROW_X >= (-headWidth-startPoint)){
        //remove the previous bar
        //recover the background
        copy.x = oldXCoord%ROW_X + headWidth + startPoint - barW/2;
        copy.y = row_num*renderHeight;
        copy.w = barW;
        copy.h = barH;

        paste.x = oldXCoord%ROW_X + headWidth + startPoint - barW/2;
        paste.y = row_num*renderHeight;
        paste.w = barW;
        paste.h = barH;
        
        SDL_RenderCopy(gRenderer, gBuffer[BACKGROUND].mTexture, &copy, &paste);
    
        //recover the notes
        copy.x = oldXCoord%ROW_X + headWidth + startPoint - barW/2 + page_num*screen_width;
        copy.y = row_num*renderHeight;
        copy.w = barW;
        copy.h = barH;
        
        paste.x = oldXCoord%ROW_X + headWidth + startPoint - barW/2;
        paste.y = row_num*renderHeight;
        paste.w = barW;
        paste.h = barH;
        SDL_RenderCopy(gRenderer, gBuffer[MUSIC_SHEET].mTexture, &copy, &paste);
    
        //Draw the bar where the user is supposed to hit
        //if(!finished){
        paste.x = xCoord%ROW_X + headWidth + startPoint - barW/2;
        if ( int(xCoord/ROW_X) == (row_num + 4*page_num) ){
            paste.y = row_num*renderHeight;
        }
        else{//Go to the next row
            nextRow = true;
            if (row_num == 3 ){
                paste.y = 0;
            }
            else{
                paste.y = (row_num+1)*renderHeight;
            }
        }
        paste.w = barW;
        paste.h = barH;
        SDL_RenderCopy(gRenderer, gBuffer[BAR].mTexture, NULL, &paste);
        //}
    }


    //printf("nextRow = %d\n", nextRow);
    if (nextRow){
        //0->1 replace = 3; 1->2 replace = 0; 2->3 replace = 1; 3->0 replace = 2;
        replace_row_num = row_num - 1;
        //if the row going to be replaced is from the previous page
        if (replace_row_num == -1 && page_num != 0){
            replace_row_num = 3;
            //Replace the old row with the new row
            //Remove the old row
            copy.x = 0;
            copy.y = replace_row_num*renderHeight;
            copy.w = screen_width;
            copy.h = renderHeight;

            paste.x = 0;
            paste.y = replace_row_num*renderHeight;
            paste.w = screen_width;
            paste.h = renderHeight;
            
            SDL_RenderCopy(gRenderer, gBuffer[BACKGROUND].mTexture, &copy, &paste);

            //Display the new row
            copy.x = page_num*screen_width;
            copy.y = replace_row_num*renderHeight;
            copy.w = screen_width;
            copy.h = renderHeight;

            SDL_RenderCopy(gRenderer, gBuffer[MUSIC_SHEET].mTexture, &copy, &paste);
        }
        //if the row going to be replaced is from the current page
        if(page_num != total_page_num && replace_row_num != -1 && replace_row_num !=3){
            //Replace the old row with the new row
            //Remove the old row
            copy.x = 0;
            copy.y = replace_row_num*renderHeight;
            copy.w = screen_width;
            copy.h = renderHeight;

            paste.x = 0;
            paste.y = replace_row_num*renderHeight;
            paste.w = screen_width;
            paste.h = renderHeight;
            
            SDL_RenderCopy(gRenderer, gBuffer[BACKGROUND].mTexture, &copy, &paste);

            //Display the new row
            copy.x = (page_num+1)*screen_width;
            copy.y = replace_row_num*renderHeight;
            copy.w = screen_width;
            copy.h = renderHeight;

            SDL_RenderCopy(gRenderer, gBuffer[MUSIC_SHEET].mTexture, &copy, &paste);
        }
        
        row_num++;
        nextRow = false;
        if (row_num == 4){
            row_num = 0;
            page_num++;
        }
        //printf("page_num = %d\n", page_num);
        //printf("row_num = %d\n", row_num);
    }

    SDL_RenderPresent( gRenderer );
    
}
