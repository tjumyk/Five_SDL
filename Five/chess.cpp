#include "chess.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

Chess::Chess() {
    srand(time(0));
    setting[0] = 1;
    setting[1] = 1;
    setting[2] = 3;
    setting[3] = 2;
    setting[4] = 10;
    setting[5] = 6;
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            piece[i][j] = 0;
    lastPieceX = lastPieceY = -1;
    for (int win = 0; win < 5; win++) {
        winPieceX[win] = -1;
        winPieceY[win] = -1;
    }
    pieceOnBoard = 0;
    ifstream fin;
    fin.open("res/file/model.five");
    modelNum = 0;
    if (fin.fail()) {
        cout << "Unable to read the model file: \"model.five\"!" << endl;
        exit(1);
    }

    while (fin >> modelX[modelNum] >> modelY[modelNum]) {
        for (int ii = 0; ii < modelX[modelNum]; ii++)
            for (int jj = 0; jj < modelY[modelNum]; jj++)
                fin >> model[modelNum][ii][jj];
        modelNum++;
    }
    fin.close();
    currentPieceType = 1;
}

bool Chess::addPiece(int x, int y, int type) {
    if (type != currentPieceType)
        return false;
    if (x >= 15 || x < 0 || y >= 15 || y < 0)
        return false;
    if (piece[x][y] != 0)
        return false;
    else {
        piece[x][y] = type;
        pieceOnBoard++;
        lastPieceX = x;
        lastPieceY = y;
        currentPieceType = 3 - currentPieceType;
        return true;
    }
}

int Chess::hasPiece(int x, int y) {
    if (x < 0 || x > 14 || y < 0 || y > 14)
        return -1;
    else
        return piece[x][y];
}

//Model Labels: 0 - whatever; 1 - check object; -1 - space
void Chess::checkModel(int type, int diff, int x, int y, int model[][10], int level[15][15]) {
    // For Test
    // ofstream fout;						//
    // fout.open("test.txt",ios_base::app);	//

    for (int i = 0; i < 16 - x; i++)
        for (int j = 0; j < 16 - y; j++) {
            int diffGet = 0;
            bool ok = true;
            for (int ii = 0; ii < x; ii++) {
                for (int jj = 0; jj < y; jj++) {
                    if (model[ii][jj] == 1 && piece[i + ii][j + jj] != type) {
                        if (piece[i + ii][j + jj] == 3 - type) {
                            ok = false;
                            break;
                        } else {
                            diffGet++;
                            if (diffGet > diff)
                                break;
                        }
                    }
                    if (model[ii][jj] == -1 && piece[i + ii][j + jj] != 0) {
                        ok = false;
                        break;
                    }
                }
                if (diffGet > diff || ok == false)
                    break;
            }
            if (diffGet == diff && ok == true) {
                for (int i3 = 0; i3 < x; i3++)
                    for (int j3 = 0; j3 < y; j3++)
                        if (diff == 0) {
                            if (model[i3][j3] == -1)
                                level[i + i3][j + j3]++;
                        } else {
                            if (model[i3][j3] == 1 && piece[i + i3][j + j3] == 0) {
                                level[i + i3][j + j3]++;
                            } else if (model[i3][j3] == -1) {
                                level[i + i3][j + j3]++;
                            }
                        }
            }
        }
    //fout.close(); // For test
}

void Chess::AIWork(int type) {
    if (pieceOnBoard == 0) {
        int x = rand() % 3 + 6;
        int y = rand() % 3 + 6;
        addPiece(x, y, type);
    } else if (pieceOnBoard <= 4) {
        bool findEnermy = false;
        for (int i0 = 0; i0 < 15; i0++) {
            for (int j0 = 0; j0 < 15; j0++)
                if (piece[i0][j0] == 3 - type) {
                    int temp = rand() % 8;
                    int dx, dy;
                    if (temp < 3) {
                        dx = temp - 1;
                        dy = -1;
                    } else if (temp > 4) {
                        dx = temp - 6;
                        dy = 1;
                    } else {
                        dx = temp == 3 ? -1 : 1;
                        dy = 0;
                    }
                    addPiece(i0 + dx, j0 + dy, type);
                    findEnermy = true;
                    break;
                }
            if (findEnermy)
                break;
        }
    } else if (!judgeLevel(type, true, 0, setting[0]))
        if (!judgeLevel(type, false, 0, setting[1]))
            if (!judgeLevel(type, true, 1, setting[2]))
                if (!judgeLevel(type, false, 1, setting[3]))
                    if (!judgeLevel(type, true, 2, setting[4]))
                        if (!judgeLevel(type, false, 2, setting[5])) {
                            bool find = false;
                            for (int i = 0; i < 15; i++) {
                                for (int j = 0; j < 15; j++)
                                    if (piece[i][j] != 0) {
                                        find = true;
                                        int findX = i, findY = j;
                                        if (hasPiece(i - 1, j) == 0)
                                            findX--;
                                        else if (hasPiece(i, j - 1) == 0)
                                            findY--;
                                        else if (hasPiece(i + 1, j) == 0)
                                            findX++;
                                        else if (hasPiece(i, j + 1) == 0)
                                            findY++;
                                        else
                                            find = false;
                                        if (find) {
                                            addPiece(findX, findY, type);
                                            break;
                                        }
                                    }
                                if (find)
                                    break;
                            }
                        }
}

bool Chess::checkFive(int type) {
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 11; j++)
            if (piece[i][j] == type &&
                piece[i][j + 1] == type &&
                piece[i][j + 2] == type &&
                piece[i][j + 3] == type &&
                piece[i][j + 4] == type) {
                for (int win = 0; win < 5; win++) {
                    winPieceX[win] = i;
                    winPieceY[win] = j + win;
                }
                return true;
            }
    for (int i2 = 0; i2 < 11; i2++)
        for (int j2 = 0; j2 < 15; j2++)
            if (piece[i2][j2] == type &&
                piece[i2 + 1][j2] == type &&
                piece[i2 + 2][j2] == type &&
                piece[i2 + 3][j2] == type &&
                piece[i2 + 4][j2] == type) {
                for (int win = 0; win < 5; win++) {
                    winPieceX[win] = i2 + win;
                    winPieceY[win] = j2;
                }
                return true;
            }
    for (int i3 = 0; i3 < 11; i3++)
        for (int j3 = 0; j3 < 11; j3++)
            if (piece[i3][j3] == type &&
                piece[i3 + 1][j3 + 1] == type &&
                piece[i3 + 2][j3 + 2] == type &&
                piece[i3 + 3][j3 + 3] == type &&
                piece[i3 + 4][j3 + 4] == type) {
                for (int win = 0; win < 5; win++) {
                    winPieceX[win] = i3 + win;
                    winPieceY[win] = j3 + win;
                }
                return true;
            }
    for (int i4 = 0; i4 < 11; i4++)
        for (int j4 = 0; j4 < 11; j4++)
            if (piece[i4 + 4][j4] == type &&
                piece[i4 + 3][j4 + 1] == type &&
                piece[i4 + 2][j4 + 2] == type &&
                piece[i4 + 1][j4 + 3] == type &&
                piece[i4][j4 + 4] == type) {
                for (int win = 0; win < 5; win++) {
                    winPieceX[win] = i4 + 4 - win;
                    winPieceY[win] = j4 + win;
                }
                return true;
            }
    return false;
}

void Chess::transformModel(int x, int y, int model[][10], int &tModelNum, int tModelX[], int tModelY[],
                           int tModel[][10][10]) {
    int temp[10][10] = {{0}};
    int tempX = x;
    int tempY = y;
    tModelNum = 1;
    tModelX[0] = x;
    tModelY[0] = y;
    for (int i0 = 0; i0 < x; i0++)
        for (int j0 = 0; j0 < y; j0++)
            temp[i0][j0] = tModel[0][i0][j0] = model[i0][j0];

    int num = 1;
    for (int i = 1; i < 8; i++, num++) {
        //create:
        if (i == 4) {//���·�ת
            tModelX[num] = tempX;
            tModelY[num] = tempY;
            for (int ii = 0; ii < tempX; ii++)
                for (int jj = 0; jj < tempY; jj++)
                    tModel[num][tempX - 1 - ii][jj] = temp[ii][jj];
        } else {//��ʱ����ת90��
            tModelX[num] = tempY;
            tModelY[num] = tempX;
            for (int ii = 0; ii < tempX; ii++)
                for (int jj = 0; jj < tempY; jj++)
                    tModel[num][jj][tempX - 1 - ii] = temp[ii][jj];
        }
        tempX = tModelX[num];
        tempY = tModelY[num];
        for (int i3 = 0; i3 < tempX; i3++)
            for (int j3 = 0; j3 < tempY; j3++)
                temp[i3][j3] = tModel[num][i3][j3];

        /*For Test:
        ofstream fout;
        fout.open("test.txt",ios_base::app);
        fout << "tModel[" << num << "] , X = " << tModelX[num] << " Y = " << tModelY[num] << "\n";
        for(int p = 0; p < tModelX[num]; p++){
            for(int q = 0; q < tModelY[num];q++){
                fout << tModel[num][p][q];
            }
            fout << endl;
        }
        fout.close();
        */

        //check:
        bool pass = true;
        for (int ii = 0; ii < num; ii++) {
            bool ok = false;
            if (tModelX[ii] != tModelX[num] || tModelY[ii] != tModelY[num])
                continue;
            for (int i3 = 0; i3 < tModelX[ii]; i3++) {
                for (int i4 = 0; i4 < tModelY[ii]; i4++)
                    if (tModel[ii][i3][i4] != tModel[num][i3][i4]) {
                        ok = true;
                        break;
                    }
                if (ok)
                    break;
            }
            if (!ok) {
                pass = false;
                break;
            }
        }
        if (!pass)
            num--;
    }
    tModelNum = num;
}

bool Chess::judgeLevel(int selfType, bool attack, int diffTest, int levelDemand) {
    int targetType = attack ? selfType : 3 - selfType;
    int level[15][15] = {{0}};
    for (int i = 0; i < modelNum; i++) {
        int tModelNum = 0;
        int tModelX[16];
        int tModelY[16];
        int tModel[16][10][10] = {{{0}}};
        transformModel(modelX[i], modelY[i], model[i], tModelNum, tModelX, tModelY, tModel);
        for (int ii = 0; ii < tModelNum; ii++)
            checkModel(targetType, diffTest, tModelX[ii], tModelY[ii], tModel[ii], level);
    }
    int maxLevel = 0;
    for (int m = 0; m < 15; m++)
        for (int n = 0; n < 15; n++)
            if (level[m][n] > maxLevel)
                maxLevel = level[m][n];

    if (maxLevel >= levelDemand) {
        int maxLevelNum = 0;
        const int maxChoiceNum = 10;
        int maxLevelX[maxChoiceNum] = {0};
        int maxLevelY[maxChoiceNum] = {0};
        bool getMax = false;
        for (int i3 = 0; i3 < 15; i3++) {
            for (int j3 = 0; j3 < 15; j3++)
                if (level[i3][j3] == maxLevel) {
                    maxLevelX[maxLevelNum] = i3;
                    maxLevelY[maxLevelNum] = j3;
                    maxLevelNum++;
                    if (maxLevelNum >= maxChoiceNum) {
                        getMax = true;
                        break;
                    }
                }
            if (getMax)
                break;
        }
        int choice = rand() % maxLevelNum;
        if (addPiece(maxLevelX[choice], maxLevelY[choice], selfType)) {
            return true;
        } else
            return false;
    } else
        return false;
}

void Chess::resetPiece() {
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            piece[i][j] = 0;
    pieceOnBoard = 0;
    lastPieceX = -1;
    lastPieceY = -1;
    for (int k = 0; k < 5; k++) {
        winPieceX[k] = -1;
        winPieceY[k] = -1;
    }
    currentPieceType = 1;
}

int Chess::getSetting(int settingNum) {
    if (settingNum >= 0 && settingNum < 10)
        return setting[settingNum];
    else
        return 0;
}

void Chess::changeSetting(int settingNum, int value) {
    if (settingNum >= 0 && settingNum < 10 && value >= 0 && value < 100)
        setting[settingNum] = value;
}

bool Chess::checkFull() {
    if (pieceOnBoard >= 15 * 15)
        return true;
    else
        return false;
}

int Chess::pieceNum() {
    return pieceOnBoard;
}

void Chess::getLastPiece(int &x, int &y) {
    x = lastPieceX;
    y = lastPieceY;
}

int Chess::getWinPieceX(int num) {
    if (num >= 0 && num < 5)
        return winPieceX[num];
    else
        return 0;
}

int Chess::getWinPieceY(int num) {
    if (num >= 0 && num < 5)
        return winPieceY[num];
    else
        return 0;
}
