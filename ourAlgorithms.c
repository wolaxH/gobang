#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define goaldepth 2
#define search mapsize *mapsize
#define mapsize 15
#define pieceset '1'
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
  //檢查橫的
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
  //直的
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
  // 從左上角開始判斷向/的線
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

  // ?中間下面一格開始判斷向/的線
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
  // 從右上角開始判斷向\的線
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

  // 從中間往下一格開始判斷向\的線
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
  // 當找到符合的棋型時，立即返回對應的分數
  if (mypiece == '1' && strstr(temp, "111111") != NULL)
    return 0; //執黑長連

  if (strstr(temp, "11111") != NULL)
    return 10000000; //連五

  if (strstr(temp, ".1111.") != NULL)
    return 1000000; //活四

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

  return 0; // 沒有找到符合的棋型，返回0分
}

int totemp(char map[][mapsize], int x, int y, char mypiece) {
  //橫
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

  //直
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

  //判斷評分是否提高
  if (count[4] >= 1 && count[3] >= 1)
    sumscore *= 8; //沖四又活三
  if (count[3] >= 2)
    sumscore *= (mypiece == '1') ? 0 : 6; //兩個或兩個以上的活三
  if (count[4] >= 1 && count[2] >= 1)
    sumscore *= 4; //沖四又活二
  if (count[3] >= 1 && count[2] >= 1)
    sumscore *= 2; //活三又活二

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
           char map[][mapsize], struct Point elements[][mapsize]) {
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
        //用現在下棋者的角度
        if (piece == '0')
          onetozero(map);
        map[y][x] = '1';
        elements[y][x].myscore = totemp(map, x, y, mypiece);
        map[y][x] = '.';
        if (piece == '0')
          onetozero(map);

        //用現在對手的角度
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
    // AI因?要??最高分，所以初始化一??以到?的低分
    score = INT_MIN;
  } else {
    // ?手要??最低分，所以初始化一??以到?的高分
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
  //搜索
  int aix, aiy;
  for (int i = 0; i < search; i++) {
    if (alpha >= beta) {
      break;
    }

    if (sortelements[i].addscore > 0) {
      // 假設下子
      map[sortelements[i].y][sortelements[i].x] = piece;
      struct Point newelements[mapsize][mapsize];
      int score = nextxy(depth - 1, alpha, beta, (piece == '1') ? '0' : '1',
                         mypiece, map, newelements);
      // 恢復棋盤
      map[sortelements[i].y][sortelements[i].x] = '.';
      // 更新最佳分數
      if (piece == mypiece && score > alpha) {
        alpha = score;
        // 在根節點時，記錄最佳下子位置
        if (depth == goaldepth) {
          aix = sortelements[i].x;
          aiy = sortelements[i].y;
        }
      } else if (piece != mypiece && score < beta) {
        // printf("score更新為 %d\n", newValue);
        beta = score;
      }
    }
  }

  if (depth == goaldepth) {
    map[aiy][aix] = piece;
    printf("%sAI下在 %d %d\n", (piece == '1') ? "黑子" : "白子", aix, aiy);
  }
  return (piece == mypiece) ? alpha : beta;
}

int main() {
  int px, py;              //玩家下棋位子
  char mypiece = pieceset; //電腦棋子 1黑 0白
  char map[mapsize][mapsize];    //初始棋盤大小
  int start = 0;           //是否開始遊戲
  struct Point elements[mapsize][mapsize];

  //初始化map棋盤 scoremap分數
  for (int i = 0; i <= mapsize - 1; i++) {
    for (int j = 0; j <= mapsize - 1; j++) {
      map[i][j] = '.';
    }
  }

  //列印棋盤
  if (mypiece == '0') {
    printf("                      1 1 1 1 1 1 1 1 1 1 2\n  0 1 2 3 4 5 6 7 8 9 "
           "0 1 2 3 4 5 6 7 8 9 0\n");
    int n = 0;
    for (int i = 0; i <= mapsize - 1; i++) {
      printf("%02d", n);
      n += 1;
      for (int j = 0; j <= mapsize - 1; j++) {
        printf("%c ", map[i][j]);
      }
      printf("\n");
    }
  }

  while (ifend(map) == 0) {
    // AI執白
    if (mypiece == '0') {
      scanf("%d %d", &px, &py);
      map[py][px] = '1';

      // if (start == 0) {
      //   map[midpoint][midpoint] = '1';
      // } else {
      //   clock_t start = clock(), end;
      //   nextxy(goaldepth, INT_MIN, INT_MAX, '1', '1', map, elements);
      //   end = clock();
      //   double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
      //   printf("執行時間為 %.6lf 秒\n", cpu_time_used);
      // }

      start += 1;
    }

    // AI落子
    if (start == 0) {
      map[midpoint][midpoint] = '1';
    } else if (start == 1) {
      map[midpoint + 1][midpoint + 1] = mypiece;
    } else {
      clock_t start = clock(), end;
      nextxy(goaldepth, INT_MIN, INT_MAX, mypiece, mypiece, map, elements);
      end = clock();
      double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
      printf("執行時間為 %.6lf 秒\n", cpu_time_used);
    }
    start += 1;
    //如果AI贏了
    if (ifend(map) != 0)
      break;

    //列印棋盤
    printf("                      1 1 1 1 1 1 1 1 1 1 2\n  0 1 2 3 4 5 6 7 8 9 "
           "0 1 2 3 4 5 6 7 8 9 0\n");
    int n = 0;
    for (int i = 0; i <= mapsize - 1; i++) {
      printf("%02d", n);
      n += 1;
      for (int j = 0; j <= mapsize - 1; j++) {
        printf("%c ", map[i][j]);
      }
      printf("\n");
    }

    //黑棋執黑
    if (mypiece == '1') {
      scanf("%d %d", &px, &py);
      map[py][px] = '0';

      // clock_t start = clock(), end;
      // nextxy(goaldepth, INT_MIN, INT_MAX, '0', '0', map, elements);
      // end = clock();
      // double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
      // printf("執行時間為 %.6lf 秒\n", cpu_time_used);

      start += 1;
    }
  }
  printf((ifend(map) % 2 == 1) ? "黑子獲勝" : "白子獲勝");

  int one = 0, zero = 0;
  for (int i = 0; i <= mapsize - 1; i++) {
    for (int j = 0; j <= mapsize - 1; j++) {
      if (map[i][j] == '1')
        one += 1;
      else if (map[i][j] == '0')
        zero += 1;
    }
  }
  printf("                      1 1 1 1 1 1 1 1 1 1 2\n  0 1 2 3 4 5 6 7 8 9 "
         "0 1 2 3 4 5 6 7 8 9 0\n");
  int n = 0;
  for (int i = 0; i <= mapsize - 1; i++) {
    printf("%02d", n);
    n += 1;
    for (int j = 0; j <= mapsize - 1; j++) {
      printf("%c ", map[i][j]);
    }
    printf("\n");
  }
  printf("黑子總數: %d ,白子總數: %d\n", one, zero);
}