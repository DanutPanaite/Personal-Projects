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
solve(board)
printBoard(board)


