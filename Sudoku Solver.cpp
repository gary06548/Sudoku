#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

// define functions' propotype
void transfer_str_to_array_row(char [9][9], string, int);
void answer(char [9][9], int, int, bool &);

int main()
{	
	char board[9][9] = {};
	string input;
	
	cout<<"Please fill numbers and '.' in empty position to complete the initial Sudoku."<<endl<<endl;
	
	for(int i=0; i<9; i++)
	{
		cout<<"Row "<<i+1<<" :";
		cin>>input;
		transfer_str_to_array_row(board, input, i);
	}
	
	cout<<endl<<"The answer is as follows:"<<endl<<endl;
	
	bool done_flag = 0;
	answer(board, 0, 0, done_flag);
	
	return 0;
} 

void transfer_str_to_array_row(char board[9][9], string str, int row)
{
	for(int i=0; i<9; i++)
	{
		board[row][i] = str[i];	
	}
}

vector <int> row_member(char board[9][9], int row)
{
	vector <int> array;
	
	for(int i=1; i<=9; i++)
	{
		for(int j=0; j<9; j++)
		{
			if((int)board[row][j]-48 == i) 
			{
				break;
			}
			else if(j == 8)
			{
				array.push_back(i);
			}
		}
	}
	
	return array;
}

vector <int> column_member(char board[9][9], int column)
{
	vector <int> array;
	
	for(int i=1; i<=9; i++)
	{
		for(int j=0; j<9; j++)
		{
			if((int)board[j][column]-48 == i) 
			{
				break;
			}
			else if(j == 8)
			{
				array.push_back(i);
			}
		}
	}
	
	return array;
}

vector <int> block_member(char board[9][9], int row, int column)
{
	int block_row = (row/3)*3, block_column = (column/3)*3;
	
	vector <int> array;
	
	for(int k=1; k<=9; k++) // posible member
	{
		for(int i=block_row; i<block_row+3; i++)
		{
			for(int j=block_column; j<block_column+3; j++)
			{
				if((int)board[i][j]-48 == k) 
				{
					goto exit;
				}
				else if(i == block_row+2 && j == block_column+2)
				{
					array.push_back(k);
				}
			}
		}
		exit:
			k = k;
	}
		
	return array;
}

vector <int> intersect_member(vector <int> row_m, vector <int> column_m, vector <int> block_m)
{
	vector <int> array, temp_m;
	
	sort(row_m.begin(), row_m.end());
	sort(column_m.begin(), column_m.end());
	sort(block_m.begin(), block_m.end());
	
	set_intersection(row_m.begin(), row_m.end(), column_m.begin(), column_m.end(), insert_iterator< vector <int> >(temp_m, temp_m.begin()));
	set_intersection(temp_m.begin(), temp_m.end(), block_m.begin(), block_m.end(), insert_iterator< vector <int> >(array, array.begin()));
	
	return array;
}

bool check_row(char board[9][9], int row)
{
	bool test[9] = {};
	
	for(int j=0; j<9; j++)
	{
		test[ (int)board[row][j] - 49 ] = 1; // -49 = -48 - 1
	}
	
	for(int i=0; i<9; i++)
	{
		if(test[i] == 0) return 0; // fail row check
	}
	
	return 1; // pass row check
}

bool check_column(char board[9][9], int column)
{
	bool test[9] = {};
	
	for(int i=0; i<9; i++)
	{
		test[ (int)board[i][column] - 49 ] = 1; // -49 = -48 - 1
	}
	
	for(int i=0; i<9; i++)
	{
		if(test[i] == 0) return 0; // fail column check
	}
	
	return 1; // pass column check
}

bool check_block(char board[9][9], int row, int column)
{
	int block_row = (row/3)*3, block_column = (column/3)*3;
		
	bool test[9] = {};
	
	for(int i=block_row; i<block_row+3; i++)
	{
		for(int j=block_column; j<block_column+3; j++)
		{
			test[ (int)board[i][j] - 49 ] = 1; // -49 = -48 - 1
		}
	}
	
	for(int i=0; i<9; i++)
	{
		if(test[i] == 0) return 0; // fail column check
	}
	
	return 1; // pass column check
}

bool check(char board[9][9], int row, int column)
{
	if(column == 8) // check row
	{
		if( !check_row(board, row) ) return 0; // fail check
	}
	
	if(row == 8) // check column
	{
		if( !check_column(board, column) ) return 0; // fail check
	}
	
	if(row%3 == 2 && column%3 == 2) // right bottom of each block -> check block
	{
		if( !check_block(board, row, column) ) return 0; // fail check 
	}
	
	return 1; // pass check
}

void answer(char board[9][9], int row, int column, bool &done_flag)
{
	if(row < 9 && column < 9)
	{
		if(board[row][column] != '.') // known number
		{
			if(row == 8 && column == 8)
			{
				done_flag = 1; 
			}
			
			if(column < 8)
			{
				answer(board, row, column+1, done_flag); // next column
			}
			else // column == 8
			{
				answer(board, row+1, 0, done_flag); // next row
			}
		}
		else // unknown number, need to find it
		{
			vector <int> row_possible = row_member(board, row);
			vector <int> column_possible = column_member(board, column);
			vector <int> block_possible = block_member(board, row, column);
			vector <int> intersect = intersect_member(row_possible, column_possible, block_possible);
		
			for(unsigned i=0; i<intersect.size(); i++)
			{
				if(!done_flag) // haven't done
				{
					board[row][column] = (char)(intersect[i]+48);
					
					if( check(board, row, column) ) // pass check
					{
						done_flag = (row == 8 && column == 8);
						
						if(column < 8)
						{
							answer(board, row, column+1, done_flag); // next column
						}
						else // column == 8
						{
							answer(board, row+1, 0, done_flag); // next row
						}
					}
					
					if(!done_flag) board[row][column] = '.'; // reset char array data
				}
			}
		}
	}
	else
	{
		
		for(int i=0; i<9; i++)
		{
			for(int j=0; j<9; j++)
			{
				cout<<board[i][j]<<" ";	
			}
			cout<<endl;	
		}
	}	
}










