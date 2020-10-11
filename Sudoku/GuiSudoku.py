import tkinter
from functools import partial
from copy import copy, deepcopy
window = tkinter.Tk()
window.title("Sudoku")
window.geometry("333x370")
window.resizable(0,0)
pixel = tkinter.PhotoImage(width=1, height=1)
w, h = 9, 9
buttonIdentities = [[0 for x in range(w)] for y in range(h)]
label = tkinter.Label(window, text="Entry:")
label.grid(column=0, row=10)
entry = tkinter.Entry(window, width=4)
entry.grid(row = 10, column = 1, pady = 2)
randomBoard = [
        [7, 8, 5, 4, 3, 9, 1, 2, 6],
        [6, 1, 2, 8, 7, 5, 3, 4, 9],
        [4, 9, 3, 6, 2, 1, 5, 7, 8],
        [8, 5, 7, 9, 4, 3, 2, 6, 1],
        [2, 6, 1, 7, 5, 8, 9, 3, 4],
        [9, 3, 4, 1, 6, 2, 7, 8, 5],
        [5, 7, 8, 3, 9, 4, 6, 1, 2],
        [1, 2, 6, 5, 8, 7, 4, 9, 3],
        [3, 4, 9, 2, 1, 6, 0, 0, 7]
    ]
solvedBoard = deepcopy(randomBoard)
def findEmpty(board):
    for i in range(len(board)):
         for j in range(len(board)):
            if board[i][j] == 0:
                return (i, j)
    return None
def valid(board, row, col, valueToCheck):

    #check column
    for i in range(len(board)):
        if board[i][col] == valueToCheck and row != i:
            return False

    #check row
    for j in range(len(board)):
        if board[row][j] == valueToCheck and col != j:
            return False

    #check square

    row = row // 3
    col = col // 3
    for i in range(row*3, row*3 + 3):
        for j in range(col*3, col*3 +3):
            if board[i][j] == valueToCheck and row != i and col != j:
                return False

    return True

def printBoard(board):
    for i in range(len(board)):
        if i % 3 == 0 and i != 0:
            print("- - - - - - - - - - - - - -")
        for j in range(len(board)):
            if j % 3 == 0:
                print(" | ",end="")

            if j == 8:
                print(board[i][j], end="\n")
            else:
                print(str(board[i][j]) + " ", end="")

def solve(board):
    find = findEmpty(board)
    if find:
        row, col = find
    else:
        return True

    for i in range(1, 10):
        if valid(board, row, col, i):
            board[row][col] = i

            if solve(board):
                return True
            board[row][col] = 0
    return False
solve(solvedBoard)
def checkVictory():
    for i in range(9):
        for j in range(9):
            if(solvedBoard[i][j] != randomBoard[i][j]):
                print("row = ")
                print(i)
                print("column = ")
                print(j)
                print(solvedBoard[i][j])
                print(randomBoard[i][j])
                return False
    return True
def clickedSquare(row, column):
    buttonId = (buttonIdentities[row][column])
    print(entry.get())
    number = entry.get()
    if(not(number.isnumeric())):
        return
    randomBoard[column][row] = int(number)
    printBoard(randomBoard)
    entry.delete(0,tkinter.END)
    buttonId.configure(text = str(number))
    if(checkVictory()):
        window.destroy()
for i in range(9):
    for j in range(9):
        if(i%3 == 2 or j%3 == 2):
            button = tkinter.Button(window, text = str(randomBoard[j][i]), image=pixel, width=25, height=25, compound="c", bd = 4, command = partial(clickedSquare, i, j))
        else:
            button = tkinter.Button(window, text = str(randomBoard[j][i]), image=pixel, width=25, height=25, compound="c", command = partial(clickedSquare, i, j))
        button.grid(column=i, row=j)
        buttonIdentities[i][j] = button
window.mainloop()


