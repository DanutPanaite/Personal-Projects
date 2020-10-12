import tkinter
import time
from functools import partial
from copy import deepcopy


def findEmpty(board):
    for i in range(len(board)):
         for j in range(len(board)):
            if board[i][j] == 0:
                return (i, j)
    return None
def findEmptyAuto():
    for i in range(9):
         for j in range(9):
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


def checkVictory():
    for i in range(9):
        for j in range(9):
            if(solvedBoard[i][j] != board[i][j]):
                return False
    return True

def automatedValid(row, col, valueToCheck):

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

def clickedSquare(row, column):
    buttonId = (buttonIdentities[row][column])
    print(entry.get())
    number = entry.get()
    if(not(number.isnumeric())):
        return
    entry.delete(0,tkinter.END)
    buttonId.configure(text = str(number), fg = "blue")
    filledBoxes.append((row, column))

def lockChoices(event):
    for i in range(len(filledBoxes)):
        row = filledBoxes[i][0]
        column = filledBoxes[i][1]
        buttonId = (buttonIdentities[row][column])
        stringX = buttonId['text']
        if(int(stringX) == solvedBoard[row][column]):
            buttonId.configure(fg = "black", bg = "SystemButtonFace", state = "disable")
            board[row][column] = int(stringX)
        else:
            buttonId.configure(bg = "red")
    if (checkVictory()):
        openVictoryScreen()

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

def clearColors():
    for i in range(len(board)):
        for j in range(len(board)):
            buttonId = buttonIdentities[i][j]
            buttonId.configure(bg = "SystemButtonFace")

def openVictoryScreen():
    newWindow = tkinter.Toplevel(window)
    newWindow.title("Victory Window")
    newWindow.geometry("300x50")
    victoryText = tkinter.Label(newWindow, text = "Congratulations, you won!").pack(side = 'top')
def automatedSolve(_event=None):
    clearColors()
    find = findEmptyAuto()
    if find:
        row, col = find
    else:
        return True
    buttonId = (buttonIdentities[row][col])
    for i in range(1, 10):
        if (automatedValid(row, col, i)):
            buttonId.configure(text=str(i), fg="blue")
            window.update()
            board[row][col] = i
            printBoard(board)
            print()
            #time.sleep(0.5)
            if automatedSolve():
                return True
                openVictoryScreen()
            buttonId.configure(bg="red")
            window.update()
            #time.sleep(0.5)
            board[row][col] = 0
            buttonId.configure(text = "0", bg = "SystemButtonFace", fg = "black")
            #time.sleep(0.3)
            window.update()
    return False

window = tkinter.Tk()
window.title("Sudoku")
window.geometry("333x370")
window.resizable(0,0)
pixel = tkinter.PhotoImage(width=1, height=1)
w, h = 9, 9
buttonIdentities = [[0 for x in range(w)] for y in range(h)]
filledBoxes = []
entryLabel = tkinter.Label(window, text="Entry")
entryLabel.grid(column=0, row=10)
entry = tkinter.Entry(window, width=4)
window.bind('z', lockChoices)
window.bind('q', automatedSolve)
entry.grid(row = 10, column = 1, pady = 2)
board = [
        [7, 8, 0, 4, 0, 0, 1, 2, 0],
        [6, 0, 0, 0, 7, 5, 0, 0, 9],
        [0, 0, 0, 6, 0, 1, 0, 7, 8],
        [0, 0, 7, 0, 4, 0, 2, 6, 0],
        [0, 0, 1, 0, 5, 0, 9, 3, 0],
        [9, 0, 4, 0, 6, 0, 0, 0, 5],
        [0, 7, 0, 3, 0, 0, 0, 1, 2],
        [1, 2, 0, 0, 0, 7, 4, 0, 0],
        [0, 4, 9, 2, 0, 6, 0, 0, 7]
    ]

solvedBoard = deepcopy(board)
solve(solvedBoard)

for i in range (9):
    for j in range (9):
        if(board[i][j] != 0):
            button = tkinter.Button(window, text = str(board[i][j]), image=pixel, width=25, height=25, compound="c", relief = "groove", command = partial(clickedSquare, i, j), state = "disable")
        else:
            button = tkinter.Button(window, text = str(board[i][j]), image=pixel, width=25, height=25, compound="c", relief = "groove", command = partial(clickedSquare, i, j))
        button.grid(column=j, row=i)
        buttonIdentities[i][j] = button
        if (i % 3 == 2):
            button.grid(column=j, row=i, pady=(0,7))
        if(j % 3 == 2):
            button.grid(column=j, row=i, padx=(0,7))
window.mainloop()


