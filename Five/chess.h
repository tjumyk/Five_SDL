#pragma once

class Chess {
public:
    Chess();

    bool addPiece(int x, int y, int type);

    int hasPiece(int x, int y);

    int getWinPieceX(int num);

    int getWinPieceY(int num);

    bool checkFull();

    int pieceNum();

    void getLastPiece(int &x, int &y);

    void resetPiece();

    void checkModel(int type, int diff, int x, int y, int model[][10], int level[][15]);

    void
    transformModel(int x, int y, int model[][10], int &tModelNum, int tModelX[], int tModelY[], int tModel[][10][10]);

    void AIWork(int type);

    bool judgeLevel(int selfType, bool attack, int diffTest, int levelDemand);

    bool checkFive(int type);

    int getSetting(int settingNum);

    void changeSetting(int settingNum, int value);

    int getCurrentPieceType() { return currentPieceType; }

private:
    //pieces:
    int piece[15][15];
    int pieceOnBoard;
    int winPieceX[5];
    int winPieceY[5];
    int lastPieceX;
    int lastPieceY;
    //Models:
    int modelNum;
    int modelX[200];
    int modelY[200];
    int model[200][10][10];
    //Settings:
    int setting[10];
    //Player
    int currentPieceType;
};
