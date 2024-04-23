#include "chessData.h"

#define goaldepth 2
#define search mapsize*mapsize
#define mapsize 25
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define midpoint mapsize / 2
#define blackend "11111"
#define whiteend "00000"

struct Point {
    int myscore;
    int foescore;
    int addscore;
    int x;
    int y;
};

int ifend(char map[][mapsize]) {
  //�ˬd�
    char temp[25] = {'x'};
    int c = 0;
    for (int y = 0; y <= mapsize - 1; y++) {
        for (int x = 0; x <= mapsize - 1; x++) {
            temp[c] = map[y][x];
            c += 1;
        }
        if (strstr(temp, blackend) != NULL)
            return 1;
        else if (strstr(temp, whiteend) != NULL)
            return 2;
        for (int i = 0; i <= 24; i++)
            temp[i] = 'x';
        c = 0;
    }
  //����
  for (int i = 0; i <= 24; i++)
    temp[i] = 'x';
  c = 0;
  for (int x = 0; x <= mapsize - 1; x++) {
    for (int y = 0; y <= mapsize - 1; y++) {
      temp[c] = map[y][x];
      c += 1;
    }
    if (strstr(temp, blackend) != NULL)
      return 3;
    else if (strstr(temp, whiteend) != NULL)
      return 4;
    for (int i = 0; i <= 24; i++)
      temp[i] = 'x';
    c = 0;
    }
    /*/*/
    // �q���W���}�l�P�_�V/���u
    for (int i = 0; i <= 24; i++)
        temp[i] = 'x';
    c = 0;
    for (int k = 0; k < mapsize; k++) {
        for (int i = 0; i <= k; i++) {
            temp[c] = map[k - i][i];
            c += 1;
        }
        if (strstr(temp, blackend) != NULL)
            return 5;
        else if (strstr(temp, whiteend) != NULL)
            return 6;
        for (int i = 0; i <= 24; i++)
            temp[i] = 'x';
        c = 0;
    }

    // ?�����U���@��}�l�P�_�V/���u
    for (int i = 0; i <= 24; i++)
        temp[i] = 'x';
    c = 0;
    for (int k = 1; k < mapsize; k++) {
        for (int j = 0; j < mapsize - k; j++) {
            temp[c] = map[mapsize - 1 - j][k + j];
            c += 1;
        }
        if (strstr(temp, blackend) != NULL)
            return 7;
        else if (strstr(temp, whiteend) != NULL)
            return 8;
        for (int i = 0; i <= 24; i++)
            temp[i] = 'x';
        c = 0;
    }

    /*\*/
    // �q�k�W���}�l�P�_�V\���u
    for (int i = 0; i <= 24; i++)
        temp[i] = 'x';
    c = 0;
    for (int k = 0; k < mapsize; k++) {
        for (int i = 0; i <= k; i++) {
            temp[c] = map[i][mapsize - 1 - k + i];
            c += 1;
        }
        if (strstr(temp, blackend) != NULL)
            return 9;
        else if (strstr(temp, whiteend) != NULL)
            return 10;
        for (int i = 0; i <= 24; i++)
            temp[i] = 'x';
        c = 0;
    }

    // �q�������U�@��}�l�P�_�V\���u
    for (int i = 0; i <= 24; i++)
        temp[i] = 'x';
    c = 0;
    for (int k = 1; k < mapsize; k++) {
        for (int j = 0; j < mapsize - k; j++) {
            temp[c] = map[j + k][j];
            c += 1;
        }
        if (strstr(temp, blackend) != NULL)
            return 11;
        else if (strstr(temp, whiteend) != NULL)
            return 12;
        for (int i = 0; i <= 24; i++)
            temp[i] = 'x';
        c = 0;
    }
    return 0;
}

int toscore(char temp[11], int count[5], char mypiece) {
  // ����ŦX���ѫ��ɡA�ߧY��^����������
  if (mypiece == '1' && strstr(temp, "111111") != NULL)
    return 0; //���ª��s

  if (strstr(temp, "11111") != NULL)
    return 10000000; //�s��

  if (strstr(temp, ".1111.") != NULL)
    return 1000000; //���|

  if (strstr(temp, "..111.") != NULL || strstr(temp, ".111..") != NULL ||
      strstr(temp, ".1.11.") != NULL || strstr(temp, ".11.1.") != NULL) {
    count[3] += 1;
    return 10000;
  }

  if (strstr(temp, "1111.") != NULL || strstr(temp, ".1111") != NULL ||
      strstr(temp, "1.111") != NULL || strstr(temp, "11.11") != NULL ||
      strstr(temp, "111.1") != NULL) {
    count[4] += 1;
    return 8000;
  }

  if (strstr(temp, "..1110") != NULL || strstr(temp, ".1.110") != NULL ||
      strstr(temp, ".11.10") != NULL || strstr(temp, "0111..") != NULL ||
      strstr(temp, "011.1.") != NULL) {
    return 1000;
  }

  if (strstr(temp, "..11..") != NULL || strstr(temp, ".11...") != NULL ||
      strstr(temp, "...11.") != NULL) {
    count[2] += 1;
    return 800;
  }

  if (strstr(temp, ".110..") != NULL || strstr(temp, "..110.") != NULL ||
      strstr(temp, "..011.") != NULL || strstr(temp, ".011..") != NULL ||
      strstr(temp, "..1.1.") != NULL || strstr(temp, ".1.1..") != NULL) {
    return 50;
  }

  if (strstr(temp, "..10..") != NULL || strstr(temp, "..01..") != NULL ||
      strstr(temp, ".0.1..") != NULL || strstr(temp, "...01.") != NULL ||
      strstr(temp, "...10.") != NULL) {
    return 10;
  }

  return 0; // �S�����ŦX���ѫ��A��^0��
}

int totemp(char map[][mapsize], int x, int y, char mypiece) {
  //��
  char temp[11] = {'x'};
  int count[5] = {0};
  int c = 0, sumscore = 0;
  for (int xi = x - 4; xi <= x + 4; xi++) {
    if (xi >= 0 && xi <= mapsize - 1) {
      temp[c] = map[y][xi];
      c += 1;
    }
  }
  sumscore += toscore(temp, count, mypiece);

  //��
  for (int i = 0; i <= 10; i++) {
    temp[i] = 'x';
  }
  c = 0;
  for (int yi = y - 4; yi <= y + 4; yi++) {
    if (yi >= 0 && yi <= mapsize - 1) {
      temp[c] = map[yi][x];
      c += 1;
    }
  }
  sumscore += toscore(temp, count, mypiece);

  /*\*/
  for (int i = 0; i <= 10; i++) {
    temp[i] = 'x';
  }
  c = 0;
  for (int change = -4; change <= 4; change++) {
    if (x + change >= 0 && x + change <= mapsize - 1 && y + change >= 0 &&
        y + change <= mapsize - 1) {
      temp[c] = map[y + change][x + change];
      c += 1;
    }
  }
  sumscore += toscore(temp, count, mypiece);

  /*/*/
  for (int i = 0; i <= 10; i++) {
    temp[i] = 'x';
  }
  c = 0;
  for (int change = -4; change <= 4; change++) {
    if (x + change >= 0 && x + change <= mapsize - 1 && y - change >= 0 &&
        y - change <= mapsize - 1) {
      temp[c] = map[y - change][x + change];
      c += 1;
    }
  }
  sumscore += toscore(temp, count, mypiece);

  //�P�_�����O�_����
  if (count[4] >= 1 && count[3] >= 1)
    sumscore *= 8; //�R�|�S���T
  if (count[3] >= 2)
    sumscore *= (mypiece == '1') ? 0 : 6; //��өΨ�ӥH�W�����T
  if (count[4] >= 1 && count[2] >= 1)
    sumscore *= 4; //�R�|�S���G
  if (count[3] >= 1 && count[2] >= 1)
    sumscore *= 2; //���T�S���G

  return sumscore;
}

void onetozero(char map[][mapsize]) {
  for (int y = 0; y <= mapsize - 1; y++) {
    for (int x = 0; x <= mapsize - 1; x++) {
      if (map[y][x] == '.')
        continue;
      map[y][x] = (map[y][x] == '1') ? '0' : '1';
    }
  }
}

void QuickSort(struct Point data[], int left, int right) {
  if (left >= right)
    return;
  int i = left, j = right;
  struct Point pivot = data[left];
  while (i != j) {
    while (data[j].addscore <= pivot.addscore && i < j)
      j--;
    while (data[i].addscore >= pivot.addscore && i < j)
      i++;
    if (i < j) {
      struct Point temp = data[i];
      data[i] = data[j];
      data[j] = temp;
    }
  }
  data[left] = data[i];
  data[i] = pivot;
  QuickSort(data, left, i - 1);
  QuickSort(data, i + 1, right);
}

int nextxy(int depth, int alpha, int beta, char piece, char mypiece,
           char map[][mapsize], struct Point elements[][mapsize], int *AIx, int *AIy) {
  for (int y = 0; y < mapsize; y++) {
    for (int x = 0; x < mapsize; x++) {
      elements[y][x].addscore = 0;
      elements[y][x].x = x;
      elements[y][x].y = y;
    }
  }

  for (int y = 0; y <= mapsize - 1; y++) {
    for (int x = 0; x <= mapsize - 1; x++) {
      if (map[y][x] == '.') {
        //�β{�b�U�Ѫ̪�����
        if (piece == '0')
          onetozero(map);
        map[y][x] = '1';
        elements[y][x].myscore = totemp(map, x, y, mypiece);
        map[y][x] = '.';
        if (piece == '0')
          onetozero(map);

        //�β{�b��⪺����
        if (piece == '1')
          onetozero(map);
        map[y][x] = '1';
        elements[y][x].foescore = totemp(map, x, y, mypiece);
        map[y][x] = '.';
        if (piece == '1')
          onetozero(map);

        elements[y][x].addscore =
            elements[y][x].myscore + elements[y][x].foescore;
      }
    }
  }

  if (depth == 0) {
    int a = 0, b = 0;
    for (int y = 0; y < mapsize; y++) {
      for (int x = 0; x < mapsize; x++) {
        if (map[y][x] == mypiece) {
          if (mypiece == '0')
            onetozero(map);
          a += totemp(map, x, y, mypiece);
          if (mypiece == '0')
            onetozero(map);
        } else if (map[y][x] == (mypiece == '1') ? '0' : '1') {
          if (mypiece == '1')
            onetozero(map);
          b += totemp(map, x, y, mypiece);
          if (mypiece == '1')
            onetozero(map);
        }
      }
    }
    return a * 2 - b;
  }

  int score;
  if (piece == mypiece) {
    // AI�]?�n??�̰����A�ҥH��l�Ƥ@??�H��?���C��
    score = INT_MIN;
  } else {
    // ?��n??�̧C���A�ҥH��l�Ƥ@??�H��?������
    score = INT_MAX;
  }

  // sort
  struct Point sortelements[mapsize * mapsize];
  for (int y = 0; y < mapsize; y++) {
    for (int x = 0; x < mapsize; x++) {
      sortelements[y * mapsize + x] = elements[y][x];
    }
  }
  QuickSort(sortelements, 0, mapsize * mapsize - 1);
  //�j��
  int aix, aiy;
  for (int i = 0; i < search; i++) {
    if (alpha >= beta) {
      break;
    }

    if (sortelements[i].addscore > 0) {
      // ���]�U�l
      map[sortelements[i].y][sortelements[i].x] = piece;
      struct Point newelements[mapsize][mapsize];
      int score = nextxy(depth - 1, alpha, beta, (piece == '1') ? '0' : '1',
                         mypiece, map, newelements, AIx, AIy);
      // ��_�ѽL
      map[sortelements[i].y][sortelements[i].x] = '.';
      // ��s�̨Τ���
      if (piece == mypiece && score > alpha) {
        alpha = score;
        // �b�ڸ`�I�ɡA�O���̨ΤU�l��m
        if (depth == goaldepth) {
            aix = sortelements[i].x;
            aiy = sortelements[i].y;
            *AIx = aix;
            *AIy = aiy;
        }
        }else if (piece != mypiece && score < beta) {
            // printf("score��s�� %d\n", newValue);
            beta = score;
        }
    }
  }

  if (depth == goaldepth) {
    map[aiy][aix] = piece;
    printf("%sAI�U�b %d %d\n", (piece == '1') ? "�¤l" : "�դl", aix, aiy);
  }
  return (piece == mypiece) ? alpha : beta;
}


void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    while(1){
        int px, py; //���a�U�Ѧ�l
        char piece; //�q���Ѥl 1�� 0�� �¥��U player 1��0�ۤ�
        char map[mapsize][mapsize]; //��l�ѽL�j�p
        struct Point elements[mapsize][mapsize];

        if (player == 0) piece ='1';
        else piece = '0';

        //��l��map�ѽL scoremap����
        for (int i = 0; i <= mapsize - 1; i++) {
            for (int j = 0; j <= mapsize - 1; j++) {
                map[i][j] = '.';
            }
        }
        //�N�w�g�U�L����m��Jmap
        for (int i = 0; i < chess->size; i++){
            if (chess->cord[i].player == 1) map[chess->cord[i].y][chess->cord[i].x] = '0';
            else map[chess->cord[i].y][chess->cord[i].x] = '1';
            //printf("chess check\n");
        }
//123
        //�Y����
        if (chess->size == 0) {
            map[midpoint][midpoint] = '1';
            *x = midpoint;
            *y = midpoint;
        }
        else if (chess->size == 1){
            map[midpoint+1][midpoint+1] = '0';
            *x = midpoint+1;
            *y = midpoint+1;
        }
        else {
            nextxy(goaldepth, INT_MIN, INT_MAX, piece, piece, map, elements, x, y);
        }

        if (setXY(chess, *x, *y, player) == 1) {
            return;
        }
    }
}
