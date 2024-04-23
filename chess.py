import cv2
import numpy as np

class const:
    class ConstError(TypeError):pass
    def __setattr__(self,name,value):
        if name in self.__dict__:
            raise self.ConstError("Can't rebind const (%s)" %name)
        self.__dict__[name]=value
       
const.MARGIN = 40
const.GRID = 25
const.NUM = 23
const.LEN = 550
const.WIN = 5
#測試
class Chess:
    def __init__(self):
        self.cord = []        
        self.size = 0
    def isValidCandidate(self,x: int, y: int, player: int)->bool:
        if x==0 or y==0: return False
        if x==22 or y==22: return False
        data = [(d[0], d[1]) for d in self.cord]
        if (x, y) in data: return False
        return True
    def setXY(self,x: int, y: int, player: int)->bool:
        if self.isValidCandidate(x, y, player)==False: return False
        self.cord.append((x, y, player))
        self.size += 1
        return True
    def printChess(self):
        for d in self.cord:
            print(d, end=' ')
        print('size=', self.size)   
        
def drawBoard():
    img = np.zeros((700,700,3), dtype='uint8')   # 繪製 300x300 ?????��??
    img[:] = (118, 208, 255)                     # ??��??淡�?????   
    for i in range(const.NUM):                   # �???��??�?
        cv2.line(img,(const.MARGIN, const.MARGIN+ i*const.GRID),(const.MARGIN+const.LEN, const.MARGIN + i*const.GRID),(0,0,0),1)  # 繪製�?�?
        cv2.line(img,(const.MARGIN+ i*const.GRID, const.MARGIN),(const.MARGIN + i*const.GRID, const.MARGIN+const.LEN),(0,0,0),1)  # 繪製�?�?        
    return img

def checkWin(data: list):
    count=0
    mark = False
    maxCount = 0
    for d in data:
        if d==0: 
            mark = False
            count=0
        elif (d==1) and (mark ==False):
            mark = True
            count = 1
        elif (d==1) and (mark == True):
            count +=1
        if count>maxCount: maxCount = count    
        
    if maxCount==const.WIN: return True
    return False

#print(checkWin([0,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,1,0]))

def checkBoardColumns(board: list):
    for d in board:                 # 檢�?? board ??��?? ??��????��?��??�? N ???
        if checkWin(d)==True:
            return True

def checkBoardRows(board: list):    
    for i in range(len(board[0])):  # 檢�?? board ??��?? 橫�????��?��??�? N ???
        t =[d[i] for d in board]    # �?橫�??�??????? list
        if checkWin(t)==True:
            return True   
    
def checkBoardSlashLR(board: list):
    for i in range(len(board)):     # 檢�?? board ??��??�?左�??????????��??�?�?�?�?�???��?��??�? N ???
        x, y, t = 0, i, []
        while True:
            if y>=len(board): break
            t.append(board[x][y])   #      
            x += 1
            y += 1
        if checkWin(t)==True:
            return True       

    for i in range(1,len(board)):     # # 檢�?? board ??��??�?左�??????????��??�?�?�?�?�???��?��??�? N ???
        x, y, t = i, 0, []
        while True:
            if x>=len(board): break
            t.append(board[x][y])        
            x += 1
            y += 1
        if checkWin(t)==True:
            return True               
    return False

def checkBoardSlashRL(board: list):
    LEN = len(board)
    for i in range(LEN):     # 檢�?? board ??��??�???��????????左�??�?�?�?�?�???��?��??�? N ???
        x, y, t = (LEN-1), i, []
        while True:
            if x<0 or y>=LEN: break
            t.append(board[x][y])   #      
            x -= 1
            y += 1
        if checkWin(t)==True:
            return True       

    for i in range(LEN-1):     # # 檢�?? board ??��??�???��????????左�??�?�?�?�?�???��?��??�? N ???
        x, y, t = i, 0, []
        while True:
            if x<0 or y>=LEN: break
            t.append(board[x][y])        
            x -= 1
            y += 1
        if checkWin(t)==True:
            return True               
    return False
    
def isGameOver(chess: Chess, player: int):
    # ?????? chess ??�件�??????? x, y 座�??
    data = [(d[0], d[1]) for d in chess.cord if d[2]==player]
    # ???�???? N*N board ??��??�???��??設�????? 0
    board =[[0 for j in range(const.NUM)] for i in range(const.NUM)]
    # �? chess ??�件 x, y �????�???? N*N board ??��??�???��?????�????�?�????�?置設�???? 1
    for i in range(const.NUM):
        for j in range(const.NUM):
            if (i, j) in data:
                board[i][j]=1
    
    if checkBoardColumns(board)==True: return True    
    if checkBoardRows(board)==True: return True    
    if checkBoardSlashLR(board)==True: return True        
    
    return False

def testGameOver():
    chess = Chess()
    player = 1
    data = [(1, 1, 1), (1, 2, 1), (1, 3, 1), (1, 4, 1), (1, 5, 1), (1, 7, 1)]
    for d in data:
        chess.setXY(d[0], d[1], player)
        
    flag = isGameOver(chess, player)
    print(flag, player)    

#testGameOver()    

def readCord(fileName: str):
    while True:
        f = open(fileName,'r')
        k = f.readline()
        if k[0]=='R':
            k = f.readline()
            data = k.split()
            x, y = int(data[0]), int(data[1])
            print('==>', x, y)
            f.close()
            return x, y
        f.close()
        cv2.waitKey(3000)
    return 0, 0

def writeBack(file: str, x:int, y: int):
    f = open(file,'w')
    f.write('W\n')
    f.write(str(x)+' '+str(y))
    f.close()    
    
def onePlayerGo(player: int, fileName: str, img, c:Chess, color)->bool:
    file1, file2 = const.fileA, const.fileB
    if player==1: file1, file2 = const.fileB, const.fileA
    x, y = readCord(file1)
    if c.isValidCandidate(x, y, player)==False: return False
    cv2.circle(img, (const.MARGIN + x*const.GRID, const.MARGIN + y*const.GRID), 10, color, -1)
    c.setXY(x, y, player)
    writeBack(file2, x, y)
    cv2.imshow("image", img)
    cv2.waitKey(2000)
    return True
    
def go(img, c: Chess):
    if onePlayerGo(0, const.fileA, img, c, (0, 0, 0))==False: return 1
    if onePlayerGo(1, const.fileB, img, c, (255, 255, 255))==False: return 0  
    c.printChess()
    return 2

def main():
    winner = 'A'
    const.fileA = input('fileName A: ')
    const.fileB = input('fileName B: ')
    writeBack(const.fileA, 0, 0)
    writeBack(const.fileB, 0, 0)
    chess = Chess()
    img = drawBoard()

    cv2.imshow("image", img)
    while True:        
        over = go(img, chess)
        if over==0:
            print('Over', 0)
            winner = 'A'
            break
        elif over==1:
            print('Over', 1)
            winner = 'B'
            break
        overA = isGameOver(chess, 0)
        overB = isGameOver(chess, 1)
        if overA == True:
            print(overA, 0)
            winner = 'A'
            break
        if overB == True:
            print(overB, 1)
            winner = 'B'
            break
        if cv2.waitKey(1000)==27:
            break      
        
    text = 'Winner ' + winner
    cv2.putText(img, text, (250, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 1, cv2.LINE_AA)
    cv2.imshow("image", img)
    while True:                      
        if cv2.waitKey(1000)==27:
            break        
    #cv2.waitKey(2000)
    cv2.destroyAllWindows()            

main()    