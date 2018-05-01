#include <stdio.h>
#include <stdbool.h>

/*Beginning with an empty board*/
void setBoard(char board[26][26], int boardDimension){
	
	//Initializing the original board to unoccupied
	int row, col;
	for (row = 0; row < boardDimension; row++){
		for (col = 0; col < boardDimension; col++){
			board[row][col] = 'U';
		}
	}
	
	//Initializing centre tiles
	board[boardDimension / 2 - 1][boardDimension / 2 - 1] = 'W';
	board[boardDimension / 2 - 1][boardDimension / 2] = 'B';
	board[boardDimension / 2][boardDimension / 2 - 1] = 'B';
	board[boardDimension / 2][boardDimension / 2] = 'W';
}

/* Prints out board with current positions of pieces*/
void printBoard(char board[26][26], int boardDimension){
	
	//Printing out column labels
	printf("  ");
	char i, rowDisplay, colDisplay;
	for (i = 'a'; i < boardDimension + 'a'; i++){
		printf("%c", i);
	}	
	printf("\n");

	//Printing out row labels
	for (rowDisplay = 'a'; rowDisplay < boardDimension + 'a'; rowDisplay++){ 
		printf("%c ", rowDisplay);
		
		//Printing out actual pieces
		for (colDisplay = 'a'; colDisplay < boardDimension + 'a'; colDisplay++){ 
			printf("%c", board[rowDisplay - 'a'][colDisplay - 'a']);
		}
		printf("\n");
	}
}

/*Checking if the legal move is in bounds*/
bool positionInBounds(int boardDimension, char row, char col){
	if(row > -1 && row < boardDimension && col > -1 && col < boardDimension)
		return true;
	else 
		return false;
}

/*Checking if the move is legal based on the direction the piece would capture*/
bool checkLegalInDirection(char board[26][26], int boardDimension, char row, char col, char colour, int deltaRow, int deltaCol){
	
	//Checks if the direction is inbounds
	if(positionInBounds(boardDimension, (row - 'a' + deltaRow), (col - 'a' + deltaCol))){
		char p = board[row - 'a' + deltaRow][col - 'a' + deltaCol];
		
		//Checks if the adjacent piece is of opposite colour & the current spot is unoccupied
		if(p != colour && p != 'U'){ 
			
			//Checks if there is a same colour piece at the end of the direction line
			int i;
			for (i = 2; (positionInBounds(boardDimension, (i * deltaRow + row - 'a'), (i * deltaCol + col - 'a'))); i++){
				if (board[i * deltaRow + row - 'a'][i * deltaCol + col - 'a'] == colour){
					return true;
				}
				else if (board[i * deltaRow + row - 'a'][i * deltaCol + col - 'a'] == 'U'){
					return false;
				}
			}
			return false;
		}
		else
			return false;
	}
	else
		return false;
}

/*Checks the available moves*/
bool checkLegalMoves(char board[26][26], int boardDimension, char colour){	
	
	//Iterating through all the board positions
	int i, j, y, z;
	for(i = 0; i < boardDimension; i++){ 
		for(j = 0; j < boardDimension; j++){
			
			if(board[i][j] == 'U'){
				//Iterating through all the deltaRow/deltaCol possibilities
				char a = i + 'a', b = j + 'a';
				for(y = -1; y < 2; y++){ 
					for(z = -1; z < 2; z++){
					
						//Checks if move is legal
						if((checkLegalInDirection(board, boardDimension, a, b, colour, y, z)) == true){ 
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

/*Captures opponent's pieces*/
void flipPieces(char board[26][26], int boardDimension, char row, char col, char colour){
	
	//Iterating through all possible capture directions
	int i, j, counter;
	for (i = -1; i < 2; i++){ 
		for (j = -1; j < 2; j++){
			
			//Indexing through tiles along direction
			if(checkLegalInDirection(board, boardDimension, row, col, colour, i , j)){
				for(counter = 1; (positionInBounds(boardDimension, row - 'a' + counter * i, col - 'a' + counter * j)); ++counter){

					//Ends loop if the indexed tile is same colour as user's tile
					if(board[row - 'a' + counter * i][col - 'a' + counter * j] == colour){ 
						break;
					}
					
					//Flips tile if indexed tile belongs to opponent
					else{ 
						board[row - 'a' + counter * i][col - 'a' + counter * j] = colour; 
					}
				}
			}
		}
	}
}

/*Checks if move is a corner move*/
bool isCorner(char board[26][26], int boardDimension, char row, char col){
	
		//Checks each corner (northeast, southeast, northwest, southwest)
		if((row == 'a' && col == 'a') || (row == 'a' && col == (boardDimension - 1 + 'a')) || 
	      (row == (boardDimension - 1 + 'a') && col == 'a') || (row == (boardDimension - 1 + 'a') && col == (boardDimension - 1 + 'a'))){
			return true;
		}
		return false;
}

/*Checks if move is a suicide (gives up a corner)*/
bool isSuicide(char board[26][26], int boardDimension, char row, char col){
	
	//Northeast
	if(board[0][0] == 'U'){
		if((row == 'b' && col == 'a') || (row == 'a' && col == 'b') || (row == 'b' && col == 'b')){
			return true;
		}
	}
	
	//Southeast
	if(board[boardDimension - 1][0] == 'U'){
		if((row == (boardDimension - 2 + 'a') && col == 'a') || (row == (boardDimension - 2 + 'a') && col == 'b') || (row == (boardDimension - 1 + 'a') && col == 'b')){
			return true;
		}
	}
	
	//Northwest
	if(board[0][boardDimension - 1] == 'U'){
		if((row == 'a' && col == (boardDimension - 2 + 'a')) || (row == 1 + 'a' && col == (boardDimension - 2 + 'a')) || (row == 1 + 'a' && col == (boardDimension - 1 + 'a'))){
			return true;
		}
	}
	
	//Southwest
	if(board[boardDimension - 1][boardDimension - 1] == 'U'){
		if((row == (boardDimension - 2  + 'a') && col == (boardDimension - 1 + 'a')) || (row == (boardDimension - 2 + 'a') && col == (boardDimension - 2 + 'a')) || 
			(row == boardDimension - 1 + 'a' && col == boardDimension - 2 + 'a')){
			return true;
		}
	}
	return false;
}

/*Checks if move occupies corner controlling tiles*/
bool isCornerControl(char board[26][26], int boardDimension, char row, char col, char colour){
	
	//First column
	if(col == 'a'){
		if(row == 'c' || row == boardDimension - 3 + 'a'){
			return true;
		}
	}
	
	//Third column
	if(col == 'c'){
		if(row == 'a' || row == 'c' || row == boardDimension - 3 + 'a' || row == boardDimension - 1 + 'a'){
			return true;
		}
	}
	
	//Third last column
	if(col == boardDimension - 3 + 'a'){
		if(row == 'a' || row == 'c' || row == boardDimension - 3 + 'a'|| row == boardDimension - 1 + 'a'){
			return true;
		}
	}
	
	//Last column
	if(col == boardDimension - 1 + 'a'){
		if(row == 'c' || row == boardDimension - 3 + 'a'){
			return true;
		}
	}
	return false;
}

bool isCornerControlFlip(char board[26][26], int boardDimension, char row, char col, char colour){
	
	//Initializing copied board
	char copiedBoard[26][26];
	int copyRow, copyCol;
	for (copyRow = 0; copyRow < boardDimension; copyRow++){
		for(copyCol = 0; copyCol < boardDimension; copyCol++){
			copiedBoard[copyRow][copyCol] = board[copyRow][copyCol];
		}
	}
	
	flipPieces(copiedBoard, boardDimension, row, col, colour);
	
	//Northwest
	if(copiedBoard[0][2] == colour || copiedBoard[2][0] == colour || copiedBoard[2][2] == colour){ 
		if(copiedBoard[1][1] != colour){
			return true;
		}
	}
	
	//Southwest
	if(copiedBoard[boardDimension - 3][0] == colour || copiedBoard[boardDimension - 3][2] == colour || copiedBoard[boardDimension - 1][2] == colour){
		if(copiedBoard[boardDimension - 2][1] != colour){
			return true;
		}
	}
	
	//Northeast
	if(copiedBoard[0][boardDimension - 3] == colour || copiedBoard[2][boardDimension - 3] == colour || copiedBoard[2][boardDimension - 3] == colour){
		if(copiedBoard[1][1] != colour){
			return true;
		}
	}
	
	//Southeast
	if(copiedBoard[boardDimension - 3][boardDimension - 1] == colour || copiedBoard[boardDimension - 3][boardDimension - 3] == colour || copiedBoard[boardDimension - 1][boardDimension - 3] == colour){
		if(copiedBoard[boardDimension - 2][1] != colour){
			return true;
		}
	}
	return false;
}

/* Checks if move is valid*/
bool checkValidMove(char board[26][26], int boardDimension, char colour, char row, char col){
	
	//Checking space is not occupied
	if(board[row - 'a'][col - 'a'] != 'U'){
		return false;
	}
	
	//Iterating through all the deltaRow/deltaCol possibilities
	int y, z;
	for(y = -1; y < 2; y++){ 
		for(z = -1; z < 2; z++){
			if((checkLegalInDirection(board, boardDimension, row, col, colour, y, z)) == true){
				return true;
			}	
		}
	}

	return false;
}

/*Rates the quality of the move*/
int currentMoveQual(char board[26][26], int boardDimension, char row, char col, char colour){
	
	int moveValue = 0;
	
	if(checkValidMove(board, boardDimension, colour, row, col) == true){
		moveValue = 10;
		if(isCorner(board, boardDimension, row, col) == true){
			moveValue += 1000;
		}
		if(isSuicide(board, boardDimension, row, col)){
			moveValue -= 5;
		}
		if(isCornerControl(board, boardDimension, row, col, colour)){
			moveValue += 200;
		}
		if(isCornerControlFlip(board, boardDimension, row, col, colour)){
			moveValue += 200;
		}
	}	
	return moveValue;
}	

void cpuMove(char board[26][26], int boardDimension, char colour){
	
	//Initializing board values
	char i, j;
	int mostValue = 0; 
	
	//Iterating through all spaces of board
	for (i = 'a'; i < boardDimension + 'a'; i++){
		for (j = 'a'; j < boardDimension + 'a'; j++){
			if (checkValidMove(board, boardDimension, colour, i, j) == true){
				
				//Checks if the move has a greater move value than the last strongest move, then sets that to the highest possible move value until the next 
				if(currentMoveQual(board, boardDimension, i, j, colour) > mostValue){
					mostValue = currentMoveQual(board, boardDimension, i, j, colour);
				}	
			}
		}
	}
	
	//Iterating through spaces of the board
	for (i = 'a'; i < boardDimension + 'a'; i++){
		for (j = 'a'; j < boardDimension + 'a'; j++){
			
			//Checks if the space is the highest value move and places the piece there
			if(currentMoveQual(board, boardDimension, i, j, colour) == mostValue){
				board[i - 'a'][j - 'a'] = colour;
				flipPieces(board, boardDimension, i, j, colour);
				printf("Computer places %c at %c%c.\n", colour, i, j);
				return;
			}
		}
	}
}	

/*User inputting move into board*/
bool userMove(char board[26][26], int boardDimension, char colour){
	
	//User input
	printf("Enter move for colour %c (RowCol): ", colour);
	char rowInput, colInput;
	scanf(" %c%c", &rowInput, &colInput);
	
	//Checking if user input is valid
	if (checkValidMove(board, boardDimension, colour, rowInput, colInput) == true){
		board[rowInput - 'a'][colInput - 'a'] = colour;
		flipPieces(board, boardDimension, rowInput, colInput, colour);
		return true;
	}
	else {
		printf("Invalid move.\n");
		return false;
	}
}

/*Checks for the winner at the end of game*/
void checkWinner(char board[26][26], int boardDimension, char userColour, char cpuColour){
	
	//Indexing across the board and counting # of CPU tiles and # of player tiles
	int i, j, userPieces = 0, cpuPieces = 0;
	for (i = 0; i < boardDimension; i++){
		for (j = 0; j < boardDimension; j++){
			if (board[i][j] == userColour){
				userPieces++;
			}
			else if (board[i][j] == cpuColour){
				cpuPieces++;
			}
		}
	}
	
	//Checks who has more tiles
	if (userPieces > cpuPieces){
		printf("%c player wins.\n", userColour);
	}
	else {
		printf("%c player wins.\n", cpuColour);
	}
}

/*Checks if there are no more available moves for both players*/
bool fullPieces(char board[26][26], int boardDimension){
	
	//Checks if all spaces are occupied
	int i, j;
	for (i = 0; i < boardDimension; i++){
		for (j = 0; j < boardDimension; j++){
			if (board[i][j] == 'U'){
				return false;
			}
		}
	}
	
	//Checks if neither player is able to make a move
	if(checkLegalMoves(board, boardDimension, 'W') == false && checkLegalMoves(board, boardDimension, 'B') == false){
		return true;
	}
	return true;
}	

int main(int argc, char **argv)
{
	int boardDimension;
	printf("Enter the board dimension: ");
	scanf("%d", &boardDimension);
	
	char board[26][26];
	setBoard(board, boardDimension);
	
	char cpuPlayer, userPlayer;
	printf("Computer plays (B/W) : ");
	scanf(" %c", &cpuPlayer);
	
	/*If CPU plays black*/
	if (cpuPlayer == 'B'){
		userPlayer = 'W';
		printBoard(board, boardDimension);
		
		//Loops back and forth between CPU turn and player turn; exits when neither player has moves left
		while(checkLegalMoves(board, boardDimension, 'B') || checkLegalMoves(board, boardDimension, 'W')){
			
			//Checks for black's possible moves
			if(checkLegalMoves(board, boardDimension, 'B')){
				cpuMove(board, boardDimension, cpuPlayer);
				printBoard(board, boardDimension);
				
				//If both players are unable to play, end game here
				//This prevents "W player has no available moves" from being printed before the game ends
				if (fullPieces(board, boardDimension)){
					break;
				}
			}
			else{
				printf("B player has no valid move.\n");
			}
			
			//Checks for white's moves
			if(checkLegalMoves(board, boardDimension, 'W')){
				
				//If user inputs illegal move, end the game
				if (userMove(board, boardDimension, 'W') == false){
					printf("B player wins.\n");
					return 0;
				}
				
				//Prevents" B player has no available moves" from being printed before game ends
				printBoard(board, boardDimension);
				if (fullPieces(board, boardDimension)){
					break;
				}
			}
			else{
				printf("W player has no valid move.\n");
			}	
		}
	}

	/*If user plays black*/
	else{
		userPlayer = 'B';
		printBoard(board, boardDimension);
		
		//Loops back and forth between user and CPU
		while(checkLegalMoves(board, boardDimension, 'B') || checkLegalMoves(board, boardDimension, 'W')){
			if(checkLegalMoves(board, boardDimension, 'B')){
				
				//If user inputs illegal move, end game
				if (userMove(board, boardDimension, 'B') == false){
					printf("W player wins.\n");
					return 0;
				}
				
				//Prevents "W has no moves" from being printed before game ends
				printBoard(board, boardDimension);
				if (fullPieces(board, boardDimension)){
					break;
				}
			}
			else{
				printf("B player has no valid move.\n");
			}

			//CPU's move
			if(checkLegalMoves(board, boardDimension, 'W')){
				cpuMove(board, boardDimension, 'W');
				printBoard(board, boardDimension);
				
				//Prevents "B has no moves left" from being printed before game ends
				if(fullPieces(board, boardDimension)){
					break;
				}
			}
			else{
				printf("W player has no valid move.\n");
			}
		}	
	}

	checkWinner(board, boardDimension, userPlayer, cpuPlayer);
	
	return 0;
}
