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

def checkVictory():
    for i in range(9):
        for j in range(9):
            if(solvedBoard[i][j] != board[i][j]):
                return False
    return True

#function for the buttons
def clickedSquare(row, column):
    buttonId = (buttonIdentities[row][column])
    print(entry.get())
    number = entry.get()
    if(not(number.isnumeric())):
        return
    entry.delete(0,tkinter.END)
    buttonId.configure(text = str(number), fg = "blue")
    filledBoxes.append((row, column))

#compares inputted numbers with the solved board, if correct it locks them in, else they are highlighted in red
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

#this function is used to get a solved board of the initial one, to compare the numbers inputted by the user
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

#solves the Sudoku board using Backtracking, the step by step process is shown in the game window
def automatedSolve(_event=None):
    clearColors()
    find = findEmpty(board)
    if find:
        row, col = find
    else:
        return True
    buttonId = (buttonIdentities[row][col])
    for i in range(1, 10):
        if (valid(board, row, col, i)):
            buttonId.configure(text=str(i), fg="blue")
            window.update()
            board[row][col] = i
            time.sleep(0.3)
            if automatedSolve():
                return True
                openVictoryScreen()
            buttonId.configure(bg="red")
            window.update()
            time.sleep(0.3)
            board[row][col] = 0
            buttonId.configure(text = "0", bg = "SystemButtonFace", fg = "black")
            time.sleep(0.3)
            window.update()
    return False

def startNewGame():
    for i in range(9):
        for j in range(9):
            if (initialBoard[i][j] != 0):
                button = tkinter.Button(window, text=str(initialBoard[i][j]), image=pixel, width=25, height=25, compound="c",
                                        relief="groove", command=partial(clickedSquare, i, j), state="disable")
            else:
                button = tkinter.Button(window, text=str(initialBoard[i][j]), image=pixel, width=25, height=25, compound="c",
                                        relief="groove", command=partial(clickedSquare, i, j))
            button.grid(column=j, row=i)
            buttonIdentities[i][j] = button
            if (i % 3 == 2):
                button.grid(column=j, row=i, pady=(0, 7))
            if (j % 3 == 2):
                button.grid(column=j, row=i, padx=(0, 7))

def quitGame():
    window.destroy()

def openInstructions():
    instructionWindow = tkinter.Toplevel(window)
    instructionWindow.title("Instruction Window")
    instructionWindow.geometry("400x80")
    instruction0 = tkinter.Label(instructionWindow, text = "Fill in numbers 1-9 in different cells taking care that a number doesnt ").pack(side = "top")
    instruction1 = tkinter.Label(instructionWindow, text = "repeat in the same row/column or in the same 3x3 grid.").pack()
    instruction2 = tkinter.Label(instructionWindow, text = "Z = check if your inputted numbers are right").pack()
    instruction3 = tkinter.Label(instructionWindow, text = "Q = activate the automated solver").pack()

#Game window setup
window = tkinter.Tk()
pixel = tkinter.PhotoImage(width=1, height=1)

window.title("Sudoku")
window.geometry("333x400")
window.resizable(0,0)

menu = tkinter.Menu(window)
window.config(menu = menu)

fileMenu = tkinter.Menu(menu)
menu.add_cascade(label = "File", menu = fileMenu)
instructionMenu = tkinter.Menu(menu)
menu.add_cascade(label = "Instructions", menu = instructionMenu)

fileMenu.add_command(label = "New game", command = startNewGame)
fileMenu.add_separator()
fileMenu.add_command(label = "Quit", command = quitGame)

instructionMenu.add_command(label = "How to play", command = openInstructions)

filledBoxes = []
entryLabel = tkinter.Label(window, text="Entry")
entryLabel.grid(column=0, row=10)
entry = tkinter.Entry(window, width=4)
window.bind('z', lockChoices)
window.bind('q', automatedSolve)
entry.grid(row = 10, column = 1, pady = 2)

#setting up an array for buttonId memorisation
w, h = 9, 9
buttonIdentities = [[0 for x in range(w)] for y in range(h)]

initialBoard = [
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


board = deepcopy(initialBoard)
solvedBoard = deepcopy(board)
solve(solvedBoard)

#buttons setup
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


